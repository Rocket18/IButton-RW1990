# IButton-RW1990
Read/Write key with Arduino

#include <OneWire.h>

#define pin 10

byte key_to_write[] = { 0x01, 0xBE, 0x40, 0x11, 0x5A, 0x36, 0x00, 0xE1 };

OneWire  ds(pin);  // pin 10 is 1-Wire interface pin now

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  byte i;
  byte data[8];

  delay(1000); // 1 sec
  
  ds.reset();
  delay(50);
  ds.write(0x33); // "READ" command
  
  ds.read_bytes(data, 8);

  Serial.print("KEY ");
  for( i = 0; i < 8; i++) {
    Serial.print(data[i], HEX);
    if (i != 7) Serial.print(":");
  }

  // Check if FF:FF:FF:FF:FF:FF:FF:FF
  // If your button is really programmed with FF:FF:FF:FF:FF:FF:FF:FF, then remove this check
  if (data[0] & data[1] & data[2] & data[3] & data[4] & data[5] & data[6] & data[7] == 0xFF)
  {
    Serial.println("...nothing found!"); 
    return;
  }

  return; // remove when ready to programm

  // Check if read key is equal to the one to be programmed
  for (i = 0; i < 8; i++)
    if (data[i] != key_to_write[i])
      break;
    else
      if (i == 7)
      {
        Serial.println("...already programmed!");
        return;
      }

  Serial.println();
  Serial.print("Programming new key...");
  
  for (uint8_t i = 0; i < 8; i++)
  {
    ds.reset();
    data[0] = 0x3C; // "WRITE" command
    data[1] = i; // programming i-th byte
    data[2] = 0;
    data[3] = key_to_write[i];
    ds.write_bytes(data, 4);
    Serial.print(".");
  
    uint8_t crc = ds.read();
    
    if (OneWire::crc8(data, 4) != crc) {
        Serial.print("error!\r\n");
        return;
    }
    else
      Serial.print(".");
    
    send_programming_impulse();
  }
  
  Serial.println("done!");
}

void send_programming_impulse()
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH); 
  delay(60);
  digitalWrite(pin, LOW); 
  delay(5);
  digitalWrite(pin, HIGH); 
  delay(50); 
}

