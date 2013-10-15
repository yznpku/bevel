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
#include "queries.hpp"
#include "blueprintcalculatorwidget.hpp"

MainWindow* mainWindow;

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  QFont defaultFont;
  defaultFont.setFamily("Georgia");
  QApplication::setFont(defaultFont);
  qApp->setStyleSheet("QLineEdit:read-only { background-color: rgba(0, 0, 0, 0) }");

  QSqlDatabase staticDatabase = QSqlDatabase::addDatabase("QSQLITE", "static");
  staticDatabase.setDatabaseName(qApp->applicationDirPath() + "/static.db");
  staticDatabase.open();
  QSqlDatabase userDatabase = QSqlDatabase::addDatabase("QSQLITE", "user");
  userDatabase.setDatabaseName(qApp->applicationDirPath() + "/user.db");
  userDatabase.open();
  QSqlDatabase marketDatabase = QSqlDatabase::addDatabase("QSQLITE", "market");
  marketDatabase.setDatabaseName(qApp->applicationDirPath() + "/market.db");
  marketDatabase.open();

  Queries::initQueries();
  Network::initNetwork();
  Market::initMarket();
  Settings::initSettings();

  makeSureUserDatabaseAvailable();
  makeSureMarketDatabaseAvailable();

  mainWindow = new MainWindow();
  mainWindow->setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);

  CharacterIndexDock* ci = new CharacterIndexDock(mainWindow);
  mainWindow->addDockWidget(Qt::LeftDockWidgetArea, ci, Qt::Vertical);
  ItemIndexDock* ii = new ItemIndexDock(mainWindow);
  mainWindow->addDockWidget(Qt::LeftDockWidgetArea, ii, Qt::Vertical);
  MarketDock* marketDock = new MarketDock(mainWindow);
  mainWindow->addDockWidget(Qt::TopDockWidgetArea, marketDock, Qt::Vertical);

  QDockWidget* blueprintCalculatorDock = new QDockWidget(QObject::tr("Blueprint Calculator"), mainWindow);
  BlueprintCalculatorWidget* bcw = new BlueprintCalculatorWidget();
  blueprintCalculatorDock->setWidget(bcw);
  mainWindow->addDockWidget(Qt::TopDockWidgetArea, blueprintCalculatorDock, Qt::Vertical);
  mainWindow->tabifyDockWidget(marketDock, blueprintCalculatorDock);

  mainWindow->show();
  return app.exec();
}
