int touchpin = 2;
int val=0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(touchpin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(touchpin);

  if (val == 1)
  {
    Serial.println("touched");
  }

  delay(100);
  Serial.println();
}
