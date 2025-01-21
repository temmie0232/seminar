#include "bmp.h"

BMPProcessor::BMPProcessor() {}
BMPProcessor::~BMPProcessor() {}

void BMPProcessor::readBMP(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("ファイルが開けません: " + filename);
    }

    // ファイルヘッダーの読み込み (14B)
    file.read(reinterpret_cast<char *>(&file_header), sizeof(file_header));

    // 情報ヘッダーの読み込み (40B)
    file.read(reinterpret_cast<char *>(&info_header), sizeof(info_header));

    // ファイルのフォーマットの検証
    validateBMPFormat();

    // ピクセルデータの開始位置までシーク
    file.seekg(file_header.offset_data, file.beg);

    // ピクセルデータの読み込み
    // BMPファイルは4バイトアライメントが必要
    // 各行のパディングバイト数を計算
    int padding = (4 - (info_header.width * sizeof(Pixel)) % 4) % 4;

    // ピクセルデータ用のメモリを確保
    pixels.resize(info_header.width * info_header.height);

    // 画像データを1行ずつ読み込む
    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {

            Pixel pixel;
            file.read(reinterpret_cast<char *>(&pixel), sizeof(Pixel));
            setPixel(x, y, pixel);
        }
        // 行末のパディングをスキップ
        file.seekg(padding, std::ios::cur);
    }
}

void BMPProcessor::writeBMP(const std::string &filename)
{

    // バイナリモードでファイルを作成
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("ファイル作成に失敗: " + filename);
    }

    // ファイルヘッダーの書き込み
    file.write(reinterpret_cast<char *>(&file_header), sizeof(file_header));

    // 情報ヘッダーの書き込み
    file.write(reinterpret_cast<char *>(&info_header), sizeof(info_header));

    // パディング用のデータ準備
    int padding = (4 - (info_header.width * sizeof(Pixel)) % 4) % 4;
    std::vector<char> pad(padding, 0);

    // ピクセルデータを1行ずつ書き込む
    for (int y = 0; y < info_header.height; y++)
    {

        for (int x = 0; x < info_header.width; x++)
        {
            Pixel pixel = getPixel(x, y);
            file.write(reinterpret_cast<char *>(&pixel), sizeof(Pixel));
        }

        // 行末のパディングの書き込み
        if (padding > 0)
        {
            file.write(pad.data(), padding);
        }
    }
}

// ピクセルデータへのアクセス（読み取り）
Pixel &BMPProcessor::getPixel(int x, int y)
{

    // 範囲外アクセスのチェック
    if (x < 0 || x >= info_header.width || y < 0 || y >= info_header.height)
    {
        throw std::out_of_range("ピクセル座標が範囲外になりました。");
    }

    // ピクセルデータは下から上に格納されているため、yの位置を反転
    return pixels[y * info_header.width + x];
}

// ピクセルデータの設定
void BMPProcessor::setPixel(int x, int y, const Pixel &pixel)
{

    // 範囲外アクセスのチェック
    if (x < 0 || x >= info_header.width || y < 0 || y >= info_header.height)
    {
        throw std::out_of_range("ピクセル座標が範囲外になりました。");
    }

    // ピクセルデータを設定
    pixels[y * info_header.width + x] = pixel;
}

// BMPファイルフォーマットの検証
void BMPProcessor::validateBMPFormat()
{
    // ファイルタイプが"BM"かチェック
    if (file_header.file_type != 0x4D42)
    {
        throw std::runtime_error("BMPファイルじゃないですね。");
    }
    // 24ビットカラーかチェック
    if (info_header.bit_count != 24)
    {
        throw std::runtime_error("24bitBMPだけサポートしてるよ。");
    }
    // 無圧縮形式かチェック
    if (info_header.compression != 0)
    {
        throw std::runtime_error("圧縮されたBMPファイルは使えないよ。");
    }
}