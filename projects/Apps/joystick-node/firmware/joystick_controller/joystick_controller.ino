// ピン設定
const int pinX = A0;
const int pinY = A1;
const int pinSW = 2;

void setup() {
  Serial.begin(9600);
  pinMode(pinSW, INPUT_PULLUP); // 内部プルアップ
}

void loop() {
  // アナログ値の読み取り
  int valX = analogRead(pinX);
  int valY = analogRead(pinY);
  int valSW = digitalRead(pinSW);

  // JSON形式で送信: {"x":512,"y":512,"sw":1}
  Serial.print("{\"x\":");
  Serial.print(valX);
  Serial.print(",\"y\":");
  Serial.print(valY);
  Serial.print(",\"sw\":");
  Serial.print(valSW);
  Serial.println("}");

  delay(50); // データの氾濫を防ぐため少し待機
}
