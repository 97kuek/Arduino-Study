/*
 * Morse Light Receiver (LCD1602 Version)
 * 
 * 光センサー(A0)への入力を監視し、モールス信号を解読して
 * LCD1602 (パラレル接続) に文字を表示するプログラム。
 * 
 * 必要なライブラリ:
 * - LiquidCrystal (Arduino標準)
 */

#include <LiquidCrystal.h>

// --- 定数定義 ---

// ハードウェア設定
#define SENSOR_PIN A0

// LCDピン設定: RS, E, D4, D5, D6, D7
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// 信号判定パラメータ (環境に合わせて調整が必要)
const int LIGHT_THRESHOLD = 500; // この値を超えたらONとみなす (0-1023)

// 時間パラメータ (ミリ秒) - Tinkercad手動操作用にかなり緩く設定
const unsigned long DOT_DURATION_MAX = 600;      // これ以下なら短点(.)
const unsigned long DASH_DURATION_MIN = 600;    // これ以上なら長点(-)
const unsigned long CHAR_GAP_MIN = 1500;         // 文字区切りとみなすOFF時間
const unsigned long WORD_GAP_MIN = 3000;         // 単語区切り(スペース)

// --- グローバル変数 ---
bool isLightOn = false;
unsigned long lastChangeTime = 0;
String currentSymbol = ""; // 受信中のモールス符号 (例: ".-")
String decodedMessage = ""; // 受信済みメッセージ

// --- モールス符号テーブル ---
struct MorseMapping {
  const char* code;
  char character;
};

const MorseMapping morseTable[] = {
  {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'},
  {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'}, {".---", 'J'},
  {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'},
  {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
  {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'},
  {"--..", 'Z'},
  {".----", '1'}, {"..---", '2'}, {"...--", '3'}, {"....-", '4'}, {".....", '5'},
  {"-....", '6'}, {"--...", '7'}, {"---..", '8'}, {"----.", '9'}, {"-----", '0'},
  {NULL, 0}
};

// --- 関数詳細 ---
char decodeMorse(String symbol);
void updateDisplay();

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);

  // LCD初期化 (16文字x2行)
  lcd.begin(16, 2);
  
  // 起動メッセージ
  lcd.print("Morse Receiver");
  lcd.setCursor(0, 1);
  lcd.print("Ready...");
  delay(1000);
  lcd.clear();
  
  updateDisplay();
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN);
  unsigned long currentTime = millis();
  bool currentLightState = (sensorValue > LIGHT_THRESHOLD);

  // 状態変化の検出
  if (currentLightState != isLightOn) {
    unsigned long duration = currentTime - lastChangeTime;
    
    if (isLightOn) {
      // ON -> OFF (点灯終了)
      if (duration < 50) {
        // ノイズ無視
      } else if (duration <= DOT_DURATION_MAX) {
        currentSymbol += ".";
        Serial.print(".");
        updateDisplay();
      } else {
        currentSymbol += "-";
        Serial.print("-");
        updateDisplay();
      }
    } else {
      // OFF -> ON (点灯開始)
      // タイマーリセット
    }

    isLightOn = currentLightState;
    lastChangeTime = currentTime;
    
  } else {
    // 状態変化なし
    if (!isLightOn) {
      // OFF継続中: 文字区切り判定
      unsigned long offDuration = currentTime - lastChangeTime;
      
      if (currentSymbol.length() > 0 && offDuration > CHAR_GAP_MIN) {
        // 文字確定
        char decodedChar = decodeMorse(currentSymbol);
        if (decodedChar != 0) {
          decodedMessage += decodedChar;
          Serial.print(" ");
          Serial.println(decodedChar);
        } else {
          Serial.println("?");
        }
        currentSymbol = ""; // リセット
        updateDisplay();
      }
    }
  }
  
  delay(10);
}

// モールス符号デコード
char decodeMorse(String symbol) {
  for (int i = 0; morseTable[i].code != NULL; i++) {
    if (symbol == morseTable[i].code) {
      return morseTable[i].character;
    }
  }
  return 0;
}

// LCD表示更新
void updateDisplay() {
  // 1行目: 現在入力中の符号とセンサー値
  lcd.setCursor(0, 0);
  lcd.print("In:");
  lcd.print(currentSymbol);
  // 残りを空白で埋める (前の残像を消すため)
  for(int i = 3 + currentSymbol.length(); i < 11; i++) lcd.print(" ");
  
  // 右端にセンサー値を表示 (デバッグ用)
  lcd.setCursor(12, 0);
  int val = analogRead(SENSOR_PIN);
  lcd.print(val);
  if(val < 1000) lcd.print(" ");
  if(val < 100) lcd.print(" ");
  if(val < 10) lcd.print(" ");

  // 2行目: 受信済みメッセージ
  lcd.setCursor(0, 1);
  
  // 文字数が16を超えたら、最新の16文字を表示
  int len = decodedMessage.length();
  if (len > 16) {
    lcd.print(decodedMessage.substring(len - 16));
  } else {
    lcd.print(decodedMessage);
    // 残りを空白で埋める
    for(int i = len; i < 16; i++) lcd.print(" ");
  }
}
