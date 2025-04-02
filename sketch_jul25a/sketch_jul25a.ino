#define buzzer A7
#define Pin1 4
#define Pin2 5
#define OK 6

#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 =10, d5 = 9 ,d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool aState;
bool aLastState; 
int counter = 0;

void setup() {
  pinMode (Pin1,INPUT);
  pinMode (Pin2,INPUT);
  pinMode (OK, INPUT_PULLUP);
  pinMode (buzzer, OUTPUT);
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print("hello, world!");

  //Serial.begin (9600);
  
}

void loop() {
  
}
