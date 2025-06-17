/**
 @file main.cc
 @brief This file contains just a main function for the all program
 */

#include <QApplication>

#include "backend/backend.h"
#include "backend/controller.h"
#include "frontend/frontend.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  s21::Controller controller;

  s21::View view(&controller);
  view.setWindowTitle("3d Viewer");
  view.setFixedSize(650, 650);

  view.show();

  return app.exec();
}
