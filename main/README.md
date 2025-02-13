==============

プログラムの実行方法
./main input_filepath output_filepath

# 1. 画像の読み込みと前処理
- BMPファイルを読み込む
- RGB形式の二次元配列にデータを格納
- 各ピクセル(1px)をRGBからHSV形式に変換

# 2. 果物の検出とバウンディングボックスの作成
## 2.1. 色範囲の事前定義
```cpp
// HSVの閾値を設定
りんご : thresholds.apple = {0, 10, 65, 237, 30, 211};    // H, S, V の最小・最大値
みかんと柿 : thresholds.orange = {11, 19, 192, 255, 162, 255};
柿のへた : thresholds.stem = {14, 41, 78, 222, 76, 161};
```

## 2.2. 検出処理
- 画像を上から1行ずつスキャン
- 果物の色判定関数を使用
  - `isAppleColor`: りんご判定（84px）
  - `isOrangeColor`: みかん/かき判定（77px/80px）
    - みかんと柿は色が近いため同じ判定基準を使用（柿の色範囲を採用）
- 半径
  - りんご : 84px
  - みかん : 77px
  - かき : 80px
- パディング: 15px (実際の値より15px大きめに重複検知防止用の円を描く)

## 2.3. 果物の特定と重複防止
1. 果物の頂点（最上部の点）を検出
2. 頂点の色から果物の種類を判別
3. 中心点を計算（果物ごとに事前定義された半径を使用し、頂点から真下に半径分だけ下の位置を中心点とする）
4. 中心点をもとに円を描画(黒色で)
5. 円の範囲内の配列に果物の種類のフラグを設定（重複検出防止）
6. 判定された果物の色で四角のバウンディングボックスを描画
6. フラグが未設定の領域のみを次の検出対象とする

## 2.4. 最終出力
- 検出された果物の位置に応じて色付きの四角形（バウンディングボックス）を描画
  - りんご: 緑色の枠
  - みかん: 赤色の枠
  - かき: 青色の枠
==============


 
# ファイルの入出力処理の作成

1. `bmp.h`
- BMPファイルの構造体定義
- BMPProcessor クラスの宣言
- ピクセル構造体の定義

1. `bmp.cpp`
- BMPProcessor クラスの実装
- ファイル入出力処理
- ピクセル操作メソッド
- フォーマット検証

1. `main.cpp`
- メイン関数
- コマンドライン引数の処理
- 基本的なエラーハンドリング

コンパイル方法
```
g++ -Wall -Wextra -O2 -std=c++17 -c main.cpp -o main.o
g++ -Wall -Wextra -O2 -std=c++17 -c bmp.cpp -o bmp.o
g++ main.o bmp.o -o main
```


改善点  
 - 連結成分分析
 - 1pxずつ → スキャンライン