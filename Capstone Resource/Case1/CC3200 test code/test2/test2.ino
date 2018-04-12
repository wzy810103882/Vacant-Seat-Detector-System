#define MQTTCLIENT_QOS2 1

#include <SPI.h>
#include <WiFi.h>
#include <WifiIPStack.h>
#include <Countdown.h>
#include <MQTTClient.h>

#include <Wire.h>
#define D6T_addr 0x0A // Address of OMRON D6T is 0x0A in hex
#define D6T_cmd 0x4C // Standard command is 4C in hex

char readbuff[35];
int tPTAT;
int tP[16];
int tPEC;
/***
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
***/

// your network name also called SSID
char ssid[] = "wahoo";
// your network password
char password[] = "";

char printbuf[100];

int arrivedcount = 0;

void messageArrived(MQTT::MessageData& md)
{
  MQTT::Message &message = md.message;
  
  sprintf(printbuf, "Message %d arrived: qos %d, retained %d, dup %d, packetid %d\n", 
   ++arrivedcount, message.qos, message.retained, message.dup, message.id);
  Serial.print(printbuf);
  sprintf(printbuf, "Payload %s\n", (char*)message.payload);
  Serial.print(printbuf);
}


WifiIPStack ipstack;
MQTT::Client<WifiIPStack, Countdown> client = MQTT::Client<WifiIPStack, Countdown>(ipstack);

const char* topic = "uvafourier";

void connect()
{
  char hostname[] = "iot.eclipse.org";
  int port = 1883;
  sprintf(printbuf, "Connecting to %s:%d\n", hostname, port);
  Serial.print(printbuf);
  int rc = ipstack.connect(hostname, port);
  if (rc != 1)
  {
    sprintf(printbuf, "rc from TCP connect is %d\n", rc);
    Serial.print(printbuf);
  }
 
  Serial.println("MQTT connecting");
  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
  data.MQTTVersion = 3;
  data.clientID.cstring = (char*)"uvafoutier";
  rc = client.connect(data);
  if (rc != 0)
  {
    sprintf(printbuf, "rc from MQTT connect is %d\n", rc);
    Serial.print(printbuf);
  }
  Serial.println("MQTT connected");
  
  rc = client.subscribe(topic, MQTT::QOS2, messageArrived);   
  if (rc != 0)
  {
    sprintf(printbuf, "rc from MQTT subscribe is %d\n", rc);
    Serial.print(printbuf);
  }
  Serial.println("MQTT subscribed");
}

void setup()
{
  Serial.begin(115200);
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);

  //Serial.begin(9600);  // start serial for output 9600 bytes per s
  Serial.println("start");
  Wire.begin();        // join i2c bus (address optional for master)
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  // We are connected and have an IP address.
  Serial.println(WiFi.localIP());
  
  Serial.println("MQTT Hello example");
  connect();
}

void loop()
{
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
  Serial.print(tPTAT);
  Serial.println(" ");

  
  int human = 0;
  for (int i = 0; i < 16; i++) {
    if( 235 < tP[i]) {
      human = 1;
    }
  }

  
  char s[6] = "1,001";
  char d[6] = "0,001";
  //s[0] = human + '0';
  if (!client.isConnected())
    connect();
  
  MQTT::Message message;
  
  arrivedcount = 0;

  // Send and receive QoS 0 message
  char buf[100];
  sprintf(buf, s);
  Serial.println(buf);
  message.qos = MQTT::QOS0;
  message.retained = false;
  message.dup = false;
  message.payload = (void*)buf;
  message.payloadlen = strlen(buf)+1;
  int rc = client.publish(topic, message);
  while (arrivedcount == 0)
    client.yield(1000);
  delay(1000);

   // Send and receive QoS 1 message
  sprintf(buf, d);
  Serial.println(buf);
  message.qos = MQTT::QOS1;
  message.payloadlen = strlen(buf)+1;
  rc = client.publish(topic, message);
  while (arrivedcount == 1)
    client.yield(1000);

  delay(1000);
        
  //delay(2000);
}
