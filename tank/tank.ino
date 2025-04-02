#define pwmA 3
#define dir1 4
#define dir2 5

#define pwmB 6
#define dir3 7
#define dir4 8

void LewyWstecz(int speed){
  digitalWrite(dir1, 0);
  digitalWrite(dir2, 1);
  analogWrite(pwmA, speed);
}

void LewyDoPrzodu(int speed){
  digitalWrite(dir1, 1);
  digitalWrite(dir2, 0);
  analogWrite(pwmA, speed);
}

void PrawyWstecz(int speed){
  digitalWrite(dir3, 0);
  digitalWrite(dir4, 1);
  analogWrite(pwmB, speed);
}

void PrawyDoPrzodu(int speed){
  digitalWrite(dir3, 1);
  digitalWrite(dir4, 0);
  analogWrite(pwmB, speed);
}

void stop(){
  analogWrite(pwmB, 0);
  analogWrite(pwmA, 0);
}

void setup() {
  pinMode(pwmA, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(dir4, OUTPUT);

  //LewyWstecz(100);
  PrawyDoPrzodu(50);
  //delay(1000);
  //stop();
}

void loop() {
  // put your main code here, to run repeatedly:
}
