#include <Arduino.h>

#define LED_PIN 2
#define POTENT 34

int brightness = 0;
int ledVal = 0;

int rangeSort(uint32_t value){
  int newval = (value * 255) / 4095;

  return newval;
} 


void ledcAnalogWrite(uint8_t pin, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 4095 from 2 ^ 12 - 1
  uint32_t duty = (4095 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(pin, duty);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(POTENT, INPUT);

  ledcAttach(LED_PIN, 5000, 12);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  brightness = analogRead(POTENT);
  ledVal = rangeSort(brightness);

  ledcAnalogWrite(LED_PIN, ledVal);

}
