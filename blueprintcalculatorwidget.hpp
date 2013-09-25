#ifndef BLUEPRINTCALCULATORWIDGET_HPP
#define BLUEPRINTCALCULATORWIDGET_HPP

#include <QWidget>

namespace Ui {
  class BlueprintCalculatorWidget;
}

class BlueprintCalculatorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BlueprintCalculatorWidget(QWidget *parent = 0);
  ~BlueprintCalculatorWidget();
private slots:
  void blueprintDropped(int blueprintId);
private:
  Ui::BlueprintCalculatorWidget *ui;
};

#endif // BLUEPRINTCALCULATORWIDGET_HPP
