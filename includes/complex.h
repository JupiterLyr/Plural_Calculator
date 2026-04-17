#ifndef COMPLEX_H
#define COMPLEX_H

class Complex {
public:
    Complex(double r) : real(r), imag(0) {}
    Complex(double r, double im): real(r), imag(im) {}

private:
    double real;
    double imag;
};

#endif