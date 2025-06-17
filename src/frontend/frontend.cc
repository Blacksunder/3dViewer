/**
 @file frontend.cc
 @brief This file contains the implementation of all View and ControlWidget
 functions
 */

#include "frontend.h"

s21::View::View(s21::Controller* src, QWidget* parent)
    : controller(src), QOpenGLWidget(parent) {
  CreateFileLoader();
  CreateButtons();
  CreateControlWidget();
  CreateLabels();
  ConnectControlWidget();
  settings->UpdateButtons(controller->GetProjectionMode());
}

s21::View::~View() { delete settings; }

void s21::View::initializeGL() { initializeOpenGLFunctions(); }

void s21::View::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

/// @brief This function is called every time when widget is updated
void s21::View::paintGL() {
  setBackgroundColor();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up projection matrix
  glMatrixMode(GL_PROJECTION);

  // Draw the model
  drawModel();
}

/// @brief Draws a model
void s21::View::drawModel() {
  if (controller->GetShapeLines() != 2) drawLines();
  if (controller->GetShapePoints() != 2) drawPoints();
}

/// @brief Sets a background color depending on the current color value
void s21::View::setBackgroundColor() {
  switch (controller->GetBackgroundColor()) {
    case 0:
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      break;
    case 1:
      glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
      break;
    case 2:
      glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
      break;
  }
}

/// @brief Draws polygons, using current shape, color, width and projection
/// values
void s21::View::drawLines() {
  int shape = controller->GetShapeLines();

  std::vector<vertice>* points = controller->GetPoints();
  std::vector<std::vector<int>>* polygons = controller->GetPolygons();
  int projection_mode = controller->GetProjectionMode();

  int current_color = controller->GetLinesColor();
  UpdateColor(current_color);

  float lineWidth = controller->GetLineWidth();
  glLineWidth(lineWidth);

  if (shape == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x3030);
  }

  for (size_t size = 0; size < polygons->size(); ++size) {
    glBegin(GL_LINE_LOOP);
    for (size_t polygon_size = 0; polygon_size < polygons->at(size).size();
         ++polygon_size) {
      size_t point_index = polygons->at(size).at(polygon_size);
      if (point_index < points->size()) {
        vertice point = points->at(point_index);
        if (projection_mode) point = CountForCentralProj(point);
        glVertex3d(point.x, point.y, point.z);
      }
    }
    glEnd();
  }
  if (shape == 1) glDisable(GL_LINE_STIPPLE);
}

/// @brief Draws points, using current shape, color, width and projection values
void s21::View::drawPoints() {
  int shape = controller->GetShapePoints();

  std::vector<vertice>* points = controller->GetPoints();
  int projection_mode = controller->GetProjectionMode();

  int color = controller->GetPointsColor();
  UpdateColor(color);

  int points_size = controller->GetPointSize();

  if (shape == 1) glEnable(GL_POINT_SMOOTH);

  glPointSize(points_size);

  glBegin(GL_POINTS);
  for (size_t size = 0; size < points->size(); ++size) {
    vertice point = points->at(size);
    if (projection_mode) point = CountForCentralProj(point);
    glVertex3d(point.x, point.y, point.z);
  }
  glEnd();

  if (shape == 1) glDisable(GL_POINT_SMOOTH);
}

/// @brief Change the current point coordinates to be showed if the central
/// projection is enabled
/// @param point The current point
/// @return Updated with central projection mode point
s21::vertice s21::View::CountForCentralProj(vertice point) {
  vertice old_point = point;
  int view_distance = -5;
  double divisor = old_point.z + view_distance;
  point.x = view_distance * old_point.x / divisor;
  point.y = view_distance * old_point.y / divisor;
  return point;
}

