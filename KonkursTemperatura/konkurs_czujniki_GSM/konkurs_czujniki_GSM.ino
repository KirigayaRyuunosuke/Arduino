//tu tworzymy zmienne:
#define buzzer A0

unsigned long aktualnyCzas = 0;
unsigned long lcdOdswiezanie = 0;  // zmienna od odświeżania wyświetlacza
unsigned long czasMenu = 0;
unsigned long czasPika = 0;
int menuPionowe = 1;  //zmienna przechowująca pozycję usera w menuPionowe
int menuPoziome = 1;
int LastMenu = 0;
long bazowaTemperatura = 0;
long maxTemperatura;  //no jak sama nazwa wskazuje

long nrTel[3];  //nr tel Alarmowego
int NrTel;      //kursor w menu nr telefonów

long PIN = 7915;
long pin = 0;
char klawisz;  //odczytany klawisz
int alarm = 0;

bool stanSMS = 0;

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

#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(A1);                  //Podłączenie do A1
DallasTemperature sensors(&oneWire);  //Przekazania informacji do biblioteki

DeviceAddress temperaturaDol = { 0x28, 0x6, 0x6A, 0x97, 0x94, 0x8, 0x3, 0x45 };
DeviceAddress temperaturaGora = { 0x28, 0x30, 0x5C, 0x97, 0x94, 0x8, 0x3, 0x2 };

#define zbiornikPusty 13
#define zbiornikPelny 12

//koniec miejsca na czujniki
//początek pomp

#define pompa1 38
#define pompa2 39

//koniec pomp
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
//konfig GSM

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    Serial3.write(Serial.read());  //Forward what Serial received to Software Serial Port
  }
  while (Serial3.available()) {
    Serial.write(Serial3.read());  //Forward what Software Serial received to Serial Port
  }
}

void sendSMS(String TrescSMS) {
  if (nrTel[0] != 0) {
    tone(buzzer, 2000);
    Serial3.println("AT+CMGF=1");  // Configuring TEXT mode
    updateSerial();
    Serial3.print("AT+CMGS=\"+48");  //change ZZ with country code and xxxxxxxxxxx with phone number to sms
    Serial3.print(nrTel[0]);
    Serial3.println("\"");
    noTone(buzzer);
    updateSerial();
    tone(buzzer, 2000);
    Serial3.print(TrescSMS);  //text content
    updateSerial();
    Serial3.write(26);
    noTone(buzzer);
    delay(3000);
  }
  if (nrTel[1] != 0) {
    tone(buzzer, 2000);
    Serial3.println("AT+CMGF=1");  // Configuring TEXT mode
    updateSerial();
    Serial3.print("AT+CMGS=\"+48");  //change ZZ with country code and xxxxxxxxxxx with phone number to sms
    Serial3.print(nrTel[1]);
    Serial3.println("\"");
    noTone(buzzer);
    updateSerial();
    tone(buzzer, 2000);
    Serial3.print(TrescSMS);  //text content
    updateSerial();
    Serial3.write(26);
    noTone(buzzer);
    delay(3000);
  }
  if (nrTel[2] != 0) {
    tone(buzzer, 2000);
    Serial3.println("AT+CMGF=1");  // Configuring TEXT mode
    updateSerial();
    Serial3.print("AT+CMGS=\"+48");  //change ZZ with country code and xxxxxxxxxxx with phone number to sms
    Serial3.print(nrTel[2]);
    Serial3.println("\"");
    noTone(buzzer);
    updateSerial();
    tone(buzzer, 2000);
    Serial3.print(TrescSMS);  //text content
    updateSerial();
    Serial3.write(26);
    noTone(buzzer);
    delay(3000);
  }
}


//koniec GSM

// a tu resztę:

void (*resetFunc)(void) = 0;  //funkcja odpowiedzialna za reset

