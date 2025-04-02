//sterownik silnika z zabezpieczeniem prądowym v2
#include "Wire.h"
#include "Adafruit_INA219.h"

#define dirA 12
#define pwmA 3
#define pot A2

Adafruit_INA219 ina219;

bool alarm = 0;
int i = 0;
int motor = 0;
float current_mA = 0;
float mA[250];
float number = 0;

void siren() {
  int czestotliwosc = 0;
  for (czestotliwosc = 2000; czestotliwosc < 2400; czestotliwosc++) {
    tone(A5, czestotliwosc);
    Serial.println(czestotliwosc);
  }
  for (int i = 0; i < 10; i++) {
    tone(A5, 2000);
    delay(100);
    tone(A5, 2400);
    delay(100);
  }
}

void motorSpeed(int rotate, int analogIn, int analogOut, int min, int max) {
  rotate = analogRead(analogIn);           //read potentiometer
  rotate = map(rotate, 0, 1023, 30, 70);  //map analog to PWM

  if (rotate < min || rotate > max)  //cut too low and too high speed
    analogWrite(pwmA, 0);
  else
    analogWrite(pwmA, rotate);
  Serial.print(rotate);
  Serial.print("\t");
  //Serial.print(rotate);                      //reporting
  //Serial.print(" - ");
  //Serial.println(analogRead(pot));
}

void ampers(float analogIn, float max) {
  int rotate = analogRead(analogIn);  //read potentiometer
  rotate = map(rotate, 0, 1023, 0, 70);
  int currentMax = 0;
  current_mA = ina219.getCurrent_mA();
  for (i = 1; i <= 250; i++) {
    current_mA = ina219.getCurrent_mA();
    mA[i] = current_mA;
    number = number + mA[i];
  }
  number = number / 250;
  if (number>35)
    alarm = 1;
  Serial.print(analogRead(analogIn));
  Serial.print("\t");
  Serial.println(number);
}

void setup() {

  alarm = 0;

  pinMode(dirA, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(pot, INPUT);

  Serial.begin(9600);

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
    motorSpeed(motor, pot, pwmA, 45, 70);
    ampers(pot, 35);
  }
}