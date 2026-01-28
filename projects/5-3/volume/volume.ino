int VOLUME_SOCKET = A0; // 半固定抵抗の入力を接続したソケットを変数で指定

void setup() {
    Serial.begin(9600);
}

void loop() {
    int analog_val; // アナログ入力の値を格納する変数
    float input_volt; // 電圧に変換した値を格納する偏す

    analog_val = analogRead(VOLUME_SOCKET); // アナログ入力して変数に格納
    input_volt = (float)analog_val * (5.0 / 1024.0); // 電圧に変換して変数に格納

    Serial.print("analog_val: ");
    Serial.println(analog_val);
    Serial.print("input_volt: ");
    Serial.println(input_volt);

    delay(500);
}