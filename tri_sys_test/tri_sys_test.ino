void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  test_tri_sys();
  exit(0);
}

void test_tri_sys(){
  digitalWrite (8, HIGH);
  digitalWrite (9, HIGH);
  delay (7000);
}
