#ifndef COMPLEX_H
#define COMPLEX_H

#include <QString>
#include <QStringList>

class Complex {
public:
    Complex() : real(0), imag(0) {}
    Complex(double re) : real(re), imag(0) {}
    Complex(double re, double im) : real(re), imag(im) {}

    double getRe() const { return real; }
    double getIm() const { return imag; }

    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imag * other.imag,
            real * other.imag + imag * other.real);
    }

    Complex operator/(const Complex& other) const {
        double denom = other.real * other.real + other.imag * other.imag;
        if (std::abs(denom) < 1e-9) return Complex(0, 0); // 防零除处理
        return Complex((real * other.real + imag * other.imag) / denom,
            (imag * other.real - real * other.imag) / denom);
    }

    QString toString() const {
        if (std::abs(imag) < 1e-9)
            return QString::number(real);
        if (std::abs(real) < 1e-9)
            return QString::number(imag) + "i";
        if (imag > 0)
            return QString("%1 + %2i").arg(real).arg(imag);
        return QString("%1 - %2i").arg(real).arg(std::abs(imag));
    }

    static Complex fromString(QString s) {
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

private:
    double real;
    double imag;
};

#endif