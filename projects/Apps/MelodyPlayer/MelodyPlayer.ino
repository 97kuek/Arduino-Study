#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6  1047

// メロディの配列（ドレミファソラシド）
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6
};

// ピン割り当て
int switchPin = 5;
int buzzerPin = 10;

void setup()
{
  // スイッチを内部プルアップで設定
  pinMode(switchPin, INPUT_PULLUP);
}

void loop()
{
  // スイッチON == LOW の判定
  if (digitalRead(switchPin) == LOW) {
    
    // 配列の中身を順番に再生
    for (int i = 0; i < 8; i++) {
      tone(buzzerPin, melody[i], 200); // カッコを閉じ、引数を指定
      delay(250);                      // 音の長さ＋少しの余韻
    }
    
  }
}
