#include <QApplication>
#include <QDockWidget>
#include <QLabel>
#include <QtDebug>
#include <QTextEdit>
#include <QtSql>
#include <QtDebug>
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

  market = new Market();
  Network::initNetwork();
  Settings::initSettings();

  mainWindow = new MainWindow();
  mainWindow->show();

  CharacterIndexDock* ci = new CharacterIndexDock();
  mainWindow->addDockWidget(Qt::LeftDockWidgetArea, ci, Qt::Vertical);
  ci->show();
  ItemIndexDock* ii = new ItemIndexDock();
  ii->show();
  MarketDock* marketDock = new MarketDock(mainWindow);
  marketDock->show();
  marketDock->setFloating(true);
  mainWindow->splitDockWidget(ci, ii, Qt::Vertical);

  BlueprintCalculatorWidget* bcw = new BlueprintCalculatorWidget(mainWindow);
  bcw->show();

  return app.exec();
}
