//PWM TESTER

#define pwm 3

void setup() {
  pinMode(pwm, OUTPUT);
}

void loop() {
  analogWrite(3, analogRead(A2) / 4);
}
