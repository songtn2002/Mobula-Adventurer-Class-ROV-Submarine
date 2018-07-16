void exit(){
  while(true);
}

boolean first_loop = true;
byte command = 0;
boolean executed = false;
void __main__(){
  if (first_loop==true){
    Serial.println("first_cap");
    cam_capture();
    first_loop = false;
  }
  if (get_buffer_pointer()==0){
    record_cam_data();
    Serial.println("Previous shot recorded");
    cam_capture();
    Serial.println("shoot!");
  }else if (Serial2.available()>0){
    byte cmd = Serial2.read ();
    Serial.println("Cmd is "+ String(cmd));
    if (cmd == 127){
      Serial.println("Requested");
      unsigned long trans_init = millis();
      //cam_capture();
      //delayMicroseconds(1500);
      send_img_data();
      //Serial.println("CAM Time: "+String(millis()-trans_init));
      while (!Serial2.available()>0);
      command = Serial2.read();
      executed = false;
      if (executed== false) execute();
      Serial.println("Recieved: "+String(command));
      Serial.println("Time Cost:" +String (millis()-trans_init));
      //Serial.print("Debug: ");
      while (Serial2.available()>0){
        //Serial.println("$");
        Serial.write (Serial2.read());
      }
      Serial.println();
    }
  }
}

void execute(){
  if (command==1){
    motor_acc();
    Serial.println( "motor_pow"+String(get_motor_pow()) );
    executed = true;
  }else if (command==2) {
    fin_turn_right();
    executed = true;
  }else if (command==3) {
    motor_dAcc();
    Serial.println( "motor_pow"+String(get_motor_pow()) );
    executed = true;
  }else if (command==4) {
    fin_turn_left();
    executed = true;
  }else if (command==5) {
    ballast_sink();//
  }else if (command==6) {
    ballast_surface(); // 
  }else if (command==7) {
    lights_on();
    executed = true;
  }else if (command==8) {
    lights_off();
    executed = true;
  }else if (command==9) {
    ballast_neutral();
  }else if (command==10) {
    motor_on();
    executed = true;
  }else if (command==11){
    motor_off();
    executed = true;
  }
}

void setCommand(byte b){
  command = b;  
}

boolean getExecuted(){
  return executed;
}

void setExecuted(boolean b){
  executed = b;
}

void sensor_val_update(){
  gyro_update();
}

void sensor_setup(){  
    camera_setup();
    gyro_setup();
}
