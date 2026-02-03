#include <Arduino.h>
#include <IRremote.h>

// ピン定義
const int RECV_PIN = 11;    // 赤外線受信モジュール
const int PIN_RED = 9;      // 赤色LED
const int PIN_GREEN = 10;   // 緑色LED
const int PIN_BLUE = 5;     // 青色LED

void setup() {
    Serial.begin(9600);
    IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);

    Serial.println("System Ready. Waiting for IR signal...");
}

void setColor(int r, int g, int b){
    analogWrite(PIN_RED, r);
    analogWrite(PIN_GREEN, g);
    analogWrite(PIN_BLUE, b);
}

void loop() {
    if (IrReceiver.decode()) {
        uint16_t command = IrReceiver.decodedIRData.command;
        
        // デバッグ用情報の表示
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Received noise or an unknown protocol"));
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        } else {
            IrReceiver.printIRResultShort(&Serial);
            Serial.print(F("Command = 0x"));
            Serial.println(command, HEX);
        }

        // リモコンのボタンごとに色を割り当てる（この0x数字を実際のボタンコードに書き換えてください）
        switch (command) {
            case 0x45: // 例: ボタン1 (メーカーによって異なります)
                setColor(255, 0, 0); //赤
                Serial.println("RED");
                break;
            case 0x46: // 例: ボタン2
                setColor(0, 255, 0); //緑
                Serial.println("GREEN");
                break;
            case 0x47: // 例: ボタン3
                setColor(0, 0, 255); //青
                Serial.println("BLUE");
                break;
            case 0x44: // 例: ボタン4
                setColor(128, 0, 128); //紫
                Serial.println("PURPLE");
                break;
            case 0x40: // 例: ボタン0
                setColor(0, 0, 0); //オフ
                Serial.println("OFF");
                break;
            default: // 未設定のボタンが押された場合
                // デフォルトでは何もしない
                break;
        }
        IrReceiver.resume(); // 受信を再開
    }
}