#include <Keyboard.h>
#include <Mouse.h>
void setup() {
  Keyboard.begin();
  Mouse.begin();
  }

void loop() {
  //Keyboard.print(' ');
  Mouse.click();                               
  delay(50);   
}
