//17.1.19  - DONE, DO NOT TOUCH

#ifndef EX3_COMPLEX_H
#define EX3_COMPLEX_H

#include <cmath>
#include <string>
#include <iostream>

#define SIZE_ERROR 0.0000000000001

namespace MtmMath {
    class Complex {
        double re, im;
    public:
        Complex(const double &r = 0.0, const double &i = 0.0) : re(r),
                                                                im(i) {} //c'tor
        Complex(const Complex &toCopy) = default;

        Complex(const int &toCopy) {
            re = toCopy;
            im = 0;
        }

        std::string to_string() const {
            return std::to_string(re) + " " + std::to_string(im) + "i";
        }

        Complex &operator=(const Complex &c); // assignment operator
        Complex &operator+=(const Complex &c); // this += c
        Complex &operator-=(const Complex &c); // -= c
        Complex &operator*=(const Complex &c) {
            double real = re * c.re - im * c.im;
            double imaginary = im * c.re + re * c.im;
            re = real;
            im = imaginary;
            return *this;
        }

        Complex operator-() const; // -this
        bool operator==(const Complex &c) const; //  this == c
        bool operator!=(const Complex &c) const;

    };

    Complex operator+(const Complex &a, const Complex &b); // a+b
    Complex operator-(const Complex &a, const Complex &b); // a-b
    Complex operator*(const Complex &a, const Complex &b);


    //Implementations
    Complex &Complex::operator=(const Complex &c) {
        if (this == &c) {
            return *this;
        }
        re = c.re;
        im = c.im;
        return *this;
    }

    Complex &Complex::operator+=(const Complex &c) {
        re += c.re;
        im += c.im;
        return *this;
    }

    Complex &Complex::operator-=(const Complex &c) {
        return this->operator+=(-c); // or *this += -c
    }

    Complex Complex::operator-() const {
        return Complex(-re, -im);
    }

    bool Complex::operator==(const Complex &c) const {
        return (fabs(c.re - re) < SIZE_ERROR && fabs(c.im - im) < SIZE_ERROR);
    }

    Complex operator*(const Complex &a, const Complex &b) {
        Complex c = a;
        return c *= b;
    }

    bool Complex::operator!=(const Complex &c) const {
        return !(operator==(c));
    }

    Complex operator+(const Complex &a, const Complex &b) {
        Complex c = a;
        return c += b;
    }

    Complex operator-(const Complex &a, const Complex &b) {
        return a + (-b);
    }


}
#endif //EX3_COMPLEX_H
