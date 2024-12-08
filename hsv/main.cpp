#include <vector>
#include <cmath>
#include <cstdio>
#include <algorithm> 

struct RGB {
    unsigned char r, g, b;
};

struct HSV {
    double h; // 色相 0-180
    double s; // 彩度 0-255
    double v; // 明度 0-255
};

/*
全画面を10として
ミカン - 1.3/10 = 0.13 | 0.13*1024 = 133(直径ピクセル数) | 133/2 = 66.5(半径ピクセル) | (66.5^2)*3.14 = 13,898(平均ピクセル数)
りんご - 1.5/10 = 0.15 | 0.15*1024 = 153(直径ピクセル数) | 153/2 = 76.5(半径ピクセル) | (76.5^2)*3.14 = 18,376(平均ピクセル数)
 へた  - 0.6/10 = 0.06 | 0.06*1024 = 61.4(直径ピクセル数)| 153/2 = 30.7(半径ピクセル) | (76.5^2)*3.14 = 2,959(平均ピクセル数)
 かき  - 1.4/10 = 0.14 | 0.14*1024 = 143(直径ピクセル数) | 143/2 = 71.5(半径ピクセル) | ((76.5^2)*3.14) - 2,959 = 13,093(平均ピクセル数)
*/

// 果物の平均サイズ（ピクセル数）
const int AVERAGE_APPLE_PIXELS = 18376;
const int AVERAGE_ORANGE_PIXELS = 13898;
const int AVERAGE_PERSIMMON_PIXELS = 13093;
const int AVERAGE_STEM_PIXELS = 2959;

// RGB から HSV への変換
HSV rgbToHsv(RGB rgb) {
    HSV hsv;
    double r = rgb.r / 255.0;
    double g = rgb.g / 255.0;
    double b = rgb.b / 255.0;
    
    double max = std::max(r, std::max(g, b));
    double min = std::min(r, std::min(g, b));
    double diff = max - min;
    
    // 色相の計算 (0-180の範囲に調整)
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
    
    // 彩度の計算 (0-255の範囲に調整)
    hsv.s = (max == 0) ? 0 : (diff / max) * 255;
    
    // 明度の計算 (0-255の範囲に調整)
    hsv.v = max * 255;
    
    return hsv;
}

// 色判定関数
bool isAppleColor(HSV hsv) {
    return (hsv.h >= 0 && hsv.h <= 11) &&
           (hsv.s >= 65 && hsv.s <= 237) &&
           (hsv.v >= 30 && hsv.v <= 211);
}


bool isOrangeColor(HSV hsv) {
    return (hsv.h >= 11 && hsv.h <= 19) &&
           (hsv.s >= 192 && hsv.s <= 255) &&
           (hsv.v >= 162 && hsv.v <= 255);
}

bool isStemColor(HSV hsv) {
    return (hsv.h >= 14 && hsv.h <= 41) &&
           (hsv.s >= 78 && hsv.s <= 222) &&
           (hsv.v >= 76 && hsv.v <= 161);
}

struct FruitCount {
    int apples;
    int oranges;
    int persimmons;
};

FruitCount countFruits(const std::vector<std::vector<RGB>>& image) {
    int applePixels = 0;
    int orangeColorPixels = 0;
    int stemPixels = 0;
    
    for(size_t y = 0; y < image.size(); y++) {
        for(size_t x = 0; x < image[0].size(); x++) {
            HSV hsv = rgbToHsv(image[y][x]);
            
            if(isAppleColor(hsv)) {
                applePixels++;
            }
            if(isOrangeColor(hsv)) {
                orangeColorPixels++;
            }
            if(isStemColor(hsv)) {
                stemPixels++;
            }
        }
    }
    
    FruitCount count = {0, 0, 0};  // 初期化を追加
    
    count.persimmons = round((double)stemPixels / AVERAGE_STEM_PIXELS);
    count.apples = round((double)applePixels / AVERAGE_APPLE_PIXELS);
    
    int orangeOnlyPixels = orangeColorPixels - (AVERAGE_PERSIMMON_PIXELS * count.persimmons);
    count.oranges = round((double)orangeOnlyPixels / AVERAGE_ORANGE_PIXELS);
    
    if(count.oranges < 0) count.oranges = 0;
    
    return count;
}

std::vector<std::vector<RGB>> loadBmpImage(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("ファイルを開けませんでした: %s\n", filename);
        return std::vector<std::vector<RGB>>();
    }

    // ファイルヘッダの読み込み
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, fp);

    // 画像の幅と高さを取得
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    // パディングバイトの計算
    int padding = (4 - (width * 3) % 4) % 4;

    // 画像データの読み込み
    std::vector<std::vector<RGB>> image(height, std::vector<RGB>(width));
    
    // BMPは下から上に格納されているため、下から読み込む
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            unsigned char color[3];
            fread(color, sizeof(unsigned char), 3, fp);
            // BMPはBGRの順で格納されているため、RGBに変換
            image[y][x].r = color[2];
            image[y][x].g = color[1];
            image[y][x].b = color[0];
        }
        // パディングをスキップ
        if (padding > 0) {
            fseek(fp, padding, SEEK_CUR);
        }
    }

    fclose(fp);
    return image;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }
    
    // 画像読み込み
    auto image = loadBmpImage(argv[1]);
    
    // 果物をカウント
    FruitCount count = countFruits(image);
    
    // 結果を出力
    printf("りんご: %d個\n", count.apples);
    printf("みかん: %d個\n", count.oranges);
    printf("柿: %d個\n", count.persimmons);
    
    return 0;
}