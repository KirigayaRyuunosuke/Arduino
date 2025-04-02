// A6

void setup() {
  pinMode(A6, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int czestotliwosc = 0;
  for (czestotliwosc = 2000; czestotliwosc < 2400; czestotliwosc++) {
    tone(A6, czestotliwosc);
    Serial.println(czestotliwosc);
  }
  for (int i = 0; i < 10; i++) {
    tone(A6, 2000);
    delay(100);
    tone(A6, 2400);
    delay(100);
  }
}
