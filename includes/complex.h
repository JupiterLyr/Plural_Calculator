#ifndef COMPLEX_H
#define COMPLEX_H

class Complex {
public:
    Complex() : real(0), imag(0) {}
    Complex(double re) : real(re), imag(0) {}
    Complex(double re, double im): real(re), imag(im) {}

private:
    double real;
    double imag;
};

#endif