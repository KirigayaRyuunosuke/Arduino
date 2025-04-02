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
//początek miejsca na czujniki








//koniec miejsca na czujniki

//tu tworzymy zmienne:

long menu = 0;           //zmienna przechowująca pozycję usera w menu
int maxTemperatura = 0;  //no jak sama nazwa wskazuje
char klawisz;            //odczytany klawisz

// a tu resztę:

long wprowadzanie() {  //funkcja odpowiedzialna za wprowadzanie danych
  bool stan = 1;
  long liczba = 0;
  long liczbaCyfr = 0;
  while (stan) {
    liczbaCyfr = log10(liczba) + 1;     //oblicz ilość znaków przy pomocy logarytmu
    if (liczbaCyfr < 9) {               //limit 9 znaków
      klawisz = klawiatura.getKey();
      if (klawisz) {                    //kiedy naciśnięto przycisk
        switch (klawisz) {
          case '1':
            liczba = liczba * 10;
            liczba++;
            break;
          case '2':
            liczba = (liczba * 10) + 2;
            break;
          case '3':
            liczba = (liczba * 10) + 3;
            break;
          case '4':
            liczba = (liczba * 10) + 4;
            break;
          case '5':
            liczba = (liczba * 10) + 5;
            break;
          case '6':
            liczba = (liczba * 10) + 6;
            break;
          case '7':
            liczba = (liczba * 10) + 7;
            break;
          case '8':
            liczba = (liczba * 10) + 8;
            break;
          case '9':
            liczba = (liczba * 10) + 9;
            break;
          case '0':
            liczba = (liczba * 10);
            break;
          case 'K':
            stan = 0;
            break;
          case 'C':
            liczba = 0;
            break;
        }

        lcd.clear();                                  //fajne menu
        lcd.setCursor(0, 1);
        lcd.print("Wprowadz liczbe");
        lcd.setCursor(0, 2);
        lcd.print(liczba);
      }
    } else
      stan = 0;
  }
  return liczba;
}








void setup() {  //TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP
  pinMode(13, OUTPUT);
  analogWrite(13, 75);
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("MENU");
  lcd.setCursor(0, 1);
}




void loop() {  //TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP
  klawisz = klawiatura.getKey();
  if (klawisz) {  //kiedy naciśnięto przycisk
    switch (klawisz) {
      case 'g':  //zwiększ wartość,
        if (menu == 3) {
          menu = 0;
        } else {
          menu++;
        }
        break;
      case 'h':  //zmniejsz wartość,
        if (menu == 0) {
          menu = 3;
        } else {
          menu--;
        }
        break;
      case 'C':  //lub wyzeruj,
        menu = 1;
        break;
    }
  //menu = wprowadzanie();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MENU");
  lcd.setCursor(0, 1);
  Serial.println(menu);  //a potem wyświetl
  lcd.print(menu);
  }
}
