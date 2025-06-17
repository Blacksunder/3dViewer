/**
 @file range_input.h
 @brief This file contains RangeInputWidget class declaration
 */

#ifndef RANGE_INPUT_H
#define RANGE_INPUT_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QSlider>
#include <QSpinBox>
#include <QString>
#include <QWidget>

namespace s21 {
/// @class RangeInputWidget
/// @brief Composite widget providing range-based input with synchronized slider
/// and spinbox
class RangeInputWidget : public QWidget {
  Q_OBJECT
 public:
  explicit RangeInputWidget(QWidget* parent = nullptr, const QString& name = "",
                            int min = -180, int max = 180);
  ~RangeInputWidget() = default;

  void setValue(int value);

 private:
  void createLabel();
  void createSlider();
  void createSpinBox();
  void createConnections();

 signals:
  void valueChanged(int value);

 private:
  QString m_name;
  int m_min;
  int m_max;

  QHBoxLayout* mainLayout;
  QVBoxLayout* sliderLayout;
  QLabel* controllerName;
  QSlider* slider;
  QSpinBox* spinBox;
};

}  // namespace s21

#endif  // RANGE_INPUT_H
