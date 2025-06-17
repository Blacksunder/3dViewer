/**
 @file gif_recorder.cc
 @brief This file contains the implementation of GifRecorder functions
 */

#include "gif_recorder.h"

/// @brief Constructs GIF recorder and connects frame capture signal
/// @param targetWidget Widget to record
/// @param fps Frame rate
/// @param parent Parent QObject
s21::GifRecorder::GifRecorder(QWidget* targetWidget, int fps, QObject* parent)
    : QObject(parent), m_widget(targetWidget), m_fps(fps) {
  connect(&m_timer, &QTimer::timeout, this, &GifRecorder::captureFrame);
}

/// @brief Ensures recording stops and resources are freed
s21::GifRecorder::~GifRecorder() { stopRecording(); }

/// @brief Starts recording session with 5 second timeout
/// @param outputFilePath Standart destination file path for GIF
void s21::GifRecorder::startRecording(const QString& outputFilePath) {
  m_outputFile = outputFilePath;
  m_frames.clear();
  if (m_fps > 0) {
    m_timer.start(1000 / m_fps);
    QTimer::singleShot(5000, this, &GifRecorder::stopRecording);
  } else {
    // qDebug() << "fps < 1";
  }
}

/// @brief Stops recording and processes frames into GIF
void s21::GifRecorder::stopRecording() {
  try {
    if (m_timer.isActive() && !m_frames.empty()) {
      m_timer.stop();
      initializeColorMap();
      initializeGifFile();
      initializeImage();
      recordGif();
      saveGif();
    } else {
      throw std::runtime_error("Timer or frame buffer error");
    }
  } catch (const std::exception& e) {
    qCritical() << "GIF creation error" << e.what();
  }
}

/// @brief Captures a single frame from the target widget
/// @details Renders widget content transparent ARGB32 image buffer
///          with fixed size (640x480) and stores it in frames collection.
///          The render() method of widget automatically captures child widgets
///          such as buttons in a given project.
void s21::GifRecorder::captureFrame() {
  if (!m_widget) return;

  QImage frame(QSize(WIDTH, HEIGHT), QImage::Format_ARGB32);
  frame.fill(Qt::transparent);

  QPainter painter(&frame);
  m_widget->render(&painter);
  painter.end();
  m_frames.push_back(frame);
}

/// @brief Creates optimazed 256-color map for GIF encoding
/// @return Pointer to allocated ColorMapObject
/// @details Generate RGB cube with 6 levels per channel (6x6x6)
///          and 40 grayscale shades from 55 to 250 in steps of 5
void s21::GifRecorder::initializeColorMap() {
  auto deleter = [](ColorMapObject* map) {
    if (map) GifFreeMapObject(map);
  };
  colorMap =
      std::shared_ptr<ColorMapObject>(GifMakeMapObject(256, nullptr), deleter);

  int index = 0;
  const uint8_t step = 255 / (RGB_LEVELS - 1);
  for (int r = 0; r < RGB_LEVELS; ++r) {
    for (int g = 0; g < RGB_LEVELS; ++g) {
      for (int b = 0; b < RGB_LEVELS; ++b) {
        colorMap->Colors[index] = {
            static_cast<uint8_t>(r * step),  // 0, 51, 102, ..., 255
            static_cast<uint8_t>(g * step), static_cast<uint8_t>(b * step)};
        index++;
      }
    }
  }

  // Добавляем 40 оттенков серого
  for (int i = 0; i < 40; ++i) {
    uint8_t gray = GRAY_START + i * GRAY_STEP;  // 55, 60, 65, ..., 250
    colorMap->Colors[index] = {gray, gray, gray};
    index++;
  }
}

/// @brief Iitializes GIF file structure and set global colorMap
void s21::GifRecorder::initializeGifFile() {
  gifFile = EGifOpenFileName("output.gif", false, nullptr);
  if (!gifFile) {
    throw std::runtime_error("Failed to open GIF file");
  }
  gifFile->SWidth = WIDTH;
  gifFile->SHeight = HEIGHT;
  gifFile->SColorResolution = 8;
  gifFile->SColorMap = colorMap.get();
  EGifSetGifVersion(gifFile, true);
}

/// @brief Iitializes GIF image structures and buffers
/// @details The SavesImage structure is filled manualy due to undefined
///          behavior of functions EGifPutImageDesc() and
///          EGifPutExtensionBlock() of the gif_lib.h library
void s21::GifRecorder::initializeImage() {
  savedImage.ImageDesc.Left = 0;
  savedImage.ImageDesc.Top = 0;
  savedImage.ImageDesc.Width = WIDTH;
  savedImage.ImageDesc.Height = HEIGHT;
  savedImage.ImageDesc.Interlace = false;
  savedImage.ImageDesc.ColorMap = nullptr;
  rasterBits.resize(WIDTH * HEIGHT);
  savedImage.RasterBits = rasterBits.data();

  extensionBlock = std::make_unique<ExtensionBlock>();
  savedImage.ExtensionBlockCount = 1;
  savedImage.ExtensionBlocks = extensionBlock.get();
  std::vector<GifByteType> bytes(4);
  savedImage.ExtensionBlocks[0].ByteCount = 4;
  savedImage.ExtensionBlocks[0].Bytes = bytes.data();
  savedImage.ExtensionBlocks[0].Bytes[0] = 4;
  savedImage.ExtensionBlocks[0].Bytes[1] = 1;
  savedImage.ExtensionBlocks[0].Bytes[2] = 0;
  savedImage.ExtensionBlocks[0].Bytes[3] = 0;
}

/// @brief Records captured frames an animated GIF file
/// @details All errors that occur when running gif_lib.h library
///          functions will most likely throw an error "Failed to finalize GIF".
///          EGifSpew() is a final function, but does not guarantee
///          that all existing structures will be freed.
void s21::GifRecorder::recordGif() {
  std::vector<GifByteType> pixels(WIDTH * HEIGHT);
  for (const QImage& frame : m_frames) {
    QImage rgbFrame = frame.convertToFormat(
        QImage::Format_Indexed8, Qt::OrderedDither | Qt::ThresholdDither);
    std::memcpy(pixels.data(), rgbFrame.constBits(), WIDTH * HEIGHT);
    std::memcpy(savedImage.RasterBits, rgbFrame.constBits(), WIDTH * HEIGHT);
    if (GifMakeSavedImage(gifFile, &savedImage) == GIF_ERROR) {
      throw std::runtime_error("Failed to add frame to GIF");
    }
  }
  if (EGifSpew(gifFile) == GIF_ERROR) {
    throw std::runtime_error("Failed to finalize GIF");
  }
}

/// @brief Save GIF file by calling QFileDialog widget
void s21::GifRecorder::saveGif() {
  if (!QFile::exists("output.gif")) {
    throw std::runtime_error("Failed to save GIF");
  } else {
    QString filter = "GIF Files (*.gif)";
    QString savePath = QFileDialog::getSaveFileName(m_widget, "Save GIF File",
                                                    QDir::homePath(), filter);
    if (!savePath.isEmpty()) {
      if (QFile::exists(savePath)) {
        QFile::remove(savePath);
      }
      if (!savePath.endsWith(".gif", Qt::CaseInsensitive)) {
        savePath += ".gif";
      }
      if (!QFile::copy("output.gif", savePath)) {
        throw std::runtime_error("Failed to copy file to savepath");
      } else {
        QFile::remove("output.gif");
      }
    } else {
      QFile::remove("output.gif");
    }
  }
}