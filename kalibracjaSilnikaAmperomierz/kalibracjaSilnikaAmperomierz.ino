#include "Wire.h"
#include "Adafruit_INA219.h"

Adafruit_INA219 ina219;

int i = 0;
int m = 0;
int n = 0;
float mA[200];
float mAAv[20];
float ampers[30];
float number = 0;
float numberA = 0;
float numberB = 0;
float current_mA = 0;

void setup() {
  Serial.begin(9600);
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  for (m = 1; m <= 200; m++) {
    mA[m] = 0;
  }
  for (m = 1; m <= 20; m++) {
    mAAv[m] = 0;
  }
  for (m = 1; m <= 30; m++) {
    ampers[m] = 0;
  }
  for (m = 1; m <= 30; m++) {
    Serial.print(70 + m);
    Serial.print("\t");
    for (n = 1; n <= 10; n++) {
      for (i = 1; i < 500; i++) {
        analogWrite(3, 70 + m);
        current_mA = ina219.getCurrent_mA();
        numberA = numberA + current_mA;
      }
      numberB = numberA / 500;
      numberA = 0;
      Serial.print(numberB);
      Serial.print("\t");
      number = number + numberB;
    }

    Serial.println(number / 10);
    ampers[m] = number / 10;
    number = 0;
  }
}

void loop() {
  for (m = 1; m <= 30; m++) {
    Serial.print(ampers[m]);
    Serial.print("\t");
  }
Serial.println();
}
