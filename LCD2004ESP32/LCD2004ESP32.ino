#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 17, 4, 0, 2, 15);

void setup() {
  lcd.begin(20, 4);
}
void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Good Morning");
  delay(1000);
  lcd.setCursor(0, 2);
  lcd.print("Enjoy the Day");
  delay(1000);
  lcd.setCursor(3, 3);
  lcd.print("Thank you");
  delay(1000);
}