/// @brief Creates buttons for settings window, saving an image and a file
/// opening functions
void s21::View::CreateButtons() {
  menuBar = new QWidget(this);
  QVBoxLayout* menuBarLayout = new QVBoxLayout(menuBar);

  openControl = new QPushButton("Settings", menuBar);
  saveAsImage = new QPushButton("Save as...", menuBar);
  openFile = new QPushButton("Open...", menuBar);

  // Set button sizes
  int buttonWidth = 70;   // Width of the buttons
  int buttonHeight = 20;  // Height of the buttons

  openControl->setFixedSize(buttonWidth, buttonHeight);
  saveAsImage->setFixedSize(buttonWidth, buttonHeight);
  openFile->setFixedSize(buttonWidth, buttonHeight);

  menuBar->setLayout(menuBarLayout);
  menuBarLayout->addWidget(openControl);
  menuBarLayout->addWidget(openFile);
  menuBarLayout->addWidget(saveAsImage);

  connect(openControl, &QPushButton::clicked, [this]() {
    if (settings->isHidden())
      settings->show();
    else
      settings->hide();
  });

  connect(openFile, &QPushButton::clicked, fileLoader, &FileLoader::openFile);

  connect(saveAsImage, &QPushButton::clicked, [this]() {
    menuBar->hide();
    saveWidgetAsImage();
    menuBar->show();
  });

#ifdef WITH_GIF_SUPPORT
  saveAsGif = new QPushButton("Record...", menuBar);
  saveAsGif->setFixedSize(buttonWidth, buttonHeight);
  menuBarLayout->addWidget(saveAsGif);

  gifRecorder = new GifRecorder(this, 10);

  connect(saveAsGif, &QPushButton::clicked,
          [this]() { gifRecorder->startRecording("output.gif"); });
#endif
}

/// @brief Creates the settings widget
void s21::View::CreateControlWidget() {
  settings = new ControlWidget(this);
  settings->setFixedSize(320, 650);
  settings->setWindowTitle("Settings");
  settings->hide();
}

/// @brief Creates the label that contains an information about the current
/// model
void s21::View::CreateLabels() {
  modelInfo = new QLabel(this);
  modelInfo->setFixedWidth(200);
  UpdateModelInfo("None");
  modelInfo->move(10, 590);
  QPalette palette;
  palette.setColor(QPalette::WindowText, Qt::black);
  palette.setColor(QPalette::Window, Qt::white);
  modelInfo->setAutoFillBackground(true);
  modelInfo->setPalette(palette);
}

/// @brief Connects all ControlWidget's control elements' signals with
/// appropriate View slots
void s21::View::ConnectControlWidget() {
  connect(settings, &ControlWidget::xRotationChanged, this,
          &View::onXRotationChanged);
  connect(settings, &ControlWidget::yRotationChanged, this,
          &View::onYRotationChanged);
  connect(settings, &ControlWidget::zRotationChanged, this,
          &View::onZRotationChanged);
  connect(settings, &ControlWidget::ZoomChanged, this, &View::onZoomChanged);
  connect(settings, &ControlWidget::xShiftChanged, this, &View::onXShift);
  connect(settings, &ControlWidget::yShiftChanged, this, &View::onYShift);
  connect(settings, &ControlWidget::zShiftChanged, this, &View::onZShift);
  connect(settings, &ControlWidget::projectionButtonClicked, this,
          &View::onProjectionButton);
  connect(settings, &ControlWidget::colorLinesButtonClicked, this,
          &View::onLinesColorButton);

  connect(settings, &ControlWidget::colorPointsButtonClicked, this,
          &View::onPointColorButton);

  connect(settings, &ControlWidget::backgroundButtonClicked, this,
          &View::onBackgroundColorButton);

  connect(settings, &ControlWidget::ShapeLinesClicked, this,
          &View::onLinesShapeButton);

  connect(settings, &ControlWidget::SizeLinesClicked, this,
          &View::onSizeLineButton);

  connect(settings, &ControlWidget::SizePointsClicked, this,
          &View::onSizePointsButton);

  connect(settings, &ControlWidget::ShapePointsClicked, this,
          &View::onPointsShapeButton);
}

/// @brief Creates a fileLoader
void s21::View::CreateFileLoader() {
  fileLoader = new FileLoader(this);
  connect(fileLoader, &FileLoader::fileSelected, this, &View::handleFileSelect);
}

/// @brief A slot which is called when rotation around X axis is changed
/// @param value A changed angle value
void s21::View::onXRotationChanged(int value) {
  controller->RotateX(value - controller->GetRotation().x);
  update();
}

/// @brief A slot which is called when rotation around Y axis is changed
/// @param value A changed angle value
void s21::View::onYRotationChanged(int value) {
  controller->RotateY(value - controller->GetRotation().y);
  update();
}

/// @brief A slot which is called when rotation around Z axis is changed
/// @param value A changed angle value
void s21::View::onZRotationChanged(int value) {
  controller->RotateZ(value - controller->GetRotation().z);
  update();
}

