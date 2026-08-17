#include <Arduino.h>

#define BUZZ_PIN 2
#define BUTTON_1 0
#define BUTTON_2 5
#define BUTTON_3 12
#define BUTTON_4 13
#define POTENT 14

// int buzz_state = 0;
// int prevBTN = 0;
// int currBTN = 0;

int baseNotes[4] = {262, 294, 330, 392};
int octaveMultiplier = 0;

int potentVal = 0;
int offset = 0;
int direction = 1;


int rangesort(uint32_t value){
  int newVal = (value * 3) / 4095;

  return newVal;
}

int sine_wave(uint32_t value){
  int direction = 1;

  if(direction >= )
}

void setup() {
  // put your setup code here, to run once:

  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
  pinMode(POTENT, INPUT);

  ledcAttach(BUZZ_PIN, 5000, 12);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(BUTTON_1) == LOW){
    ledcWriteTone(BUZZ_PIN, (baseNotes[0] + offset) * (octaveMultiplier + 1));
  }
  else if(digitalRead(BUTTON_2) == LOW){
    ledcWriteTone(BUZZ_PIN, (baseNotes[1] + offset) * (octaveMultiplier + 1));
  }
  else if(digitalRead(BUTTON_3) == LOW){
    ledcWriteTone(BUZZ_PIN, (baseNotes[2] + offset) * (octaveMultiplier + 1));
  }
  else if(digitalRead(BUTTON_4) == LOW){
    ledcWriteTone(BUZZ_PIN, (baseNotes[3] + offset) * (octaveMultiplier + 1));
  }
  else{
    ledcWriteTone(BUZZ_PIN, 0);
  }
  
  offset += direction;

  if(offset >= 5){
    direction = -1;
  }

  if(offset <= -5){
    direction = 1;
  }

  potentVal = analogRead(POTENT);
  octaveMultiplier = rangesort(potentVal);

  delay(20);
}
