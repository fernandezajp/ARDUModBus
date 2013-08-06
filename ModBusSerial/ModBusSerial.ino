#include <SoftwareSerial.h>
#include "modbusseriallib.h"

#define RXPIN        2
#define TXPIN        3

SoftwareSerial PLCSerialPort(RXPIN, TXPIN);
ModbusSerial ModbusSerial(&PLCSerialPort);

void setup() {
  Serial.begin(9600);
  PLCSerialPort.begin(9600);
  
  /*
  byte deviceId=17;
  unsigned short start=20;
  int numValues=37;
  unsigned short values[numValues];
  
  ModbusSerial.fc1(deviceId,start,numValues,values);
  
  for(int i=0;i<numValues-1;i++)
      {
      Serial.print(values[i]);
      Serial.print(",");
      }
  Serial.println(values[numValues-1]);
  */
  
  /*
  byte deviceId=17;
  unsigned short start=10197;
  int numValues=22;
  unsigned short values[numValues];
  
  ModbusSerial.fc2(deviceId, start, numValues,values);
  
  for(int i=0;i<numValues-1;i++)
      {
      Serial.print(values[i]);
      Serial.print(",");
      }
  Serial.println(values[numValues-1]);
  */
  
  /*
  byte deviceId=17;
  unsigned short start=40108;
  int numValues=3;
  unsigned short values[numValues];
  
  ModbusSerial.fc3(deviceId, start, numValues, values);

  for(int i=0;i<numValues-1;i++)
      {
      Serial.print(values[i],HEX);
      Serial.print(",");
      }
  Serial.println(values[numValues-1],HEX);
  */
  
  /*
  byte deviceId=17;
  unsigned short start=30009;
  int numValues=1;
  unsigned short values[numValues];
  
  ModbusSerial.fc4(deviceId, start, numValues, values);

  for(int i=0;i<numValues-1;i++)
      {
      Serial.print(values[i],HEX);
      Serial.print(",");
      }
  Serial.println(values[numValues-1],HEX);
  */
  
  /*
  // Trama FC5
  byte deviceId=17;
  unsigned short start=173;
  
  ModbusSerial.fc5(deviceId, start, true);
  */
  
  /*
  // Trama FC6
  byte deviceId=17;
  unsigned short start=40002;
  unsigned short value=3;
  
  ModbusSerial.fc6(deviceId, start, value);
  */
  
  /*
  byte deviceId=17;
  unsigned short start=20;
  int numValues=10;
  boolean values[numValues];
  
  values[0] = 1;
  values[1] = 1;
  values[2] = 0;
  values[3] = 1;
  values[4] = 0;
  values[5] = 0;
  values[6] = 1;
  values[7] = 1;
  values[8] = 1;
  values[9] = 0;
 
  ModbusSerial.fc15(deviceId, start, numValues, values);
  */
  
  /*
  byte deviceId=17;
  unsigned short start=40002;
  int numValues=2;
  unsigned short values[numValues];
  
  values[0] = 0xA;
  values[1] = 0x102;
 
  ModbusSerial.fc16(deviceId, start, numValues, values);
  */
}

void loop() {
}