/// @brief A slot which is called when zoom is changed
/// @param value A changed zoom value
void s21::View::onZoomChanged(int value) {
  controller->ZoomValue(value - controller->GetZoom());
  update();
}

/// @brief A slot which is called when shift by X axis is changed
/// @param value A changed shift value
void s21::View::onXShift(int value) {
  controller->ShiftXValue((double)value / 100.0 - controller->GetShift().x);
  update();
}

/// @brief A slot which is called when shift by Y axis is changed
/// @param value A changed shift value
void s21::View::onYShift(int value) {
  controller->ShiftYValue((double)value / 100.0 - controller->GetShift().y);
  update();
}

/// @brief A slot which is called when shift by Z axis is changed
/// @param value A changed shift value
void s21::View::onZShift(int value) {
  controller->ShiftZValue((double)value / 100.0 - controller->GetShift().z);
  update();
}

/// @brief A slot which is called when projection button is pushed
void s21::View::onProjectionButton() {
  controller->ChangeProjection();
  settings->UpdateButtons(controller->GetProjectionMode());
  update();
}

/// @brief A slot which is called when lines color button is pushed
void s21::View::onLinesColorButton() {
  controller->ChangeLinesColor();
  update();
}

/// @brief A slot which is called when points color button is pushed
void s21::View::onPointColorButton() {
  controller->ChangePointsColor();
  update();
}

/// @brief A slot which is called when background color button is pushed
void s21::View::onBackgroundColorButton() {
  controller->ChangeBackgroundColor();
  setBackgroundColor();
  update();
}

/// @brief A slot which is called when lines shape button is pushed
void s21::View::onLinesShapeButton() {
  controller->ChangeLinesShape();
  update();
}

/// @brief A slot which is called when lines size button is pushed
void s21::View::onSizeLineButton() {
  controller->ChangeLinesWidth();
  update();
}

/// @brief A slot which is called when points size button is pushed
void s21::View::onSizePointsButton() {
  controller->ChangePointsSize();
  update();
}

/// @brief A slot which is called when points shape button is pushed
void s21::View::onPointsShapeButton() {
  controller->ChangePointsShape();
  update();
}

s21::ControlWidget::ControlWidget(QWidget* parent) { CreateControlElements(); }

/// @brief Creates all control elements on the settings window
void s21::ControlWidget::CreateControlElements() {
  mainLayout = new QVBoxLayout(this);

  CreateRotationGroup();  // order of calls set order of widgets !
  CreateShiftGroup();
  CreateZoomCotroller();
  CreateProjectionToogle();
  CreateColorSettings();
  CreateSizeSettings();
  CreateShapeSettings();

  setLayout(mainLayout);

  ResetParams();

  ConnectControlElements();
}

/// @brief Creates rotation control elements
void s21::ControlWidget::CreateRotationGroup() {
  rotationGroup = new QGroupBox("Rotation", this);
  rotationLayout = new QVBoxLayout(rotationGroup);
  rotationControlX = new RangeInputWidget(this, "X", Constants::MIN_ANGLE,
                                          Constants::MAX_ANGLE);
  rotationControlY = new RangeInputWidget(this, "Y", Constants::MIN_ANGLE,
                                          Constants::MAX_ANGLE);
  rotationControlZ = new RangeInputWidget(this, "Z", Constants::MIN_ANGLE,
                                          Constants::MAX_ANGLE);
  rotationLayout->addWidget(rotationControlX);
  rotationLayout->addWidget(rotationControlY);
  rotationLayout->addWidget(rotationControlZ);
  mainLayout->addWidget(rotationGroup);
}

/// @brief Creates shift control elements
void s21::ControlWidget::CreateShiftGroup() {
  shiftGroup = new QGroupBox("Shift", this);
  shiftLayout = new QVBoxLayout(shiftGroup);
  shiftControlX = new RangeInputWidget(this, "X", Constants::MIN_SHIFT * 100,
                                       Constants::MAX_SHIFT * 100);
  shiftControlY = new RangeInputWidget(this, "Y", Constants::MIN_SHIFT * 100,
                                       Constants::MAX_SHIFT * 100);
  shiftControlZ = new RangeInputWidget(this, "Z", Constants::MIN_SHIFT * 100,
                                       Constants::MAX_SHIFT * 100);
  shiftLayout->addWidget(shiftControlX);
  shiftLayout->addWidget(shiftControlY);
  shiftLayout->addWidget(shiftControlZ);
  mainLayout->addWidget(shiftGroup);
}

