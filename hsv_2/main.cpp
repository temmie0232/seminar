#include "hsv_filter.hpp"
#include <vector>
#include <string>
#include <iostream>

struct TestCase
{
    std::string filename;
    int actual_apples;
    int actual_oranges;
    int actual_persimmons;
};

void printThresholds(const FruitThresholds &thresholds)
{
    std::cout << "現在の閾値:\n";
    std::cout << "りんご - H:" << thresholds.apple.h_min << "-" << thresholds.apple.h_max
              << " S:" << thresholds.apple.s_min << "-" << thresholds.apple.s_max
              << " V:" << thresholds.apple.v_min << "-" << thresholds.apple.v_max << "\n";
    std::cout << "みかん - H:" << thresholds.orange.h_min << "-" << thresholds.orange.h_max
              << " S:" << thresholds.orange.s_min << "-" << thresholds.orange.s_max
              << " V:" << thresholds.orange.v_min << "-" << thresholds.orange.v_max << "\n";
    std::cout << "へた  - H:" << thresholds.stem.h_min << "-" << thresholds.stem.h_max
              << " S:" << thresholds.stem.s_min << "-" << thresholds.stem.s_max
              << " V:" << thresholds.stem.v_min << "-" << thresholds.stem.v_max << "\n\n";
}

int main()
{
    HSVFilter filter;
    std::vector<TestCase> testCases = {
        {"/home/temmie0232/Project/seminar/images/L11.bmp", 0, 5, 0},
        {"/home/temmie0232/Project/seminar/images/L12.bmp", 0, 5, 0},
        {"/home/temmie0232/Project/seminar/images/L21.bmp", 0, 4, 3},
        {"/home/temmie0232/Project/seminar/images/L22.bmp", 2, 3, 3},
        {"/home/temmie0232/Project/seminar/images/L31.bmp", 2, 4, 2},
        {"/home/temmie0232/Project/seminar/images/L32.bmp", 3, 5, 3},
    };
    // 現在の閾値を出力
    printThresholds(filter.getThresholds());

    // 各テストケースを実行
    for (const auto &test : testCases)
    {
        std::cout << "画像: " << test.filename << "\n";

        auto image = filter.loadBmpImage(test.filename);
        if (image.empty())
            continue;

        auto count = filter.countFruits(image);

        std::cout << "実際の数 - りんご:" << test.actual_apples
                  << " みかん:" << test.actual_oranges
                  << " かき:" << test.actual_persimmons << "\n";

        std::cout << "検出結果 - りんご:" << count.apples
                  << " みかん:" << count.oranges
                  << " かき:" << count.persimmons << "\n";

        std::cout << "　誤差　 - りんご:" << (count.apples - test.actual_apples)
                  << " みかん:" << (count.oranges - test.actual_oranges)
                  << " かき:" << (count.persimmons - test.actual_persimmons) << "\n\n";
    }

    return 0;
}