/**
 @file controller.h
 @brief Containts Controller class and vertice struct declaration
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <vector>

namespace s21 {

/// @brief A struct, which is used to describe points' coordinates and all their
/// params
typedef struct {
  double x;
  double y;
  double z;
} vertice;

/// @brief Controller class, is needed to connect model and view levels
class Controller {
 private:
  std::unique_ptr<class Model> model;

 public:
  Controller();

  std::vector<vertice>* GetPoints();
  std::vector<std::vector<int>>* GetPolygons();
  double GetZoom();
  vertice GetShift();
  vertice GetRotation();
  int GetProjectionMode();

  void RotateX(int angle);
  void RotateY(int angle);
  void RotateZ(int angle);

  void ZoomValue(int value);

  void ShiftXValue(double value);
  void ShiftYValue(double value);
  void ShiftZValue(double value);

  void ChangeProjection();

  void ResetParams();

  void OpenFile(const char* file);

  int GetLinesColor();
  int GetPointsColor();
  int GetBackgroundColor();
  int GetShapeLines();
  int GetPointSize();
  int GetShapePoints();
  int GetLineWidth();

  void ChangeLinesColor();
  void ChangePointsColor();
  void ChangeBackgroundColor();
  void ChangeLinesShape();
  void ChangePointsSize();
  void ChangePointsShape();
  void ChangeLinesWidth();
};

}  // namespace s21

#endif  // CONTROLLER_H
