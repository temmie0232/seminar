#pragma once
#include <vector>
#include <string>
#include <cmath>

struct RGB {
    unsigned char r, g, b;
};

struct HSV {
    double h; // 色相 0-180
    double s; // 彩度 0-255
    double v; // 明度 0-255
};

struct FruitCount {
    int apples;
    int oranges;
    int persimmons;
};

struct FruitThresholds {
    struct {
        double h_min, h_max;
        double s_min, s_max;
        double v_min, v_max;
    } apple, orange, stem;
};

class HSVFilter {
public:
    HSVFilter();
    FruitCount countFruits(const std::vector<std::vector<RGB>>& image);
    std::vector<std::vector<RGB>> loadBmpImage(const std::string& filename);
    const FruitThresholds& getThresholds() const { return thresholds; }

private:
    static const int AVERAGE_APPLE_PIXELS = 18376;
    static const int AVERAGE_ORANGE_PIXELS = 13898;
    static const int AVERAGE_PERSIMMON_PIXELS = 13093;
    static const int AVERAGE_STEM_PIXELS = 2959;

    FruitThresholds thresholds;
    HSV rgbToHsv(RGB rgb);
    bool isAppleColor(HSV hsv);
    bool isOrangeColor(HSV hsv);
    bool isStemColor(HSV hsv);
};