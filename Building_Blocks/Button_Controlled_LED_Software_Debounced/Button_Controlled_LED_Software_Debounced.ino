#include <Arduino.h>

#define BTN_PRESS 0
#define ledPin 2

int ledState = 0;
int prevBTN = 0;
int currBTN = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(BTN_PRESS, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  currBTN = digitalRead(BTN_PRESS);

  if(prevBTN == HIGH && currBTN == LOW){
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }

  prevBTN = currBTN;
}
