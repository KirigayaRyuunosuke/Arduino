//################################################
//  "URUCAHMIANIE"
//  "GOTOWE"
//  "CZAS 1"
//  "min" "sek"
//  "CZAS 2"
//  "min" "sek"
//################################################
// funkcja trybu
// funkcja wprowadzania
// funkcja odliczania
//################################################

#define buzzer 3
#define Pin1 12
#define Pin2 11
#define OK 10

#include <LiquidCrystal.h>
const int rs = 8, en = 9, d4 = 7, d5 = 6 , d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool aState;
bool aLastState;

int counter = 0;
int lastCounter = 2;

unsigned long previousTime = 0;
unsigned long myTime;
bool mode; // 0 - input, 1 - counting down

int timer1 = 3;
int timer2 = 3;

int time;

int sec1 = 0;
int min1 = 0;
int sec2 = 0;
int min2 = 0;

void displayTime(int timer) {
  if ( (timer / 60) != 0) {
    lcd.print(timer / 60 );
    lcd.print("m ");
  }
  lcd.print(timer % 60 );
  lcd.print("s");
}

void displayTimes(int time1, int time2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CZAS 1    CZAS 2");
  lcd.setCursor(0, 1);
  displayTime(time1);
  lcd.setCursor(9, 1);
  displayTime(time2);
}

void countingMashine(int time1, int time2, unsigned long delayTimer) {
  int bufor1 = time1;
  int bufor2 = time2;
  bool mode = 1;
  previousTime = delayTimer;
  displayTimes(time1, time2);
  delay(250);
  while (1) {
    delayTimer = millis();
    if (((delayTimer - previousTime) >= 1000)) {
      if (mode) {
        time1--;
      } else {
        time2--;
      }
      previousTime = delayTimer;
      displayTimes(time1, time2);
      if (mode && (time1 == 0)) {
        mode = 0;
        time1 = bufor1;
        bip();
      } else if (!mode && (time2 == 0)) {
        mode = 1;
        time2 = bufor2;
        bip();
      }
    }
    if (!digitalRead(OK)) {
      lcd.clear();
      delay(250);
      return;
    }
  }
}

void inputMode() {}



void howToSwitch(int mode) {
  switch (mode) {
    case 1:
      break;
    case 2:
      break;
  }
}

void bip() {
  //tone(buzzer, 1100, 200);
  //delay(200);
  //tone(buzzer, 1400, 500);
}

void setup() {
  pinMode (Pin1, INPUT_PULLUP);
  pinMode (Pin2, INPUT_PULLUP);
  pinMode (OK, INPUT_PULLUP);
  pinMode (buzzer, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("");

  tone(buzzer, 1000, 1000);

  Serial.begin(9600);

  aState = digitalRead(Pin1);
  aLastState = aState;
}

void loop() {
  myTime = millis();
  

  aState = digitalRead(Pin1); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState) {  // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(Pin2) == aState) {
      counter++;
    } else {
      counter--;
    }
  }

  if (digitalRead(OK) == 0) {
    countingMashine(timer1, timer2, myTime);
  }
  if (lastCounter != counter) {
    Serial.println(counter);
    lastCounter = counter;
    lcd.clear();
    lcd.setCursor(0, 0);
    displayTime(counter/2);
  }



  delay(1);
  aLastState = aState; // Updates the previous state of the outputA with the current state

}
