
#include <Wire.h>
#define GATE_PIN                        6

int state = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(GATE_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (state == 0)
  {
    digitalWrite(GATE_PIN, HIGH);
    state = 1;
    Serial.println("IT'S OFF!!!");
  }  
  else if (state == 1)
  {
    digitalWrite(GATE_PIN, LOW);
    state = 0;
    Serial.println("IT'S ON!!!");
  }
    
  delay(5000);
}
