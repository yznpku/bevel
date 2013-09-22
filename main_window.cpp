#include "main_window.hpp"

#include <QLabel>

MainWindow::MainWindow() {
  setCentralWidget(new QWidget());
  setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
  resize(640, 480);
}
