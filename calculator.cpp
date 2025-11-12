#include "calculator.h"
#include "ui_calculator.h"
#include <QPushButton>
#include <cmath>

calculator::calculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::calculator)
    , currentValue(0.0)
    , storedValue(0.0)
    , waitingForOperand(true)
{
    ui->setupUi(this);
    ui->display->setText("0");
    connectSignals();
}

calculator::~calculator()
{
    delete ui;
}

void calculator::connectSignals()
{
    // Connect digit buttons
    connect(ui->btn0, &QPushButton::clicked, this, &calculator::digitPressed);
    connect(ui->btn1, &QPushButton::clicked, this, &calculator::digitPressed);
    connect(ui->btn2, &QPushButton::clicked, this, &calculator::digitPressed);
    connect(ui->btn3, &QPushButton::clicked, this, &calculator::digitPressed);
    connect(ui->btn4, &QPushButton::clicked, this, &calculator::digitPressed);
    connect(ui->btn5, &QPushButton::clicked, this, &calculator::digitPressed);
    connect(ui->btn6, &QPushButton::clicked, this, &calculator::digitPressed);
    connect(ui->btn7, &QPushButton::clicked, this, &calculator::digitPressed);
    connect(ui->btn8, &QPushButton::clicked, this, &calculator::digitPressed);
    connect(ui->btn9, &QPushButton::clicked, this, &calculator::digitPressed);
    
    // Connect decimal point
    connect(ui->btnDecimal, &QPushButton::clicked, this, &calculator::decimalPressed);
    
    // Connect binary operations
    connect(ui->btnAdd, &QPushButton::clicked, this, &calculator::binaryOperationPressed);
    connect(ui->btnSubtract, &QPushButton::clicked, this, &calculator::binaryOperationPressed);
    connect(ui->btnMultiply, &QPushButton::clicked, this, &calculator::binaryOperationPressed);
    connect(ui->btnDivide, &QPushButton::clicked, this, &calculator::binaryOperationPressed);
    connect(ui->btnPowerY, &QPushButton::clicked, this, &calculator::binaryOperationPressed);
    
    // Connect unary operations
    connect(ui->btnSqrt, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    connect(ui->btnPower, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    connect(ui->btnReciprocal, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    connect(ui->btnSin, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    connect(ui->btnCos, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    connect(ui->btnTan, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    connect(ui->btnLog, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    connect(ui->btnLn, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    connect(ui->btnPi, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    connect(ui->btnE, &QPushButton::clicked, this, &calculator::unaryOperationPressed);
    
    // Connect other buttons
    connect(ui->btnEquals, &QPushButton::clicked, this, &calculator::equalsPressed);
    connect(ui->btnClear, &QPushButton::clicked, this, &calculator::clearPressed);
    connect(ui->btnBackspace, &QPushButton::clicked, this, &calculator::backspacePressed);
    connect(ui->btnSign, &QPushButton::clicked, this, &calculator::signPressed);
}

void calculator::digitPressed()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QString digit = button->text();
    
    if (waitingForOperand) {
        ui->display->setText(digit);
        waitingForOperand = false;
    } else {
        QString currentText = ui->display->text();
        if (currentText == "0") {
            ui->display->setText(digit);
        } else {
            ui->display->setText(currentText + digit);
        }
    }
}

void calculator::decimalPressed()
{
    QString currentText = ui->display->text();
    
    if (waitingForOperand) {
        ui->display->setText("0.");
        waitingForOperand = false;
    } else if (!currentText.contains('.')) {
        ui->display->setText(currentText + ".");
    }
}

void calculator::unaryOperationPressed()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QString operation = button->text();
    double operand = ui->display->text().toDouble();
    double result = 0.0;
    
    if (operation == "√") {
        if (operand >= 0) {
            result = std::sqrt(operand);
        } else {
            ui->display->setText("Error");
            return;
        }
    } else if (operation == "x²") {
        result = operand * operand;
    } else if (operation == "1/x") {
        if (operand != 0) {
            result = 1.0 / operand;
        } else {
            ui->display->setText("Error");
            return;
        }
    } else if (operation == "sin") {
        result = std::sin(operand * M_PI / 180.0); // Convert to radians
    } else if (operation == "cos") {
        result = std::cos(operand * M_PI / 180.0);
    } else if (operation == "tan") {
        result = std::tan(operand * M_PI / 180.0);
    } else if (operation == "log") {
        if (operand > 0) {
            result = std::log10(operand);
        } else {
            ui->display->setText("Error");
            return;
        }
    } else if (operation == "ln") {
        if (operand > 0) {
            result = std::log(operand);
        } else {
            ui->display->setText("Error");
            return;
        }
    } else if (operation == "π") {
        result = M_PI;
        ui->display->setText(QString::number(result, 'g', 12));
        waitingForOperand = true;
        return;
    } else if (operation == "e") {
        result = M_E;
        ui->display->setText(QString::number(result, 'g', 12));
        waitingForOperand = true;
        return;
    }
    
    ui->display->setText(QString::number(result, 'g', 12));
    waitingForOperand = true;
}

void calculator::binaryOperationPressed()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QString operation = button->text();
    double operand = ui->display->text().toDouble();
    
    if (!pendingOperation.isEmpty()) {
        double result = calculate(storedValue, operand, pendingOperation);
        ui->display->setText(QString::number(result, 'g', 12));
        storedValue = result;
    } else {
        storedValue = operand;
    }
    
    pendingOperation = operation;
    waitingForOperand = true;
}

void calculator::equalsPressed()
{
    if (pendingOperation.isEmpty()) return;
    
    double operand = ui->display->text().toDouble();
    double result = calculate(storedValue, operand, pendingOperation);
    
    ui->display->setText(QString::number(result, 'g', 12));
    storedValue = 0.0;
    pendingOperation.clear();
    waitingForOperand = true;
}

void calculator::clearPressed()
{
    ui->display->setText("0");
    currentValue = 0.0;
    storedValue = 0.0;
    pendingOperation.clear();
    waitingForOperand = true;
}

void calculator::backspacePressed()
{
    if (waitingForOperand) return;
    
    QString currentText = ui->display->text();
    if (currentText.length() > 1) {
        currentText.chop(1);
        ui->display->setText(currentText);
    } else {
        ui->display->setText("0");
        waitingForOperand = true;
    }
}

void calculator::signPressed()
{
    QString currentText = ui->display->text();
    double value = currentText.toDouble();
    
    if (value != 0) {
        value = -value;
        ui->display->setText(QString::number(value, 'g', 12));
    }
}

double calculator::calculate(double leftOperand, double rightOperand, const QString &operation)
{
    if (operation == "+") {
        return leftOperand + rightOperand;
    } else if (operation == "-") {
        return leftOperand - rightOperand;
    } else if (operation == "×") {
        return leftOperand * rightOperand;
    } else if (operation == "÷") {
        if (rightOperand != 0) {
            return leftOperand / rightOperand;
        } else {
            ui->display->setText("Error");
            return 0.0;
        }
    } else if (operation == "xʸ") {
        return std::pow(leftOperand, rightOperand);
    }
    
    return 0.0;
}
