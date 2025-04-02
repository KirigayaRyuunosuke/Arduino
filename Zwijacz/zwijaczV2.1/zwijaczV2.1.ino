#include "Wire.h"
#include "Adafruit_INA219.h"

#define dirA 12
#define pwmA 3
#define pot A2
#define sirenPin A0

Adafruit_INA219 ina219;

bool alarm = 0;
int n = 0;
int i = 0;
int motorSpeed = 0;
float current_mA = 0.0000;
float number = 0.0000;
float numbera = 0.0000;
float numberb = 0.0000;

void siren() {
  int czestotliwosc = 0;
  for (czestotliwosc = 2000; czestotliwosc < 2400; czestotliwosc++) {
    tone(sirenPin, czestotliwosc);
    Serial.println(czestotliwosc);
  }
  for (czestotliwosc = 2400; czestotliwosc > 2000; czestotliwosc--) {
    tone(sirenPin, czestotliwosc);
    Serial.println(czestotliwosc);
  }
  noTone(sirenPin);
  delay(300000);
}

void motor(int rotate, int analogIn, int analogOut, int min, int max) {
  for (n = 1; n <= 10; n++) {
    for (i = 1; i <= 200; i++) {

      rotate = analogRead(analogIn);                //read potentiometer
      rotate = map(rotate, 0, 1023, min - 2, max);  //map analog to PWM

      if (rotate < min || rotate > max)  //cut too low and too high speed
        analogWrite(pwmA, 0);
      else
        analogWrite(pwmA, rotate);


      current_mA = -(ina219.getCurrent_mA());
      numbera = numbera + current_mA;
    }
    numbera = numbera / 200;
    number = number + numbera;
    numbera = 0;
  }
  numberb = number / 10;
  if (numberb > 29 && analogRead(analogIn) < 256 || numberb > 35 && analogRead(analogIn) < 512 || numberb > 40 && analogRead(analogIn) < 768 || numberb > 45 && analogRead(analogIn) >= 768){  //turning on alarm, if too high ampers
    //alarm = 1;
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

  pinMode(dirA, OUTPUT);
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
    motor(motorSpeed, pot, pwmA, 80, 100);
  }
}
