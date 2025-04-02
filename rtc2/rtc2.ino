#include <DS3231.h>
#include <Wire.h>
DS3231 rtc(SDA, SCL);
void setup() {
  Serial.begin(57600);
  rtc.begin();

  //////////pamiętaj usuń poniższe 3 linijki jesli ustawiles juz poprawny czas
  //rtc.setDOW(1);             //dzien tygodnia
  //rtc.setTime(21, 50, 0);     //czas
  //rtc.setDate(13, 3, 2023);  //data
}

void loop() {
  Serial.print(rtc.getTimeStr(2));
  Serial.print(" - ");
  Serial.println(rtc.getDateStr());
  Serial.println(rtc.getDOWStr(1));
  delay(1000);
}