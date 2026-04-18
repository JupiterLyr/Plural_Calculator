#ifndef CALCULATE_H
#define CALCULATE_H

#include "complex.h"

// 输入模式
enum InputMode {
    Mode_Normal,     // 普通输入模式（纯实数）
    Mode_Cartesian,  // 笛卡尔坐标模式 (a+bi)
    Mode_Polar       // 极坐标模式 (A∠θ)
};

// 当前正在编辑的部分
enum EditPart {
    Part_RealOrAmp,  // 正在编辑实部(a)或振幅(A)
    Part_ImagOrAngle // 正在编辑虚部(b)或角度(θ)
};

class CalculatorLogic {
public:
    CalculatorLogic();

    void inputDigit(const QString& digit);   // 输入数字(0-9)或小数点(.)
    void inputSymbolI();                     // 按下 i
    void inputSymbolAngle();                 // 按下 ∠
    void moveNext();                         // 按下 →
    void movePrev();                         // 按下 ←
    void backspace();                        // 按下 DEL
    void inputOperator(const QString& op);   // 按下 +, -, *, /
    void calculateEqual();                   // 按下 =
    void clearAll();                         // 按下 AC

    QString getFormulaDisplay() const;       // 获取显示在 show_formulas 的文本
    QString getResultDisplay() const;        // 获取显示在 show_results 的文本

private:
    InputMode currentMode;      // 当前输入模式
    EditPart currentEditPart;   // 当前编辑位置
    QString part1Str;           // 缓存第一部分(实部/振幅)
    QString part2Str;           // 缓存第二部分(虚部/角度)
    
    Complex currentResult;      // 累计结果
    QString pendingOperator;    // 挂起的运算符
    bool isWaitingForNewNum;    // 运算后是否等待新输入
    QString historyFormula;     // 历史公式字符串

    Complex getCurrentInput() const;    // 获取输入的复数
    void calculateStep(const Complex& nextOperand); 
};

#endif