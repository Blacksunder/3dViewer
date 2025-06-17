/**
 @file file_loader.cc
 @brief This file contains the implementation of FileLoader functions
 */

#include "file_loader.h"

/// @brief Opens a file selection dialog and emits fileselected() with the
/// chosen file`s path and name
void s21::FileLoader::openFile() {
  QString filePath = QFileDialog::getOpenFileName(
      this, "Select file", QDir::homePath(), m_fileFilter);
  if (!filePath.isEmpty()) {
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    emit fileSelected(filePath, fileName);
  }
}
