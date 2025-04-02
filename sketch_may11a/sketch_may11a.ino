/* pot1 - 4
   pot2 - 5
   pot3 - 6
   keypad 7, 15, 16 x 17, 18, 8, 3
   led - 48
*/
#include <Keypad.h>


#define pot1 4
#define pot2 5
#define pot3 6

#define ledPin 48

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  3 // three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pin_rows[ROW_NUM] = {17, 18, 8, 3}; // GPIO18, GPIO5, GPIO17, GPIO16 connect to the row pins
byte pin_column[COLUMN_NUM] = {7, 15, 16};  // GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

void setup() {
  Serial.begin(115200);
}


void loop() {
  char key = keypad.getKey();
  Serial.print(key);
  Serial.print(" ");
  Serial.print("Test ");
  Serial.print(analogRead(pot1));
  Serial.print(" ");
  Serial.print(analogRead(pot2));
  Serial.print(" ");
  Serial.println(analogRead(pot3));
  delay(1000);
}
