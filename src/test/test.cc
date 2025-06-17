#include <gtest/gtest.h>

#include "../backend/backend.h"
#include "../backend/constants.h"
#include "../backend/controller.h"

GTEST_TEST(files, get_file) {
  s21::Controller controller;

  ASSERT_EQ(controller.GetPoints()->size(), 0);
  ASSERT_EQ(controller.GetPolygons()->size(), 0);

  controller.OpenFile("wrong_name.obj");

  ASSERT_EQ(controller.GetPoints()->size(), 0);
  ASSERT_EQ(controller.GetPolygons()->size(), 0);

  controller.OpenFile("test/test.obj");

  ASSERT_EQ(controller.GetPoints()->size(), 8);
  ASSERT_EQ(controller.GetPolygons()->size(), 6);

  std::vector<s21::vertice> points = {{0.5, 0.5, -0.5},  {0.5, -0.5, -0.5},
                                      {0.5, 0.5, 0.5},   {0.5, -0.5, 0.5},
                                      {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5},
                                      {-0.5, 0.5, 0.5},  {-0.5, -0.5, 0.5}};
  std::vector<std::vector<int>> polygons = {{0, 4, 6, 2}, {3, 2, 6, 7},
                                            {7, 6, 4, 5}, {5, 1, 3, 7},
                                            {1, 0, 2, 3}, {5, 4, 0, 1}};

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }

  for (size_t size = 0; size < polygons.size(); ++size) {
    for (size_t in_size = 0; in_size < polygons.at(size).size(); ++in_size) {
      ASSERT_EQ(controller.GetPolygons()->at(size).at(in_size),
                polygons.at(size).at(in_size));
    }
  }

  controller.OpenFile("wrong_name.obj");

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }

  for (size_t size = 0; size < polygons.size(); ++size) {
    for (size_t in_size = 0; in_size < polygons.at(size).size(); ++in_size) {
      ASSERT_EQ(controller.GetPolygons()->at(size).at(in_size),
                polygons.at(size).at(in_size));
    }
  }
}

GTEST_TEST(zoom, zoom_plus) {
  s21::Controller controller;

  controller.OpenFile("test/test.obj");

  std::vector<s21::vertice> points = *controller.GetPoints();

  controller.ZoomValue(25);

  ASSERT_EQ(controller.GetZoom(), 50);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x * 2);
    ASSERT_FLOAT_EQ(current.y, expected.y * 2);
    ASSERT_FLOAT_EQ(current.z, expected.z * 2);
  }

  controller.ZoomValue(25);

  ASSERT_EQ(controller.GetZoom(), 75);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x * 3);
    ASSERT_FLOAT_EQ(current.y, expected.y * 3);
    ASSERT_FLOAT_EQ(current.z, expected.z * 3);
  }

  controller.ZoomValue(500);

  ASSERT_EQ(controller.GetZoom(), 100);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x * 4);
    ASSERT_FLOAT_EQ(current.y, expected.y * 4);
    ASSERT_FLOAT_EQ(current.z, expected.z * 4);
  }
}

GTEST_TEST(zoom, zoom_minus) {
  s21::Controller controller;

  controller.OpenFile("test/test.obj");

  std::vector<s21::vertice> points = *controller.GetPoints();

  controller.ZoomValue(-20);

  ASSERT_EQ(controller.GetZoom(), 5);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x / 5);
    ASSERT_FLOAT_EQ(current.y, expected.y / 5);
    ASSERT_FLOAT_EQ(current.z, expected.z / 5);
  }

  controller.ZoomValue(-2000);

  ASSERT_EQ(controller.GetZoom(), 1);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x / 25);
    ASSERT_FLOAT_EQ(current.y, expected.y / 25);
    ASSERT_FLOAT_EQ(current.z, expected.z / 25);
  }
}

