#define pump_enable 40
#define valve_in 29
#define valve_out 31

void ballast_setup(){
  pinMode (pump_enable, OUTPUT);
  pinMode (valve_in, OUTPUT);
  pinMode (valve_out, OUTPUT);
}

void pump_on(){
  digitalWrite (pump_enable, HIGH);
}

void pump_off(){
  digitalWrite (pump_enable, LOW);
}

void valve_mode_in(){
  digitalWrite (valve_in, HIGH);
  digitalWrite (valve_out, LOW);
}

void valve_mode_out(){
  digitalWrite (valve_in, LOW);
  digitalWrite (valve_out, HIGH);
}

void valve_mode_off(){
  digitalWrite (valve_in, LOW);
  digitalWrite (valve_out, LOW);
}

void ballast_in(){
  valve_mode_in();
  pump_on();
  //pump_off();
  //valve_mode_off();
}

void ballast_out(){
  valve_mode_out();
  pump_on();
  delay (1000);
  pump_off();
  valve_mode_off();
}

void ballast_off(){
  valve_mode_off();
  pump_off();
}

void ballast_surface(){
  
}

void ballast_sink(){

}

void ballast_neutral(){

}
