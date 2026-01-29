int SWITCH_SOCKET = 2; // タクトスイッチを接続したソケット(PD2)

void setup() {
  pinMode(SWITCH_SOCKET, INPUT); // 2番ピンを入力モードに設定
  Serial.begin(9600); // シリアル接続の初期化
}

void loop() {
  Serial.println(digitalRead(SWITCH_SOCKET)); // タクトスイッチの状態を入力し，結果をシリアル出力
  delay(1000); // 1000ms待機
}