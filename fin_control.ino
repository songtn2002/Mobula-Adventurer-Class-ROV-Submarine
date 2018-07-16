#include <Servo.h>

Servo left_fin;
Servo right_fin;

unsigned long last_op = 0;

void fin_setup(){
  if (!operable()) return;
  left_fin.attach(9);
  right_fin.attach(10);
}

void fin_turn_left(){
  left_fin.write(90);
  right_fin.write(0);
}

void fin_turn_right(){
  if (!operable()) return;
  left_fin.write(0);
  right_fin.write(90);
}

void fin_normal(){
  if (!operable()) return;
  left_fin.write (0);
  right_fin.write (0);  
}

boolean operable(){
  if (millis()-last_op>400||last_op==0){
    last_op = millis();
    return true;
  }else{
    return false;  
  }
}


