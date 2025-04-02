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
  { '0', 'K', ' ', ' ', 'a' }
};

Keypad klawiatura = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);  //inicjalizacja klawiatury

// koniec dodatwania klawiatury
//początek dodawanie LCD 20 x 04

#include <LiquidCrystal.h>

#define lcdPinPodswietlenia 37
const int rs = 27, en = 26, d4 = 25, d5 = 24, d6 = 23, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//koniec dodawania LCD, begin w setup()
//dodatkowe literki
byte literaL[8] = {  //małe ł
  0b01100,
  0b00100,
  0b00110,
  0b00100,
  0b01100,
  0b00100,
  0b01110,
  0b00000
};
byte literaZ[8] = {  //ż
  0b00100,
  0b00000,
  0b11111,
  0b00010,
  0b00100,
  0b01000,
  0b11111,
  0b00000
};
byte literaZZ[8] = {
  0b00010,
  0b00100,
  0b11111,
  0b00010,
  0b00100,
  0b01000,
  0b11111,
  0b00000
};
byte uZamkniete[8] = {  // ;D
  0b00010,
  0b00100,
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b01110,
  0b00000
};
byte st[8] = {  //stopień
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

//koniec literek
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
long lcdOdswiezanie;  // zmienna od odświeżania wyświetlacza
int menuPionowe = 1;  //zmienna przechowująca pozycję usera w menuPionowe
int menuPoziome = 1;
int LastMenu = 0;
long bazowaTemperatura = 0;
long maxTemperatura = 60;  //no jak sama nazwa wskazuje
long nrTel[3];             //nr tel alarmowego
int NrTel;                 //miejsce w menu nr telefonów
long PIN = 7915;
long pin = 0;
char klawisz;  //odczytany klawisz

// a tu resztę:

void (*resetFunc)(void) = 0;  //funkcja odpowiedzialna za reset

long wprowadzanie(long bufor, int ileCyfrMax, int ileCyfrMin, bool NrTemp) {  //funkcja odpowiedzialna za wprowadzanie danych
  bool stan = 1;
  long liczba = 0;
  long liczbaCyfr = 0;
  lcd.blink();
  lcd.clear();  //fajne menuPionowe
  lcd.setCursor(1, 1);
  lcd.print(" Wprowad");
  lcd.print((char)4);
  lcd.print(" ");
  if (NrTemp) {
    lcd.print("telefon");
    lcd.setCursor(5, 2);
    if (liczba > 0)
      lcd.print(liczba);
  } else {
    lcd.print("temp max");
    lcd.setCursor(8, 2);
    if (liczba > 0)
      lcd.print(liczba);
    lcd.print(liczba);
    lcd.print((char)3);
    lcd.print("C");
  }
  while (stan) {
    liczbaCyfr = log10(liczba) + 1;  //oblicz ilość znaków przy pomocy logarytmu
    if (liczbaCyfr < ileCyfrMax) {   //limit znaków
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
            if (liczbaCyfr > ileCyfrMin) {
              stan = 0;
              break;
            } else
              liczba = bufor;
            stan = 0;
            break;
          case 'C':
            if (liczba == 0) {
              liczba = bufor;
              stan = 0;
              break;
            }
            liczba = 0;
            lcd.setCursor(1, 2);
            lcd.print("                   ");
            break;
        }
        if (NrTemp) {
          lcd.setCursor(1, 2);
          if (liczba > 0)
            lcd.print(liczba);
        } else {
          lcd.setCursor(8, 2);
          if (liczba > 0)
            lcd.print(liczba);
          lcd.print((char)3);
          lcd.print("C");
        }
      }
    } else
      stan = 0;
  }
  return liczba;
}
void MenuLcd() {
  if (menuPoziome == 1) {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("PANEL ZBIORNIKA");
    lcd.setCursor(0, 3);
    lcd.print("TEMPERATURA I ALARMY");
    lcd.setCursor(4, 1);
    lcd.print("Nape");
    lcd.print((char)0);
    lcd.print("nianie");
    lcd.setCursor(4, 2);
    lcd.print("Opr");
    lcd.print((char)2);
    lcd.print((char)1);
    lcd.print("nianie");
    lcd.setCursor(0, menuPionowe);
    lcd.blink();
    LastMenu = menuPoziome;
  } else if (menuPoziome == 2) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("USTAWIENIA");
    lcd.setCursor(1, 3);
    lcd.print("   Test alarmu");
    lcd.setCursor(1, 1);
    lcd.print(" Podaj temp. max");
    lcd.setCursor(1, 2);
    lcd.print("Ustaw nr telefonu");
    lcd.setCursor(0, menuPionowe);
    lcd.blink();
    LastMenu = menuPoziome;
  }
}

