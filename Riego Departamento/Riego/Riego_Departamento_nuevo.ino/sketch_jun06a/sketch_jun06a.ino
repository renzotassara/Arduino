int steps = 5;
int dir = 4;
#define led 13

void setup() {
  pinMode(steps, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(led, OUTPUT);

  
}

void loop() {
  digitalWrite(led, HIGH);
  digitalWrite(dir, HIGH);
  for (int x=0; x<=200; x++)
  {
    digitalWrite(steps,HIGH);
    delayMicroseconds(500);
    digitalWrite(steps,LOW);
    delayMicroseconds(500);
  }
  digitalWrite(led,LOW);
  delay(2000);
  
}
