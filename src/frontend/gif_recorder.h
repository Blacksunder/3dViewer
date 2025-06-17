/**
 @file gif_recorder.h
 @brief This file contains GifRecorder class declaration
 */

#ifndef GIF_RECORDER_H
#define GIF_RECORDER_H

#include <gif_lib.h>

#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QImageWriter>
#include <QMovie>
#include <QPainter>
#include <QPixmap>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <memory>

namespace s21 {
/// @class GifRecorder
/// @brief Records widget content as animated GIF
class GifRecorder : public QObject {
  Q_OBJECT
 public:
  explicit GifRecorder(QWidget* targetWidget, int fps = 10, QObject* = nullptr);
  ~GifRecorder();

  void startRecording(const QString& outputFilePath);
  void stopRecording();

 private slots:
  void captureFrame();

 private:
  GifFileType* gifFile;
  QWidget* m_widget;
  QTimer m_timer;
  std::vector<QImage> m_frames;
  QString m_outputFile;
  int m_fps;
  SavedImage savedImage;
  std::unique_ptr<ExtensionBlock> extensionBlock;
  std::vector<GifByteType> rasterBits;
  std::shared_ptr<ColorMapObject> colorMap;

  static constexpr int WIDTH = 640;
  static constexpr int HEIGHT = 480;
  static constexpr int RGB_LEVELS = 6;
  static constexpr int GRAY_START = 55;
  static constexpr int GRAY_STEP = 5;
  static constexpr int GRAY_COUNT = 40;

  void recordGif();
  void initializeColorMap();
  void initializeGifFile();
  void initializeImage();
  void saveGif();
};
}  // namespace s21

#endif  // GIF_RECORDER_H