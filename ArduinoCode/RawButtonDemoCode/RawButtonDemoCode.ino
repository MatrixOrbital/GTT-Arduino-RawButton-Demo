//Arduino Uno with Matrix Orbital GTT50A-TPR-BLM-B0-H1-CS-V5 and BBC
//Created by Clark, 09/05/2018
//support@matrixorbital.ca 
//www.matrixorbital.ca/appnotes

#include <Wire.h>
#include <gtt.h>
#include <gtt_device.h>
#include <gtt_enum.h>
#include <gtt_events.h>
#include <gtt_ext_types.h>
#include <gtt_packet_builder.h>
#include <gtt_parser.h>
#include <gtt_protocol.h>
#include <gtt_text.h>
#include "RawButtonDemo.c"
#include "RawButtonDemo.h"

// Define default 8bit I2C address of 0x50 >> 1 for 7bit Arduino
int I2C_Address = 0x28;

// Define gtt device for communication and state for toggle buttons
gtt_device gtt;
eButtonState toggle_state;

// Buffer for incoming data
uint8_t rx_buffer[32];

// Buffer for outgoing data
uint8_t tx_buffer[32];

int Arduino_i2c_write(gtt_device *device, uint8_t *data, size_t data_length)
{
  //TODO: This function is a platform dependend write function that
  //      transfer 'length' bytes of 'data' to the unit. 
  //      this function returns the number of bytes written 
  size_t buffer_length = (data_length & 0x1F);
  byte offset = 0;
  while (data_length)
  {
    Wire.beginTransmission(I2C_Address);
    for (int i = 0; i < buffer_length; i++) 
    {
      Wire.write(data[i+offset]);
    }
    Wire.endTransmission();
    data_length -= buffer_length;
    buffer_length = (data_length & 0x1F); 
    offset += 0x1F;
  };  
  return 0;
}

int Arduino_i2c_read(gtt_device *device)
{
  //TODO: This function is a platform dependend read function that 
  //      reads a single byte from the unit. 
  //      this function returns the byte read when data is available
  //      and returns -1 if no data is currently available. 
  byte data = 0;
  Wire.beginTransmission(I2C_Address);
  Wire.requestFrom(I2C_Address, 1);
  if(Wire.available()<1)
  {
    Wire.endTransmission();
    return -1;
  }
  else
  {
    data = Wire.read();
    Wire.endTransmission();
    return(data); 
  }
}

void GTT25ButtonHandler(gtt_device* device, uint16_t ObjectID, uint8_t State)
{
  switch(ObjectID)
  {
    case id_screen1_raw_button_blue:
    switch(State)
    {
      case 0:
      Serial.println("Blue Button Released!\n");
      gtt_set_screen1_raw_button_test_background_b(&gtt, 128);
      break;
      case 1:
      Serial.println("Blue Button Pressed!\n");
      gtt_set_screen1_raw_button_test_background_b(&gtt, 255);
      break;
    }
    break;
    case id_screen1_raw_button_red:
    switch(State)
    {
      case 0:
      Serial.println("Red Button Released!\n");
      gtt_set_screen1_raw_button_test_background_r(&gtt, 128);
      break;
      case 1:
      Serial.println("Red Button Pressed!\n");
      gtt_set_screen1_raw_button_test_background_r(&gtt, 255);
      break;
    }
    break;
    case id_screen1_raw_button_green:
    switch(State)
    {
      case 0:
      Serial.println("Green Button Released!\n");
      gtt_set_screen1_raw_button_test_background_g(&gtt, 128);
      break;
      case 1:
      Serial.println("Green Button Pressed!\n");
      gtt_set_screen1_raw_button_test_background_g(&gtt, 255);
      break;
    }
    break;
    case id_screen1_raw_toggle_blue:
    gtt_get_screen1_raw_toggle_blue_state(&gtt, &toggle_state);
    switch(toggle_state)
    {
      case eButtonState_Up:
      Serial.println("Blue Toggle Released!\n");
      gtt_set_screen1_raw_button_test_foreground_b(&gtt, 0);
      break;
      case eButtonState_Down:
      Serial.println("Blue Toggle Pressed!\n");
      gtt_set_screen1_raw_button_test_foreground_b(&gtt, 255);
      break;
    }
    break;
    case id_screen1_raw_toggle_red:
    gtt_get_screen1_raw_toggle_red_state(&gtt, &toggle_state);
    switch(toggle_state)
    {
      case eButtonState_Up:
      Serial.println("Red Toggle Released!\n");
      gtt_set_screen1_raw_button_test_foreground_r(&gtt, 0);
      break;
      case eButtonState_Down:
      Serial.println("Red Toggle Pressed!\n");
      gtt_set_screen1_raw_button_test_foreground_r(&gtt, 255);
      break;
    }
    break;
    case id_screen1_raw_toggle_green:
    gtt_get_screen1_raw_toggle_green_state(&gtt, &toggle_state);
    switch(toggle_state)
    {
      case eButtonState_Up:
      Serial.println("Green Toggle Released!\n");
      gtt_set_screen1_raw_button_test_foreground_g(&gtt, 0);
      break;
      case eButtonState_Down:
      Serial.println("Green Toggle Pressed!\n");
      gtt_set_screen1_raw_button_test_foreground_g(&gtt, 255);
      break;
    }
    break;
  }
}

void setup() {
  // put your setup code here, to run once:

  // The gtt_device structre keeps the state of the gtt protocol and allows
  // the library to talk to several devices connected to the same system
  // it needs a platform depenend read/write function and some basic 
  // information about the size and location of the instance specific rx/tx buffers. 
  gtt.Write = Arduino_i2c_write;
  gtt.Read = Arduino_i2c_read;
  gtt.rx_buffer = rx_buffer;
  gtt.rx_buffer_size = sizeof(rx_buffer);
  gtt.tx_buffer = tx_buffer;
  gtt.tx_buffer_size = sizeof(tx_buffer);
  
  gtt25_set_button_clickhandler(&gtt, GTT25ButtonHandler);
  
  //Setup I2C bus and serial reporting
  Wire.begin();
  Serial.begin(9600);
  
  Serial.println("Resetting GTT...\n");
  gtt_reset(&gtt);
  delay(3500);
  Serial.println("Starting.\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  gtt_parser_process(&gtt);
}
