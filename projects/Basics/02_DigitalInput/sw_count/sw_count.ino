int SWITCH_SOCKET = 2; // タクトスイッチを接続したソケット(PD2)
int count = 0; // カウンタ変数

void setup() {
  pinMode(SWITCH_SOCKET, INPUT); // 2番ピンを入力モードに設定
  Serial.begin(9600); // シリアル接続の初期化
}

void loop() {
  if (digitalRead(SWITCH_SOCKET)==1){
    count = count + 1;
    Serial.print("Count : ");
    Serial.println(count);
    // 押し続けてカウントが増えないように、タクトスイッチが押されている間はループして次の処理を行わないようにする
    // 1回の押下で複数回カウントしてしまうことを"チャタリング"という
    while(digitalRead(SWITCH_SOCKET)==1){
      delay(100);
    }
  }
}