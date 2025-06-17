/**
 @file backend.h
 @brief This file contains a declaration of Model class
 */

#ifndef BACKEND_H
#define BACKEND_H

#include <string.h>

#include <iostream>
#include <vector>

#include "constants.h"
#include "controller.h"

namespace s21 {
/** @brief Model class is responsible for all business logic, contains the
 * oroginal data */
class Model {
 private:
  std::vector<vertice>* points;
  std::vector<std::vector<int>>* polygons;
  double current_zoom;
  vertice current_coord_shift;
  vertice current_coord_angles;
  int projection_mode;

  int lines_color;
  int points_color;
  int background_color;
  int lines_shape;
  float lines_width;
  float points_size;
  int points_shape;

  char* FillLine(FILE* f, bool* end);
  void ParseLine(char* line);
  void Centrelize();
  void FillVectors(FILE* f);
  void ClearVectors();

  void CountMaxMin(vertice* max, vertice* min, vertice* current);

  int CountDeltaAngle(bool plus, int angle, int current_angle);

  void Zoom(double delta);
  void Shift(char coord, double value);

  void RotateZ(bool plus, int angle);
  void RotateY(bool plus, int angle);
  void RotateX(bool plus, int angle);

 public:
  Model()
      : current_zoom((Constants::MAX_ZOOM - Constants::MIN_ZOOM) / 4 + 1),
        projection_mode(0),
        lines_color(0),
        points_color(0),
        background_color(0),
        lines_shape(0),
        lines_width(1),
        points_size(1),
        points_shape(2) {
    current_coord_shift = {0, 0, 0};
    current_coord_angles = {0, 0, 0};
    points = new std::vector<vertice>;
    polygons = new std::vector<std::vector<int>>;
    LoadSettings();
  };
  ~Model() {
    ClearVectors();
    SaveSettings();
    delete points;
    delete polygons;
  }
  void GetFile(const char* file);

  void ResetParams();

  void ZoomValue(int value);

  void ShiftXValue(double value);
  void ShiftYValue(double value);
  void ShiftZValue(double value);

  void RotateXAngle(int angle);
  void RotateYAngle(int angle);
  void RotateZAngle(int angle);

  void ChangeProjection();

  /// @brief Returns original points vector
  /// @return points vector
  std::vector<vertice>* GetPoints() { return points; }

  /// @brief Returns original polygons vector
  /// @return polygons vector
  std::vector<std::vector<int>>* GetPoligons() { return polygons; }

  /// @brief Returns current zoom values
  /// @return Current zoom values
  double GetCurrentZoom() { return current_zoom; }

  /// @brief Returns current shift values
  /// @return Current shift values
  vertice GetCurrentShift() { return current_coord_shift; }

  /// @brief Returns current rotation values
  /// @return Current rotation values
  vertice GetCurrentRotation() { return current_coord_angles; }

  /// @brief Returns current projection mode
  /// @return Current projection mode
  int GetProjectionMode() { return projection_mode; }

  /// @brief Returns current lines' color value
  /// @return Current lines' color value
  int GetCurrentLineColor() { return lines_color; }

  /// @brief Returns current points' color value
  /// @return Current points' color value
  int GetCurrentPointColor() { return points_color; }

  /// @brief Returns current background color value
  /// @return Current background color value
  int GetCurrentBackgroungColor() { return background_color; }

  /// @brief Returns current lines' shape value
  /// @return Current lines' shape value
  int GetCurrentShapeLines() { return lines_shape; }

  /// @brief Returns current points' size value
  /// @return Current points' size value
  int GetCurrentPointSize() { return points_size; }

  /// @brief Returns current points' shape value
  /// @return Current points' shape value
  int GetCurrentShapePoints() { return points_shape; }

  /// @brief Returns current lines' width value
  /// @return Current lines' width value
  int GetCurrentLineWidth() { return lines_width; }

  void ChangeLineColor();
  void ChangePointColor();
  void ChangeBackgroungColor();
  void ChangePointSize();
  void ChangeLineWidth();
  void ChangeLineShape();
  void ChangePointShape();

  void SaveSettings();
  void LoadSettings();
};

}  // namespace s21

#endif  // BACKEND_H
