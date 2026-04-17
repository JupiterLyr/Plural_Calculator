#include "calculate.h"

Calculator::Calculator() { clear(); }

void Calculator::clear() {
    formula = "";
    resultStr = "0";
    currentToken = "";
    accumulator = Complex(0, 0);
    pendingOp = "";
    hasAccumulator = false;
    inBracket = false;
}

void Calculator::inputToken(QString t) {
    if (t == "(") {
        inBracket = true;
        currentToken = "(";
        formula += t;
    } 
    else if (t == ")") {
        if (!inBracket) return;
        inBracket = false;
        currentToken += ")";
        formula += ")";
        
        // 核心逻辑：括号闭合时，如果是链式计算，立即算出结果
        Complex currentCpx = Complex::fromString(currentToken);
        if (!hasAccumulator) {
            accumulator = currentCpx;
            hasAccumulator = true;
        } else if (!pendingOp.isEmpty()) {
            performCalc(); // 自动执行上一步运算
        }
        resultStr = accumulator.toString();
    }
    else if (t == "+" || t == "-" || t == "*" || t == "/") {
        if (inBracket) {
            currentToken += t; // 括号内作为正负号或虚部连接符
        } else {
            pendingOp = t;     // 括号外作为运算符
        }
        formula += t;
    }
    else if (t == "=") {
        resultStr = accumulator.toString();
        // 可以在这里重置 formula 或做其他结算
    }
    else if (t == "a+bi") {
        if (inBracket) currentToken += "i";
        formula += "i";
    }
    else { // 数字、小数点、pi等
        if (inBracket) currentToken += t;
        formula += t;
    }
}

void Calculator::performCalc() {
    Complex nextVal = Complex::fromString(currentToken);
    if (pendingOp == "+") accumulator = accumulator + nextVal;
    else if (pendingOp == "-") accumulator = accumulator - nextVal;
    else if (pendingOp == "*") accumulator = accumulator * nextVal;
    else if (pendingOp == "/") accumulator = accumulator / nextVal;
}

void Calculator::backspace() {
    if (!formula.isEmpty()) formula.chop(1);
    if (inBracket && !currentToken.isEmpty()) currentToken.chop(1);
}