/// @brief Creates zoom control elements
void s21::ControlWidget::CreateZoomCotroller() {
  zoomGroup = new QGroupBox("Zoom", this);
  zoomLayout = new QVBoxLayout(zoomGroup);
  zoomControl =
      new RangeInputWidget(this, "", Constants::MIN_ZOOM, Constants::MAX_ZOOM);
  zoomLayout->addWidget(zoomControl);
  mainLayout->addWidget(zoomGroup);
}

/// @brief Creates projection control elements
void s21::ControlWidget::CreateProjectionToogle() {
  projectionGroup = new QGroupBox("Projection mode", this);
  projectionLayout = new QHBoxLayout(projectionGroup);
  projection = new QPushButton(this);
  projection->setFixedSize(80, 25);
  projectionLayout->addWidget(projection);
  mainLayout->addWidget(projectionGroup);
}

/// @brief Creates color control elements
void s21::ControlWidget::CreateColorSettings() {
  colorGroup = new QGroupBox("Color", this);

  colorLayout = new QHBoxLayout(colorGroup);

  linesColor = new QPushButton("Lines", this);
  linesColor->setFixedSize(80, 25);
  colorLayout->addWidget(linesColor);

  pointsColor = new QPushButton("Points", this);
  pointsColor->setFixedSize(80, 25);
  colorLayout->addWidget(pointsColor);

  backgroundColor = new QPushButton("Background", this);
  backgroundColor->setFixedSize(80, 25);
  colorLayout->addWidget(backgroundColor);

  mainLayout->addWidget(colorGroup);
}

/// @brief Creates size control elements
void s21::ControlWidget::CreateSizeSettings() {
  sizeGroup = new QGroupBox("Size", this);

  sizeLayout = new QHBoxLayout(sizeGroup);

  sizeLines = new QPushButton("Lines", this);
  sizeLines->setFixedSize(80, 25);
  sizeLayout->addWidget(sizeLines);

  sizePoints = new QPushButton("Points", this);
  sizePoints->setFixedSize(80, 25);
  sizeLayout->addWidget(sizePoints);

  mainLayout->addWidget(sizeGroup);
}

/// @brief Creates shape control elements
void s21::ControlWidget::CreateShapeSettings() {
  shapeGroup = new QGroupBox("Shape", this);
  shapeLayout = new QHBoxLayout(shapeGroup);

  shapeLines = new QPushButton("Lines", this);
  shapeLines->setFixedSize(80, 25);
  shapeLayout->addWidget(shapeLines);

  shapePoints = new QPushButton("Points", this);
  shapePoints->setFixedSize(80, 25);
  shapeLayout->addWidget(shapePoints);

  mainLayout->addWidget(shapeGroup);
}

/// @brief Connects all control elements signals with signals of the settings
/// window
void s21::ControlWidget::ConnectControlElements() {
  connect(rotationControlX, &RangeInputWidget::valueChanged, this,
          &ControlWidget::xRotationChanged);
  connect(rotationControlY, &RangeInputWidget::valueChanged, this,
          &ControlWidget::yRotationChanged);
  connect(rotationControlZ, &RangeInputWidget::valueChanged, this,
          &ControlWidget::zRotationChanged);
  connect(zoomControl, &RangeInputWidget::valueChanged, this,
          &ControlWidget::ZoomChanged);
  connect(shiftControlX, &RangeInputWidget::valueChanged, this,
          &ControlWidget::xShiftChanged);
  connect(shiftControlY, &RangeInputWidget::valueChanged, this,
          &ControlWidget::yShiftChanged);
  connect(shiftControlZ, &RangeInputWidget::valueChanged, this,
          &ControlWidget::zShiftChanged);
  connect(projection, &QPushButton::clicked, this,
          &ControlWidget::projectionButtonClicked);

  connect(linesColor, &QPushButton::clicked, this,
          &ControlWidget::colorLinesButtonClicked);

  connect(pointsColor, &QPushButton::clicked, this,
          &ControlWidget::colorPointsButtonClicked);

  connect(backgroundColor, &QPushButton::clicked, this,
          &ControlWidget::backgroundButtonClicked);

  connect(sizeLines, &QPushButton::clicked, this,
          &ControlWidget::SizeLinesClicked);

  connect(sizePoints, &QPushButton::clicked, this,
          &ControlWidget::SizePointsClicked);

  connect(shapeLines, &QPushButton::clicked, this,
          &ControlWidget::ShapeLinesClicked);

  connect(shapePoints, &QPushButton::clicked, this,
          &ControlWidget::ShapePointsClicked);
}

