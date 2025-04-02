#include <ESP32Servo.h>

#define potX 2
#define potY 15
#define ServoPin1 17
#define ServoPin2 16

Servo sg901;
Servo sg902;

int analogValue = 0;

void setup() {
  sg901.setPeriodHertz(50);  // PWM frequency for SG90
  sg901.attach(ServoPin1, 500, 2400);
   sg902.setPeriodHertz(50);  // PWM frequency for SG90
  sg902.attach(ServoPin2, 500, 2400);
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  analogValue = analogRead(potX);
  int degree = map(analogValue, 0, 4096, 0, 180);
  sg901.write(degree);

  analogValue = analogRead(potY);
  degree = map(analogValue, 0, 4096, 0, 180);
  sg902.write(degree);
}
