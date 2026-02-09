#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128    // 横幅
#define SCREEN_HEIGHT 64    // 縦幅
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2            // DHT11を接続するピン番号
#define DHTTYPE DHT11       // DHT11というモデルを使うという指定
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;); // I2Cの接続に失敗したらここで停止する
  }
  
  dht.begin(); // センサー開始
  display.clearDisplay(); // 画面を一度まっさらに
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) return;

  display.clearDisplay();

  // --- UIデザインの描画 ---
  
  // 外枠の角丸長方形
  display.drawRoundRect(0, 0, 128, 64, 4, SSD1306_WHITE);
  
  // ヘッダー部分（白黒反転）
  display.fillRoundRect(0, 0, 128, 14, 4, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(18, 3);
  display.print("ROOM MONITOR v1.0");

  // メイン表示（ここから白文字）
  display.setTextColor(SSD1306_WHITE);

  // 温度表示
  display.setCursor(10, 22);
  display.setTextSize(1);
  display.print("Temp");
  
  display.setCursor(45, 20);
  display.setTextSize(2);
  display.print(t, 1);
  display.setTextSize(1);
  display.drawCircle(100, 22, 2, SSD1306_WHITE); // 度数記号
  display.setCursor(105, 20);
  display.print("C");

  // 区切り線
  display.drawLine(10, 40, 118, 40, SSD1306_WHITE);

  // 湿度表示
  display.setCursor(10, 48);
  display.setTextSize(1);
  display.print("Humi");

  display.setCursor(45, 46);
  display.setTextSize(2);
  display.print((int)h);
  display.setTextSize(1);
  display.setCursor(100, 46);
  display.print("%");

  display.display();
}