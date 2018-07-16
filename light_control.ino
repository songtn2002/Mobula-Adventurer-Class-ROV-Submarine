#define light_pin 22

void lights_setup(){
  pinMode (light_pin, OUTPUT);
}

void lights_on(){
  digitalWrite(light_pin, HIGH);
}

void lights_off(){
  digitalWrite(light_pin, LOW);
}
