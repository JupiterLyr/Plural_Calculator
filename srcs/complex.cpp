#include <cmath>
#include "complex.h"

Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::operator*(const Complex& other) const {
    return Complex(
        real * other.real - imag * other.imag,
        real * other.imag + imag * other.real
    );
}

Complex Complex::operator/(const Complex& other) const {
    // (a+bi) / (c+di) = [(ac+bd) + (bc-ad)i] / (c^2+d^2)
    double denom = other.real * other.real + other.imag * other.imag;  // 分母
    if (denom < 1e-15) {  // 防零除保护
        return Complex(0, 0);
    }
    return Complex(
        (real * other.real + imag * other.imag) / denom,
        (imag * other.real - real * other.imag) / denom
    );
}

QString Complex::toCartesianString() const {
    if (std::abs(imag) < 1e-9)
        return QString::number(real);
    if (std::abs(real) < 1e-9)
        return QString::number(imag) + "i";
    if (imag > 0)
        return QString("%1 + %2i").arg(real).arg(imag);
    return QString("%1 - %2i").arg(real).arg(std::abs(imag));
}

QString Complex::toPolarString() const {
    if (std::abs(real) < 1e-9 && std::abs(imag) < 1e-9)
        return "0";
    // 计算振幅 A 和角度 θ (角度制)
    double amp = std::hypot(real, imag);
    double theta = std::atan2(imag, real) * 180.0 / M_PI;
    return QString("%1 ∠ %2°").arg(amp).arg(theta);
}

Complex Complex::fromString(QString s) {
    s.remove('(').remove(')').remove(' ');
    if (s.contains("∠")) {
        QStringList parts = s.split("∠");
        double r = parts[0].toDouble();
        double theta = parts[1].toDouble() * M_PI / 180.0; // 假设输入是角度
        return { r * cos(theta), r * sin(theta) };
    }
    if (s.contains('i')) {
        s.remove('i');
        // 处理形如 "3+4", "3-4", "-3-4" 的情况
        // 寻找最后一个出现的 + 或 - (排除第一个位置)
        int signPos = qMax(s.lastIndexOf('+'), s.lastIndexOf('-'));
        if (signPos <= 0)
            return { 0, s.toDouble() }; // 只有虚部
        double re = s.left(signPos).toDouble();
        double im = s.mid(signPos).toDouble();
        return { re, im };
    }
    return { s.toDouble(), 0 }; // 纯实数
}