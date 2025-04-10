#include <M5Unified.h>
#include <WiFi.h>
#include "time.h"

// WiFi pripojenie
const char* ssid     = "JoeSPass";
const char* password = "Pass";

// NTP server
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600; // GMT+1 pre Slovensko
const int   daylightOffset_sec = 3600; // Letný čas (ak je aktuálny)

int red = 0, green = 255, blue = 0;
int colorStep = 5;
int colorState = 0;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.fillScreen(WHITE);

  // Pripojenie na WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Inicializácia času z NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  // Napätie batérie
  float battery_voltage = analogRead(35);
  battery_voltage = battery_voltage * (3.3 / 4095.0) * 2;
  int battery_percentage = (battery_voltage - 3.2) / (4.2 - 3.2) * 100;

  if (battery_percentage > 100) battery_percentage = 100;
  if (battery_percentage < 0) battery_percentage = 0;

  // Zmena farby pozadia
  M5.Lcd.fillScreen(M5.Lcd.color565(red, green, blue));

  // Stredová pozícia displeja (160x120 pre M5StickC Plus2 pri rotácii 1)
  int centerX = 100;

  // Výpis Batéria
  M5.Lcd.setCursor(centerX - 70, 30);
  M5.Lcd.print("Bateria : ");
  M5.Lcd.print(battery_percentage);
  M5.Lcd.print("%");

  // Výpis Volty
  M5.Lcd.setCursor(centerX - 70, 60);
  M5.Lcd.print("Volty   : ");
  M5.Lcd.print(battery_voltage, 2);

  // Získanie aktuálneho času z RTC
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    char timeStr[6];
    strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);

    // Výpis času
    M5.Lcd.setCursor(centerX - 70, 90);
    M5.Lcd.print("Cas     : ");
    M5.Lcd.print(timeStr);
  } else {
    M5.Lcd.setCursor(centerX - 70, 90);
    M5.Lcd.print("Cas     : --:--");
  }

  // Efekt fade farby
  if (colorState == 0) {
    green -= colorStep;
    if (green <= 0) { green = 0; red += colorStep; if (red >= 255) { red = 255; colorState = 1; } }
  } else if (colorState == 1) {
    red -= colorStep;
    if (red <= 0) { red = 0; blue += colorStep; if (blue >= 255) { blue = 255; colorState = 2; } }
  } else if (colorState == 2) {
    blue -= colorStep;
    if (blue <= 0) { blue = 0; green += colorStep; if (green >= 255) { green = 255; colorState = 3; } }
  } else if (colorState == 3) {
    green -= colorStep;
    if (green <= 0) { green = 0; blue += colorStep; if (blue >= 255) { blue = 255; colorState = 0; } }
  }

  delay(100);
}
