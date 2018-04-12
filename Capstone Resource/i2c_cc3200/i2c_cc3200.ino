// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define D6T_addr 0x0A // Address of OMRON D6T is 0x0A in hex
#define D6T_cmd 0x4C // Standard command is 4C in hex

char readbuff[35];
int tPTAT;
int tP[16];
int tPEC;
#define SW 19

void setup()
{
  Serial.begin(9600);  // start serial for output 9600 bytes per s
  Serial.println("start");
  Wire.begin();        // join i2c bus (address optional for master)
  pinMode(SW, OUTPUT);   
  
}

unsigned char calc_crc( unsigned char data ) {
   int  index;
   unsigned char  temp;
for(index=0;index<8;index++){ temp = data;
data <<= 1;
if(temp & 0x80) data ^= 0x07;
}
   return data;
}

int D6T_checkPEC( char* buf , int pPEC ){
   unsigned char  crc;
   int  i;
crc = calc_crc( 0x15 );
for(i=0;i<pPEC;i++){
      crc = calc_crc( buf[i] ^ crc );
   }
   return (crc == buf[pPEC]);
}

void loop()
{
  digitalWrite(SW,LOW); 
  delay(500);

  Wire.beginTransmission(D6T_addr); // transmit to device 0001010b
  Wire.write(D6T_cmd);  //14h = { 0Ah(Addr7) : Write(0b) }
  Wire.endTransmission(false); //repeated start
  
  
  // 0001010b is the slave address (10 in decimal)
  // 35 bytes output from sensor (17*2 + 1 = 35)
  Wire.requestFrom(D6T_addr, 35);    // request 2 bytes from slave device

  while(Wire.available() < 35);    // wait for availability
  for(int x = 0; x < 35; x++){
    readbuff[x] = Wire.read();
  }
  
  //if(D6T_checkPEC(readbuff,34)){
    
  tPTAT = 256*readbuff[1] + readbuff[0]; 
  tP[0] = 256*readbuff[3] + readbuff[2];
  tP[1] = 256*readbuff[5] + readbuff[4]; 
  tP[2] = 256*readbuff[7] + readbuff[6];
  tP[3] = 256*readbuff[9] + readbuff[8];
  tP[4] = 256*readbuff[11] + readbuff[10];
  tP[5] = 256*readbuff[13] + readbuff[12]; 
  tP[6] = 256*readbuff[15] + readbuff[14];
  tP[7] = 256*readbuff[17] + readbuff[16]; 
  tP[8] = 256*readbuff[19] + readbuff[18]; 
  tP[9] = 256*readbuff[21] + readbuff[20];
  tP[10] = 256*readbuff[23] + readbuff[22];
  tP[11] = 256*readbuff[25] + readbuff[24];
  tP[12] = 256*readbuff[27] + readbuff[26];
  tP[13] = 256*readbuff[29] + readbuff[28];
  tP[14] = 256*readbuff[31] + readbuff[30];
  tP[15] = 256*readbuff[33] + readbuff[32];
  tPEC = readbuff[34];
  

  for (int i = 0; i < 16; i++){
    Serial.print(tP[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
  //}
  Wire.endTransmission();
  digitalWrite(SW,HIGH); 
  delay(1000);
}
