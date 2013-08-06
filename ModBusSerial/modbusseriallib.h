#ifndef MODBUSSERIAL_h
#define MODBUSSERIAL_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#define VERBOSE

class ModbusSerial
{
  private:
    SoftwareSerial *SerialPort;
    void GetCRC(byte*, short, byte*);
    void BuildMessage(byte, byte, unsigned short, unsigned short, byte*, short);
    boolean CheckResponse(byte*, short);
    void GetResponse(byte*, short&);
  public:
    int status;
    ModbusSerial(SoftwareSerial*);
    boolean fc1(byte, unsigned short, unsigned short, unsigned short*);
    boolean fc2(byte, unsigned short, unsigned short, unsigned short*);
    boolean fc3(byte, unsigned short, unsigned short, unsigned short*);
    boolean fc4(byte, unsigned short, unsigned short, unsigned short*);
    boolean fc5(byte, unsigned short, boolean);
    boolean fc6(byte, unsigned short, unsigned short);
    boolean fc15(byte, unsigned short, unsigned short, boolean*);
    boolean fc16(byte, unsigned short, unsigned short, unsigned short*);
};

#endif
