const int TRIG_PIN = 10;
const int ECHO_PIN = 9;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // 直前にノイズなどでHIGHになっていた場合の修正
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 10µ秒以上のHIGH信号を受け取ると、超音波を発射する
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // ECHOピンがHIGHになるまで待つ
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) { // 距離が0の場合
    Serial.println("-1");
  } else {
    float distance = (duration * 0.034) / 2; // 距離を計算
    if (distance > 500) { // 距離が500cmを超えた場合
      Serial.println("-1");
    } else {
      Serial.println(distance); // 距離を出力
    }
  }

  delay(100);
}
