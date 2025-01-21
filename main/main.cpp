#include "bmp.h"
#include <iostream>

int main(int argc, char *argv[])
{
    // コマンドライン引数の数をチェック
    if (argc != 3)
    {
        std::cerr << "使い方: " << argv[0] << " <入力画像ファイルパス> <出力画像ファイルパス>" << std::endl;
        return 1;
    }

    try
    {
        // BMPプロセッサーのインスタンスを作成
        BMPProcessor processor;

        // 入力BMPファイルを読み込む
        processor.readBMP(argv[1]);

        // ここに画像処理のコードを追加

        // 処理結果をファイルに保存
        processor.writeBMP(argv[2]);
    }
    catch (const std::exception &e)
    {
        // エラーメッセージを表示
        std::cerr << "エラー: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}