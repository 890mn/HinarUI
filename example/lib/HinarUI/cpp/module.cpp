#include "UI.h"
#include "resource/module.h"

void module_LIGHT() {
    display.clearDisplay();
    menu.drawTopBar();
    menu.drawFrame();

    display.setCursor(1, 20);
    display.setTextSize(2);
    display.print("LIGHT");
    display.display();
    display.setTextSize(1);
}

void module_TIME() {
    // Wait for DS3231
    while (true) {
        display.clearDisplay();
        menu.drawTopBar();
        menu.drawFrame();
        uint32_t total_seconds = RTC_Time();
    
        uint32_t hours = total_seconds / 3600;
        uint32_t minutes = (total_seconds % 3600) / 60;
        uint32_t seconds = total_seconds % 60;

        display.fillRect(17, 30, 100, 8, BLACK);
        display.setCursor(17, 30);
        display.setTextSize(2);
        display.printf("%02d:%02d:%02d", hours, minutes, seconds);

        display.display();
        display.setTextSize(1);
        if (digitalRead(KEY_BACK) == LOW) break;
    }
}

void drawScaledText(const char* text, int x, int y, float scale) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);

  // 创建位图缓冲区
  uint8_t* buffer = (uint8_t*)calloc(w * h, sizeof(uint8_t));
  if (!buffer) return;

  // 在缓冲区绘制原始文字
  Adafruit_SSD1306 bufferDisplay(w, h, &Wire, -1);
  bufferDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  bufferDisplay.clearDisplay();
  bufferDisplay.setFont(&FreeSans9pt7b);
  bufferDisplay.setTextColor(SSD1306_WHITE);
  bufferDisplay.setCursor(0, h / 2);
  bufferDisplay.print(text);
  bufferDisplay.display();

  // 提取缓冲区数据
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      buffer[i * w + j] = bufferDisplay.getPixel(j, i);
    }
  }

  // 使用双线性插值缩放
  int scaledW = w * scale;
  int scaledH = h * scale;
  for (int i = 0; i < scaledH; i++) {
    for (int j = 0; j < scaledW; j++) {
      int srcX = j / scale;
      int srcY = i / scale;
      uint8_t pixel = buffer[srcY * w + srcX];
      display.drawPixel(x + j, y + i, pixel);
    }
  }

  free(buffer);
}

void module_SHT30() {
    display.clearDisplay();
    menu.drawTopBar();
    menu.drawFrame();
    display.setFont(&FreeSans9pt7b);
    float t = SHT.readTemperature();
    float h = SHT.readHumidity();

    display.setCursor(1, 20);
    if (! isnan(t)) {  // check if 'is not a number'
        char* temper = (char*)malloc(20);
        sprintf(temper, "Temper: %.2f", t);
        //drawScaledText(temper, 1, 20, 1.5);
        display.print(temper); 
    } else { 
        drawScaledText("Temper Failed", 1, 20, 2);
    }
    
    display.setCursor(1, 40);
    if (! isnan(h)) {  // check if 'is not a number'
        char* humi = (char*)malloc(20);
        sprintf(humi, "Humi: %.2f", h);
        //drawScaledText(humi, 1, 40, 1.5);
        display.print(humi);
    } else { 
        drawScaledText("Humi Failed", 1, 40, 1.5);
    }

    display.display();
}

void module_UICORE() {
    display.clearDisplay();
    menu.drawTopBar("UI-CORE", "WROVER-E");
    menu.drawFrame();

    display.setTextSize(2);
    display.setCursor(8, 25);
    display.print("ESP32");

    display.setTextSize(1);
    display.setCursor(8, 48);
    display.print("4MB Flash 8MB PSRAM");
    
    display.display();
}

void module_github() {
    display.clearDisplay();
    menu.drawTopBar("Board RELEASE", "V0.1");
    menu.drawFrame();

    display.setTextSize(2);
    display.setCursor(8, 25);
    display.print("HinarUI");

    display.drawBitmap(98, 23, bitmap_github, 24, 24, SELECTED_COLOR);
    display.setCursor(10, 48);

    display.setTextSize(1);
    display.print("@890mn ORIGINAL");
    
    display.display();
}

void module_ABOUT() {
    display.clearDisplay();
    menu.drawTopBar("SETTING");
    menu.drawFrame();

    display.setTextSize(1);
    display.setCursor(3, 20);
    display.print("FLOW SPEED");
    display.setCursor(70, 20);
    display.print(menu.getFlowSpeed());

    display.setCursor(3, 30);
    display.print("BOOT TIME");

    while (true) {
        uint32_t Boot = RTC_Time();

        display.fillRect(70, 30, 50, 8, BLACK);
        display.setCursor(70, 30);
        display.print(Boot);
        display.setCursor(100, 30);
        display.print("Sec");

        display.display();
        if (digitalRead(KEY_BACK) == LOW) break;
    }
}