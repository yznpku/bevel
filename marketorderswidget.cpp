#include "marketorderswidget.hpp"
#include "ui_marketorderswidget.h"

#include <QSqlQuery>
#include <QListIterator>
#include <QPushButton>
#include <QTableWidget>
#include <QtXmlPatterns>
#include <QMovie>
#include "network.hpp"
#include "queries.hpp"
#include "settings.hpp"
#include "global.hpp"

MarketOrdersWidget::MarketOrdersWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MarketOrdersWidget)
{
  ui->setupUi(this);
  for (QListIterator<MarketOrdersTable*> i({ui->sellOrdersTable, ui->buyOrdersTable}); i.hasNext();) {
    MarketOrdersTable* table = i.next();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({tr("Station"), tr("Price"), tr("Quantity"), tr("Reported Time")});
    table->verticalHeader()->hide();
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  }

  refreshOrStopButton = new QPushButton();
  setButtonState(RefreshState);
  ui->tabs->setCornerWidget(refreshOrStopButton);
  refreshOrStopButton->hide();

  typeId = -1;
  connect(ui->typePixmapLabel, SIGNAL(typeDropped(int)),
          this, SLOT(typeDropped(int)));
  connect(refreshOrStopButton, SIGNAL(clicked()),
          this, SLOT(refreshOrStop()));
}

MarketOrdersWidget::~MarketOrdersWidget()
{
  delete ui;
}

void MarketOrdersWidget::typeDropped(int typeId)
{
  this->typeId = typeId;
  QSqlQuery* typeNameQuery = Queries::getQuery(Queries::TypeNameQuery);
  typeNameQuery->bindValue(":id", typeId);
  typeNameQuery->exec();
  typeNameQuery->next();
  QString typeName = typeNameQuery->value(0).toString();
  ui->typeNameLabel->setText(QString("<h2>%1</h2>").arg(typeName));
  ui->infoButton->init(typeId);

  reply = Network::getOrders(typeId, Settings::getMarketOrdersTimeLimitSetting());
  refreshOrStopButton->show();
  setButtonState(StopState);

  connect(reply, SIGNAL(finished()),
          this, SLOT(replyFinished()));
}

void MarketOrdersWidget::replyFinished()
{
  QString xmlString = reply->readAll();
  reply->deleteLater();
  clearTable(ui->sellOrdersTable);
  clearTable(ui->buyOrdersTable);
  parseReply(xmlString);
  setButtonState(RefreshState);
}

void MarketOrdersWidget::parseReply(const QString& xmlString)
{
  parseReplyForTable(xmlString, ui->sellOrdersTable, "sell_orders");
  parseReplyForTable(xmlString, ui->buyOrdersTable, "buy_orders");
}

void MarketOrdersWidget::parseReplyForTable(const QString& xmlString, QTableWidget* table, const QString& tagName)
{
  QXmlQuery query;
  query.setFocus(xmlString);
  query.setQuery(QString("for $x in //%1/order \n"
                         "return fn:concat($x/station/text(), \"/\","
                         "                 $x/price/text(), \"/\","
                         "                 $x/vol_remain/text(), \"/\","
                         "                 $x/reported_time/text())").arg(tagName));
  QStringList strlist;
  query.evaluateTo(&strlist);
  for (int i = 0; i < strlist.size(); i++) {
    QStringList splitted = strlist[i].split("/");
    int dotIndex = splitted[3].indexOf(".");
    addTableRow(table, splitted[0].toInt(), splitted[1].toDouble(),
                       splitted[2].toInt(), splitted[3].left(dotIndex));
  }
}

void MarketOrdersWidget::addTableRow(QTableWidget* table, int stationId, double price,
                                     int quantity, QString reportedTime)
{
  QLocale locale(QLocale::English);
  int rowId = table->rowCount();
  table->insertRow(rowId);

  QSqlQuery* stationNameQuery = Queries::getQuery(Queries::StationNameQuery);
  stationNameQuery->bindValue(":id", stationId);
  stationNameQuery->exec();
  stationNameQuery->next();
  table->setItem(rowId, 0, new QTableWidgetItem(stationNameQuery->value(0).toString()));
  table->setItem(rowId, 1, new QTableWidgetItem(locale.toString(price, 'f', 2)));
  table->setItem(rowId, 2, new QTableWidgetItem(locale.toString(quantity)));
  table->setItem(rowId, 3, new QTableWidgetItem(reportedTime));
  for (QListIterator<int> i(QList<int>({1, 2, 3})); i.hasNext();)
    table->item(rowId, i.next())->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
}

void MarketOrdersWidget::clearTable(QTableWidget* table)
{
  while (table->rowCount())
    table->removeRow(0);
}

void MarketOrdersWidget::setButtonState(MarketOrdersWidget::ButtonState state)
{
  buttonState = state;
  switch (state) {
  case RefreshState:
    refreshOrStopButton->setIcon(QIcon(getIconPixmap("73_16_11")));
    break;
  case StopState:
    refreshOrStopButton->setIcon(QIcon(getIconPixmap("73_16_45")));
    break;
  }
}

void MarketOrdersWidget::refreshOrStop()
{
  switch (buttonState) {
  case RefreshState:
    refresh();
    break;
  case StopState:
    stop();
    break;
  }
}

void MarketOrdersWidget::refresh()
{
  typeDropped(typeId);
}

void MarketOrdersWidget::stop()
{
  reply->deleteLater();
  setButtonState(RefreshState);
}
