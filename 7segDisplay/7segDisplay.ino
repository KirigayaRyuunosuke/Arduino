//wyswietlacz 7segmentowy i rejestr przesuwny 74hc595

#define SRClock 32
#define SRData 34
#define SRMove 23

int SevenSegNumbers[20]={11001110, // 0
                             11001111, // 0.
                             01010000, // 1
                             01010001, // 1.
                             11101100, // 2
                             11101101, // 2.
                             11111000, // 3
                             11111001, // 3.
                             01110010, // 4
                             01110011, // 4.
                             10111010, // 5
                             10111011, // 5.
                             10111110, // 6 
                             10111111, // 6.
                             11011000, // 7 
                             11011001, // 7.
                             11111110, // 8
                             11111111, // 8.
                             11111010, // 9
                             11111011}; // 9.


void setup() {

  pinMode(SRClock,        OUTPUT);
  pinMode(SRData,         OUTPUT);
  pinMode(SRMove,         OUTPUT);

}

void loop() {
    for(int b = 0; b < 8; b++){
      digitalWrite(SRMove, 1);
      shiftOut(SRData, SRClock, LSBFIRST,SevenSegNumbers[b]);
      digitalWrite(SRMove, 0);
      };
      
      
    }
