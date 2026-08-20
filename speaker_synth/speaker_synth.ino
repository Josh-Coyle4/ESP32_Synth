#include <Arduino.h>
#include <ESP_I2S.h>

// The GPIO pins are not fixed, most other pins could be used for the I2S function.
#define I2S_LRC  25
#define I2S_BCLK 26
#define I2S_DIN  22
#define BUTTON_1 0
#define BUTTON_2 5
#define BUTTON_3 12
#define BUTTON_4 13
#define POTENT_VOL 14
#define POTENT_TYPE 35

float frequency = 0.0;    // frequency of square wave in Hz
int amplitude = 3000;    // amplitude of square wave
const int sampleRate = 44100;  // sample rate in Hz

i2s_data_bit_width_t bps = I2S_DATA_BIT_WIDTH_16BIT;
i2s_mode_t mode = I2S_MODE_STD;
i2s_slot_mode_t slot = I2S_SLOT_MODE_STEREO;

unsigned int halfWavelength = 0;  // half wavelength of square wave
unsigned int count = 0;

float phase = 0.0;
float phaseIncrement = 0.0;
uint16_t potentVol = 0;
unsigned long lastPotRead = 0;
uint8_t type = 0;
uint16_t potentType = 0;

int rangesort(uint32_t value, uint32_t maxVal){
  int newVal = (value * maxVal) / 4095;

  return newVal;
}

I2SClass i2s;

void setup() {
  Serial.begin(115200);
  Serial.println("I2S simple tone");

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
  pinMode(POTENT_VOL, INPUT);

  i2s.setPins(I2S_BCLK, I2S_LRC, I2S_DIN);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(mode, sampleRate, bps, slot)) {
    Serial.println("Failed to initialize I2S!");
    while (1);  // do nothing
  }
}

void loop() {


  if (digitalRead(BUTTON_1) == LOW) {
    frequency = 261.63;
  }
  else if (digitalRead(BUTTON_2) == LOW) {
    frequency = 293.66;
  }
  else if (digitalRead(BUTTON_3) == LOW) {
    frequency = 329.63;
  }
  else if (digitalRead(BUTTON_4) == LOW) {
    frequency = 349.23;
  }
  else {
    frequency = 0.0;
  }

  int16_t sample = 0;

  if (millis() - lastPotRead >= 10) {
    potentVol = analogRead(POTENT_VOL);
    amplitude = rangesort(potentVol, 5000);

    potentType = analogRead(POTENT_TYPE);

    if(potentType < 1024){
      type = 0;
    }
    else if(1024 <= potentType && potentType < 2048){
      type = 1;
    }
    else if(2048 <= potentType && potentType < 3072){
      type = 2;
    }
    else{
      type = 3;
    }

    Serial.println(type);

    lastPotRead = millis();
  }


  if(frequency == 0.0){
    sample = 0;
  }
  else{
      phaseIncrement = 2.0 * PI * frequency / sampleRate;

    if(type == 0){

      sample = amplitude * sin(phase);

    }
    else if(type == 1){

      sample = amplitude * ((phase / PI) - 1.0);

    }
    else if(type == 2){
      if(phase < PI){
        sample = -amplitude + (2.0 * amplitude * phase / PI);
      }
      else{
        sample = amplitude - (2.0 * amplitude * (phase - PI) / PI);
      }
    }
    else{
        if(phase < PI){
          sample = amplitude;
        }
        else{
          sample = -amplitude;
        }
    }

    phase += phaseIncrement;

    if (phase >= 2.0 * PI){
      phase -= 2.0 * PI;
    }
  }

  int16_t left = sample;
  int16_t right = sample;
  i2s.write(&left, sizeof(left));
  i2s.write(&right, sizeof(right));


}
