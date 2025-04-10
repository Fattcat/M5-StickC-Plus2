#include <M5Unified.h>

// Pole farieb, ktoré budeme používať pre postupné zmeny pozadia
uint16_t colors[] = {GREEN, RED, ORANGE, VIOLET, BLUE, MAGENTA};
int currentColor = 0; // Index aktuálnej farby

void setup() {
  M5.begin();  // Inicializácia M5StickC Plus2
  M5.Lcd.setRotation(1);  // Nastavenie orientácie na šírku (0, 1, 2, 3)
  M5.Lcd.setTextSize(2); // Nastavenie veľkosti textu
  M5.Lcd.setTextColor(WHITE); // Farba textu (biela)
  M5.Lcd.fillScreen(GREEN);  // Počiatočná farba pozadia
}

void loop() {
  // Načítanie napätia batérie (pin 35 je priradený k batérii na M5StickC Plus2)
  float battery_voltage = analogRead(35); 

  // Kalibrácia ADC na napätie batérie
  battery_voltage = battery_voltage * (3.3 / 4095.0) * 2; // 3.3V referenčné napätie a zisk 2 (pre Li-Po batériu)

  // Výpočet percenta nabitia batérie (medzi 3.2V a 4.2V)
  int battery_percentage = (battery_voltage - 3.2) / (4.2 - 3.2) * 100;

  // Obmedzenie percenta do rozsahu 0 až 100
  if (battery_percentage > 100) {
    battery_percentage = 100;
  } else if (battery_percentage < 0) {
    battery_percentage = 0;
  }

  // Zmena farby pozadia (pomaly prechádza cez farby)
  M5.Lcd.fillScreen(colors[currentColor]);
  
  // Nastavenie pozície a farby textu
  M5.Lcd.setCursor(40, 50); // Nastavenie pozície pre "Bateria"
  M5.Lcd.print("Bateria: ");
  M5.Lcd.print(battery_percentage); 
  M5.Lcd.print("%");

  M5.Lcd.setCursor(40, 80); // Nastavenie pozície pre "Volty"
  M5.Lcd.print("Volty  : ");
  M5.Lcd.print(battery_voltage, 2); // Zobrazenie voltáže s dvoma desatinnými miestami

  // Prepneme na ďalšiu farbu v poli
  currentColor++;
  if (currentColor >= sizeof(colors) / sizeof(colors[0])) {
    currentColor = 0; // Ak sme dosiahli koniec poľa, začneme znovu
  }

  // Zobrazenie informácií
  delay(1000);  // Počkáme 1 sekundu pred ďalším meraním a zmenou farby
}
