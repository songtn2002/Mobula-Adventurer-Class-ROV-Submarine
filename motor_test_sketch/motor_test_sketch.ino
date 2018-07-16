void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  analogWrite (9, 255);
  analogWrite (10, 0);
  digitalWrite(7, 255);
  digitalWrite(4, HIGH);
  delay(2000);
  analogWrite (10, 255);
  analogWrite (9, 0);
  digitalWrite (7, 0);
  digitalWrite(4, LOW);
  delay(2000);
  analogWrite (9, 0);
  analogWrite (10, 0);
  delay(2000);
}
