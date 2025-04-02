/* pot1 - 4
   pot2 - 5
   pot3 - 6
   keypad 7, 15, 16 x 17, 18, 8, 3
   led - 48
*/
#include "Adafruit_MPU6050.h"
#include "Wire.h"
#include "Adafruit_Sensor.h"
#include "Arduino.h"
#include "Wire.h"
#include "Keypad.h"
#include "U8g2lib.h"
#include "Joystick_ESP32S2.h"

Adafruit_MPU6050 mpu;
int oY = 0;
int oZ = 0;

Joystick_ Joystick;

U8G2_ST7567_ENH_DG128064I_F_HW_I2C u8g2(U8G2_R2, SCL, SDA, U8X8_PIN_NONE);

#define pot1 4
#define pot2 5
#define pot3 6
#define potX 9
#define potY 10

#define Button 46

#define ledPin 48

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  3 // three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '5', '0'},
  {'2', '6', 'a'},
  {'3', '7', 'b'},
  {'4', '8', 'c'}
};

byte pin_rows[ROW_NUM] = {3, 8, 18, 17}; // GPIO18, GPIO5, GPIO17, GPIO16 connect to the row pins
byte pin_column[COLUMN_NUM] = {15, 7, 16};  // GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

char key;
bool anyPressed = 1;
bool lastButtonState = 0;

unsigned long previousMillis = 0;
const long interval = 42;

void setup() {
  pinMode(Button, INPUT_PULLUP);

  Wire.begin(12, 11);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  u8g2.setI2CAddress(0x3F * 2);
  u8g2.setBusClock(400000);
  u8g2.begin();

  Serial.begin(115200);

  Joystick.begin();
  Joystick.setXAxisRange(-32767, 32767);
  Joystick.setYAxisRange(-32767, 32767);
  Joystick.setRxAxisRange(-32767, 32767);
  Joystick.setRyAxisRange(-32767, 32767);
  Joystick.setRzAxisRange(-32767, 32767);
  Joystick.setThrottleRange(-32767, 32767);
  Joystick.setAcceleratorRange(-32767, 32767);

}

void loop() {
  unsigned long currentMillis = millis();

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  bool currentButtonState = !digitalRead(Button);
  if (currentButtonState != lastButtonState)
  {
    Joystick.setButton(11, currentButtonState);
    lastButtonState = currentButtonState;
  }
  //if (!anyPressed)
  key = keypad.getKey();
  switch (key) {
    case '1':
      Joystick.setButton(0, 1);
      anyPressed = 1;
      break;
    case '2':
      Joystick.setButton(1, 1);
      anyPressed = 1;
      break;
    case '3':
      Joystick.setButton(2, 1);
      anyPressed = 1;
      break;
    case '4':
      Joystick.setButton(3, 1);
      anyPressed = 1;
      break;
    case '5':
      Joystick.setButton(4, 1);
      anyPressed = 1;
      break;
    case '6':
      Joystick.setButton(5, 1);
      anyPressed = 1;
      break;
    case '7':
      Joystick.setButton(6, 1);
      anyPressed = 1;
      break;
    case '8':
      Joystick.setButton(7, 1);
      anyPressed = 1;
      break;
    case 'a':
      Joystick.setButton(8, 1);
      anyPressed = 1;
      break;
    case 'b':
      Joystick.setButton(9, 1);
      anyPressed = 1;
      break;
    case 'c':
      Joystick.setButton(10, 1);
      anyPressed = 1;
      break;
    default:
      if (anyPressed == 1) {
        for (int i = 0; i < 11; i++) {
          Joystick.setButton(i, 0);
        }
        anyPressed = 0;
      }
      break;
  }


  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
    u8g2.setCursor(0, 11);
    u8g2.print("Pot1 ");
    u8g2.print(map(analogRead(pot1), 0, 4095, 0, 100));
    u8g2.setCursor(0, 24);
    u8g2.print("Pot2 ");
    u8g2.print(map(analogRead(pot2), 0, 4095, 0, 100));
    u8g2.setCursor(0, 37);
    u8g2.print("Pot3 ");
    u8g2.print(map(analogRead(pot3), 0, 4095, 0, 100));
    u8g2.setCursor(0, 50);
    u8g2.print("PotX ");
    u8g2.print(map(analogRead(potX), 0, 4095, 0, 100));
    u8g2.setCursor(0, 63);
    u8g2.print("PotY ");
    u8g2.print(map(analogRead(potY), 0, 4095, 0, 100));
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
  Joystick.setRxAxis(map(analogRead(pot1), 0, 4095, -32767, 32767));
  Joystick.setRyAxis(map(analogRead(pot2), 0, 4095, -32767, 32767));
  Joystick.setRzAxis(map(analogRead(pot3), 0, 4095, -32767, 32767));
  Joystick.setXAxis(map(analogRead(potX), 0, 4095, -32767, 32767));
  Joystick.setYAxis(map(analogRead(potY), 0, 4095, -32767, 32767));

  oY = g.gyro.y * 15;
  oZ = g.gyro.z * 15;
  //Serial.print(oY);
  //Serial.println(oZ);
  Joystick.setThrottle(map(oY, -15, 15, -32767, 32767));
  Joystick.setAccelerator(map(oZ, -20, 20, -32767, 32767));

}
