/**
 @file backend.cc
 @brief This file contains the implementation of all Model functions
 */

#include "backend.h"

/// @brief Gets .obj file to be opened and used to fill points and polygons
/// vectors
/// @param file A full file path
void s21::Model::GetFile(const char* file) {
  FILE* f = fopen(file, "r");
  if (f != NULL) {
    ClearVectors();
    FillVectors(f);
    fclose(f);
    Centrelize();
    ResetParams();
  }
}

/// @brief Clears points and polygons vectors
void s21::Model::ClearVectors() {
  points->clear();
  for (size_t main_size = 0; main_size < polygons->size(); ++main_size) {
    polygons[main_size].clear();
  }
  polygons->clear();
}

/// @brief Resets rotation, shift and zoom parameters to default values when new
/// file is opened
void s21::Model::ResetParams() {
  current_zoom = (int)(Constants::MAX_ZOOM - Constants::MIN_ZOOM) / 4 + 1;
  current_coord_shift = {0, 0, 0};
  current_coord_angles = {0, 0, 0};
}

/// @brief Gets lines one by one from a file and uses them to fill points and
/// polygons vectors
/// @param f An opened .obj file
void s21::Model::FillVectors(FILE* f) {
  bool end = false;

  while (!end) {
    char* line = FillLine(f, &end);
    if (!end) ParseLine(line);
    free(line);
  }
}

/// @brief Gets one line from a file
/// @param f An opened .obj file
/// @param end Tells us if we reached an end of a file
/// @return A line in a form of alloced char*
char* s21::Model::FillLine(FILE* f, bool* end) {
  char first_ch = 0;
  int i = 1;
  char* line = (char*)malloc(sizeof(char));
  while (first_ch != '\n' && !*end) {
    if ((first_ch = fgetc(f)) == EOF) *end = true;
    if (!*end) {
      line[i - 1] = first_ch;
      line = (char*)realloc(line, i + 1);
      ++i;
    }
  }
  line[i - 1] = '\0';
  return line;
}

/// @brief Parses one line to fill an appropriate (points or polygons) vector
/// @param line A line to be parsed in form of char*
void s21::Model::ParseLine(char* line) {
  if (strlen(line) < 2) return;
  if (line[0] == 'v' && line[1] == ' ') {
    line[0] = ' ';
    char* part_spaces = strtok(line, " ");
    int curr_coord = 1;
    vertice point = {0, 0, 0};
    while (part_spaces != nullptr && curr_coord <= 3) {
      if (curr_coord == 1)
        point.x = atof(part_spaces);
      else if (curr_coord == 2)
        point.y = atof(part_spaces);
      else if (curr_coord == 3)
        point.z = atof(part_spaces);
      ++curr_coord;
      part_spaces = strtok(nullptr, " ");
    }
    points->push_back(point);
  } else if (line[0] == 'f' && line[1] == ' ') {
    line[0] = ' ';
    char* part_spaces = strtok(line, " ");
    std::vector<int> loop;
    while (part_spaces != nullptr) {
      int to_push = atoi(part_spaces);
      if (to_push != 0) loop.push_back(to_push - 1);
      part_spaces = strtok(nullptr, " ");
    }
    polygons->push_back(loop);
  }
}

/// @brief Moves all points of a model to a center and resizes them to [-0.5;
/// 0.5] diapason
void s21::Model::Centrelize() {
  vertice max = {0, 0, 0};
  vertice min = {0, 0, 0};
  vertice current = {0, 0, 0};

  CountMaxMin(&max, &min, &current);

  for (size_t size = 0; size < points->size(); ++size) {
    current = (*points)[size];
    current.x -= min.x + (max.x - min.x) / 2;
    current.y -= min.y + (max.y - min.y) / 2;
    current.z -= min.z + (max.z - min.z) / 2;
    (*points)[size] = current;
  }

  CountMaxMin(&max, &min, &current);

  vertice delta = {max.x - min.x, max.y - min.y, max.z - min.z};
  double dmax = delta.x;
  if (delta.y > dmax) dmax = delta.y;
  if (delta.z > dmax) dmax = delta.z;

  for (size_t size = 0; size < points->size(); ++size) {
    current = (*points)[size];
    current.x *= 1 / dmax;
    current.y *= 1 / dmax;
    current.z *= 1 / dmax;
    (*points)[size] = current;
  }
}

/// @brief Finds max and min coordinate values of the current model
/// @param max Maximum coordinates
/// @param min Minimum coordinates
/// @param current Current coordinates
void s21::Model::CountMaxMin(vertice* max, vertice* min, vertice* current) {
  if (points->size() > 0) {
    *current = (*points)[0];
    *max = *current;
    *min = *current;
  }
  for (size_t size = 0; size < points->size(); ++size) {
    *current = (*points)[size];
    if (max->x < current->x) max->x = current->x;
    if (max->y < current->y) max->y = current->y;
    if (max->z < current->z) max->z = current->z;
    if (min->x > current->x) min->x = current->x;
    if (min->y > current->y) min->y = current->y;
    if (min->z > current->z) min->z = current->z;
  }
}

