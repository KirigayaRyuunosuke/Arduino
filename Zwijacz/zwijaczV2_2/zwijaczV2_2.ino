/*
Zwijacz na mały silniczek (4 prędkości obrotowe)

*/

#include "Wire.h"
#include "Adafruit_INA219.h"

#define pwmA 3
#define pot A2
#define sirenPin A0 

Adafruit_INA219 ina219;

bool alarmTest = 0;
bool alarm = 0;
int n = 0;
int i = 0;
int motorSpeed = 0;
float current_mA = 0.0000;
float number = 0;
float numbera = 0;
float numberb = 0;

void siren() {
  int czestotliwosc = 0;
  for (czestotliwosc = 1750; czestotliwosc < 2250; czestotliwosc = czestotliwosc + 5) {
    tone(sirenPin, czestotliwosc);
    Serial.println(czestotliwosc);
  }
  for (czestotliwosc = 2250; czestotliwosc > 1750; czestotliwosc = czestotliwosc - 5) {
    tone(sirenPin, czestotliwosc);
    Serial.println(czestotliwosc);
  }
  noTone(sirenPin);
  delay(300000);
}

void motor(int rotate, int analogIn, int analogOut) {
  for (n = 1; n <= 10; n++) {
    for (i = 1; i <= 200; i++) {

      rotate = analogRead(analogIn);  //read potentiometer
      if (rotate < 254) {
        rotate = 0;
      } else if (254 < rotate && rotate < 574) {
        rotate = 80;
      } else if (574 < rotate && rotate < 952) {
        rotate = 90;
      } else if (592 < rotate) {
        rotate = 100;
      }
      analogWrite(pwmA, rotate);

      current_mA = -(ina219.getCurrent_mA());
      numbera = numbera + current_mA;
    }
    numbera = numbera / 200;
    number = number + numbera;
    numbera = 0;
  }
  numberb = number / 10;

  if (rotate == 80 && numberb > 50 || rotate == 90 && numberb > 60 || rotate == 100 && numberb > 70) {  //turning on alarm, if too high ampers
    if (alarmTest == 0) {
      alarmTest = 1;
    } else if (alarmTest == 1) {
      alarm = 1;
    }
  } else {
    alarmTest = 0;    
  }
  number = 0;


  Serial.print(analogRead(analogIn));
  Serial.print("\t");
  Serial.print(numberb);
  Serial.print("\t");
  Serial.print(rotate);
  Serial.println("\t");
}

void setup() {
  alarm = 0;

  pinMode(pwmA, OUTPUT);
  pinMode(pot, INPUT);
  pinMode(sirenPin, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(9600);

  digitalWrite(4, 1);
  digitalWrite(5, 0);
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
}

void loop() {
  if (alarm) {
    Serial.println("Uwaga!");
    digitalWrite(pwmA, 0);
    siren();
  } else {
    motor(motorSpeed, pot, pwmA);
  }
}