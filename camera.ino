 // ArduCAM Mini demo (C)2017 Lee
// Web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with ArduCAM Mini camera, and can run on any Arduino platform.
// This demo was made for ArduCAM_Mini_5MP_Plus.
// It needs to be used in combination with PC software.
// It can take photo continuously as video streaming.
//
// The demo sketch will do the following tasks:
// 1. Set the camera to JPEG output mode.
// 2. Read data from Serial port and deal with it
// 3. If receive 0x00-0x08,the resolution will be changed.
// 4. If receive 0x10,camera will capture a JPEG photo and buffer the image to FIFO.Then write datas to Serial port.
// 5. If receive 0x20,camera will capture JPEG photo and write datas continuously.Stop when receive 0x21.
// 6. If receive 0x30,camera will capture a BMP  photo and buffer the image to FIFO.Then write datas to Serial port.
// 7. If receive 0x11 ,set camera to JPEG output mode.
// 8. If receive 0x31 ,set camera to BMP  output mode.
// This program requires the ArduCAM V4.0.0 (or later) library and ArduCAM_Mini_5MP_Plus
// and use Arduino IDE 1.6.8 compiler or above
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"
//This demo can only work on OV2640_MINI_2MP or OV5642_MINI_5MP or OV5642_MINI_5MP_BIT_ROTATION_FIXED platform.
#if !(defined OV5642_MINI_5MP || defined OV5642_MINI_5MP_BIT_ROTATION_FIXED || defined OV2640_MINI_2MP || defined OV3640_MINI_3MP)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif
#define BMPIMAGEOFFSET 66
const char bmp_header[BMPIMAGEOFFSET] PROGMEM =
{
  0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00,
  0x00, 0x00
};
SPISettings cam_spi_settings = SPISettings (8000000, MSBFIRST, SPI_MODE0);
int serialSize = 0;
// set pin 7 as the slave select for the digital pot:
boolean cap = true;
const int CS = 53;
bool is_header = false;
uint8_t start_capture = 0;
#if defined (OV2640_MINI_2MP)
  ArduCAM myCAM( OV2640, CS );
#elif defined (OV3640_MINI_3MP)
ArduCAM myCAM( OV3640, CS );
#else
  ArduCAM myCAM( OV5642, CS );
#endif
uint8_t read_fifo_burst(ArduCAM myCAM);
void camera_setup() {
// put your setup code here, to run once:
Serial2.begin (2000000);
uint8_t vid, pid;
uint8_t temp;
#if defined(__SAM3X8E__)
  Wire1.begin();
  Serial.begin(9600);
#else
  Wire.begin();
  //Serial.begin(9600);
#endif
Serial.println(F("ACK CMD ArduCAM Start!"));
// set the CS as an output:
pinMode(CS, OUTPUT);
//pinMode(53, OUTPUT);
// initialize SPI:
SPI.begin();
//SPI.setClockDivider(SPI_CLOCK_DIV2);
while(1){
  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  //Serial.println(myCAM.read_reg(ARDUCHIP_TEST1));
  if (temp != 0x55){
    Serial.println(F("ACK CMD SPI interface Error!"));
    delay(1000);continue;
  }else{
    Serial.println(F("ACK CMD SPI interface OK."));break;
  }
}

#if defined (OV2640_MINI_2MP)
  while(1){
    //Check if the camera module type is OV2640
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
      Serial.println(F("ACK CMD Can't find OV2640 module!"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("ACK CMD OV2640 detected."));break;
    } 
  }
#elif defined (OV3640_MINI_3MP)
  while(1){
    //Check if the camera module type is OV3640
    myCAM.rdSensorReg16_8(OV3640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV3640_CHIPID_LOW, &pid);
    if ((vid != 0x36) || (pid != 0x4C)){
      Serial.println(F("Can't find OV3640 module!"));
      delay(1000);continue; 
    }else{
      Serial.println(F("OV3640 detected."));break;    
    }
 } 
  
#else
  while(1){
    //Check if the camera module type is OV5642
    myCAM.wrSensorReg16_8(0xff, 0x01);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
    if((vid != 0x56) || (pid != 0x42)){
      Serial.println(F("ACK CMD Can't find OV5642 module!"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("ACK CMD OV5642 detected."));break;
    } 
  }
#endif
//Change to JPEG capture mode and initialize the OV5642 module
myCAM.set_format(JPEG);
myCAM.InitCAM();
#if defined (OV2640_MINI_2MP)
  Serial.println("Size set!");
  myCAM.OV2640_set_JPEG_size(OV2640_176x144);
#elif defined (OV3640_MINI_3MP)
  myCAM.OV3640_set_JPEG_size(OV3640_320x240);
#else
  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_320x240);
#endif
delay(1000);
myCAM.clear_fifo_flag();
#if !(defined (OV2640_MINI_2MP))
myCAM.write_reg(ARDUCHIP_FRAMES,0x00);
#endif
temp = 0xff;
//serial_prompt();
//SPI.end();
}

void cam_capture (){
  //SPI.begin();
  unsigned long cap_start = millis();
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  //Start capture
  myCAM.start_capture();
  //record_cam_data();
  Serial.println("This shot lasts: "+String (millis()-cap_start)+" ms");
  //SPI.end();
}

void record_cam_data(){
  //SPI.begin();
  unsigned long d_time = millis();
   while(!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) {
      if (getExecuted()==false){
        execute();
        setExecuted(true);
      }
   }
   Serial.println ("Wait time: "+String (millis()-d_time));
    Serial.println(F("ACK CMD CAM Capture Done."));
    read_fifo_burst(myCAM);
    //Clear the capture done flag
    myCAM.clear_fifo_flag();
  //SPI.end();
}

uint8_t read_fifo_burst(ArduCAM myCAM)
{   //SPI.setClockDivider(SPI_CLOCK_DIV2);
  //SPI.beginTransaction(cam_spi_settings);
  uint8_t temp = 0, temp_last = 0;
  uint32_t length = 0;
  length = myCAM.read_fifo_length();
  Serial.println(length, DEC);
  if (length >= MAX_FIFO_SIZE) //512 kb
  {
    Serial.println(F("Over size."));
    return 0;
  }
  if (length == 0 ) //0 kb
  {
    Serial.println(F("Size is 0."));
    return 0;
  }
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();//Set fifo burst mode
  temp =  SPI.transfer(0x00);
  length --;
  while ( length-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    if (is_header == true)
    {
      write_buffer(temp);
      serialSize += sizeof(temp);
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      Serial.println(F("ACK IMG"));
      write_buffer(temp_last);
      serialSize += sizeof (temp_last);
      write_buffer(temp);
      serialSize += sizeof (temp);
    }
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    break;
    delayMicroseconds(15);
  }
  myCAM.CS_HIGH();
  is_header = false;
  //Serial.write("\nAll");
  return 1;
  //SPI.endTransaction();
}

uint8_t image_buffer[4000];
int buffer_pointer = 0;

void write_buffer(uint8_t b){
  image_buffer[buffer_pointer] = b;
  buffer_pointer++;
}

void clear_img_buffer(){
  buffer_pointer = 0;
}

int get_buffer_pointer(){
  return buffer_pointer;
}

void send_img_data(){
  Serial2.write (image_buffer, buffer_pointer);
  Serial.println("A total of "+String (buffer_pointer)+" bytes sent");
  clear_img_buffer();
}