void Menu() {  //funkcja pisząca menu
  bool stan = 1;
  MenuLcd();
  while (stan) {
    klawisz = klawiatura.getKey();
    if (klawisz) {  //kiedy naciśnięto przycisk
      switch (klawisz) {
        case 'C':
          resetFunc();
          break;
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
        case 'K':                 //wybierz
          switch (menuPionowe) {  //switch odpowiedzialny za nastawianie wartości
            case 1:
              switch (menuPoziome) {
                case 1:
                  Napelnianie();
                  break;
                case 2:
                  maxTemperatura = wprowadzanie(maxTemperatura, 3, 0, 0);
                  if (maxTemperatura > 99)
                    maxTemperatura = 99;
                  save(1, maxTemperatura);
                  resetFunc();
                  break;
              }
              break;
            case 2:
              switch (menuPoziome) {
                case 1:
                  Oproznianie();
                  break;
                case 2:
                  UstawNrTelefonu();
                  break;
              }
              break;
            case 3:
              switch (menuPoziome) {
                case 1:
                  menuPoziome = 2;
                  menuPionowe = 1;
                  break;
                case 2:
                  TestAlarmu();
                  break;
              }
              break;
          }

          break;
      }
      if (LastMenu != menuPoziome) {
        MenuLcd();
      }
      lcd.setCursor(0, menuPionowe);
      lcd.blink();
    }
    delay(250);
  }
}


void OdczytZCzujnikow() {
  bool stan = 1;
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(" Odczyt  pomiar");
  lcd.print((char)2);
  lcd.print("w");
  lcd.setCursor(1, 1);
  lcd.print(" Temp. max: ");
  lcd.print(maxTemperatura);
  lcd.setCursor(16, 1);
  lcd.print((char)3);
  lcd.print("C");
  lcd.setCursor(1, 2);
  lcd.print(" Czujnik 1: ");

  lcd.setCursor(16, 2);
  lcd.print((char)3);
  lcd.print("C");
  lcd.setCursor(1, 3);
  lcd.print(" Czujnik 2: ");

  lcd.setCursor(16, 3);
  lcd.print((char)3);
  lcd.print("C");
  lcd.setCursor(0, 0);
  while (stan) {
    klawisz = klawiatura.getKey();
    if (klawisz == 'a') {
      Menu();
    }
  }
}


void Napelnianie() {
  bool stan = 1;
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print("Nape");
  lcd.print((char)0);
  lcd.print("nianie");
  lcd.setCursor(0, 0);
  while (stan) {
    klawisz = klawiatura.getKey();
    if (klawisz == 'a') {
      Menu();
    }
  }
}


void Oproznianie() {
  bool stan = 1;
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print("Opr");
  lcd.print((char)2);
  lcd.print((char)1);
  lcd.print("nianie");
  lcd.setCursor(0, 0);
  while (stan) {
    klawisz = klawiatura.getKey();
    if (klawisz == 'a') {
      Menu();
    }
  }
}


void TestAlarmu() {
  bool stan = 1;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Test alarmu");
  lcd.setCursor(0, 0);
  while (stan) {
    klawisz = klawiatura.getKey();
    if (klawisz == 'a') {
      Menu();
    }
  }
}


void UstawNrTelefonu() {  //funkcja odpowiedzialna za menuPionowe nr telefonu
  NrTel = 1;
  bool stan = 1;
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Numery alarmowe:");
  lcd.setCursor(4, 1);
  lcd.print("1. ");
  lcd.print(nrTel[0]);
  lcd.setCursor(4, 2);
  lcd.print("2. ");
  lcd.print(nrTel[1]);
  lcd.setCursor(4, 3);
  lcd.print("3. ");
  lcd.print(nrTel[2]);
  lcd.setCursor(4, NrTel);

  while (stan) {
    klawisz = klawiatura.getKey();
    if (klawisz) {  //kiedy naciśnięto przycisk
      switch (klawisz) {
        case 'h':  //zwiększ wartość,
          if (NrTel == 3) {
            NrTel = 1;
          } else {
            NrTel++;
          }
          break;

        case 'g':  //zmniejsz wartość,
          if (NrTel == 1) {
            NrTel = 3;
          } else {
            NrTel--;
          }
          break;

        case 'a':
          Menu();
          break;

        case 'K':
          switch (NrTel) {
            case 1:
              nrTel[0] = wprowadzanie(nrTel[0], 9, 9, 1);
              save(5, nrTel[0]);
              //resetFunc();
              break;
            case 2:
              nrTel[1] = wprowadzanie(nrTel[1], 9, 9, 1);
              save(9, nrTel[1]);
              //resetFunc();
              break;
            case 3:
              nrTel[2] = wprowadzanie(nrTel[2], 9, 9, 1);
              save(13, nrTel[2]);
              //resetFunc();
              break;
          }
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Numery alarmowe:");
          lcd.setCursor(1, 1);
          lcd.print("1. ");
          lcd.print(nrTel[0]);
          lcd.setCursor(1, 2);
          lcd.print("2. ");
          lcd.print(nrTel[1]);
          lcd.setCursor(1, 3);
          lcd.print("3. ");
          lcd.print(nrTel[2]);
          lcd.setCursor(4, NrTel);
          break;
      }
      lcd.setCursor(0, NrTel);
    }
  }
}


void setup() {  //TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUPmaxTemperatura = load(1);
  for (int i = 1; i < 4; i = i + 1) {
    nrTel[i - 1] = load(1 + (i * 4));
  }
  maxTemperatura = load(1);
  lcd.begin(20, 4);

  pinMode(lcdPinPodswietlenia, OUTPUT);
  digitalWrite(lcdPinPodswietlenia, 1);

  lcd.createChar(0, literaL);
  lcd.createChar(1, literaZ);
  lcd.createChar(2, uZamkniete);
  lcd.createChar(3, st);
  lcd.createChar(4, literaZZ);

  Serial.begin(9600);

  lcd.setCursor(0, menuPionowe);
  lcd.blink();
}


void loop() {  //TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP
  OdczytZCzujnikow();
}