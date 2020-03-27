#ifndef NUMERICO_H
#define NUMERICO_H

#include <QApplication>
#include <QDesktopWidget>
#include <QFont>
#include <QMainWindow>
#include <QMessageBox>
#include <QPalette>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QProcess>
#include <QScreen>
#include <QScrollBar>
#include <QSize>
#include <QStyle>
#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <vector>


namespace Ui {
class Numerico;
}

class Numerico : public QMainWindow {
  Q_OBJECT

public:
  explicit Numerico(QWidget *parent = nullptr);
  ~Numerico();

private slots:
  void on_objBtnConfirm_clicked();

  void on_objInputNumberInCard_returnPressed();

  void on_objRadioFound_clicked();

  void on_objRadioNotFound_clicked();

private:
  Ui::Numerico *ui;

  void endGame();

  void nextTable();

  void correctTable();

  void endOrNextGame();

  void initUpperObjs();

  void badImaginaryNumber();

  void setStyleSheet();

  void showInstructions();

  bool isNumber(std::string check);

  void showHideElements(bool show);

  void creator(int upperRangeLimit);

  void centerAndResize(int width, int height);

  std::string vectorCardToStringCard(std::vector<int> cardV);
};

#endif // NUMERICO_H