/// @brief Makes a model bigger or smaller
/// @param delta A number to multiply with all points' coordinates of a model
void s21::Model::Zoom(double delta) {
  for (size_t size = 0; size < points->size(); ++size) {
    vertice current = (*points)[size];
    current.x *= delta;
    current.y *= delta;
    current.z *= delta;
    (*points)[size] = current;
  }
}

/// @brief Counts delta number and calls Zoom function
/// @param value A value got from the user interface
void s21::Model::ZoomValue(int value) {
  if (current_zoom + value > Constants::MAX_ZOOM)
    value = Constants::MAX_ZOOM - current_zoom;
  else if (current_zoom + value < Constants::MIN_ZOOM)
    value = Constants::MIN_ZOOM - current_zoom;
  double delta = (current_zoom + value) / current_zoom;
  Zoom(delta);
  current_zoom += value;
}

/// @brief Counts shift value and call Shift function for X coordinate
/// @param value A value got from the user interface
void s21::Model::ShiftXValue(double value) {
  if (current_coord_shift.x + value > Constants::MAX_SHIFT)
    value = Constants::MAX_SHIFT - current_coord_shift.x;
  else if (current_coord_shift.x + value < Constants::MIN_SHIFT)
    value = Constants::MIN_SHIFT - current_coord_shift.x;
  Shift('x', value);
  current_coord_shift.x += value;
}

/// @brief Counts shift value and call Shift function for Y coordinate
/// @param value A value got from the user interface
void s21::Model::ShiftYValue(double value) {
  if (current_coord_shift.y + value > Constants::MAX_SHIFT)
    value = Constants::MAX_SHIFT - current_coord_shift.y;
  else if (current_coord_shift.y + value < Constants::MIN_SHIFT)
    value = Constants::MIN_SHIFT - current_coord_shift.y;
  Shift('y', value);
  current_coord_shift.y += value;
}

/// @brief Counts shift value and call Shift function for Z coordinate
/// @param value A value got from the user interface
void s21::Model::ShiftZValue(double value) {
  if (current_coord_shift.z + value > Constants::MAX_SHIFT)
    value = Constants::MAX_SHIFT - current_coord_shift.z;
  else if (current_coord_shift.z + value < Constants::MIN_SHIFT)
    value = Constants::MIN_SHIFT - current_coord_shift.z;
  Shift('z', value);
  current_coord_shift.z += value;
}

/// @brief Shifts all model's points with a value by an approptiate coordinate
/// @param coord A coordinate (x, y, or z) to be changed
/// @param value A value on which points are to be shifted
void s21::Model::Shift(char coord, double value) {
  for (size_t size = 0; size < points->size(); ++size) {
    vertice current = (*points)[size];
    switch (coord) {
      case 'x':
        current.x += value;
        break;
      case 'y':
        current.y += value;
        break;
      case 'z':
        current.z += value;
        break;
    }
    (*points)[size] = current;
  }
}

/// @brief Rotates a model around Z axis
/// @param plus True if the angle is positive, otherwise - false
/// @param angle An angle (in degrees) on which a model must be turned
void s21::Model::RotateZ(bool plus, int angle) {
  angle %= 360;
  double delta_angle = CountDeltaAngle(plus, angle, current_coord_angles.z);

  delta_angle *= Constants::CONVERT_RAD;

  for (size_t size = 0; size < points->size(); ++size) {
    vertice point = (*points)[size];
    double old_x = point.x, old_y = point.y;
    point.x = old_x * cos(delta_angle) - old_y * sin(delta_angle);
    point.y = old_x * sin(delta_angle) + old_y * cos(delta_angle);
    (*points)[size] = point;
  }

  delta_angle /= Constants::CONVERT_RAD;

  current_coord_angles.z += delta_angle;
}

/// @brief Rotates a model around Y axis
/// @param plus True if the angle is positive, otherwise - false
/// @param angle An angle (in degrees) on which a model must be turned
void s21::Model::RotateY(bool plus, int angle) {
  angle %= 360;
  double delta_angle = CountDeltaAngle(plus, angle, current_coord_angles.y);

  delta_angle *= Constants::CONVERT_RAD;

  for (size_t size = 0; size < points->size(); ++size) {
    vertice point = (*points)[size];
    double old_x = point.x, old_z = point.z;
    point.x = old_x * cos(delta_angle) + old_z * sin(delta_angle);
    point.z = old_x * (-1) * sin(delta_angle) + old_z * cos(delta_angle);
    (*points)[size] = point;
  }

  delta_angle /= Constants::CONVERT_RAD;

  current_coord_angles.y += delta_angle;
}

