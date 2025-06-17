/**
 @file file_loader.h
 @brief This file contains FileLoader class declaration
 */

#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <QFileDialog>
#include <QFileInfo>
#include <QString>
#include <QWidget>

namespace s21 {
/// @class FileLoader
/// @brief A class that provides file selection functionality
class FileLoader : public QWidget {
  Q_OBJECT
 public:
  explicit FileLoader(QWidget* parent = nullptr,
                      QString filter = "Model (*.obj)")
      : QWidget(parent), m_fileFilter(filter) {}
  ~FileLoader() = default;

  void setFileFilter(const QString& filter);

 public slots:
  void openFile();

 signals:
  void fileSelected(const QString& filePath, const QString& fileName);

 private:
  QString m_fileFilter;
};

}  // namespace s21

#endif  // FILE_LOADER_H