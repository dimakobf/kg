#pragma once
#include <stdio.h>
#include <math.h>

class Coords {
public:
    float x, y, z;

    Coords() {
        this->x = 0;
        this->y = 0;
        this->z = 0.0;
    }

    Coords get_coords() {
        return *this;
    }

    void set_coords(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void dif(Coords a, Coords b) {
        x = a.x - b.x;
        y = a.y - b.y;
        z = a.z - b.z;
    }

    void del(Coords d) {
        if (d.x == 0)
            x = 0;
        else
            x = 1 / d.x;
        if (d.y == 0)
            y = 0;
        else
            y = 1 / d.y;
        if (d.z == 0)
            z = 0;
        else
            z = -1 / d.z;
    }

    Coords plus(Coords a, Coords b) {
        Coords temp;
        temp.x = a.x + b.x;
        temp.y = a.y + b.y;
        temp.z = a.z + b.z;
        return temp;
    }

    void mult(float k) {
        this->x = this->x * k;
        this->y = this->y * k;
        this->z = this->z * k;
    }

    float dot(Coords a, Coords b) {
        float x = a.x * b.x;
        float y = a.y * b.y;
        float z = a.z * b.z;
        return x + y + z;
    }

    float len() {
        return sqrt(x * x + y * y + z * z);
    }

    Coords norm() {
        float l = this->len();
        Coords o;
        o.x = this->x / l;
        o.y = this->y / l;
        o.z = this->z / l;
        return o;
    }

    float angle(Coords a, Coords b) {
        float s = a.scalar(a, b);
        return s / (a.len() * b.len());
    }

    float scalar(Coords a, Coords b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    void reverseY() {
        y = -y;
    }

};