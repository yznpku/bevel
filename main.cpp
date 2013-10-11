#include <QApplication>
#include <QDockWidget>
#include <QLabel>
#include <QTextEdit>
#include <QtSql>
#include <QVBoxLayout>
#include <QtXmlPatterns>

#include "character_index_dock.hpp"
#include "global.hpp"
#include "item_index_dock.hpp"
#include "marketdock.hpp"
#include "main_window.hpp"
#include "character.hpp"
#include "network.hpp"
#include "settings.hpp"
#include "market.hpp"
#include "blueprintcalculatorwidget.hpp"

MainWindow* mainWindow;

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  QFont defaultFont;
  defaultFont.setFamily("Georgia");
  QApplication::setFont(defaultFont);

  QSqlDatabase staticDatabase = QSqlDatabase::addDatabase("QSQLITE", "static");
  staticDatabase.setDatabaseName(qApp->applicationDirPath() + "/static.db");
  staticDatabase.open();
  makeSureUserDatabaseAvailable();
  makeSureMarketDatabaseAvailable();

  Network::initNetwork();
  Market::initMarket();
  Settings::initSettings();

  mainWindow = new MainWindow();
  mainWindow->setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
  mainWindow->show();

  CharacterIndexDock* ci = new CharacterIndexDock();
  mainWindow->addDockWidget(Qt::LeftDockWidgetArea, ci, Qt::Vertical);
  ci->show();
  ItemIndexDock* ii = new ItemIndexDock();
  ii->show();
  MarketDock* marketDock = new MarketDock();
  mainWindow->addDockWidget(Qt::TopDockWidgetArea, marketDock, Qt::Vertical);
  marketDock->show();
  mainWindow->splitDockWidget(ci, ii, Qt::Vertical);

  QDockWidget* blueprintCalculatorDock = new QDockWidget(QObject::tr("Blueprint Calculator"));
  BlueprintCalculatorWidget* bcw = new BlueprintCalculatorWidget(mainWindow);
  blueprintCalculatorDock->setWidget(bcw);
  mainWindow->addDockWidget(Qt::TopDockWidgetArea, blueprintCalculatorDock, Qt::Vertical);
  mainWindow->tabifyDockWidget(marketDock, blueprintCalculatorDock);
  blueprintCalculatorDock->show();

  return app.exec();
}
