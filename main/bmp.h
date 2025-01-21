#ifndef BMP_H
#define BMP_H

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#pragma pack(push, 1) // 構造体のアラインメントを1バイトに設定

// BMPファイルヘッダー
struct BMPFileHeader
{
    uint16_t file_type{0x4D42}; // "BM" (0x4D42)
    uint32_t file_size{0};      // ファイルサイズ
    uint16_t reserved1{0};      // 予約領域1
    uint16_t reserved2{0};      // 予約領域2
    uint32_t offset_data{0};    // ピクセルデータ開始位置までのオフセット
};

// BMPファイル情報ヘッダー
struct BMPInfoHeader
{
    uint32_t size{0};              // 情報ヘッダーのサイズ
    int32_t width{0};              // 画像の幅 (px)
    int32_t height{0};             // 画像の高さ (px)
    uint16_t planes{1};            // プレーン数（常に1）
    uint16_t bit_count{0};         // 1ピクセルあたりのビット数
    uint32_t compression{0};       // 圧縮形式
    uint32_t size_image{0};        // 画像データのサイズ
    int32_t x_pixels_per_meter{0}; // 水平解像度
    int32_t y_pixels_per_meter{0}; // 垂直解像度
    uint32_t colors_used{0};       // 使用する色数
    uint32_t colors_important{0};  // 重要な色数
};

#pragma pack(pop) // アラインメント設定を元に戻す

// ピクセルの色情報を保持する構造体
struct Pixel
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

// BMPファイル処理のためのクラス
class BMPProcessor
{

public:
    BMPProcessor();  // コンストラクタ
    ~BMPProcessor(); // デストラクタ

    // BMPファイルを読み込む
    void readBMP(const std::string &filename);

    // BMPファイルを保存する
    void writeBMP(const std::string &filename);

    // 画像の幅を取得
    int32_t getWidth() const { return info_header.width; }

    // 画像の高さを取得
    int32_t getHeight() const { return info_header.height; }

    // 指定座標のピクセルへの参照を取得
    Pixel &getPixel(int x, int y);

    // 指定座標にピクセルを設定
    void setPixel(int x, int y, const Pixel &pixel);

private:
    BMPFileHeader file_header; // BMPファイルヘッダー
    BMPInfoHeader info_header; // BMPファイル情報ヘッダー
    std::vector<Pixel> pixels; // ピクセルデータを格納する動的配列

    // BMPファイルフォーマットの検証を行う
    void validateBMPFormat();
};

#endif