/// @brief Rotates a model around X axis
/// @param plus True if the angle is positive, otherwise - false
/// @param angle An angle (in degrees) on which a model must be turned
void s21::Model::RotateX(bool plus, int angle) {
  angle %= 360;
  double delta_angle = CountDeltaAngle(plus, angle, current_coord_angles.x);

  delta_angle *= Constants::CONVERT_RAD;

  for (size_t size = 0; size < points->size(); ++size) {
    vertice point = (*points)[size];
    double old_y = point.y, old_z = point.z;
    point.y = old_y * cos(delta_angle) - old_z * sin(delta_angle);
    point.z = old_y * sin(delta_angle) + old_z * cos(delta_angle);
    (*points)[size] = point;
  }

  delta_angle /= Constants::CONVERT_RAD;

  current_coord_angles.x += delta_angle;
}

/// @brief Counts an angle to be used in Rotate functions
/// @param plus True if the angle is positive, otherwise - false
/// @param angle A value got from the user interface
/// @param current_angle Current angle value around an appropriate axis
/// @return An angle to be used in Rotate functions (in degrees)
int s21::Model::CountDeltaAngle(bool plus, int angle, int current_angle) {
  double delta_angle = 0;

  if (plus) {
    if (current_angle + angle > Constants::MAX_ANGLE)
      delta_angle = -Constants::MAX_ANGLE * 2 + angle;
    else
      delta_angle = angle;
  } else {
    if (current_angle + angle < Constants::MIN_ANGLE)
      delta_angle = -Constants::MIN_ANGLE * 2 + angle;
    else
      delta_angle = angle;
  }
  return delta_angle;
}

/// @brief Gets a changed angle from the user interface and gives it to RotateX
/// function
/// @param angle A changed angle from the user interface
void s21::Model::RotateXAngle(int angle) {
  bool plus = true;
  if (angle < 0) plus = false;
  RotateX(plus, angle);
}

/// @brief Gets a changed angle from the user interface and gives it to RotateY
/// function
/// @param angle A changed angle from the user interface
void s21::Model::RotateYAngle(int angle) {
  bool plus = true;
  if (angle < 0) plus = false;
  RotateY(plus, angle);
}

/// @brief Gets a changed angle from the user interface and gives it to RotateZ
/// function
/// @param angle A changed angle from the user interface
void s21::Model::RotateZAngle(int angle) {
  bool plus = true;
  if (angle < 0) plus = false;
  RotateZ(plus, angle);
}

/// @brief Changes the param that shows current projection mode
void s21::Model::ChangeProjection() {
  if (projection_mode == 0)
    projection_mode = 1;
  else
    projection_mode = 0;
}

/// @brief Changes the param that shows current lines color
void s21::Model::ChangeLineColor() {
  if (lines_color < 6)
    lines_color++;
  else
    lines_color = 0;
}

/// @brief Changes the param that shows current points color
void s21::Model::ChangePointColor() {
  if (points_color < 6)
    points_color++;
  else
    points_color = 0;
}

/// @brief Changes the param that shows current background color
void s21::Model::ChangeBackgroungColor() {
  if (background_color < 2)
    background_color++;
  else
    background_color = 0;
}

/// @brief Changes the param that shows current lines width
void s21::Model::ChangeLineWidth() {
  if (lines_width < 5) {
    lines_width++;
  } else
    lines_width = 1;
}

/// @brief Changes the param that shows current points size
void s21::Model::ChangePointSize() {
  if (points_size < 5)
    points_size++;
  else
    points_size = 1;
}

/// @brief Changes the param that shows current points shape
void s21::Model::ChangePointShape() {
  if (points_shape < 2) {
    points_shape++;
  } else
    points_shape = 0;
}

/// @brief Changes the param that shows current lines shape
void s21::Model::ChangeLineShape() {
  if (lines_shape < 2)
    lines_shape++;
  else
    lines_shape = 0;
}

/// @brief Saves all color, shape, size and projection settings to a file to be
/// used in the next session
void s21::Model::SaveSettings() {
  FILE* f = fopen("prefs.txt", "w");
  if (f != NULL) {
    fprintf(f, "%d %d %d %d %d %f %f %d", projection_mode, points_color,
            lines_color, background_color, lines_shape, lines_width,
            points_size, points_shape);
    fclose(f);
  }
}

/// @brief Loads all color, shape, size and projection settings to be used for
/// the current session
void s21::Model::LoadSettings() {
  FILE* f = fopen("prefs.txt", "r");
  if (f != NULL) {
    fscanf(f, "%d %d %d %d %d %f %f %d", &projection_mode, &points_color,
           &lines_color, &background_color, &lines_shape, &lines_width,
           &points_size, &points_shape);
    fclose(f);
  }
}
