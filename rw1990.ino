#include <OneWire.h>
#define pin 10

OneWire ibutton (pin); // I button connected on PIN 2.
byte addr[8]; //array to store the Ibutton ID.

void setup(){
 Serial.begin(115200); 
}

void loop(){

readKey();
 
 if (Serial.read() == 'w' ){
    writeKey();  
  }
} 

void readKey()
{
  byte i;
  ibutton.reset();
  delay(60);
  ibutton.write(0x33); // "READ" command
  
  ibutton.read_bytes(addr, 8);

  Serial.print("KEY ");
  for( i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    if (i != 7) Serial.print(":");
  }

  // Check if FF:FF:FF:FF:FF:FF:FF:FF
  // If your button is really programmed with FF:FF:FF:FF:FF:FF:FF:FF, then remove this check
  if (addr[0] & addr[1] & addr[2] & addr[3] & addr[4] & addr[5] & addr[6] & addr[7] == 0xFF)
  {
    Serial.println("...nothing found!"); 
    return;
  }
  
  //compute crc//
  byte crc;
  crc = ibutton.crc8(addr, 7);
  Serial.print(" CRC: ");
  Serial.println(crc,HEX);
 }

 void writeKey()
 {
//    ibutton.skip();
//  ibutton.reset();
//  ibutton.write(0x33);
//    Serial.print("  ID before write:");
//    for (byte x=0; x<8; x++){
//      Serial.print(' ');
//      Serial.print(ibutton.read(), HEX);
//    }


    ibutton.skip();
    ibutton.reset();
    ibutton.write(0xD1);//unlock write
    sendZero();
     
    Serial.print('\n');
    Serial.print("  Writing iButton ID:\n    ");
    // Hardcode here your desired ID //
    // 01 D5 9F DC 02 00 00 96
    byte newID[8] = {0x01, 0x88, 0x5B, 0xA3, 0x00, 0x00, 0x00, 0xB2};
                     
    ibutton.skip();
    ibutton.reset();
    ibutton.write(0xD5);
    for (byte x = 0; x<8; x++){
      writeByte(newID[x]);
      Serial.print('*');
    }
    Serial.print('\n');
    ibutton.reset();
    ibutton.write(0xD1);   //lock wrire
    sendOne();
  }

void sendZero()
{
      digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
    delayMicroseconds(60);
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
    delay(10);
  }

  void sendOne()
  {
       //send logical 1
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
    delayMicroseconds(15);
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
    delay(10);
  }

int writeByte(byte data){
  int data_bit;
  for(data_bit=0; data_bit<8; data_bit++){
    if (data & 1){
      sendZero();
    } else {
     sendOne();
    }
    data = data >> 1;
  }
  return 0;
}


