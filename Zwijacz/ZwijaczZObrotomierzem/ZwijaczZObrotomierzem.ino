#define PwmA 3  //piny odpowiedzialne za steroawnie silnikiem
#define in1 4
#define in2 5

#define obrotomierz 12  //odczyt sygnałow z transoptora

#define sirenPin A5

int obrotyZwijacza = 75;
long obroty = 0;
long poprzednieObroty = 0;
bool stan;
bool ostatniStan;
unsigned long aktualnyCzas = 0;
unsigned long czasPomiaru = 0;

void alarm() {
  while (1) {
    digitalWrite(PwmA, 0);
  for (int i = 0; i < 5; i++) {
    tone(sirenPin, 1500);
    delay(100);
    tone(sirenPin, 1750);
    delay(100);
    tone(sirenPin, 2000);
    delay(100);
  }
    noTone(sirenPin);
    delay(300000);
  }
}

void setup() {
  pinMode(PwmA, OUTPUT);  //sygnałPwm
  pinMode(in1, OUTPUT);   // kierunek obrotów
  pinMode(in2, OUTPUT);
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);

  pinMode(obrotomierz, INPUT);

  pinMode(sirenPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  analogWrite(PwmA, obrotyZwijacza);
  aktualnyCzas = millis();
  stan = digitalRead(obrotomierz);
  if (stan != ostatniStan) {
    obroty++;
    ostatniStan = stan;
    //Serial.println(obroty);
  }
  if (aktualnyCzas - czasPomiaru == 5000) {
    czasPomiaru = aktualnyCzas;
    obroty = obroty / 18;
    Serial.println(obroty);
    if (obroty == 0) {
      alarm();
    } else {
      obrotyZwijacza = 100;
    }
    obroty = 0;
  }
  /*
  odczyt z czujnika, potem przilczanie na obroty
  */
}
