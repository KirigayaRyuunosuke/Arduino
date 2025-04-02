//początek dodawania klawiatury

#include <Keypad.h>  //biblioteka od klawiatury

const byte ROWS = 4;  // ile wierszy
const byte COLS = 5;  //ile kolumn

byte rowPins[ROWS] = { 28, 29, 30, 31 };      //piny wierszy
byte colPins[COLS] = { 32, 33, 34, 35, 36 };  //piny kolum

char keys[ROWS][COLS] = {  //mapowanie klawiatury
  { '7', '8', '9', 'a', 'C' },
  { '4', '5', '6', 'b', 'g' },
  { '1', '2', '3', 'c', 'h' },
  { '0', 'e', 'f', 'd', 'K' }
};

Keypad klawiatura = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);  //inicjalizacja klawiatury

// koniec dodatwania klawiatury
//początek dodawanie LCD 20 x 04

#include <LiquidCrystal.h>

const int rs = 27, en = 26, d4 = 25, d5 = 24, d6 = 23, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//koniec dodawania LCD, begin w setup()

int menu = 0;  //zmienna przechowująca pozycję usera w menu


void setup() {                                //TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP 
  pinMode(13, OUTPUT);
  analogWrite(13, 75);
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("MENU");
  lcd.setCursor(0, 1);
}




void loop() {                                 //TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP
  char klawisz = klawiatura.getKey();
  if (klawisz) {  //kiedy naciśnięto przycisk
    switch (klawisz) {
      case 'g':  //zwiększ wartość,
        if (menu == 255) {
          menu = 0;
        } else {
          menu++;
        }
        break;
      case 'h':  //zmniejsz wartość,
        if (menu == 0) {
          menu = 255;
        } else {
          menu--;
        }
        break;
      case 'C':  //lub wyzeruj,
        menu = 0;
        break;
    }
    Serial.println(menu);  //a potem wyświetl
    lcd.print("                    ");
    lcd.setCursor(0, 1);
    lcd.print(menu);
    lcd.setCursor(0, 1);
  }
}