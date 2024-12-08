# OpenCVとC++の関数

## 画像入出力・操作関連

### imread()
- **形式**: `imread(const String& filename, int flags = IMREAD_COLOR) -> Mat`
- **引数**:
  - `filename`: 画像ファイルのパス
  - `flags`: 読み込みフラグ（デフォルトはカラー）
- **戻り値**: 読み込んだ画像（Mat形式）
- **処理**: 画像ファイルをメモリに読み込む

### cvtColor()
- **形式**: `cvtColor(InputArray src, OutputArray dst, int code, int dstCn = 0)`
- **引数**:
  - `src`: 入力画像
  - `dst`: 出力画像
  - `code`: 変換コード
  - `dstCn`: 出力チャンネル数
- **戻り値**: なし（出力は第2引数で受け取る）
- **処理**: 色空間を変換（例：BGR→HSV）

### inRange()
- **形式**: `inRange(InputArray src, InputArray lowerb, InputArray upperb, OutputArray dst)`
- **引数**:
  - `src`: 入力画像
  - `lowerb`: 下限値
  - `upperb`: 上限値
  - `dst`: 出力画像
- **戻り値**: なし（出力は第4引数で受け取る）
- **処理**: 指定した範囲内の値を持つピクセルを255、それ以外を0とするマスク画像を作成

### bitwise_and()
- **形式**: `bitwise_and(InputArray src1, InputArray src2, OutputArray dst, InputArray mask = noArray())`
- **引数**:
  - `src1`: 入力画像1
  - `src2`: 入力画像2
  - `dst`: 出力画像
  - `mask`: マスク画像
- **戻り値**: なし（出力は第3引数で受け取る）
- **処理**: 2つの画像のビット単位のAND演算を行う

### hconcat()
- **形式**: `hconcat(InputArrayOfArrays src, OutputArray dst)`
- **引数**:
  - `src`: 入力画像の配列
  - `dst`: 出力画像
- **戻り値**: なし（出力は第2引数で受け取る）
- **処理**: 複数の画像を水平方向に連結

### resize()
- **形式**: `resize(InputArray src, OutputArray dst, Size dsize, double fx = 0, double fy = 0, int interpolation = INTER_LINEAR)`
- **引数**:
  - `src`: 入力画像
  - `dst`: 出力画像
  - `dsize`: 出力サイズ
  - `fx, fy`: x軸、y軸の倍率
  - `interpolation`: 補間方法
- **戻り値**: なし（出力は第2引数で受け取る）
- **処理**: 画像のサイズを変更

## ウィンドウ操作関連

### namedWindow()
- **形式**: `namedWindow(const String& winname, int flags = WINDOW_AUTOSIZE)`
- **引数**:
  - `winname`: ウィンドウ名
  - `flags`: フラグ
- **戻り値**: なし
- **処理**: 新しいウィンドウを作成

### resizeWindow()
- **形式**: `resizeWindow(const String& winname, int width, int height)`
- **引数**:
  - `winname`: ウィンドウ名
  - `width`: 幅
  - `height`: 高さ
- **戻り値**: なし
- **処理**: ウィンドウのサイズを設定

### createTrackbar()
- **形式**: `createTrackbar(const String& trackbarname, const String& winname, int* value, int count, TrackbarCallback onChange = 0, void* userdata = 0)`
- **引数**:
  - `trackbarname`: トラックバー名
  - `winname`: ウィンドウ名
  - `value`: 値のポインタ
  - `count`: 最大値
  - `onChange`: コールバック関数
  - `userdata`: ユーザーデータ
- **戻り値**: なし
- **処理**: スライダー（トラックバー）を作成

### getTrackbarPos()
- **形式**: `getTrackbarPos(const String& trackbarname, const String& winname) -> int`
- **引数**:
  - `trackbarname`: トラックバー名
  - `winname`: ウィンドウ名
- **戻り値**: トラックバーの現在値
- **処理**: トラックバーの現在値を取得

### setTrackbarPos()
- **形式**: `setTrackbarPos(const String& trackbarname, const String& winname, int pos)`
- **引数**:
  - `trackbarname`: トラックバー名
  - `winname`: ウィンドウ名
  - `pos`: 設定値
- **戻り値**: なし
- **処理**: トラックバーの値を設定

### imshow()
- **形式**: `imshow(const String& winname, InputArray mat)`
- **引数**:
  - `winname`: ウィンドウ名
  - `mat`: 表示する画像
- **戻り値**: なし
- **処理**: 指定したウィンドウに画像を表示

### waitKey()
- **形式**: `waitKey(int delay = 0) -> int`
- **引数**: `delay`: 待機時間（ミリ秒）
- **戻り値**: 押されたキーのコード（タイムアウト時は-1）
- **処理**: キー入力を待機。ウィンドウの更新にも必要

## プログラムの流れ
1. コマンドライン引数で指定された画像を読み込み
2. HSV色空間に変換
3. トラックバーでHSV値の範囲を調整可能
4. リアルタイムで元画像、マスク画像、抽出結果を表示
5. 'q'キーで終了
