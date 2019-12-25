#pragma once
#include <stdio.h>
#include <vector>
#include "trace.h"
#include "Objects/planet.h"
#include "Objects/light.h"

void rotation(float& y, float& z, float angle)
{
    y = y * cos(angle) + z * sin(angle);
    z = -y * sin(angle) + z * cos(angle);
}

Vec3f rotation(const Vec3f& source)
{
    Vec3f coords = source;
    rotation(coords.y, coords.z, ax);
    rotation(coords.x, coords.z, ay);
    rotation(coords.x, coords.y, az);

    return coords;
}

void thread_for(const std::vector<Sphere>& spheres, const std::vector<Light>& lights, std::vector<Vec3f>& framebuffer,
    int height, int width, int a, int b)
{

    for (size_t j = a; j < b; j++) { // actual rendering loop
        for (size_t i = 0; i < width; i++) {
            float dir_x = (i + 0.5) - width / 2.;
            float dir_y = (j + 0.5) - height / 2.;
            float dir_z = -height / (2. * tan(fov / 2.));
            framebuffer[i + j * width] = cast_ray(camera, rotation(Vec3f(dir_x, dir_y, dir_z)).normalize(), spheres, lights);
        }
    }
}

std::vector<Vec3f> sphere_render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
    const int   width = 1000;
    const int   height = 1000;
    const float fov = M_PI / 3.;
    std::vector<Vec3f> framebuffer(width * height);

    int tcount = 10;
    int a = 0;
    int b = height / tcount;

    boost::thread_group threads;
    for (int i = 0; i < tcount; ++i) {
        threads.create_thread(boost::bind(&thread_for, boost::cref(spheres), boost::cref(lights), boost::ref(framebuffer), height, width, a, b));
        a = b;
        b = height / tcount + b;
    }
    threads.join_all();

    return framebuffer;
}

std::vector<Vec3f> dorender() {

    std::vector<Sphere> spheres;
    for (auto &planet : planets)
    {
        spheres.push_back(planet.sphere);
    }

    for (auto &m : moons)
    {
        spheres.push_back(m.sphere);
    }

    std::vector<Light>  lights;
    lights.push_back(Light(Vec3f(0, 0, -100), 3.0));

    return sphere_render(spheres, lights);

}