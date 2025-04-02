//początek dodawania klawiatury

#include <Keypad.h>  //biblioteka od klawiatury

const byte ROWS = 4;  // ile wierszy
const byte COLS = 5;  //ile kolumn

byte rowPins[ROWS] = { 28, 29, 30, 31 };      //piny wierszy
byte colPins[COLS] = { 32, 33, 34, 35, 36 };  //piny kolum

char keys[ROWS][COLS] = {  //mapowanie klawiatury
  { '7', '8', '9', 'C', ' ' },
  { '4', '5', '6', 'g', ' ' },
  { '1', '2', '3', 'h', ' ' },
  { '0', 'K', 'K', ' ', 'a' }
};

Keypad klawiatura = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);  //inicjalizacja klawiatury

// koniec dodatwania klawiatury
//początek dodawanie LCD 20 x 04

#include <LiquidCrystal.h>

#define lcdPinPodswietlenia 37
const int rs = 27, en = 26, d4 = 25, d5 = 24, d6 = 23, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//koniec dodawania LCD, begin w setup()

void setup() {
  
  pinMode(lcdPinPodswietlenia, OUTPUT);
  digitalWrite(lcdPinPodswietlenia, 1);
  pinMode(13, OUTPUT);
  analogWrite(13, 75);
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
}

void loop() {
  char klawisz = klawiatura.getKey();

  if (klawisz) {
    lcd.print(klawisz);
    Serial.println(klawisz);
    lcd.setCursor(0, 0);
  }
}
