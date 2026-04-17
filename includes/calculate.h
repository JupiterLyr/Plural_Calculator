#ifndef CALCULATE_H
#define CALCULATE_H

#include "complex.h"

class Calculator {
public:
    Calculator();

    void inputToken(QString token); // 输入数字、运算符、括号等
    void clear();                   // AC
    void backspace();               // ←

    QString getFormula() const { return formula; }
    QString getDisplayResult() const { return resultStr; }
    Complex getRawResult() const { return accumulator; }

private:
    QString formula;      // 对应 show_formulas
    QString resultStr;    // 对应 show_result
    QString currentToken; // 当前括号内正在输入的字符串

    Complex accumulator;  // 累加的结果
    QString pendingOp;    // 待处理的运算符 (+, -, *, /)
    bool hasAccumulator;  // 是否已有第一个运算数
    bool inBracket;       // 是否正在括号内输入

    void performCalc();   // 执行实际计算
};

#endif