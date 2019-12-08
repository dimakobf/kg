#pragma once
#include <stdio.h>
#include "../coords.h"
#include "../color.h"

class Pixel
{
private:
    Color color;
    Coords coordinates;
    float density;
public:
    Pixel() {
        color.set_color(1, 1, 1);
        coordinates.set_coords(1, 1, 1);
        density = 0;
    }

    void set—oords(int x, int y, int z) {
        coordinates.set_coords(x, y, z);
    }

    void set—olor(float r, float g, float b) {
        color.set_color(r, g, b);
    }

    void setDensity(float d) {
        density = d;
    }

    Color getColor() {
        return color;
    }
    Coords getCoordinates() {
        return coordinates;
    }

    float getDensity() {
        return density;
    }
};