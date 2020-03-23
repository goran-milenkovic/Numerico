#include "numerico.h"
#include "ui_numerico.h"

using namespace std;

static vector<vector<int>> vectorCards;
static vector<string> stringCards;
static vector<vector<int>> foundNotFound(2);
static unsigned long currentCard = 0;
static int answer = 0;

Numerico::Numerico(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Numerico) {
  ui->setupUi(this);
  setWindowTitle("Numerico");
  setStyleSheet();
  centerAndResize(580, 60);
  ui->objCurrentCard->setReadOnly(true);
  ui->objHistory->setReadOnly(true);

  this->setWindowIcon(QIcon("/home/goran/Numerico/icon.png"));

  showInstructions();
  showHideElements(false);
  QFont font = ui->objBtnConfirm->font();
  font.setBold(true);
  ui->objBtnConfirm->setFont(font);
}

Numerico::~Numerico() { delete ui; }

void Numerico::on_objBtnConfirm_clicked() {
  int upperRangeLimit = -1;
  bool tmp = isNumber(ui->objInputUpperRangeLimit->text().toStdString());
  ui->objLabelEnterNum->setText("Upper range limit: (from 1 to ...) ");
  ui->objInputNumberInCard->setPlaceholderText(
      "Enter a number and then press enter for check...");

  if ((tmp == false) || (ui->objInputUpperRangeLimit->text().toInt() < 1)) {
    badImaginaryNumber();
  } else {
    upperRangeLimit = ui->objInputUpperRangeLimit->text().toInt();
    ui->objBtnConfirm->setEnabled(false);
    ui->objInputUpperRangeLimit->setEnabled(false);
    ui->objPlainTextEditNotFound->setReadOnly(true);
    ui->objPlainTextEditFound->setReadOnly(true);
    centerAndResize(800, 324);
  }

  ui->objPlainTextEditNotFound->setLineWrapMode(
      QPlainTextEdit::LineWrapMode::NoWrap);
  ui->objPlainTextEditFound->setLineWrapMode(
      QPlainTextEdit::LineWrapMode::NoWrap);
  // int cardsNumber = int(log2(100));
  creator(upperRangeLimit);
  initUpperObjs();
  showHideElements(true);
}

void Numerico::on_objInputNumberInCard_returnPressed() {

  if (isNumber(ui->objInputNumberInCard->text().toStdString()) == true)
    if (ui->objInputNumberInCard->text().toInt() <=
        ui->objInputUpperRangeLimit->text().toInt()) {
      int checkNumber = ui->objInputNumberInCard->text().toInt();

      std::vector<int>::iterator it =
          std::find(vectorCards[currentCard].begin(),
                    vectorCards[currentCard].end(), checkNumber);
      if (it != vectorCards[currentCard].end()) {
        std::vector<int>::iterator iter = std::find(
            foundNotFound[0].begin(), foundNotFound[0].end(), checkNumber);
        if (iter == foundNotFound[0].end()) {
          ui->objPlainTextEditFound->setPlainText(
              ui->objPlainTextEditFound->toPlainText() +
              QString::number(checkNumber) + " ");
          foundNotFound[0].push_back(checkNumber);
        }
      } else {
        std::vector<int>::iterator iter = std::find(
            foundNotFound[1].begin(), foundNotFound[1].end(), checkNumber);
        if (iter == foundNotFound[1].end()) {
          ui->objPlainTextEditNotFound->setPlainText(
              ui->objPlainTextEditNotFound->toPlainText() +
              QString::number(checkNumber) + " ");
          foundNotFound[1].push_back(checkNumber);
        }
      }
    }
  ui->objInputNumberInCard->clear();
}

void Numerico::on_objRadioFound_clicked() {
  answer += vectorCards[currentCard][0];
  endOrNextGame();
}

void Numerico::on_objRadioNotFound_clicked() { endOrNextGame(); }

string Numerico::vectorCardToStringCard(vector<int> cardV) {
  std::stringstream card;
  std::copy(cardV.begin() + 1, cardV.end(),
            std::ostream_iterator<int>(card, " "));
  return card.str();
}

void Numerico::centerAndResize(int width, int height) {
  QSize newSize(width, height);
  setGeometry(
      QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, newSize,
                          QGuiApplication::screens().first()->geometry()));
}

bool Numerico::isNumber(string check) {
  std::regex e("[1-9][0-9]*");
  if (std::regex_match(check, e))
    return true;
  else
    return false;
}

