int LED_SOCKET = 10; // LEDを接続したピン番号

void setup() {
  // 10番ピンを出力モードに設定
  pinMode(LED_SOCKET, OUTPUT);
}

void loop() {
  // 10番ピンをHIGH（点灯）にする
  digitalWrite(LED_SOCKET, HIGH);
}