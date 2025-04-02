/*bool stan = 0;
#define TMP36 A0

void setup()
{
  pinMode(2, OUTPUT); //pin 2 - wyjcie
  Serial.begin(9600);
}

void loop()
{
  int tmp = analogRead(TMP36); //odczyt 10 bitowej wartoci z czujnika
  float voltage = (tmp * 5.0)/1024;//konwersja odczytanej wartoci do wartoci napiêcia
  float milliVolt = voltage * 1000;//Konwersja napiêcia do mV
  float temperatura =  (milliVolt-500)/10 ;//uwzglêdnienie wartoci ujemnej temperatury - przesuniêcie
  Serial.print("Aktualna temperatura: ");
  Serial.print(temperatura);
  Serial.println("*C");
  
  /*if (temperatura > 50)
  {  
    digitalWrite(2, HIGH); //ustawienie poziomu H na wyjciu 2
  }
  else
  {
    digitalWrite(2, LOW); //ustawienie poziomu L niskiego na wyjciu 2
  }
    delay(200);
    
    if(temperatura>30 && stan  == 0){
    digitalWrite(2,1);
    stan == 1;
    }
    else if(temperatura<25 && stan ==1){
    digitalWrite(2,0);
    stan == 0;
    }
}
*/

float temperatura = 0;
bool stan = 0;
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
OneWire oneWire(A5); //Podłączenie do A5
DallasTemperature sensors(&oneWire); //Przekazania informacji do biblioteki
 
void setup(void) {
  Serial.begin(9600);
  sensors.begin(); //Inicjalizacja czujnikow
  lcd.begin(16, 2);
  pinMode(7,OUTPUT);
}
 
void loop(void) { 
  sensors.requestTemperatures(); //Pobranie temperatury czujnika
  //Serial.print(stan);
  Serial.print("Aktualna temperatura: ");
  Serial.println(sensors.getTempCByIndex(0));  //Wyswietlenie informacji
  temperatura = sensors.getTempCByIndex(0);
  lcd.setCursor(0, 0);
  lcd.print("T=");
   if(temperatura>30 && stan  == 0){
    digitalWrite(7,1);
    stan = 1;
    }
   if(temperatura<25 && stan == 1){
    digitalWrite(7,0);
    stan = 0;}
  lcd.print(temperatura);
  lcd.print("*C");
  delay(500);
}
