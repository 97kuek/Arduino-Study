#include <Servo.h> // サーボモーター制御用ライブラリ

int SERVO_SOCKET = 9; // サーボモーターを接続するピン

Servo servo; // サーボモーター制御用のインスタンスを作成

void setup() {
    servo.attach(SERVO_SOCKET); // サーボモーターを接続するピンを指定
}

void loop() {
    servo.write(0); // 0度回転
    delay(1000); // 1秒待機
    servo.write(90); // 90度回転
    delay(1000); // 1秒待機
    servo.write(180); // 180度回転
    delay(1000); // 1秒待機
}

