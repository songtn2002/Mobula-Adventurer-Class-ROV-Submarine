void test0520(){
  Serial.println("lights on");
  lights_on();
  delay(2000);
  Serial.println("lights off");
  lights_off();
  delay(2000);
  Serial.println("motor_on");
  motor_pow(255);
  delay(2000);
  Serial.println("motor_off");
  motor_off();
  delay(2000);
  sensor_val_update();
  Serial.println("Pitch: "+String(gyro_getPitch()));
  Serial.println("Roll: "+String(gyro_getRoll()));
  Serial.println("fin_left");
  fin_turn_left();
  delay(2000);
  Serial.println("fin_right");
  fin_turn_right();
  delay(2000);
  Serial.println("VALVE_IN");
  valve_mode_in();
  pump_on();
  delay(2000);
  Serial.println("VALVE_OUT");
  valve_mode_out();
  pump_on();
  delay(2000);
  Serial.println("VALVE_OFF");
  valve_mode_off();
  pump_off();
}

void demo_right(){
  Serial.println("turn right");
  motor_on();
  fin_turn_right();
  delay(5000);
  motor_off();
  fin_normal();
}

void demo_left(){
  Serial.println("turn left");
  motor_on();
  fin_turn_left();
  delay(5000);
  motor_off();
  fin_normal();
}

void demo_lights(int x){
   while(x<1000){
      Serial.println("lights on");
      lights_on();
      delay(x);
      lights_off();
      delay(x);
      x=x+10;
   }
}
