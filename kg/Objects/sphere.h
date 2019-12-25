#pragma once
#include "material.h"

struct Sphere {
    Vec3f center;
    float radius;
    Material material;
    Image* image;
    float rotation;

    Sphere(const Vec3f& c, const float r, const Material& m, Image* im, float _rotation) : center(c), radius(r), material(m), rotation(_rotation) 
    {
        image = im;
    }

    bool ray_intersect(const Vec3f& orig, const Vec3f& dir, float& t0) const {
        Vec3f L = center - orig;
        float tca = L * dir;
        float d2 = L * L - tca * tca;
        if (d2 > radius* radius) return false;
        float thc = sqrtf(radius * radius - d2);
        t0 = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return false;
        return true;
    }

    Vec3f get_color(const Vec3f& N) const
    {
        int a = rotation * image->width / 64;
        if (a > image->width) a = a % image->width;

        double temp = acos(N.z);
        double temp2 = atan2(N.y, N.x);
        //u,v 
        double v = temp / M_PI;

        if (temp2 < 0) {
            temp2 = temp2 + (2 * M_PI);
        }

        double u = temp2 / (2 * M_PI);


        int x = u * (image->width - 1) + a;
        int y = v * (image->height - 1);

        if (x > image->width - 1) x = x - image->width;
        if (y > image->height - 1) y = image->height - 1;
        
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        return image->colors[y][x];
    }
};