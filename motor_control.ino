#define motor_A 5 //grey wire
#define motor_B 6 //purple wire

int power = 255;

void motor_setup(){
  pinMode (motor_A, OUTPUT);
  pinMode (motor_B, OUTPUT);
}

void motor_on(){
  power = 255;
  motor_pow(255);
}

void motor_pow(int v){
  power = v;
  analogWrite (motor_A, 0);
  analogWrite (motor_B, v);
}

void motor_acc(){
  power+= 50;
  if (motor_pow>255){
    power = 255;
  }
  motor_pow(power);
}

void motor_off (){
  digitalWrite(motor_A, LOW);
  digitalWrite(motor_B, LOW);  
}

void motor_dAcc(){
  power-= 50;
  if (power<100){
    power = 100;
  }
  motor_pow(power);
}

byte get_motor_pow(){
  return power;
}
