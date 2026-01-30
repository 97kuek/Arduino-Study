/*
 * HC-SR04 超音波距離センサ
 * 接続:
 *   Trig: 10番ピン
 *   Echo: 9番ピン
 *   VCC:  5V
 *   GND:  GND
 */

const int TRIG_PIN = 10;
const int ECHO_PIN = 9;

void setup() {
  Serial.begin(9600); // シリアル通信の開始
  pinMode(TRIG_PIN, OUTPUT); // Trigピンを出力に設定
  pinMode(ECHO_PIN, INPUT);  // Echoピンを入力に設定
}

void loop() {
  // TrigピンをLOWにしてリセット（ノイズ対策）
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Trigピンを10µs HIGHにして超音波を発射
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // EchoピンがHIGHになっている時間（往復時間）を計測（単位：マイクロ秒）
  // タイムアウトを設定する場合は第3引数に追加（例：30000）
  long duration = pulseIn(ECHO_PIN, HIGH);

  // 距離を計算
  // 音速 340m/s = 0.034cm/µs
  // 距離 = 時間 × 速さ ÷ 2 （往復のため）
  float distance = duration * 0.034 / 2;

  // シリアルモニタに出力
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100); // 0.1秒待機
}
