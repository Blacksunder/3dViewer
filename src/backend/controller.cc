/**
 @file controller.cc
 @brief Contains all Controller functions implementation
 */

#include "controller.h"

#include "backend.h"

s21::Controller::Controller() : model(std::make_unique<s21::Model>()) {}

/// @brief Gets points vector from the model and returns it
/// @return points vector
std::vector<s21::vertice>* s21::Controller::GetPoints() {
  return model->GetPoints();
}

/// @brief Gets polygons vector from the model and returns it
/// @return polygons vector
std::vector<std::vector<int>>* s21::Controller::GetPolygons() {
  return model->GetPoligons();
}

/// @brief Gets current zoom values from the model and returns it
/// @return Current zoom values
double s21::Controller::GetZoom() { return model->GetCurrentZoom(); }

/// @brief Gets current shift values from the model and returns it
/// @return Current shift values
s21::vertice s21::Controller::GetShift() { return model->GetCurrentShift(); }

/// @brief Gets current rotation values from the model and returns it
/// @return Current rotation values
s21::vertice s21::Controller::GetRotation() {
  return model->GetCurrentRotation();
}

/// @brief Gets current projection mode value from the model and returns it
/// @return Current projection mode value
int s21::Controller::GetProjectionMode() { return model->GetProjectionMode(); }

/// @brief Gives to the model a value to be used for rotation around X axis
/// @param angle A changed angle got from the view
void s21::Controller::RotateX(int angle) { model->RotateXAngle(angle); }

/// @brief Gives to the model a value to be used for rotation around Y axis
/// @param angle A changed angle got from the view
void s21::Controller::RotateY(int angle) { model->RotateYAngle(angle); }

/// @brief Gives to the model a value to be used for rotation around Z axis
/// @param angle A changed angle got from the view
void s21::Controller::RotateZ(int angle) { model->RotateZAngle(angle); }

/// @brief Gives to the model a value to be used for zoom
/// @param value A changed zoom value from the view
void s21::Controller::ZoomValue(int value) { model->ZoomValue(value); }

/// @brief Gives to the model a value to be used for shift by X axis
/// @param value A changed shift value got from the view
void s21::Controller::ShiftXValue(double value) { model->ShiftXValue(value); }

// @brief Gives to the model a value to be used for shift by Y axis
/// @param value A changed shift value got from the view
void s21::Controller::ShiftYValue(double value) { model->ShiftYValue(value); }

// @brief Gives to the model a value to be used for shift by Z axis
/// @param value A changed shift value got from the view
void s21::Controller::ShiftZValue(double value) { model->ShiftZValue(value); }

/// @brief Tells the model to change the projection mode
void s21::Controller::ChangeProjection() { model->ChangeProjection(); }

/// @brief Tells the model to reset parameters
void s21::Controller::ResetParams() { model->ResetParams(); }

/// @brief Tells the model to open a given file
/// @param file File path
void s21::Controller::OpenFile(const char* file) { model->GetFile(file); }

/// @brief Gets current lines color value from the model and returns it
/// @return Current lines color value
int s21::Controller::GetLinesColor() { return model->GetCurrentLineColor(); }

/// @brief Gets current points color value from the model and returns it
/// @return Current points color value
int s21::Controller::GetPointsColor() { return model->GetCurrentPointColor(); }

/// @brief Gets current background color value from the model and returns it
/// @return Current background color value
int s21::Controller::GetBackgroundColor() {
  return model->GetCurrentBackgroungColor();
}

/// @brief Gets current lines shape value from the model and returns it
/// @return Current lines shape value
int s21::Controller::GetShapeLines() { return model->GetCurrentShapeLines(); }

/// @brief Gets current points size value from the model and returns it
/// @return Current points size value
int s21::Controller::GetPointSize() { return model->GetCurrentPointSize(); }

/// @brief Gets current points shape value from the model and returns it
/// @return Current points shape value
int s21::Controller::GetShapePoints() { return model->GetCurrentShapePoints(); }

/// @brief Gets current lines width value from the model and returns it
/// @return Current lines width value
int s21::Controller::GetLineWidth() { return model->GetCurrentLineWidth(); }

/// @brief Tells the model to change the current background color value
void s21::Controller::ChangeBackgroundColor() {
  model->ChangeBackgroungColor();
}

/// @brief Tells the model to change the current lines color value
void s21::Controller::ChangeLinesColor() { model->ChangeLineColor(); }

/// @brief Tells the model to change the current lines shape value
void s21::Controller::ChangeLinesShape() { model->ChangeLineShape(); }

/// @brief Tells the model to change the current lines width value
void s21::Controller::ChangeLinesWidth() { model->ChangeLineWidth(); }

/// @brief Tells the model to change the current points color value
void s21::Controller::ChangePointsColor() { model->ChangePointColor(); }

/// @brief Tells the model to change the current points shape value
void s21::Controller::ChangePointsShape() { model->ChangePointShape(); }

/// @brief Tells the model to change the current points size value
void s21::Controller::ChangePointsSize() { model->ChangePointSize(); }
