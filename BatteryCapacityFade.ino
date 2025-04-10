#include <M5Unified.h>

int red = 0, green = 255, blue = 0; // Začíname so zelenou farbou
int colorStep = 5;  // Zrýchlenie/zníženie farby
int colorState = 0; // Stav premennej, ktorá určuje, ktorú farbu meníme

void setup() {
  M5.begin();  // Inicializácia M5StickC Plus2
  M5.Lcd.setRotation(1);  // Nastavenie orientácie na šírku (0, 1, 2, 3)
  M5.Lcd.setTextSize(2); // Nastavenie veľkosti textu
  M5.Lcd.setTextColor(WHITE); // Farba textu (biela)
  M5.Lcd.fillScreen(WHITE);  // Počiatočná farba pozadia (biela)
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

  // Zmena farby pozadia postupne, podľa aktuálneho stavu
  M5.Lcd.fillScreen(M5.Lcd.color565(red, green, blue));  // Nastavenie farby pozadia

  // Nastavenie pozície a farby textu
  M5.Lcd.setCursor(40, 50); // Nastavenie pozície pre "Bateria"
  M5.Lcd.print("Bateria: ");
  M5.Lcd.print(battery_percentage); 
  M5.Lcd.print("%");

  M5.Lcd.setCursor(40, 80); // Nastavenie pozície pre "Volty"
  M5.Lcd.print("Volty  : ");
  M5.Lcd.print(battery_voltage, 2); // Zobrazenie voltáže s dvoma desatinnými miestami

  // Zmena farby pozadia
  if (colorState == 0) {
    green -= colorStep;  // Znižujeme zelenú (prechod zintenzívni)
    if (green <= 0) {
      green = 0;  // Zastavíme znižovanie zelenej
      red += colorStep;  // Začneme zvyšovať červenú (prechod na oranžovú)
      if (red >= 255) {
        red = 255;  // Zastavíme zvyšovanie červenej
        colorState = 1;  // Prejdeme na ďalší stav (prechod na oranžovú)
      }
    }
  } else if (colorState == 1) {
    red -= colorStep;  // Znižujeme červenú (prechod na žltú)
    if (red <= 0) {
      red = 0;  // Zastavíme znižovanie červenej
      blue += colorStep;  // Začneme zvyšovať modrú (prechod na fialovú)
      if (blue >= 255) {
        blue = 255;  // Zastavíme zvyšovanie modrej
        colorState = 2;  // Prejdeme na ďalší stav (prechod na fialovú)
      }
    }
  } else if (colorState == 2) {
    blue -= colorStep;  // Znižujeme modrú (prechod na modrú)
    if (blue <= 0) {
      blue = 0;  // Zastavíme znižovanie modrej
      green += colorStep;  // Začneme zvyšovať zelenú (prechod na ružovú)
      if (green >= 255) {
        green = 255;  // Zastavíme zvyšovanie zelenej
        colorState = 3;  // Prejdeme na ďalší stav (prechod na ružovú)
      }
    }
  } else if (colorState == 3) {
    green -= colorStep;  // Znižujeme zelenú (prechod na modrú)
    if (green <= 0) {
      green = 0;  // Zastavíme znižovanie zelenej
      blue += colorStep;  // Začneme zvyšovať modrú
      if (blue >= 255) {
        blue = 255;  // Zastavíme zvyšovanie modrej
        colorState = 0;  // Prejdeme späť na pôvodnú farbu (zelenú)
      }
    }
  }

  // Zobrazenie informácií
  delay(100);  // Počkáme 0.1 sekundy pred ďalšou zmenou farby
}
