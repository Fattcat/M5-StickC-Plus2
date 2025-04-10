#include <M5Unified.h>
#include <time.h>

// Premenné pre fade efekt
int red = 0, green = 255, blue = 0;
int colorStep = 5;
int colorState = 0;

// Inicializačný RTC čas (z času kompilácie)
void setCompileTimeRTC() {
  struct tm tm;
  const char *compile_date = __DATE__; // napr. "Apr 10 2025"
  const char *compile_time = __TIME__; // napr. "14:35:20"

  // Parsovanie dátumu
  char month_str[4];
  int day, year;
  sscanf(compile_date, "%3s %d %d", month_str, &day, &year);

  // Parsovanie času
  int hour, minute, second;
  sscanf(compile_time, "%d:%d:%d", &hour, &minute, &second);

  // Konverzia názvu mesiaca na číslo
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
  settimeofday(&now, nullptr); // Nastavenie času zariadenia
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.fillScreen(WHITE);

  setCompileTimeRTC(); // Nastavenie RTC podľa času kompilácie
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

  int centerX = 100;

  // Výpis Batéria
  M5.Lcd.setCursor(centerX - 70, 10);
  M5.Lcd.print("Bateria : ");
  M5.Lcd.print(battery_percentage);
  M5.Lcd.print("%");

  // Výpis Volty
  M5.Lcd.setCursor(centerX - 70, 40);
  M5.Lcd.print("Volty   : ");
  M5.Lcd.print(battery_voltage, 2);

  // Výpis času a dátumu
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    char timeStr[6]; // HH:MM
    char dateStr[11]; // DD.MM.YYYY

    strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
    strftime(dateStr, sizeof(dateStr), "%d.%m.%Y", &timeinfo);

    // Čas
    M5.Lcd.setCursor(centerX - 70, 70);
    M5.Lcd.print("Cas     : ");
    M5.Lcd.print(timeStr);

    // Dátum
    M5.Lcd.setCursor(centerX - 90, 100);
    M5.Lcd.print("Datum : ");
    M5.Lcd.print(dateStr);
  } else {
    M5.Lcd.setCursor(centerX - 70, 80);
    M5.Lcd.print("Cas     : --:--");
    M5.Lcd.setCursor(centerX - 70, 110);
    M5.Lcd.print("Datum   : --.--.----");
  }

  // Fade efekt farby
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
