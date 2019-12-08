#pragma once
#include <stdio.h>

class Color {
public:
    int r, g, b;

    Color() {
        r = 0;
        g = 0;
        b = 0;
    }

    Color get_color() {
        return *this;
    }

    void set_color(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void mult(double i) {
        this->r *= i;
        this->g *= i;
        this->b *= i;
    }

    void plus(Color a) {
        this->r += a.r;
        this->g += a.g;
        this->b += a.b;
    }
};