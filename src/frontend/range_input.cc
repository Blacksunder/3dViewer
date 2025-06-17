/**
 @file range_input.cc
 @brief This file contains the implementation of RangeInputWidget functions
 */

#include "range_input.h"

/// @brief Constructs range input widget
/// @param parent Parent widget
/// @param name Control label text
/// @param min min value range bounds
/// @param max max value range bounds
s21::RangeInputWidget::RangeInputWidget(QWidget* parent, const QString& name,
                                        int min, int max)
    : QWidget(parent), m_name(name), m_min(min), m_max(max) {
  mainLayout = new QHBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 10, 0);

  createLabel();
  createSlider();
  createSpinBox();
  createConnections();

  mainLayout->addWidget(controllerName);
  mainLayout->addWidget(slider);
  mainLayout->addWidget(spinBox);

  setLayout(mainLayout);
}

/// @brief Creatres and configure the name label widget
void s21::RangeInputWidget::createLabel() {
  controllerName = new QLabel;
  controllerName->setText(m_name);
}

/// @brief Creates horizontal slider with configured range and visual style
void s21::RangeInputWidget::createSlider() {
  slider = new QSlider(Qt::Orientation::Horizontal);
  slider->setRange(m_min, m_max);
  slider->setFixedSize(180, 20);

  QPalette palette = slider->palette();
  palette.setColor(QPalette::Highlight, Qt::red);
  palette.setColor(QPalette::Button, Qt::gray);
  slider->setPalette(palette);
}

/// @brief Creates spinbox with configured range and fixed size
void s21::RangeInputWidget::createSpinBox() {
  spinBox = new QSpinBox;
  spinBox->setRange(m_min, m_max);
  spinBox->setFixedSize(55, 25);
}

/// @brief Creates slider and spinbox signals for synchronized values
void s21::RangeInputWidget::createConnections() {
  connect(slider, &QSlider::valueChanged, spinBox, &QSpinBox::setValue);
  connect(spinBox, &QSpinBox::valueChanged, slider, &QSlider::setValue);
  connect(slider, &QSlider::valueChanged, this,
          &RangeInputWidget::valueChanged);
}

/// @brief Sets new value for both slider and spinbox
/// @param value New value to set
void s21::RangeInputWidget::setValue(int value) {
  slider->setValue(value);
  spinBox->setValue(value);
}