GTEST_TEST(shift, shift_x) {
  s21::Controller controller;

  controller.OpenFile("test/test.obj");

  std::vector<s21::vertice> points = *controller.GetPoints();

  controller.ShiftXValue(0.5);

  ASSERT_EQ(controller.GetShift().x, 0.5);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x + 0.5);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }

  controller.ShiftXValue(100);

  ASSERT_EQ(controller.GetShift().x, 1);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x + 1);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }

  controller.ShiftXValue(-1.5);

  ASSERT_EQ(controller.GetShift().x, -0.5);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x - 0.5);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }

  controller.ShiftXValue(-100);

  ASSERT_EQ(controller.GetShift().x, -1);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x - 1);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }
}

GTEST_TEST(shift, shift_y) {
  s21::Controller controller;

  controller.OpenFile("test/test.obj");

  std::vector<s21::vertice> points = *controller.GetPoints();

  controller.ShiftYValue(0.5);

  ASSERT_EQ(controller.GetShift().y, 0.5);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y + 0.5);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }

  controller.ShiftYValue(100);

  ASSERT_EQ(controller.GetShift().y, 1);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y + 1);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }

  controller.ShiftYValue(-1.5);

  ASSERT_EQ(controller.GetShift().y, -0.5);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y - 0.5);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }

  controller.ShiftYValue(-100);

  ASSERT_EQ(controller.GetShift().y, -1);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y - 1);
    ASSERT_FLOAT_EQ(current.z, expected.z);
  }
}

GTEST_TEST(shift, shift_z) {
  s21::Controller controller;

  controller.OpenFile("test/test.obj");

  std::vector<s21::vertice> points = *controller.GetPoints();

  controller.ShiftZValue(0.5);

  ASSERT_EQ(controller.GetShift().z, 0.5);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z + 0.5);
  }

  controller.ShiftZValue(100);

  ASSERT_EQ(controller.GetShift().z, 1);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z + 1);
  }

  controller.ShiftZValue(-1.5);

  ASSERT_EQ(controller.GetShift().z, -0.5);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z - 0.5);
  }

  controller.ShiftZValue(-100);

  ASSERT_EQ(controller.GetShift().z, -1);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice expected = points.at(size);
    ASSERT_FLOAT_EQ(current.x, expected.x);
    ASSERT_FLOAT_EQ(current.y, expected.y);
    ASSERT_FLOAT_EQ(current.z, expected.z - 1);
  }
}

GTEST_TEST(rotation, rotation_x) {
  s21::Controller controller;

  controller.OpenFile("test/test.obj");

  std::vector<s21::vertice> points = *controller.GetPoints();

  controller.RotateX(90);

  ASSERT_EQ(controller.GetRotation().x, 90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).y = old.y * cos(90 * s21::Constants::CONVERT_RAD) -
                        old.z * sin(90 * s21::Constants::CONVERT_RAD);
    points.at(size).z = old.y * sin(90 * s21::Constants::CONVERT_RAD) +
                        old.z * cos(90 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }

  controller.RotateX(-180);

  ASSERT_EQ(controller.GetRotation().x, -90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).y = old.y * cos(-180 * s21::Constants::CONVERT_RAD) -
                        old.z * sin(-180 * s21::Constants::CONVERT_RAD);
    points.at(size).z = old.y * sin(-180 * s21::Constants::CONVERT_RAD) +
                        old.z * cos(-180 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }

  controller.RotateX(-90);
  controller.RotateX(-90);

  ASSERT_EQ(controller.GetRotation().x, 90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).y = old.y * cos(900 * s21::Constants::CONVERT_RAD) -
                        old.z * sin(900 * s21::Constants::CONVERT_RAD);
    points.at(size).z = old.y * sin(900 * s21::Constants::CONVERT_RAD) +
                        old.z * cos(900 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }

  controller.RotateX(90);
  controller.RotateX(90);
  controller.RotateX(630);
  controller.RotateX(90);

  ASSERT_EQ(controller.GetRotation().x, -90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).y = old.y * cos(-900 * s21::Constants::CONVERT_RAD) -
                        old.z * sin(-900 * s21::Constants::CONVERT_RAD);
    points.at(size).z = old.y * sin(-900 * s21::Constants::CONVERT_RAD) +
                        old.z * cos(-900 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }
}