/// @brief Updates the projection button label
/// @param proj_mode The current projection mode
void s21::ControlWidget::UpdateButtons(int proj_mode) {
  if (proj_mode)
    projection->setText("Central");
  else
    projection->setText("Parallel");
}

/// @brief Resets rotation, zoom and shift values to default values
void s21::ControlWidget::ResetParams() {
  rotationControlX->setValue(0);
  rotationControlY->setValue(0);
  rotationControlZ->setValue(0);
  zoomControl->setValue((Constants::MAX_ZOOM - Constants::MIN_ZOOM) / 4 + 1);
  shiftControlX->setValue(
      (Constants::MAX_SHIFT * 100 + Constants::MIN_SHIFT * 100) / 2);
  shiftControlY->setValue(
      (Constants::MAX_SHIFT * 100 + Constants::MIN_SHIFT * 100) / 2);
  shiftControlZ->setValue(
      (Constants::MAX_SHIFT * 100 + Constants::MIN_SHIFT * 100) / 2);
}

/// @brief The slot, which is called when a file was selected with the
/// fileLoader
/// @param filePath Full file path to be opened
/// @param fileName Just a name of the file
void s21::View::handleFileSelect(const QString& filePath,
                                 const QString& fileName) {
  const char* cstrPath = filePath.toUtf8().constData();
  controller->OpenFile(cstrPath);
  UpdateModelInfo(fileName);
  controller->ResetParams();
  settings->ResetParams();
  this->update();
}

/// @brief Updates the label describing the current model
/// @param file Current model's filename
void s21::View::UpdateModelInfo(QString file) {
  modelInfo->setText(QString::asprintf("Vertexes: %d\nPolygons: %d\nFile: ",
                                       (int)controller->GetPoints()->size(),
                                       (int)controller->GetPolygons()->size()) +
                     file);
}

/// @brief Updates the points/lines color depending on current color values for
/// points/polygons
/// @param LineColor The current color value
void s21::View::UpdateColor(int LineColor) {
  switch (LineColor) {
    case 0:
      glColor3d(1, 1, 1);
      break;
    case 1:
      glColor3d(1, 0, 0);
      break;
    case 2:
      glColor3d(1, 1, 0);
      break;
    case 3:
      glColor3d(0, 1, 1);
      break;
    case 4:
      glColor3d(0, 1, 0);
      break;
    case 5:
      glColor3d(0, 0, 1);
      break;
    case 6:
      glColor3d(1, 0, 1);
      break;
  }
}

/// @brief Saves a screenshot of the widget
void s21::View::saveWidgetAsImage() {
  QPixmap pixmap = this->grab();
  QString filter = "JPEG Files (*.jpeg);;PNG Files (*.png);;BMP Files (*.bmp)";
  QString selectedFormat;
  QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "",
                                                  filter, &selectedFormat);

  if (!fileName.isEmpty()) {
    if (selectedFormat.startsWith("PNG")) {
      selectedFormat = "PNG";
      if (!fileName.endsWith(".png", Qt::CaseInsensitive)) {
        fileName += ".png";
      }
    } else if (selectedFormat.startsWith("JPEG")) {
      selectedFormat = "JPEG";
      if (!fileName.endsWith(".jpeg", Qt::CaseInsensitive) &&
          !fileName.endsWith(".jpg", Qt::CaseInsensitive)) {
        fileName += ".jpeg";
      }
    } else if (selectedFormat.startsWith("BMP")) {
      selectedFormat = "BMP";
      if (!fileName.endsWith(".bmp", Qt::CaseInsensitive)) {
        fileName += ".bmp";
      }
    } else {
      fileName += ".png";      // if add (*) All files in filter
      selectedFormat = "PNG";  // or another case
    }

    if (pixmap.save(fileName, selectedFormat.toStdString().c_str())) {
      // qDebug() << "Image saved successfuly";
    } else {
      // qDebug() << "Error! Failed to save image";
    }
  }
}
