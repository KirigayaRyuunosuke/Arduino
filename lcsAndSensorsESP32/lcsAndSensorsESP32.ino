#include <LiquidCrystal.h>
#include "DHT.h"

#define DHT11Pin 5

DHT dht(DHT11Pin, DHT11);
LiquidCrystal lcd(8,7,9,10,11,12);

void setup() {
  pinMode(6, OUTPUT);
  lcd.begin(20, 4);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello");

  dht.begin();
}

void loop() {
  if(analogRead(A4)<950)
digitalWrite(6,1);
else
digitalWrite(6,0);

  delay(2000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)

  lcd.setCursor(0, 0);
  lcd.print(h);
  lcd.print("%");

  float t = dht.readTemperature();
  lcd.setCursor(0, 1);
  lcd.print(t);
  lcd.print("'C ");
}
