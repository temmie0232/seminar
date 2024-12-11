#include "hsv_filter.hpp"
#include <algorithm>
#include <cstdio>

HSVFilter::HSVFilter() {
    // 初期閾値の設定
    thresholds.apple = {0, 10, 65, 237, 30, 211};   // (11->10)
    thresholds.orange = {11, 19, 192, 255, 162, 255};
    thresholds.stem = {14, 41, 78, 222, 76, 161};
}

HSV HSVFilter::rgbToHsv(RGB rgb) {
    HSV hsv;
    double r = rgb.r / 255.0;
    double g = rgb.g / 255.0;
    double b = rgb.b / 255.0;
    
    double max = std::max(r, std::max(g, b));
    double min = std::min(r, std::min(g, b));
    double diff = max - min;
    
    if(diff == 0) {
        hsv.h = 0;
    } else if(max == r) {
        hsv.h = 30 * fmod(((g - b) / diff), 6);
    } else if(max == g) {
        hsv.h = 30 * ((b - r) / diff + 2);
    } else {
        hsv.h = 30 * ((r - g) / diff + 4);
    }
    if(hsv.h < 0) hsv.h += 180;
    
    hsv.s = (max == 0) ? 0 : (diff / max) * 255;
    hsv.v = max * 255;
    
    return hsv;
}

bool HSVFilter::isAppleColor(HSV hsv) {
    return (hsv.h >= thresholds.apple.h_min && hsv.h <= thresholds.apple.h_max) &&
           (hsv.s >= thresholds.apple.s_min && hsv.s <= thresholds.apple.s_max) &&
           (hsv.v >= thresholds.apple.v_min && hsv.v <= thresholds.apple.v_max);
}

bool HSVFilter::isOrangeColor(HSV hsv) {
    return (hsv.h >= thresholds.orange.h_min && hsv.h <= thresholds.orange.h_max) &&
           (hsv.s >= thresholds.orange.s_min && hsv.s <= thresholds.orange.s_max) &&
           (hsv.v >= thresholds.orange.v_min && hsv.v <= thresholds.orange.v_max);
}

bool HSVFilter::isStemColor(HSV hsv) {
    return (hsv.h >= thresholds.stem.h_min && hsv.h <= thresholds.stem.h_max) &&
           (hsv.s >= thresholds.stem.s_min && hsv.s <= thresholds.stem.s_max) &&
           (hsv.v >= thresholds.stem.v_min && hsv.v <= thresholds.stem.v_max);
}

FruitCount HSVFilter::countFruits(const std::vector<std::vector<RGB>>& image) {
    int applePixels = 0;
    int orangeColorPixels = 0;
    int stemPixels = 0;
    
    for(size_t y = 0; y < image.size(); y++) {
        for(size_t x = 0; x < image[0].size(); x++) {
            HSV hsv = rgbToHsv(image[y][x]);
            
            if(isAppleColor(hsv)) applePixels++;
            if(isOrangeColor(hsv)) orangeColorPixels++;
            if(isStemColor(hsv)) stemPixels++;
        }
    }
    
    FruitCount count = {0, 0, 0};
    
    count.persimmons = round((double)stemPixels / AVERAGE_STEM_PIXELS);
    count.apples = round((double)applePixels / AVERAGE_APPLE_PIXELS);
    
    int orangeOnlyPixels = orangeColorPixels - (AVERAGE_PERSIMMON_PIXELS * count.persimmons);
    count.oranges = round((double)orangeOnlyPixels / AVERAGE_ORANGE_PIXELS);
    
    if(count.oranges < 0) count.oranges = 0;
    
    return count;
}

std::vector<std::vector<RGB>> HSVFilter::loadBmpImage(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) {
        printf("ファイルを開けませんでした: %s\n", filename.c_str());
        return std::vector<std::vector<RGB>>();
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, fp);
    
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int padding = (4 - (width * 3) % 4) % 4;

    std::vector<std::vector<RGB>> image(height, std::vector<RGB>(width));
    
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            unsigned char color[3];
            fread(color, sizeof(unsigned char), 3, fp);
            image[y][x].r = color[2];
            image[y][x].g = color[1];
            image[y][x].b = color[0];
        }
        if (padding > 0) {
            fseek(fp, padding, SEEK_CUR);
        }
    }

    fclose(fp);
    return image;
}