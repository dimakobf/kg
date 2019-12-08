#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "geometry.h"
#include "Objects/sphere.h"
#include "Objects/light.h"

Vec3f space(45.0 / 255, 57.0 / 255, 58.0 / 255);
Vec3f camera(0.0, 0.0, 0.0);
float fov = M_PI / 3.;
float ax = 0, ay = 0, az = 0;

Vec3f reflect(const Vec3f& I, const Vec3f& N) {
    return I - N * 2.f * (I * N);
}

Vec3f refract(const Vec3f& I, const Vec3f& N, const float eta_t, const float eta_i = 1.f) { // Snell's law
    float cosi = -std::max(-1.f, std::min(1.f, I * N));
    if (cosi < 0) return refract(I, -N, eta_i, eta_t); // if the ray comes from the inside the object, swap the air and the media
    float eta = eta_i / eta_t;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vec3f(1, 0, 0) : I * eta + N * (eta * cosi - sqrtf(k)); // k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
}

bool scene_intersect(const Vec3f& orig, const Vec3f& dir, const std::vector<Sphere>& spheres, Vec3f& hit, Vec3f& N, Material& material) {
    float spheres_dist = std::numeric_limits<float>::max();
    for (size_t i = 0; i < spheres.size(); i++) {
        float dist_i;
        if (spheres[i].ray_intersect(orig, dir, dist_i) && dist_i < spheres_dist) {
            spheres_dist = dist_i;
            hit = orig + dir * dist_i;
            N = (hit - spheres[i].center).normalize();
            material = spheres[i].material;
        }
    }

    return spheres_dist < 1000;
}

Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, const std::vector<Sphere>& spheres, const std::vector<Light>& lights, size_t depth = 0) {
    Vec3f point, N;
    Material material;

    if (depth > 4 || !scene_intersect(orig, dir, spheres, point, N, material)) {
        return space; // background color
    }

    Vec3f reflect_dir = reflect(dir, N).normalize();
    Vec3f refract_dir = refract(dir, N, material.refractive_index).normalize();
    Vec3f reflect_orig = reflect_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3; // offset the original point to avoid occlusion by the object itself
    Vec3f refract_orig = refract_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
    Vec3f reflect_color = cast_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);
    Vec3f refract_color = cast_ray(refract_orig, refract_dir, spheres, lights, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (size_t i = 0; i < lights.size(); i++) {
        Vec3f light_dir = (lights[i].position - point).normalize();
        float light_distance = (lights[i].position - point).norm();

        Vec3f shadow_orig = light_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3; // checking if the point lies in the shadow of the lights[i]
        Vec3f shadow_pt, shadow_N;
        Material tmpmaterial;
        if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_N, tmpmaterial) && (shadow_pt - shadow_orig).norm() < light_distance)
            continue;

        diffuse_light_intensity += lights[i].intensity * std::max(0.f, light_dir * N);
        specular_light_intensity += powf(std::max(0.f, -reflect(-light_dir, N) * dir), material.specular_exponent) * lights[i].intensity;
    }
    return material.diffuse_color * diffuse_light_intensity * material.albedo[0] + Vec3f(1., 1., 1.) * specular_light_intensity * material.albedo[1] + reflect_color * material.albedo[2] + refract_color * material.albedo[3];
}

void rotation(float &y, float &z, float angle)
{
    y =  y*cos(angle)+z*sin(angle) ;
    z = -y*sin(angle)+z*cos(angle) ;
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
    Material      ivory(1.0, Vec4f(0.6, 0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3), 50.);
    Material      glass(1.5, Vec4f(0.0, 0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8), 125.);
    Material red_rubber(1.0, Vec4f(0.9, 0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1), 10.);
    Material     mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

    //Material     sun(0.0, Vec4f(0.0, 0.0, 0.0, 0.0), Vec3f(1.0, 1.0, 0.0), 0.);
    Material sun(1.5, Vec4f(0.3, 1.5, 0.2, 0.5), Vec3f(1.0, 1.0, 0.0), 125.);


    std::vector<Sphere> spheres;
    for (auto planet : planets)
    {
        spheres.push_back(Sphere(planet.current_place(), planet.radius, planet.material));
    }
    //spheres.push_back(Sphere(Vec3f(-3, -1, -16), 2, ivory));
    //spheres.push_back(Sphere(Vec3f(0, 0, 0), 2, glass));
    //spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, red_rubber));
    //spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, mirror));

    /*spheres.push_back(Sphere(Vec3f(-7, -7, -30), 1, mercury));
    spheres.push_back(Sphere(Vec3f(-5, -5, -40), 2, venus));
    spheres.push_back(Sphere(Vec3f(0, 20, -50), 4, earth));
    spheres.push_back(Sphere(Vec3f(15, -1, -50), 3, mars));*/

    spheres.push_back(Sphere(Vec3f(0, 0, -120), 2, sun));

    std::vector<Light>  lights;
    lights.push_back(Light(Vec3f(0, 0, -100), 1.9));
    //lights.push_back(Light(Vec3f(30, 50, -25), 1.8));
    lights.push_back(Light(Vec3f(30, 20, 30), 0.1));

    return sphere_render(spheres, lights);

}