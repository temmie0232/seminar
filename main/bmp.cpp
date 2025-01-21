#include "bmp.h"

BMPProcessor::BMPProcessor() {}

BMPProcessor::~BMPProcessor() {}

// BMPファイルを読み込む
void BMPProcessor::readBMP(const std::string &filename)
{
    // ファイルをバイナリモードで開く
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    // ファイルヘッダーと情報ヘッダーを読み込む
    file.read(reinterpret_cast<char *>(&file_header), sizeof(file_header));
    file.read(reinterpret_cast<char *>(&info_header), sizeof(info_header));

    // フォーマットの検証
    validateBMPFormat();

    // ピクセルデータの開始位置までシーク
    file.seekg(file_header.offset_data, file.beg);

    // BMPファイルは4バイトアラインメントが必要
    // 各行のパディングバイト数を計算
    int padding = (4 - (info_header.width * sizeof(Pixel)) % 4) % 4;

    // ピクセルデータ用の配列を確保
    pixels.resize(info_header.width * info_header.height);
    hsv_pixels.resize(info_header.width * info_header.height);

    // ピクセルデータの読み込み
    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {
            Pixel pixel;
            file.read(reinterpret_cast<char *>(&pixel), sizeof(Pixel));
            setPixel(x, y, pixel);
        }
        // パディングをスキップ
        file.seekg(padding, std::ios::cur);
    }
}

// BMPファイルフォーマットの検証
void BMPProcessor::validateBMPFormat()
{
    // BMPファイルシグネチャの検証
    if (file_header.file_type != 0x4D42)
    {
        throw std::runtime_error("Not a BMP file");
    }

    // ビット深度の検証（24ビットのみサポート）
    if (info_header.bit_count != 24)
    {
        throw std::runtime_error("Only 24-bit BMP files are supported");
    }

    // 圧縮形式の検証（無圧縮のみサポート）
    if (info_header.compression != 0)
    {
        throw std::runtime_error("Compressed BMP files are not supported");
    }
}

// ピクセルデータの取得
Pixel &BMPProcessor::getPixel(int x, int y)
{
    // 座標の範囲チェック
    if (x < 0 || x >= info_header.width || y < 0 || y >= info_header.height)
    {
        throw std::out_of_range("Pixel coordinates out of range");
    }
    return pixels[y * info_header.width + x];
}

// ピクセルデータの設定
void BMPProcessor::setPixel(int x, int y, const Pixel &pixel)
{
    // 座標の範囲チェック
    if (x < 0 || x >= info_header.width || y < 0 || y >= info_header.height)
    {
        throw std::out_of_range("Pixel coordinates out of range");
    }
    pixels[y * info_header.width + x] = pixel;
}

// RGB値をHSV値に変換する
HSVColor BMPProcessor::rgbToHSV(const Pixel &pixel) const
{
    HSVColor hsv;

    // RGB値を0-1の範囲に正規化
    double r = pixel.r / 255.0;
    double g = pixel.g / 255.0;
    double b = pixel.b / 255.0;

    // RGB値の最大値と最小値を求める
    double cmax = std::max({r, g, b});
    double cmin = std::min({r, g, b});
    double diff = cmax - cmin;

    // 色相(H)の計算
    if (diff == 0)
    {
        // RGBが全て同じ値の場合（グレースケール）
        hsv.h = 0;
    }
    else if (cmax == r)
    {
        // Rが最大値の場合
        hsv.h = 60.0 * fmod(((g - b) / diff), 6);
    }
    else if (cmax == g)
    {
        // Gが最大値の場合
        hsv.h = 60.0 * ((b - r) / diff + 2);
    }
    else if (cmax == b)
    {
        // Bが最大値の場合
        hsv.h = 60.0 * ((r - g) / diff + 4);
    }

    // 負の値の場合は360を加えて正の値にする
    if (hsv.h < 0)
    {
        hsv.h += 360;
    }

    // 彩度(S)の計算 (0-255にスケール)
    hsv.s = (cmax == 0) ? 0 : (diff / cmax) * 255;

    // 明度(V)の計算 (0-255にスケール)
    hsv.v = cmax * 255;

    return hsv;
}

// 画像全体をHSV色空間に変換
void BMPProcessor::convertToHSV()
{
    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {
            // 各ピクセルのRGB値を取得してHSVに変換
            Pixel pixel = getPixel(x, y);
            HSVColor hsv = rgbToHSV(pixel);
            hsv_pixels[y * info_header.width + x] = hsv;
        }
    }
}

// HSVピクセルの取得
HSVColor BMPProcessor::getHSVPixel(int x, int y) const
{
    // 座標の範囲チェック
    if (x < 0 || x >= info_header.width || y < 0 || y >= info_header.height)
    {
        throw std::out_of_range("Pixel coordinates out of range");
    }
    return hsv_pixels[y * info_header.width + x];
}

// BMPファイルの書き込み
void BMPProcessor::writeBMP(const std::string &filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Cannot create file: " + filename);
    }

    // ファイルヘッダーの書き込み
    file.write(reinterpret_cast<char *>(&file_header), sizeof(file_header));

    // 情報ヘッダーの書き込み
    file.write(reinterpret_cast<char *>(&info_header), sizeof(info_header));

    // ピクセルデータの書き込み
    // BMPファイルは4バイトアラインメントが必要
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
        // パディングの書き込み
        if (padding > 0)
        {
            file.write(pad.data(), padding);
        }
    }
}