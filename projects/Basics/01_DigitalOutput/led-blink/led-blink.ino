int LED_SOCKET = 10; // LEDを接続したピン番号
int INTERVAL = 1000; // 点滅の間隔を1000ms

void setup() {
  // 10番ピンを出力モードに設定
  pinMode(LED_SOCKET, OUTPUT);
}

void loop() {
  digitalWrite(LED_SOCKET, HIGH); // 10番ピンをHIGHにする
  delay(INTERVAL);
  digitalWrite(LED_SOCKET, LOW); // 10番ピンをLOWにする
  delay(INTERVAL);
}