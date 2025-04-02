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

long menu = 1;  //zmienna przechowująca pozycję usera w menu
long bazowaTemperatura = 60;
long maxTemperatura = 60;  //no jak sama nazwa wskazuje
long nrTel;                //nr tel alarmowego
long PIN = 7915;
long pin = 0;
char klawisz;  //odczytany klawisz

// a tu resztę:

long wprowadzanie(long bufor, int ileCyfr) {  //funkcja odpowiedzialna za wprowadzanie danych
  bool stan = 1;
  long liczba = bufor;
  long liczbaCyfr = 0;
  while (stan) {
    lcd.clear();  //fajne menu
    lcd.setCursor(1, 1);
    lcd.print("Wprowadz liczbe");
    lcd.setCursor(1, 2);
    lcd.print(liczba);
    liczbaCyfr = log10(liczba) + 1;  //oblicz ilość znaków przy pomocy logarytmu
    if (liczbaCyfr < ileCyfr) {      //limit znaków
      klawisz = klawiatura.getKey();
      if (klawisz) {  //kiedy naciśnięto przycisk
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
        lcd.clear();  //fajne menu
        lcd.setCursor(1, 1);
        lcd.print("Wprowadz liczbe");
        lcd.setCursor(1, 2);
        lcd.print(liczba);
      }
    } else
      stan = 0;
    delay(100);
  }
  return liczba;
}

void Menu() {  //funkcja pisząca menu
  menu = 1;
  bool stan = 1;
  while (stan) {
    klawisz = klawiatura.getKey();
    if (klawisz) {  //kiedy naciśnięto przycisk
      switch (klawisz) {
        case 'h':  //zwiększ wartość,
          if (menu == 3) {
            menu = 1;
          } else {
            menu++;
          }
          break;
        case 'g':  //zmniejsz wartość,
          if (menu == 1) {
            menu = 3;
          } else {
            menu--;
          }
          break;
        case 'K':          //wybierz
          switch (menu) {  //switch odpowiedzialny za nastawianie wartości
            case 2:
              MaksymalneTemperatury();
              break;
            case 3:
              UstawNrTelefonu();
              break;
            case 1:
              OdczytZCzujnikow();
              break;
          }

          break;
      }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MENU");
    lcd.setCursor(1, 1);
    lcd.print("Odczyt z czujnikow");
    lcd.setCursor(1, 2);
    lcd.print("Wartosci max");
    lcd.setCursor(1, 3);
    lcd.print("Ustaw nr telefonu");
    lcd.setCursor(0, menu);
    lcd.blink();
    delay(100);
  }
}


void OdczytZCzujnikow() {
  bool stan = 1;
  while (stan) {
    klawisz = klawiatura.getKey();
    if (klawisz == 'a') {
      Menu();
    } else {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Odczyt z czujnikow");
      lcd.setCursor(1, 2);
      lcd.print("Nr 1:");
      lcd.setCursor(1, 3);
      lcd.print("Nr 2:");
      lcd.setCursor(0, 0);
      delay(100);
    }
  }
}


void MaksymalneTemperatury() {  //funkcja odpowiedzialna za menu temperatur maksymalnych
  bool stan = 1;
  while (stan) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Temperatura max:");
    lcd.setCursor(1, 1);
    lcd.print(maxTemperatura);
    lcd.print("'C");
    lcd.setCursor(0, 0);
    klawisz = klawiatura.getKey();
    if (klawisz) {  //kiedy naciśnięto przycisk
      switch (klawisz) {
        case 'a':
          Menu();
          break;
        case 'K':
          maxTemperatura = wprowadzanie(bazowaTemperatura, 3);
          if (maxTemperatura > 100)
            maxTemperatura = 100;
          break;
      }
    }
    delay(100);
  }
}

void UstawNrTelefonu() {  //funkcja odpowiedzialna za menu nr telefonu
  bool stan = 1;
  while (stan) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Nr tel alarmowego:");
    lcd.setCursor(1, 1);
    lcd.print(nrTel);
    lcd.setCursor(0, 0);
    klawisz = klawiatura.getKey();
    if (klawisz) {  //kiedy naciśnięto przycisk
      switch (klawisz) {
        case 'a':
          Menu();
          break;
        case 'K':
          nrTel = wprowadzanie(0, 9);


          break;
      }
    }

    delay(100);
  }
}


void setup() {  //TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP
  pinMode(13, OUTPUT);
  analogWrite(13, 75);
  Serial.begin(9600);
  lcd.begin(20, 4);

  lcd.setCursor(0, menu);
  lcd.blink();
}


void loop() {  //TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP
  OdczytZCzujnikow();
}