void Numerico::endGame() {

  QMessageBox msgBox;
  msgBox.setWindowTitle("Game over");
  if (answer <= 0 || answer > ui->objInputUpperRangeLimit->text().toInt())
    msgBox.setText("Error encountered, please try again!");
  else {
    msgBox.setText("The number you imagined is " + QString::number(answer));
  }
  QAbstractButton *pButtonNo =
      msgBox.addButton(tr("Quit"), QMessageBox::YesRole);
  QAbstractButton *pButtonYes =
      msgBox.addButton(tr("New Game"), QMessageBox::NoRole);

  msgBox.setDefaultButton(QMessageBox::Yes);
  msgBox.exec();
  if (msgBox.clickedButton() == pButtonNo) {
    QApplication::quit();
  } else if (msgBox.clickedButton() == pButtonYes) {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
  }
}

void Numerico::nextTable() {
  QString text = ui->objHistory->toPlainText() +
                 QString::fromStdString(stringCards[currentCard]) + "" +
                 ui->objPlainTextEditFound->toPlainText() + " \n" +
                 ui->objPlainTextEditNotFound->toPlainText() + " \n";
  ui->objHistory->setText(text);
  ui->objHistory->show();
  ui->objCurrentCard->setText(
      QString::fromStdString(stringCards[++currentCard]));
  ui->objRadioNotFound->setAutoExclusive(false);
  ui->objRadioNotFound->setChecked(false);
  ui->objInputNumberInCard->clear();
  ui->objRadioFound->setAutoExclusive(false);
  ui->objRadioFound->setChecked(false);
  ui->objPlainTextEditFound->setPlainText(
      "Numbers found in card " + QString::number(currentCard + 1) + ": ");
  ui->objPlainTextEditNotFound->setPlainText(
      "Numbers not found in card " + QString::number(currentCard + 1) + ": ");
}

void Numerico::creator(int upperRangeLimit) {
  const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  vector<int> currentCard;
  int i, j, k, degreeCounter = 1;
  for (i = 1; i <= upperRangeLimit; i = (1 << degreeCounter), degreeCounter++) {
    currentCard.push_back(i);
    for (j = i; j <= upperRangeLimit; j += (2 * i)) {
      for (k = j; k < (j + i) && k <= upperRangeLimit; k++)
        currentCard.push_back(k);
    }
    shuffle(currentCard.begin() + 1, currentCard.end(),
            std::default_random_engine(seed));
    vectorCards.push_back(currentCard);
    currentCard.clear();
  }
  shuffle(vectorCards.begin(), vectorCards.end(),
          std::default_random_engine(seed));

  correctTable();
}

void Numerico::correctTable() {
  for (unsigned long i = 0; i < vectorCards.size(); i++) {
    stringCards.push_back(vectorCardToStringCard(vectorCards[i]));
    stringCards[i].insert(0, "Card Number " + to_string(i + 1) + ": ");
    stringCards[i].insert(stringCards[i].size(), "\n");
  }
}

void Numerico::endOrNextGame() {
  foundNotFound[0].clear();
  foundNotFound[1].clear();
  centerAndResize(800, 619);
  if (currentCard + 1 == vectorCards.size())
    endGame();
  else {
    nextTable();
  }
}

void Numerico::initUpperObjs() {
  ui->objPlainTextEditFound->setPlainText(
      "Numbers found in card " + QString::number(currentCard + 1) + ": ");
  ui->objPlainTextEditNotFound->setPlainText(
      "Numbers not found in card " + QString::number(currentCard + 1) + ": ");

  ui->objLabelCardsNumber->setText(QString::fromStdString(
      "               "
      "You will be shown " +
      to_string(vectorCards.size()) + " cards with numbers"));
  ui->objCurrentCard->setText(QString::fromStdString(stringCards[currentCard]));
}

void Numerico::badImaginaryNumber() {
  QMessageBox msgBox;
  msgBox.setWindowTitle("Bad Input");
  msgBox.setText("Enter an integer greater than 0");
  QAbstractButton *pButtonYes =
      msgBox.addButton(tr("OK"), QMessageBox::YesRole);

  msgBox.setDefaultButton(QMessageBox::Yes);
  msgBox.exec();
  if (msgBox.clickedButton() == pButtonYes) {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
  }
}

