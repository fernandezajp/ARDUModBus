#include <SoftwareSerial.h>
#include "modbusseriallib.h"

//#define VERBOSE

#define RXPIN        4
#define TXPIN        5

SoftwareSerial PLCSerialPort(RXPIN, TXPIN);
ModbusSerial ModbusSerial(&PLCSerialPort);

struct Register {
  byte pt;           // point type
  unsigned int sa;   // start address
  byte np;           // num points
};

static short slaveId=0;
static short polltime=0;
static short numregisters=0;
static short offset=6;

void setup() {  
}

void loop() {
}
