#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

// グローバル変数
Mat frame, hsv, mask;
string trackbarWindow = "Trackbars";
string imageWindow = "Images";

// トラックバーのコールバック関数
void onTrackbar(int, void*) {
    // トラックバーの現在値を取得
    int hMin = getTrackbarPos("H Min", trackbarWindow);
    int hMax = getTrackbarPos("H Max", trackbarWindow);
    int sMin = getTrackbarPos("S Min", trackbarWindow);
    int sMax = getTrackbarPos("S Max", trackbarWindow);
    int vMin = getTrackbarPos("V Min", trackbarWindow);
    int vMax = getTrackbarPos("V Max", trackbarWindow);

    // マスクを作成
    Scalar lower(hMin, sMin, vMin);
    Scalar upper(hMax, sMax, vMax);
    inRange(hsv, lower, upper, mask);

    // 結果を作成
    Mat result;
    bitwise_and(frame, frame, result, mask);

    // マスクを3チャンネルに変換
    Mat mask_3ch;
    cvtColor(mask, mask_3ch, COLOR_GRAY2BGR);

    // 画像を縦に連結
    Mat combined;
    vector<Mat> images = {frame, mask_3ch, result};
    hconcat(images, combined);

    // ウィンドウサイズに合わせてリサイズ
    Mat resizedCombined;
    resize(combined, resizedCombined, Size(1800, 500)); // 幅600、高さ400にリサイズ

    // 表示
    imshow(imageWindow, resizedCombined);

    // 現在の値を出力
    cout << "\rH: " << hMin << "-" << hMax << " "
         << "S: " << sMin << "-" << sMax << " "
         << "V: " << vMin << "-" << vMax << "    " << flush;
}

int main(int argc, char** argv) {
    // 画像を読み込む
    if (argc != 2) {
        cout << "使用方法: " << argv[0] << " <画像パス>" << endl;
        return -1;
    }
    frame = imread(argv[1]);
    if (frame.empty()) {
        cout << "画像を読み込めませんでした。" << endl;
        return -1;
    }

    // BGRからHSVに変換
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    // トラックバー用ウィンドウ
    namedWindow(trackbarWindow, WINDOW_NORMAL);
    resizeWindow(trackbarWindow, 300, 600); // トラックバーウィンドウのサイズ

    // トラックバーを作成
    createTrackbar("H Min", trackbarWindow, 0, 180, onTrackbar);
    createTrackbar("H Max", trackbarWindow, 0, 180, onTrackbar);
    createTrackbar("S Min", trackbarWindow, 0, 255, onTrackbar);
    createTrackbar("S Max", trackbarWindow, 0, 255, onTrackbar);
    createTrackbar("V Min", trackbarWindow, 0, 255, onTrackbar);
    createTrackbar("V Max", trackbarWindow, 0, 255, onTrackbar);

    // 初期値を設定
    setTrackbarPos("H Max", trackbarWindow, 180);
    setTrackbarPos("S Max", trackbarWindow, 255);
    setTrackbarPos("V Max", trackbarWindow, 255);

    // 画像表示用ウィンドウ
    namedWindow(imageWindow, WINDOW_NORMAL);
    resizeWindow(imageWindow, 600, 400); // 画像ウィンドウのサイズ

    // 初回表示
    onTrackbar(0, 0);

    // キー入力待ち
    while (true) {
        char key = (char)waitKey(30);
        if (key == 'q') 
            break;
    }

    return 0;
}
