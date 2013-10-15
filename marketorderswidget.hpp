#ifndef MARKETORDERSWIDGET_HPP
#define MARKETORDERSWIDGET_HPP

#include <QWidget>

namespace Ui {
  class MarketOrdersWidget;
}

class QNetworkReply;
class QTableWidget;
class QMovie;
class QLabel;
class QPushButton;

class MarketOrdersWidget : public QWidget
{
  Q_OBJECT
public:
  explicit MarketOrdersWidget(QWidget *parent = 0);
  ~MarketOrdersWidget();
public slots:
  void typeDropped(int typeId);
  void replyFinished();
private:
  void parseReply(const QString& xmlString);
  void parseReplyForTable(const QString& xmlString, QTableWidget* table, const QString& tagName);
  void addTableRow(QTableWidget* table, int stationId, double price,
                   int quantity, QString reportedTime);
  void clearTable(QTableWidget* table);
  void setBusy(bool busy);
private slots:
  void refreshOrStop();
  void refresh();
  void stop();
private:
  int typeId;
  QNetworkReply* reply;
  QPushButton* refreshOrStopButton;
  bool busy;
private:
  Ui::MarketOrdersWidget *ui;
};

#endif // MARKETORDERSWIDGET_HPP
