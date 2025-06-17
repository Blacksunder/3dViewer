/**
 @file constants.h
 @brief Contains Constants class with all needed constants expressions
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

namespace s21 {
/// @brief Containts all needed constants expressions
class Constants {
 public:
  static constexpr double CONVERT_RAD = (M_PI / 180);
  static constexpr int MAX_ANGLE = 180;
  static constexpr int MIN_ANGLE = -180;
  static constexpr double MAX_SHIFT = 1.0;
  static constexpr double MIN_SHIFT = -1.0;
  static constexpr int MIN_ZOOM = 1;
  static constexpr int MAX_ZOOM = 100;
};
}  // namespace s21

#endif  // CONSTANTS_H
