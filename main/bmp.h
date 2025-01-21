#ifndef BMP_H
#define BMP_H

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>

// BMPファイルのヘッダー構造体を1バイトアラインメントで定義
#pragma pack(push, 1)

// BMPファイルヘッダー (14 bytes)
struct BMPFileHeader
{
    uint16_t file_type{0x4D42}; // BMPファイルのマジックナンバー "BM" (0x4D42)
    uint32_t file_size{0};      // ファイル全体のサイズ（バイト単位）
    uint16_t reserved1{0};      // 予約領域1（未使用）
    uint16_t reserved2{0};      // 予約領域2（未使用）
    uint32_t offset_data{0};    // ファイル先頭から画像データまでのオフセット
};

// BMPファイル情報ヘッダー (40 bytes)
struct BMPInfoHeader
{
    uint32_t size{0};              // 情報ヘッダーのサイズ（40バイト）
    int32_t width{0};              // 画像の幅（ピクセル単位）
    int32_t height{0};             // 画像の高さ（ピクセル単位）
    uint16_t planes{1};            // プレーン数（常に1）
    uint16_t bit_count{0};         // 1ピクセルあたりのビット数（本プログラムでは24ビットのみ対応）
    uint32_t compression{0};       // 圧縮形式（0=無圧縮）
    uint32_t size_image{0};        // 画像データ部のサイズ
    int32_t x_pixels_per_meter{0}; // 水平解像度
    int32_t y_pixels_per_meter{0}; // 垂直解像度
    uint32_t colors_used{0};       // カラーパレット数
    uint32_t colors_important{0};  // 重要なカラーパレット数
};

#pragma pack(pop) // アラインメント設定を元に戻す

// ピクセルのRGB値を保持する構造体
struct Pixel
{
    uint8_t b; // 青成分 (0-255)
    uint8_t g; // 緑成分 (0-255)
    uint8_t r; // 赤成分 (0-255)
};

// HSV色空間の値を保持する構造体
struct HSVColor
{
    double h; // 色相 (0-360度)
    double s; // 彩度 (0-255)
    double v; // 明度 (0-255)
};

// BMPファイルの処理を行うクラス
class BMPProcessor
{
public:
    BMPProcessor();
    ~BMPProcessor();

    // BMPファイルの読み込み・書き込み
    void readBMP(const std::string &filename);  // BMPファイルを読み込む
    void writeBMP(const std::string &filename); // BMPファイルを保存する

    // 画像サイズの取得
    int32_t getWidth() const { return info_header.width; }
    int32_t getHeight() const { return info_header.height; }

    // ピクセル操作
    Pixel &getPixel(int x, int y);                   // 指定座標のRGBピクセルを取得
    void setPixel(int x, int y, const Pixel &pixel); // 指定座標にRGBピクセルを設定

    // HSV関連の操作
    HSVColor getHSVPixel(int x, int y) const; // 指定座標のHSV値を取得
    void convertToHSV();                      // 画像全体をHSV色空間に変換

private:
    BMPFileHeader file_header;        // BMPファイルヘッダー
    BMPInfoHeader info_header;        // BMPファイル情報ヘッダー
    std::vector<Pixel> pixels;        // RGBピクセルデータ配列
    std::vector<HSVColor> hsv_pixels; // HSV変換後のデータ配列

    void validateBMPFormat();                    // BMPファイルフォーマットの検証
    HSVColor rgbToHSV(const Pixel &pixel) const; // RGB→HSV変換
};

#endif // BMP_H
