#pragma once
#include "../geometry.h"

typedef Vec4f Albedo;
typedef Vec3f Color3;

Albedo MercuryA(0.14, 0.3, 0.1, 0.1);
Color3 MercuryC(128.0/255, 128.0/255, 128.0/255);

Albedo VenusA(0.69, 0.3, 0.1, 0.1);
Color3 VenusC(238.0 / 255, 232.0 / 255, 170.0 / 255);

Albedo EarthA(0.43, 0.3, 0.1, 0);
Color3 EarthC(0.0, 0.0, 1.0);

Albedo MarsA(0.17, 0.3, 0.1, 0);
Color3 MarsC(89.0 / 255, 0.0, 0.0);

Albedo JupiterA(0.54, 0, 0, 0);

Albedo SaturnA(0.50, 0, 0, 0);

Albedo UranusA(0.49, 0, 0, 0);

Albedo NeptuneA(0.44, 0, 0, 0);

struct Material {
    Material(const float r, const Vec4f& a, const Vec3f& color, const float spec) : refractive_index(r), albedo(a), diffuse_color(color), specular_exponent(spec) {}
    Material() : refractive_index(1), albedo(1, 0, 0, 0), diffuse_color(), specular_exponent() {}
    float refractive_index;
    Vec4f albedo;
    Vec3f diffuse_color;
    float specular_exponent;
};