long wprowadzanie(long bufor, int ileCyfrMax, int ileCyfrMin, bool NrTemp) {  //funkcja odpowiedzialna za wprowadzanie danych
  czasMenu = aktualnyCzas;
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
    lcd.setCursor(5 + liczbaCyfr, 2);
  } else {
    lcd.print("temp max");
    lcd.setCursor(8, 2);
    lcd.print(liczba);
    lcd.print((char)3);
    lcd.print("C");
    lcd.setCursor(8 + liczbaCyfr, 2);
    liczbaCyfr = log10(liczba) + 1;  //oblicz ilość znaków przy pomocy logarytmu
  }
  while (stan) {
    aktualnyCzas = millis();
    if (aktualnyCzas - czasMenu >= 30000) {
      resetFunc();
    }
    if (liczbaCyfr < ileCyfrMax) {  //limit znaków
      klawisz = klawiatura.getKey();
      if (klawisz) {  //kiedy naciśnięto przycisk
        czasMenu = aktualnyCzas;
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
            if (liczba == 0) {
              return 0;
              break;
            }
            if (liczbaCyfr > ileCyfrMin) {
              stan = 0;
              break;
            } else
              liczba = bufor;
            stan = 0;
            break;
          case 'C':
            if (liczba == 0) {
              return bufor;
              break;
            }
            liczba = 0;
            lcd.setCursor(1, 2);
            lcd.print("                   ");
            break;
        }
        liczbaCyfr = log10(liczba) + 1;  //oblicz ilość znaków przy pomocy logarytmux
        if (NrTemp) {
          lcd.setCursor(1, 2);
          if (liczba > 0)
            lcd.print(liczba);
        } else {
          lcd.setCursor(8, 2);
          // if (liczba > 0)
          lcd.print(liczba);
          lcd.print((char)3);
          lcd.print("C");
          lcd.setCursor(8 + liczbaCyfr, 2);
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
    lcd.print("   Test Alarmu");
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
  czasMenu = aktualnyCzas;
  bool stan = 1;
  MenuLcd();
  while (stan) {
    aktualnyCzas = millis();
    if (aktualnyCzas - czasMenu >= 10000) {
      resetFunc();
    }
    klawisz = klawiatura.getKey();
    if (klawisz) {  //kiedy naciśnięto przycisk
      czasMenu = aktualnyCzas;
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
                  MenuLcd();
                  break;
                case 2:
                  int prevTemp = maxTemperatura;
                  maxTemperatura = wprowadzanie(maxTemperatura, 3, 0, 0);
                  if (maxTemperatura > 99)
                    maxTemperatura = 99;
                  save(1, maxTemperatura);
                  if (prevTemp == maxTemperatura) {
                    MenuLcd();
                  } else
                    resetFunc();
                  break;
              }
              break;
            case 2:
              switch (menuPoziome) {
                case 1:
                  Oproznianie();
                  MenuLcd();
                  break;
                case 2:
                  UstawNrTelefonu();
                  MenuLcd();
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
                  sendSMS("Test");
                  //Alarm(1);
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
  }
}

void OdczytZCzujnikow() {
  bool stan = 1;
  sensors.requestTemperatures();
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(" Odczyt  pomiar");
  lcd.print((char)2);
  lcd.print("w");
  lcd.setCursor(1, 1);
  Alarm(1);
  lcd.setCursor(1, 2);
  lcd.print(" Czujnik 1:");
  lcd.print(sensors.getTempC(temperaturaDol));
  lcd.setCursor(16, 2);
  lcd.print((char)3);
  lcd.print("C");
  lcd.setCursor(1, 3);
  lcd.print(" Czujnik 2:");
  lcd.print(sensors.getTempC(temperaturaGora));
  lcd.setCursor(16, 3);
  lcd.print((char)3);
  lcd.print("C");
  lcd.setCursor(0, 0);
  while (stan) {



    aktualnyCzas = millis();
    if (aktualnyCzas - lcdOdswiezanie >= 1000) {
      lcdOdswiezanie = aktualnyCzas;
      Alarm(1);
      //Serial.print(digitalRead(zbiornikPusty), " ");
      //Serial.println(digitalRead(zbiornikPelny));
      sensors.requestTemperatures();
      lcd.setCursor(1, 2);
      lcd.print(" Czujnik 1:");
      lcd.print(sensors.getTempC(temperaturaGora));
      lcd.setCursor(16, 2);
      lcd.print((char)3);
      lcd.print("C");
      lcd.setCursor(1, 3);
      lcd.print(" Czujnik 2:");
      lcd.print(sensors.getTempC(temperaturaDol));
      lcd.setCursor(16, 3);
      lcd.print((char)3);
      lcd.print("C");
      lcd.setCursor(0, 0);
    }
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
    if (klawisz == 'C') {
      digitalWrite(pompa1, 1);
      tone(buzzer, 2000);
      delay(150);
      noTone(buzzer);
      return;
    }
    if (digitalRead(zbiornikPelny)) {
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("Zbiornik Pe");
      lcd.print((char)0);
      lcd.print("ny");
      digitalWrite(pompa1, 1);
      tone(buzzer, 2000);
      delay(150);
      noTone(buzzer);
      delay(2500);
      return;
    } else {
      digitalWrite(pompa1, 0);
      //Alarm(0);
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
    if (klawisz == 'C') {
      digitalWrite(pompa2, 1);
      tone(buzzer, 2000);
      delay(150);
      noTone(buzzer);
      return;
    }
    if (digitalRead(zbiornikPusty)) {
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("Zbiornik Pusty");
      digitalWrite(pompa2, 1);
      tone(buzzer, 2000);
      delay(150);
      noTone(buzzer);
      delay(2500);
      return;
    } else {
      digitalWrite(pompa2, 0);
      //Alarm(0);
    }
  }
}

void Alarm(bool stan) {

  int alarm;
  if (digitalRead(zbiornikPusty) == 1 && digitalRead(zbiornikPelny) == 1) {
    alarm = 1;
  } else if (sensors.getTempC(temperaturaDol) > maxTemperatura) {
    alarm = 2;
  } else if (sensors.getTempC(temperaturaGora) > maxTemperatura) {
    alarm = 3;
  } else
    alarm = 0;
  switch (alarm) {
    case 0:
      if (stan) {
        lcd.setCursor(0, 1);
        lcd.print("                    ");
        lcd.setCursor(1, 1);
        lcd.print(" Temp. max: ");
        lcd.print(maxTemperatura);
        lcd.setCursor(16, 1);
        lcd.print((char)3);
        lcd.print("C");
        noTone(buzzer);
      } else {
      }
      break;
    case 1:
      if (stan) {
        lcd.setCursor(0, 1);
        lcd.print("                    ");
        lcd.setCursor(2, 1);
        lcd.print("Uszkodzany p");
        lcd.print((char)0);
        lcd.print("ywak");
        if (stanSMS) {
          sendSMS("Uszkodzony pływak");
          stanSMS = 0;
        }
        Pikanie();
      } else {
        resetFunc();
      }
      break;
    case 2:
      if (stan) {
        lcd.setCursor(0, 1);
        lcd.print("                    ");
        lcd.setCursor(2, 1);
        lcd.print("Cz.2 > Temp. max");
        if (stanSMS) {
          sendSMS("Czujnik 2 - przekroczona temperatura maksymalna");
          stanSMS = 0;
        }
        Pikanie();
      } else {
        resetFunc();
      }
      break;
    case 3:
      if (stan) {
        lcd.setCursor(0, 1);
        lcd.print("                    ");
        lcd.setCursor(2, 1);
        lcd.print("Cz.1 > Temp. max");
        if (stanSMS) {
          sendSMS("Czujnik 1 - przekroczona temperatura maksymalna");
          stanSMS = 0;
        }
        Pikanie();
      } else {
        resetFunc();
      }
      break;
  }
}

void UstawNrTelefonu() {  //funkcja odpowiedzialna za menuPionowe nr telefonu
  NrTel = 1;
  bool stan = 1;
  czasMenu = aktualnyCzas;
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Numery Alarmowe:");
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
    aktualnyCzas = millis();
    if (aktualnyCzas - czasMenu >= 10000) {
      resetFunc();
    }
    klawisz = klawiatura.getKey();
    if (klawisz) {  //kiedy naciśnięto przycisk
      czasMenu = aktualnyCzas;
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
          return;
          break;

          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("Numery Alarmowe:");
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
          lcd.setCursor(2, 0);
          lcd.print("Numery Alarmowe:");
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
          break;
      }
      lcd.setCursor(4, NrTel);
    }
  }
}

void Pikanie() {
  aktualnyCzas = millis();
  if (aktualnyCzas - czasPika >= 750) {
    tone(buzzer, 2000);
    delay(150);
    noTone(buzzer);
    czasPika = aktualnyCzas;
  }
}



void setup() {  //TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP TU JEST SETUP
  for (int i = 1; i < 4; i = i + 1) {
    nrTel[i - 1] = load(1 + (i * 4));
  }
  maxTemperatura = load(1);
  lcd.begin(20, 4);

  pinMode(lcdPinPodswietlenia, OUTPUT);  //podświetlenieLCD
  digitalWrite(lcdPinPodswietlenia, 1);

  pinMode(pompa1, OUTPUT);  //pompy
  pinMode(pompa2, OUTPUT);  // 0 - działa
  digitalWrite(pompa1, 1);  //1 - nie działa
  digitalWrite(pompa2, 1);

  pinMode(zbiornikPusty, OUTPUT);  //poziom wody
  pinMode(zbiornikPelny, OUTPUT);

  lcd.createChar(0, literaL);
  lcd.createChar(1, literaZ);
  lcd.createChar(2, uZamkniete);
  lcd.createChar(3, st);
  lcd.createChar(4, literaZZ);

  stanSMS = 1;

  Serial.begin(9600);

  Serial3.begin(9600);
  Serial3.println("AT");  //Once the handshake test is successful, it will back to OK
  updateSerial();
  Serial3.println("AT+CSQ");  //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  Serial3.println("AT+CCID");  //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  Serial3.println("AT+CREG?");  //Check whether it has registered in the network
  updateSerial();

  lcd.setCursor(0, menuPionowe);
  lcd.blink();

  sensors.begin();
}

void loop() {  //TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP TU JEST LOOP
  OdczytZCzujnikow();
}