GTEST_TEST(rotation, rotation_y) {
  s21::Controller controller;

  controller.OpenFile("test/test.obj");

  std::vector<s21::vertice> points = *controller.GetPoints();

  controller.RotateY(90);

  ASSERT_EQ(controller.GetRotation().y, 90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).x = old.x * cos(90 * s21::Constants::CONVERT_RAD) +
                        old.z * sin(90 * s21::Constants::CONVERT_RAD);
    points.at(size).z = -old.x * sin(90 * s21::Constants::CONVERT_RAD) +
                        old.z * cos(90 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }

  controller.RotateY(-180);

  ASSERT_EQ(controller.GetRotation().y, -90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).x = old.x * cos(-180 * s21::Constants::CONVERT_RAD) +
                        old.z * sin(-180 * s21::Constants::CONVERT_RAD);
    points.at(size).z = -old.x * sin(-180 * s21::Constants::CONVERT_RAD) +
                        old.z * cos(-180 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }

  controller.RotateY(-90);
  controller.RotateY(-90);

  ASSERT_EQ(controller.GetRotation().y, 90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).x = old.x * cos(900 * s21::Constants::CONVERT_RAD) +
                        old.z * sin(900 * s21::Constants::CONVERT_RAD);
    points.at(size).z = -old.x * sin(900 * s21::Constants::CONVERT_RAD) +
                        old.z * cos(900 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }

  controller.RotateY(90);
  controller.RotateY(90);
  controller.RotateY(630);
  controller.RotateY(90);

  ASSERT_EQ(controller.GetRotation().y, -90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).x = old.x * cos(-900 * s21::Constants::CONVERT_RAD) +
                        old.z * sin(-900 * s21::Constants::CONVERT_RAD);
    points.at(size).z = -old.x * sin(-900 * s21::Constants::CONVERT_RAD) +
                        old.z * cos(-900 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }
}

GTEST_TEST(rotation, rotation_z) {
  s21::Controller controller;

  controller.OpenFile("test/test.obj");

  std::vector<s21::vertice> points = *controller.GetPoints();

  controller.RotateZ(90);

  ASSERT_EQ(controller.GetRotation().z, 90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).x = old.x * cos(90 * s21::Constants::CONVERT_RAD) -
                        old.y * sin(90 * s21::Constants::CONVERT_RAD);
    points.at(size).y = old.x * sin(90 * s21::Constants::CONVERT_RAD) +
                        old.y * cos(90 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }

  controller.RotateZ(-180);

  ASSERT_EQ(controller.GetRotation().z, -90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).x = old.x * cos(-180 * s21::Constants::CONVERT_RAD) -
                        old.y * sin(-180 * s21::Constants::CONVERT_RAD);
    points.at(size).y = old.x * sin(-180 * s21::Constants::CONVERT_RAD) +
                        old.y * cos(-180 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }

  controller.RotateZ(-90);
  controller.RotateZ(-90);

  ASSERT_EQ(controller.GetRotation().z, 90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).x = old.x * cos(900 * s21::Constants::CONVERT_RAD) -
                        old.y * sin(900 * s21::Constants::CONVERT_RAD);
    points.at(size).y = old.x * sin(900 * s21::Constants::CONVERT_RAD) +
                        old.y * cos(900 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }

  controller.RotateZ(90);
  controller.RotateZ(90);
  controller.RotateZ(630);
  controller.RotateZ(90);

  ASSERT_EQ(controller.GetRotation().z, -90);

  for (size_t size = 0; size < points.size(); ++size) {
    s21::vertice current = controller.GetPoints()->at(size);
    s21::vertice old = points.at(size);
    points.at(size).x = old.x * cos(-900 * s21::Constants::CONVERT_RAD) -
                        old.y * sin(-900 * s21::Constants::CONVERT_RAD);
    points.at(size).y = old.x * sin(-900 * s21::Constants::CONVERT_RAD) +
                        old.y * cos(-900 * s21::Constants::CONVERT_RAD);
    ASSERT_FLOAT_EQ(current.x, points.at(size).x);
    ASSERT_FLOAT_EQ(current.y, points.at(size).y);
    ASSERT_FLOAT_EQ(current.z, points.at(size).z);
  }
}

