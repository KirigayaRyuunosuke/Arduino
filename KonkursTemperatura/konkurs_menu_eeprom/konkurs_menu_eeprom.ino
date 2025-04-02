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

#define lcdPinKontrastu 13
#define lcdPinPodswietlenia 37
const int rs = 27, en = 26, d4 = 25, d5 = 24, d6 = 23, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//koniec dodawania LCD, begin w setup()
//początek miejsca na czujniki








//koniec miejsca na czujniki
//konfigaracja EEPROM-u

#include <EEPROM.h>

void save(int adres, long value) {   // UWAGA!!! value zajmuje cztery bajty!!!
  byte jeden = (value & 0xFF);       // nie chce mi sie dzielic liczb
  byte dwa = ((value >> 8) & 0xFF);  //wiec zrobie przesuniecie bitowe
  byte trzy = ((value >> 16) & 0xFF);
  byte cztery = ((value >> 24) & 0xFF);

  EEPROM.update(adres, jeden);     // jesli wartosc sie nie zmienila
  EEPROM.update(adres + 1, dwa);   // to nie zapisuj
  EEPROM.update(adres + 2, trzy);  // to zaoszczedzi zywotnosci kosci
  EEPROM.update(adres + 3, cztery);
}

long load(long adres) {
  long cztery = EEPROM.read(adres);
  long trzy = EEPROM.read(adres + 1);
  long dwa = EEPROM.read(adres + 2);
  long jeden = EEPROM.read(adres + 3);

  return ((cztery << 0) & 0xFF) + ((trzy << 8) & 0xFFFF) + ((dwa << 16) & 0xFFFFFF) + ((jeden << 24) & 0xFFFFFFFF);
}

//koniec konfiguracji
//tu tworzymy zmienne:

long menuPionowe = 1;  //zmienna przechowująca pozycję usera w menuPionowe
long menuPoziome = 1;
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
    lcd.clear();  //fajne menuPionowe
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
        lcd.clear();  //fajne menuPionowe
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

void Menu() {  //funkcja pisząca menuPionowe
  menuPionowe = 1;
  bool stan = 1;
  while (stan) {
    klawisz = klawiatura.getKey();
    if (klawisz) {  //kiedy naciśnięto przycisk
      switch (klawisz) {
        case 'h':  //zwiększ wartość,
          if (menuPionowe == 3) {
            menuPionowe = 1;
          } else {
            menuPionowe++;
          }
          break;
        case 'g':  //zmniejsz wartość,
          if (menuPionowe == 1) {
            menuPionowe = 3;
          } else {
            menuPionowe--;
          }
          break;
        case 'K':          //wybierz
          switch (menuPionowe) {  //switch odpowiedzialny za nastawianie wartości
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
    lcd.setCursor(0, menuPionowe);
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


void MaksymalneTemperatury() {  //funkcja odpowiedzialna za menuPionowe temperatur maksymalnych
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
          save(1, maxTemperatura);
          break;
      }
    }
    delay(100);
  }
}


void UstawNrTelefonu() {  //funkcja odpowiedzialna za menuPionowe nr telefonu
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
          save(5, nrTel);


          break;
      }
    }

    delay(100);
  }
}


void setup() {  //TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP
  pinMode(lcdPinKontrastu, OUTPUT);
  analogWrite(lcdPinKontrastu, 75);
  pinMode(lcdPinPodswietlenia, OUTPUT);
  digitalWrite(lcdPinPodswietlenia, 1);
  Serial.begin(9600);
  lcd.begin(20, 4);

  lcd.setCursor(0, menuPionowe);
  lcd.blink();

  maxTemperatura = load(1);
  nrTel = load(5);
}


void loop() {  //TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP
  OdczytZCzujnikow();
}