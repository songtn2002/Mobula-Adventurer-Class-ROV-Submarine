void setup() {
  lights_setup();
  ballast_setup();
  Serial.begin(9600);
  sensor_setup();
  motor_setup();
  fin_setup();
  Serial2.begin (2000000);
  Serial.println("Begun");
}

void loop() {
  __main__();
}

/*
 * log: troubleshoot
 * 1. lights do not work, lights removed
 * 2. fins do not work, fins removed
 * 3. pump does not work, pump removed
 */

