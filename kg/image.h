#pragma once
#include <stdint.h>
#include <vector>
#include "geometry.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Image
{
    int width, height;
    std::vector <std::vector<Vec3f>> colors;
};

Image get_image(const char* str) {
    int bpp;
    Image image;

    uint8_t* rgb_image = stbi_load(str, &image.width, &image.height, &bpp, 3);

    for (int j = 0; j < image.height; j++) {
        std::vector<Vec3f> temp;
        for (int i = 0; i < image.width; i++) {

            unsigned char* pixelOffset = rgb_image + (i + image.height * j) * bpp;
            float r = (float)pixelOffset[0] / 255;
            float g = (float)pixelOffset[1] / 255;
            float b = (float)pixelOffset[2] / 255;

            temp.push_back(Vec3f(r, g, b));
        }
        image.colors.push_back(temp);
    }

    stbi_image_free(rgb_image);

    return image;
}