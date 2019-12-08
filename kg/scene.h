#pragma once
#include <stdio.h>
#include "Objects/pixel.h"
#include <vector>
#include "color.h"

class PixelContainer
{
protected:
    std::vector<Pixel> p;
public:
    std::vector<Pixel> getPixels() {
        return p;
    }

    void setPixels(Pixel s, int number) {
        p[number] = s;
    }

    void add(Pixel s) {
        p.push_back(s);
    }
    int size() {
        return p.size();
    }
};

struct resultPixel
{
    int x, y;
    Color color;

    resultPixel(int x1, int y1, Color color1) 
    {
        x = x1;
        y = y1;
        color = color1;
    }
};
class Scene
{
    std::vector<resultPixel> dots;
public:

    void set_color(Color c, int n) {
        dots[n].color.set_color(c.r, c.g, c.b);
    }

    void add(int x, int y, Color color) {
        dots.push_back(resultPixel(x, y, color));
    }

    std::vector<resultPixel> getDots() {
        return dots;
    }

};