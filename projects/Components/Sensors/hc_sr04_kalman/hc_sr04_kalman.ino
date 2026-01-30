/*
 * HC-SR04 超音波距離センサ (カルマンフィルタ比較用)
 * 接続:
 *   Trig: 10番ピン
 *   Echo: 9番ピン
 *   VCC:  5V
 *   GND:  GND
 * 
 * 使い方:
 *   Arduino IDEの「ツール」>「シリアルプロッタ」を開くと、
 *   青線（生データ）と赤線（フィルタ後）で比較できます。
 */

// --- 簡易カルマンフィルタクラスの定義 ---
class SimpleKalmanFilter {
  private:
    float _err_measure;
    float _err_estimate;
    float _q;
    float _current_estimate;
    float _last_estimate;
    float _kalman_gain;

  public:
    // コンストラクタ
    // mea_e: 測定の不確かさ（ノイズの大きさ）
    // est_e: 推定の不確かさ（初期値）
    // q: プロセスノイズ（値の変動しやすさ。小さいと滑らか、大きいと追従性が高い）
    SimpleKalmanFilter(float mea_e, float est_e, float q) {
      _err_measure = mea_e;
      _err_estimate = est_e;
      _q = q;
      _current_estimate = 0;
      _last_estimate = 0;
    }

    float updateEstimate(float mea) {
      _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
      _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
      _err_estimate = (1.0 - _kalman_gain) * _err_estimate + abs(_last_estimate - _current_estimate) * _q;
      _last_estimate = _current_estimate;

      return _current_estimate;
    }
};
// ------------------------------------

const int TRIG_PIN = 10;
const int ECHO_PIN = 9;

// --- 設定 ---
// true : シリアルプロッタ用 (グラフ描画向け: "Raw:10,Filtered:10"形式)
// false: シリアルモニタ用 (人間が見やすい形式: "Raw: 10.00 cm | Filtered: 10.00 cm"形式)
const bool USE_PLOTTER = false; 

/*
 * カルマンフィルタの設定
 * e_mea (測定誤差): 2.0 (センサのぶれ幅など)
 * e_est (推定誤差): 2.0 (初期値)
 * q (プロセスノイズ): 0.1 (0.01だと遅いため0.1に変更)
 */
SimpleKalmanFilter kf(2.0, 2.0, 0.1);

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  if (!USE_PLOTTER) {
    Serial.println("--- HC-SR04 Kalman Filter Test ---");
    Serial.println("Raw (cm)\tFiltered (cm)");
  }
}

void loop() {
  // --- 距離測定 (既存のコードと同じ) ---
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30msタイムアウト

  float raw_distance = 0;
  if (duration == 0) {
    raw_distance = 0; 
  } else {
    // 340m/s = 0.034cm/us
    raw_distance = duration * 0.034 / 2;
  }

  // --- カルマンフィルタ適用 ---
  float filtered_distance = kf.updateEstimate(raw_distance);

  // --- 出力 ---
  if (raw_distance > 0 && raw_distance < 400) { 
    if (USE_PLOTTER) {
      // シリアルプロッタ用フォーマット
      Serial.print("Raw:");
      Serial.print(raw_distance);
      Serial.print(",");
      Serial.print("Filtered:");
      Serial.println(filtered_distance);
    } else {
      // 人間が見やすいフォーマット
      Serial.print("Raw: ");
      Serial.print(raw_distance);
      Serial.print(" cm\t | \tFiltered: ");
      Serial.print(filtered_distance);
      Serial.println(" cm");
    }
  } else {
     // 範囲外の場合
     if (!USE_PLOTTER) {
       Serial.println("Out of range");
     }
  }

  delay(100); // 読みやすいように少し遅くする
}
