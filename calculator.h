#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class calculator;
}
QT_END_NAMESPACE

class calculator : public QWidget
{
    Q_OBJECT

public:
    calculator(QWidget *parent = nullptr);
    ~calculator();

private slots:
    void digitPressed();
    void decimalPressed();
    void unaryOperationPressed();
    void binaryOperationPressed();
    void equalsPressed();
    void clearPressed();
    void backspacePressed();
    void signPressed();

private:
    Ui::calculator *ui;
    
    double currentValue;
    double storedValue;
    QString pendingOperation;
    bool waitingForOperand;
    
    void connectSignals();
    double calculate(double leftOperand, double rightOperand, const QString &operation);
};
#endif // CALCULATOR_H
