#include <LiquidCrystal.h>
#include "DHT.h"

#define DHT11Pin 16

DHT dht(DHT11Pin, DHT11);
LiquidCrystal lcd(5, 17, 4, 0, 2, 15);

TaskHandle_t Task1;
TaskHandle_t Task2;

float t = 0;
float h = 0;

void setup() {
  Serial.begin(115200);

  lcd.begin(20, 4);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello");

  dht.begin();


  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    Task1code, /* Task function. */
    "Task1",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task1,    /* Task handle to keep track of created task */
    0);        /* pin task to core 0 */
  delay(500);

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    Task2code, /* Task function. */
    "Task2",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task2,    /* Task handle to keep track of created task */
    1);        /* pin task to core 1 */
  delay(500);
}


void Task1code(void* pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  while(1) {
    lcd.setCursor(0, 1);
    lcd.print(t);
    lcd.print("'C ");

    lcd.setCursor(0, 0);
    lcd.print(h);
    lcd.print("%");
    delay(10);
  }
}


void Task2code(void* pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  while(1) {
    delay(2000);
    t = dht.readTemperature();
    h = dht.readHumidity();
  }
}

void loop() {
}
