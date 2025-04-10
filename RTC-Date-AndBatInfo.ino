#include <M5Unified.h>
#include <time.h>

// Fade farby
int red = 0, green = 255, blue = 0;
int colorStep = 5;
int colorState = 0;

// Premenná pre sledovanie hodiny
int lastHour = -1;

// Inicializačný RTC čas z času kompilácie
void setCompileTimeRTC() {
  struct tm tm;
  const char *compile_date = __DATE__;
  const char *compile_time = __TIME__;

  // Parsovanie dátumu
  char month_str[4];
  int day, year;
  sscanf(compile_date, "%3s %d %d", month_str, &day, &year);

  // Parsovanie času
  int hour, minute, second;
  sscanf(compile_time, "%d:%d:%d", &hour, &minute, &second);

  // Konverzia názvu mesiaca
  const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
  int month = (strstr(months, month_str) - months) / 3 + 1;

  tm.tm_year = year - 1900;
  tm.tm_mon = month - 1;
  tm.tm_mday = day;
  tm.tm_hour = hour;
  tm.tm_min = minute;
  tm.tm_sec = second;
  tm.tm_isdst = 0;

  time_t t = mktime(&tm);
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, nullptr);
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.fillScreen(WHITE);
  setCompileTimeRTC();
}

void loop() {
  float battery_voltage = analogRead(35);
  battery_voltage = battery_voltage * (3.3 / 4095.0) * 2;
  int battery_percentage = (battery_voltage - 3.2) / (4.2 - 3.2) * 100;
  battery_percentage = constrain(battery_percentage, 0, 100);

  M5.Lcd.fillScreen(M5.Lcd.color565(red, green, blue));
  int centerX = 100;

  M5.Lcd.setCursor(centerX - 70, 10);
  M5.Lcd.print("Bateria : ");
  M5.Lcd.print(battery_percentage);
  M5.Lcd.print("%");

  M5.Lcd.setCursor(centerX - 70, 40);
  M5.Lcd.print("Volty   : ");
  M5.Lcd.print(battery_voltage, 2);

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    char timeStr[6], dateStr[11];
    strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
    strftime(dateStr, sizeof(dateStr), "%d.%m.%Y", &timeinfo);

    M5.Lcd.setCursor(centerX - 70, 70);
    M5.Lcd.print("Cas     : ");
    M5.Lcd.print(timeStr);

    M5.Lcd.setCursor(centerX - 90, 100);
    M5.Lcd.print("Datum : ");
    M5.Lcd.print(dateStr);

    // Kontrola či nastala nová celá hodina
    if (timeinfo.tm_min == 0 && timeinfo.tm_hour != lastHour) {
      lastHour = timeinfo.tm_hour;

      // Zabzuč 2x
      for (int i = 0; i < 2; i++) {
        M5.Speaker.tone(1000, 200); // 1000 Hz, 200 ms
        delay(300); // malá pauza medzi bzučaním
      }
    }

  } else {
    M5.Lcd.setCursor(centerX - 70, 80);
    M5.Lcd.print("Cas     : --:--");
    M5.Lcd.setCursor(centerX - 70, 110);
    M5.Lcd.print("Datum   : --.--.----");
  }

  // Fade efekt farieb
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
