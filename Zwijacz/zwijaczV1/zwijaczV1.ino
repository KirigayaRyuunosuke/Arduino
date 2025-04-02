//sterownik silnika z zabezpieczeniem prądowym v1

#define dirA 12
#define pwmA 3
#define pot A0

int motor = 0;

void motorSpeed(int rotate, int analoigIn, int analogOut, int min, int max){
  rotate = analogRead(analoigIn);           //read potentiometer 
  rotate = map(rotate, 0, 1023, 30, 500);   //map analog to PWM

  if(rotate<min || rotate>max)               //cut too low and too high speed
    analogWrite(pwmA,0);
  else
    analogWrite(pwmA, rotate);

  /*Serial.print(rotate);                      //reporting 
  Serial.print(" - ");
  Serial.println(analogRead(pot));
  */
}

void ampers(){}

void setup() {
  pinMode(dirA, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(pot, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  motorSpeed(motor, pot, pwmA, 45, 120);
}
