#define LED_GPIO   18
#define PWM1_Ch    0
#define PWM1_Res   16
#define PWM1_Freq  1000

void setup()
{
  Serial.begin(115200);
  ledcAttachPin(LED_GPIO, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
}
 

void loop() {
for(int i = 0; i < 4095; i++){
    ledcWrite(PWM1_Ch, i);
    Serial.println(i);
}
  
}
