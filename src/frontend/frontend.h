/**
 @file frontend.h
 @brief This file contains View and ControlWidget classes declaration
 */

#ifndef FRONTEND_H
#define FRONTEND_H

#include <QGroupBox>
#include <QLabel>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPalette>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

#include "../backend/constants.h"
#include "../backend/controller.h"
#include "file_loader.h"
#include "range_input.h"

#ifdef WITH_GIF_SUPPORT
#include "gif_recorder.h"
#endif

namespace s21 {
/// @brief A class that implements the user interface widget to transform a
/// model
class ControlWidget : public QWidget {
  Q_OBJECT

 private:
  QVBoxLayout* mainLayout;

  QGroupBox* rotationGroup;
  QVBoxLayout* rotationLayout;
  RangeInputWidget* rotationControlX;
  RangeInputWidget* rotationControlY;
  RangeInputWidget* rotationControlZ;

  QGroupBox* shiftGroup;
  QVBoxLayout* shiftLayout;
  RangeInputWidget* shiftControlX;
  RangeInputWidget* shiftControlY;
  RangeInputWidget* shiftControlZ;

  QGroupBox* zoomGroup;
  QVBoxLayout* zoomLayout;
  RangeInputWidget* zoomControl;

  QGroupBox* projectionGroup;
  QHBoxLayout* projectionLayout;
  QPushButton* projection;

  QGroupBox* colorGroup;
  QHBoxLayout* colorLayout;
  QPushButton* linesColor;
  QPushButton* pointsColor;
  QPushButton* backgroundColor;

  QGroupBox* sizeGroup;
  QHBoxLayout* sizeLayout;
  QPushButton* sizeLines;
  QPushButton* sizePoints;

  QGroupBox* shapeGroup;
  QHBoxLayout* shapeLayout;
  QPushButton* shapeLines;
  QPushButton* shapePoints;

  void CreateControlElements();

  void CreateRotationGroup();
  void CreateShiftGroup();
  void CreateZoomCotroller();
  void CreateProjectionToogle();
  void ConnectControlElements();

  void CreateColorSettings();
  void CreateSizeSettings();
  void CreateShapeSettings();

 public:
  explicit ControlWidget(QWidget* parent = nullptr);

  void UpdateButtons(int proj_mode);
  void ResetParams();

 signals:
  void xRotationChanged(int value);
  void yRotationChanged(int value);
  void zRotationChanged(int value);
  void ZoomChanged(int value);
  void xShiftChanged(int value);
  void yShiftChanged(int value);
  void zShiftChanged(int value);
  void projectionButtonClicked();
  void colorLinesButtonClicked();
  void colorPointsButtonClicked();
  void backgroundButtonClicked();
  void ShapeLinesClicked();
  void SizeLinesClicked();
  void SizePointsClicked();
  void ShapePointsClicked();
};

/// @brief A class that implements the main window with a model view
class View : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit View(s21::Controller* src, QWidget* parent = nullptr);
  ~View();

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void closeEvent(QCloseEvent* event) override { settings->close(); }
  void UpdateModelInfo(QString file);
  void UpdateColor(int line_color);
  void drawModel();
  void setBackgroundColor();

  void drawLines();
  void drawPoints();

 private:
  s21::Controller* controller;

  QLabel* modelInfo;
  QWidget* menuBar;
  QPushButton* openControl;
  QPushButton* openFile;
  QPushButton* saveAsImage;
  s21::ControlWidget* settings;
  FileLoader* fileLoader;

#ifdef WITH_GIF_SUPPORT
  QPushButton* saveAsGif;
  GifRecorder* gifRecorder;
#endif

  vertice CountForCentralProj(vertice point);
  void CreateButtons();
  void CreateControlWidget();
  void CreateLabels();
  void CreateFileLoader();
  void ConnectControlWidget();
  void saveWidgetAsImage();

 private slots:
  void onXRotationChanged(int value);
  void onYRotationChanged(int value);
  void onZRotationChanged(int value);
  void onZoomChanged(int value);
  void onXShift(int value);
  void onYShift(int value);
  void onZShift(int value);
  void onProjectionButton();
  void onLinesColorButton();
  void onPointColorButton();
  void onBackgroundColorButton();
  void onLinesShapeButton();
  void onSizeLineButton();
  void onSizePointsButton();
  void onPointsShapeButton();

  void handleFileSelect(const QString& filePath, const QString& fileName);
};

}  // namespace s21

#endif  // FRONTEND_H