// GTEST_TEST(projection, change_projection) {

//   s21::Controller controller;

//   ASSERT_EQ(controller.GetProjectionMode(), 0);

//   controller.ChangeProjection();

//   ASSERT_EQ(controller.GetProjectionMode(), 1);

//   controller.ChangeProjection();

//   ASSERT_EQ(controller.GetProjectionMode(), 0);
// }

GTEST_TEST(reset, reset) {
  s21::Controller controller;

  controller.ZoomValue(30);

  controller.ShiftXValue(10);
  controller.ShiftYValue(-5);
  controller.ShiftZValue(20);

  controller.RotateX(2);
  controller.RotateY(-4);
  controller.RotateZ(1);

  controller.ResetParams();

  ASSERT_EQ(controller.GetZoom(), 25);

  ASSERT_EQ(controller.GetShift().x, 0);
  ASSERT_EQ(controller.GetShift().y, 0);
  ASSERT_EQ(controller.GetShift().z, 0);

  ASSERT_EQ(controller.GetRotation().x, 0);
  ASSERT_EQ(controller.GetRotation().y, 0);
  ASSERT_EQ(controller.GetRotation().z, 0);
}

GTEST_TEST(visibility, all_params) {
  s21::Controller controller;

  ASSERT_EQ(controller.GetLinesColor(), 0);
  ASSERT_EQ(controller.GetProjectionMode(), 0);
  ASSERT_EQ(controller.GetPointsColor(), 0);
  ASSERT_EQ(controller.GetBackgroundColor(), 0);
  ASSERT_EQ(controller.GetShapeLines(), 0);
  ASSERT_EQ(controller.GetLineWidth(), 1);
  ASSERT_EQ(controller.GetPointSize(), 1);
  ASSERT_EQ(controller.GetShapePoints(), 2);

  for (int i = 1; i < 7; ++i) {
    controller.ChangeLinesColor();
    ASSERT_EQ(controller.GetLinesColor(), i);
  }

  controller.ChangeProjection();
  ASSERT_EQ(controller.GetProjectionMode(), 1);

  for (int i = 1; i < 7; ++i) {
    controller.ChangePointsColor();
    ASSERT_EQ(controller.GetPointsColor(), i);
  }

  for (int i = 1; i < 3; ++i) {
    controller.ChangeBackgroundColor();
    ASSERT_EQ(controller.GetBackgroundColor(), i);
  }

  for (int i = 1; i < 3; ++i) {
    controller.ChangeLinesShape();
    ASSERT_EQ(controller.GetShapeLines(), i);
  }

  for (int i = 2; i < 6; ++i) {
    controller.ChangeLinesWidth();
    ASSERT_EQ(controller.GetLineWidth(), i);
  }

  for (int i = 2; i < 6; ++i) {
    controller.ChangePointsSize();
    ASSERT_EQ(controller.GetPointSize(), i);
  }

  controller.ChangePointsShape();
  ASSERT_EQ(controller.GetShapePoints(), 0);
  controller.ChangePointsShape();
  ASSERT_EQ(controller.GetShapePoints(), 1);
}

GTEST_TEST(visibility, save_load_prefs) {
  s21::Controller controller;

  ASSERT_EQ(controller.GetLinesColor(), 6);
  ASSERT_EQ(controller.GetProjectionMode(), 1);
  ASSERT_EQ(controller.GetPointsColor(), 6);
  ASSERT_EQ(controller.GetBackgroundColor(), 2);
  ASSERT_EQ(controller.GetShapeLines(), 2);
  ASSERT_EQ(controller.GetLineWidth(), 5);
  ASSERT_EQ(controller.GetPointSize(), 5);
  ASSERT_EQ(controller.GetShapePoints(), 1);

  controller.ChangeBackgroundColor();
  controller.ChangeLinesColor();
  controller.ChangeLinesShape();
  controller.ChangeLinesWidth();
  controller.ChangePointsColor();
  controller.ChangePointsShape();
  controller.ChangePointsSize();
  controller.ChangeProjection();
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
