#include <Arduino.h>

#define led 2

void setup() {
  // put your setup code here, to run once:

  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < 5; i++)
  {
      digitalWrite(led ,HIGH);

      delay(500);

      digitalWrite(led, LOW);

      delay(500);
  }
  
  for (int i = 0; i < 5; i++)
  {
      digitalWrite(led ,HIGH);

      delay(1000);

      digitalWrite(led, LOW);

      delay(1000);
  }

}

// this code first blinks the LED on esp32 5 times with time interval 500ms and then blinks it 5 times with time interval 1000ms
// then it repeats the pattern 