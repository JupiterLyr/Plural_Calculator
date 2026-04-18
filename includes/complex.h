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

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const Complex& other) const;
    QString toCartesianString() const;
    QString toPolarString() const;
    static Complex fromString(QString s);

private:
    double real;
    double imag;
};

#endif