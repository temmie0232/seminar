
// main.cpp
#include "bmp.h"
#include <iostream>

int main(int argc, char *argv[])
{
    // コマンドライン引数のチェック
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " input_filepath output_filepath" << std::endl;
        return 1;
    }

    try
    {
        BMPProcessor processor;

        // BMPファイルの読み込み
        processor.readBMP(argv[1]);

        // RGB値をHSV色空間に変換
        processor.convertToHSV();

        // この後に果物検出処理を追加予定

        // 処理結果を保存
        processor.writeBMP(argv[2]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}