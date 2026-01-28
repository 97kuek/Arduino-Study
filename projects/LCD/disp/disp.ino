#include <LiquidCrystal.h>

// RS, E, D4, D5, D6, D7 の順で設定
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2); // 16文字 x 2行
  lcd.print("WASA HPA Team"); // 1行目
  lcd.setCursor(0, 1); // 2行目
  lcd.print("Sim Success!"); // 2行目
}

void loop() {
}