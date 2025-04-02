
  int czestotliwosc = 0;

void setup() {
  pinMode(A0, OUTPUT); //Konfiguracja A0 jako wyjście
Serial.begin(9600);
}

void loop() {
  for (czestotliwosc = 2000; czestotliwosc < 2400; czestotliwosc++) {
    tone(A0, czestotliwosc);
    Serial.println(czestotliwosc);
  }
  for (int i = 0; i < 10; i++) {
    tone(A0, 2000);
    delay(100); 
    tone(A0, 2400);
    delay(100);
  }
}
