#include "calculate.h"
#include <cmath>

CalculatorLogic::CalculatorLogic() {
    clearAll(); // 初始化状态
}

QString CalculatorLogic::getFormulaDisplay() const {
    QString currentEditStr = "";
    if (currentMode == Mode_Normal) {
        currentEditStr = part1Str.isEmpty() ? "0" : part1Str;
    }
    else if (currentMode == Mode_Cartesian) {
        QString p1 = part1Str.isEmpty() ? "0" : part1Str;
        QString p2 = part2Str.isEmpty() ? "0" : part2Str;
        if (!isComplexEditing) {
            if (p2.startsWith("-"))  // 判断虚部是否为负数
                currentEditStr = QString("(%1 - %2i)").arg(p1, p2.mid(1));
            else
                currentEditStr = QString("(%1 + %2i)").arg(p1, p2);
        }
        else if (currentEditPart == Part_RealOrAmp)
            currentEditStr = QString("[%1]_ i %2").arg(p1, p2);
        else
            currentEditStr = QString("%1 i [%2]_").arg(p1, p2);
    }
    else if (currentMode == Mode_Polar) {
        QString p1 = part1Str.isEmpty() ? "0" : part1Str;
        QString p2 = part2Str.isEmpty() ? "0" : part2Str;
        if (!isComplexEditing)
            currentEditStr = QString("(%1 ∠ %2°)").arg(p1, p2);
        else if (currentEditPart == Part_RealOrAmp)
            currentEditStr = QString("[%1]_ ∠ %2").arg(p1, p2);
        else
            currentEditStr = QString("%1 ∠ [%2]_").arg(p1, p2);
    }
    return historyFormula + currentEditStr;
}

QString CalculatorLogic::getResultDisplay() const {
    if (isWaitingForNewNum && pendingOperator.isEmpty() && historyFormula.isEmpty())
        return currentResult.toString();  // 如果按了等号或者刚提交了一个数，显示当前结果或输入
    return currentResult.toString(); // 动态显示当前累计结果
}

Complex CalculatorLogic::getCurrentInput() const {
    double val1 = part1Str.toDouble();
    double val2 = part2Str.toDouble();
    if (currentMode == Mode_Cartesian) {
        return Complex(val1, val2);
    }
    else if (currentMode == Mode_Polar) {
        double theta = val2 * M_PI / 180.0;
        return Complex(val1 * cos(theta), val1 * sin(theta));
    }
    return Complex(val1, 0);
}

void CalculatorLogic::calculateStep(const Complex& nextOperand) {
    QString op = pendingOperator.trimmed();  // 去除空格
    if (op.isEmpty())
        currentResult = nextOperand;
    else if (op == "+" || op == "＋") {
        currentResult = currentResult + nextOperand;
    }
    else if (op == "-" || op == "−" || op == "－") {
        currentResult = currentResult - nextOperand;
    }
    else if (op == "×" || op == "*" || op == "x") {
        currentResult = currentResult * nextOperand;
    }
    else if (op == "÷" || op == "/") {
        currentResult = currentResult / nextOperand;
    }
}

void CalculatorLogic::inputDigit(const QString& digit) {
    if (currentMode != Mode_Normal && !isComplexEditing && !isWaitingForNewNum) return;
    if (isWaitingForNewNum) {
        part1Str.clear(); part2Str.clear();
        isWaitingForNewNum = false;
        currentMode = Mode_Normal;
        currentEditPart = Part_RealOrAmp;
    }
    if (currentMode == Mode_Normal || currentEditPart == Part_RealOrAmp) {
        if (digit == "." && part1Str.contains(".")) return;
        part1Str.append(digit);
    }
    else {
        if (digit == "." && part2Str.contains(".")) return;
        part2Str.append(digit);
    }
}

void CalculatorLogic::inputSymbol_i() {
    if (isWaitingForNewNum) {
        part1Str.clear(); part2Str.clear();
        isWaitingForNewNum = false;
    }
    currentMode = Mode_Cartesian;
    currentEditPart = Part_RealOrAmp;
    isComplexEditing = true;
}

void CalculatorLogic::inputSymbol_angle() {
    if (isWaitingForNewNum) {
        part1Str.clear(); part2Str.clear();
        isWaitingForNewNum = false;
    }
    currentMode = Mode_Polar;
    currentEditPart = Part_RealOrAmp;
    isComplexEditing = true;
}

/// @brief 符号取反
void CalculatorLogic::toggleSign() {
    // 如果刚计算完结果，按下 +/- 直接开始一个新的负数输入
    if (isWaitingForNewNum) {
        part1Str = "-";
        part2Str.clear();
        isWaitingForNewNum = false;
        currentMode = Mode_Normal;
        currentEditPart = Part_RealOrAmp;
        isComplexEditing = false;
        return;
    }
    QString* currentEditStr =
        (currentMode == Mode_Normal || currentEditPart == Part_RealOrAmp) ? &part1Str : &part2Str;
    if (currentEditStr->isEmpty() || *currentEditStr == "0")
        *currentEditStr = "-";
    else if (currentEditStr->startsWith("-"))
        currentEditStr->remove(0, 1); // 已经是负数则去掉负号
    else
        currentEditStr->prepend("-"); // 否则在前面加上负号
}

void CalculatorLogic::moveNext() {
    if (currentMode == Mode_Normal) return;
    if (currentEditPart == Part_RealOrAmp) {
        currentEditPart = Part_ImagOrAngle;
    }
    else {  // 在第二部分按右箭头，提交当前复数但不触发运算
        isComplexEditing = false;
        // isWaitingForNewNum = true;
        // currentResult = getCurrentInput();
    }
}

void CalculatorLogic::movePrev() {
    if (currentMode == Mode_Normal) return;
    if (currentEditPart == Part_ImagOrAngle) {
        currentEditPart = Part_RealOrAmp;
    }
}

void CalculatorLogic::backspace() {
    if (isWaitingForNewNum) return;
    if (currentMode != Mode_Normal && !isComplexEditing) return; // 复数已提交，不允许使用退格
    if (currentMode == Mode_Normal) {
        if (!part1Str.isEmpty()) part1Str.chop(1);
    }
    else {
        if (currentEditPart == Part_ImagOrAngle) {
            if (!part2Str.isEmpty()) part2Str.chop(1);
        }
        else {
            if (!part1Str.isEmpty()) {
                part1Str.chop(1);
            }
            else if (part2Str.isEmpty()) {
                currentMode = Mode_Normal; // 退回普通模式
            }
        }
    }
}

void CalculatorLogic::inputOperator(const QString& op) {
    Complex currentOperand = getCurrentInput();
    if (!isWaitingForNewNum)
        calculateStep(currentOperand);
    pendingOperator = op;
    isWaitingForNewNum = true;
    historyFormula = currentResult.toString() + " " + op + " ";
}

void CalculatorLogic::calculateEqual() {
    if (pendingOperator.isEmpty()) return;
    Complex currentOperand = getCurrentInput();
    calculateStep(currentOperand);
    pendingOperator.clear();
    isWaitingForNewNum = true;
    historyFormula = "";
}

void CalculatorLogic::clearAll() {
    currentMode = Mode_Normal;
    currentEditPart = Part_RealOrAmp;
    isComplexEditing = false;
    part1Str.clear();
    part2Str.clear();
    currentResult = Complex(0, 0);
    pendingOperator.clear();
    isWaitingForNewNum = false;
    historyFormula.clear();
}