void Numerico::setStyleSheet() {
  QString scrollBarV = QString::fromUtf8("QScrollBar:vertical {"
                                         "    border: 1px solid #999999;"
                                         "    background:white;"
                                         "    border-radius: 8px;"
                                         "    width:10px;    "
                                         "    margin: 0px 0px 0px 0px;"
                                         "}"
                                         "QScrollBar::handle:vertical {"
                                         "    background: rgb(122,122,122);"
                                         "    border-radius: 8px;"
                                         "    min-height: 0px;"
                                         "}"
                                         "QScrollBar::add-line:vertical {"
                                         "    background: rgb(122,122,122);"
                                         "    height: 0px;"
                                         "    border-radius: 8px;"
                                         "    subcontrol-position: bottom;"
                                         "    subcontrol-origin: margin;"
                                         "}"
                                         "QScrollBar::sub-line:vertical {"
                                         "    background: rgb(122,122,122);"
                                         "    border-radius: 8px;"
                                         "    height: 0 px;"
                                         "    subcontrol-position: top;"
                                         "    subcontrol-origin: margin;"
                                         "}");

  QString scrollBarH = QString::fromUtf8("QScrollBar:horizontal {"
                                         "    border: 1px solid #999999;"
                                         "    background:white;"
                                         "    border-radius: 8px;"
                                         "    height:10px;    "
                                         "    margin: 0px 0px 0px 0px;"
                                         "}"
                                         "QScrollBar::handle:horizontal {"
                                         "    background: rgb(122,122,122);"
                                         "    border-radius: 8px;"
                                         "    min-width: 0px;"
                                         "}"
                                         "QScrollBar::add-line:horizontal {"
                                         "    background: rgb(122,122,122);"
                                         "    border-radius: 8px;"
                                         "    width: 0px;"
                                         "    subcontrol-position: left;"
                                         "    subcontrol-origin: margin;"
                                         "}"
                                         "QScrollBar::sub-line:horizontal {"
                                         "    background: rgb(122,122,122);"
                                         "    border-radius: 8px;"
                                         "    width: 0 px;"
                                         "    subcontrol-position: right;"
                                         "    subcontrol-origin: margin;"
                                         "}");
  ui->objCurrentCard->verticalScrollBar()->setStyleSheet(scrollBarV);
  ui->objHistory->verticalScrollBar()->setStyleSheet(scrollBarV);
  ui->objPlainTextEditNotFound->horizontalScrollBar()->setStyleSheet(
      scrollBarH);
  ui->objPlainTextEditFound->horizontalScrollBar()->setStyleSheet(scrollBarH);
  ui->objLabelCardsNumber->setStyleSheet(
      "* { background-color: rgb(255,125,100); "
      "border-style: outset; "
      "border-width: 2px; "
      "border-radius: 9px; "
      "border-color: beige; "
      "font: bold 14px; "
      "min-width: 10em; "
      "padding: 6px; }");
  ui->objInputUpperRangeLimit->setStyleSheet(
      "* { background-color:rgb(176, 174, 174);"
      "font-weight: bold}");
}

void Numerico::showHideElements(bool show) {
  if (show == true) {
    ui->objLabelCardsNumber->show();
    ui->objCurrentCard->show();
    ui->objLabelQuestion->show();
    ui->objPlainTextEditFound->show();
    ui->objPlainTextEditNotFound->show();
    ui->objRadioFound->show();
    ui->objRadioNotFound->show();
    ui->objInputNumberInCard->show();
    ui->objBtnConfirm->hide();
  } else {
    ui->objLabelCardsNumber->hide();
    ui->objCurrentCard->hide();
    ui->objLabelQuestion->hide();
    ui->objPlainTextEditFound->hide();
    ui->objPlainTextEditNotFound->hide();
    ui->objHistory->hide();
    ui->objRadioFound->hide();
    ui->objRadioNotFound->hide();
    ui->objInputNumberInCard->hide();
  }
}

void Numerico::showInstructions() {
  QMessageBox::StandardButton msgBox1 = QMessageBox::information(
      this, tr("Instructions"),
      tr("First you need to enter number for the upper range limit "
         "and imagine one number between 1 and the number you entered. "
         "Then you will be informed how many cards will be shown to you. "
         "For each card you have to tell does it contains your imaginary "
         "number or not. "
         "Numbers are mixed on cards and before answering whether your "
         "imaginary number "
         "is on the current card or not, you can (optionally) ask computer as "
         "many time "
         "as you want for any number from the range does it belongs to the "
         "current card."),
      QMessageBox::Ok);
  if (msgBox1 == QMessageBox::Ok) {
    Numerico::show();
  }
}
