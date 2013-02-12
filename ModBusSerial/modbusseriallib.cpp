#include "modbusseriallib.h"
#include <malloc.h>

ModbusSerial::ModbusSerial(SoftwareSerial *SerialPort)
{
    this->SerialPort = SerialPort;
}

void ModbusSerial::GetCRC(byte message[],short message_length, byte CRC[])
{
    unsigned int CRCFull = 0xFFFF;
    byte CRCHigh = 0xFF;
    byte CRCLow = 0xFF;
    char CRCLSB;

    for (int i = 0; i < message_length - 2; i++)
    {
        CRCFull = (unsigned short)(CRCFull ^ message[i]);

        for (int j = 0; j < 8; j++)
        {
            CRCLSB = (char)(CRCFull & 0x0001);
            CRCFull = (unsigned short)((CRCFull >> 1) & 0x7FFF);

            if (CRCLSB == 1)
                CRCFull = (unsigned short)(CRCFull ^ 0xA001);
        }
    }
    CRC[1] = CRCHigh = (byte)((CRCFull >> 8) & 0xFF);
    CRC[0] = CRCLow = (byte)(CRCFull & 0xFF);
}

void ModbusSerial::BuildMessage(byte address, byte type, unsigned short start, unsigned short registers, byte message[], short message_length)
{
    byte CRC[2];

    message[0] = address;
    message[1] = type;
    message[2] = (byte)(start >> 8);
    message[3] = (byte)start;
    message[4] = (byte)(registers >> 8);
    message[5] = (byte)registers;

    GetCRC(message,message_length, CRC);
    message[message_length - 2] = CRC[0];
    message[message_length - 1] = CRC[1];
    
    #ifdef VERBOSE
      Serial.print("MSG:");
      for (int i = 0; i < message_length-1; i++)
        {
        Serial.print(message[i]);
        Serial.print(",");
        }
      Serial.println(message[message_length]);
    #endif
}

boolean ModbusSerial::CheckResponse(byte response[], short response_length)
{
    byte CRC[2];
    GetCRC(response, response_length, CRC);
    if (CRC[0] == response[response_length - 2] && CRC[1] == response[response_length - 1])
        return true;
    else
        return false;
}

void ModbusSerial::GetResponse(byte response[], short &response_length)
{
    delay(100);
    int bytes = SerialPort->available();
    
    #ifdef VERBOSE
      Serial.print(bytes);
      Serial.println(" received.");
    #endif

    for (int i = 0; i < bytes; i++)
        response[i] = (byte)SerialPort->read();
    response_length = bytes;
        
    #ifdef VERBOSE
      Serial.print("PACKET:");
      for (int i = 0; i < bytes-1; i++)
        {
        Serial.print(response[i]);
        Serial.print(",");
        }
      Serial.println(response[bytes-1]);
    #endif
}

boolean ModbusSerial::fc1(byte address, unsigned short start, unsigned short coils, unsigned short values[])
{
    short message_length = 8;
    byte message[message_length];                                // Es Resquest fc1 posee 8 bytes
    short response_length = 5 + (int)ceil((float)((float)coils / 8.0));
    byte response[response_length];                              //FC1 response buffer
    
    start -= 1;                                                  //offset
    
    BuildMessage(address, (byte)1, start, coils, message, message_length);
    
    SerialPort->write(message, message_length);
    GetResponse(response, response_length);
    
    if (CheckResponse(response, response_length))
    {
        int totalBits = (response_length - 5) * 8;
        int count = coils;
        int index = 0;
        for (int i = 0; i < response_length - 5; i++) {
            totalBits -= 8;
            count -= 8;
            int temp = response[i + 3];
            int subs = 0;
            if (count < 0) subs = count;
            for (int j = 0; j < 8 + subs; j++) {
                values[index] = (short)(temp & 1);
                temp = temp >> 1;
                index++;
            }
        }
        #ifdef VERBOSE
        Serial.println("Read successful");
        #endif
        status = 1;
        return true;
    }
    else
    {
        #ifdef VERBOSE
        Serial.println("CRC error");
        #endif
        status = 0;
        return false;
    }
}

boolean ModbusSerial::fc2(byte address, unsigned short start, unsigned short coils, unsigned short values[])
{
    short message_lenght = 8;
    byte message[message_lenght];                                // El Resquest fc3 posee 8 bytes
    short response_length = 5 + (int)ceil((float)((float)coils / 8.0));
    byte response[response_length];                              // FC2 response buffer
    
    start -= 10001;                                              //offset
    
    BuildMessage(address, (byte)2, start, coils, message, message_lenght);
    
    SerialPort->write(message, message_lenght);
    GetResponse(response, response_length);
    
    if (CheckResponse(response, response_length)) {
        int totalBits = (response_length - 5) * 8;
        int count = coils;
        int index = 0;
        for (int i = 0; i < (response_length - 5); i++){
            totalBits -= 8;
            count -= 8;
            int temp = response[i + 3];
            int subs = 0;
            if (count < 0) subs = count;
            for (int j=0; j < 8 + subs; j++)
            {
                values[index] = (short)(temp & 1);
                temp = temp >> 1;
                index++;
            }
        }
        #ifdef VERBOSE
        Serial.println("Read successful");
        #endif
        status = 1;//"Read successful";
        return true;
        }
    else {
        #ifdef VERBOSE
        Serial.println("CRC error");
        #endif
        status = 0;// "CRC error";
        return false;
    }
}

boolean ModbusSerial::fc3(byte address, unsigned short start, unsigned short registers, unsigned short values[])
{
    byte message[8];                            //El request fc3 posee 8 bytes
    short response_length = 5 + 2 * registers;
    byte response[response_length];             //Response buffer
   
    start -= 40001;                             //offset
    
    BuildMessage(address, (byte)3, start, registers, message, (short)8);
    
    SerialPort->write(message, 8);
    delay(100);
    
    GetResponse(response, response_length);
    
    if (CheckResponse(response, response_length))
    {
        for (int i = 0; i < (response_length-5)/2; i++)
        {
            values[i]=0;
            values[i] = response[2*i+3];
            values[i] = values[i]<<8;
            values[i] += response[2*i+4];
        }
        status = 1;  //"Read successful";
        #ifdef VERBOSE
        Serial.println("Read successful");
        #endif
        return true;
    }
    else
    {
        status = 0;  // "CRC error";
        #ifdef VERBOSE
        Serial.println("CRC error");
        #endif
        return false;
    }
}

boolean ModbusSerial::fc4(byte address, unsigned short start, unsigned short registers, unsigned short values[])
{
    byte message[8];                        //El request fc3 posee 8 bytes
    short response_length = 5 + 2 * registers;
    byte response[response_length];       // Response buffer
    
    start -= 30001;
    
    BuildMessage(address, (byte)4, start, registers, message, (short)8); 
    
    #ifdef VERBOSE
      Serial.print("MSG1:FC4:");
      for (int i = 0; i < 7; i++)
        {
        Serial.print(message[i]);
        Serial.print(" ");
        }
      Serial.println(message[7]);
    #endif
    
    SerialPort->write(message, 8);
    delay(100);
    
    GetResponse(response, response_length);
    
    if (CheckResponse(response, response_length))
    {
        #ifdef VERBOSE
        Serial.print("MSG1:FC3VALUES:");
        #endif
        for (int i = 0; i < (response_length-5)/2; i++)
        {
            values[i]=0;
            values[i] = response[2*i+3];
            values[i] = values[i]<<8;
            values[i] += response[2*i+4];
            #ifdef VERBOSE
            Serial.print(values[i]);
            Serial.print(" ");
            #endif
        }
        status = 1;  //"Read successful";
        #ifdef VERBOSE
        Serial.println("MSG0:Respuesta Exitosa");
        #endif
        return true;
    }
    else
    {
        status = 0;  // "CRC error";
        #ifdef VERBOSE
        Serial.println("MSG0:Error CRC");
        #endif
        return false;
    }
}

boolean ModbusSerial::fc5(byte address, unsigned short start, boolean status)
{
    short message_lenght = 8;
    byte message[message_lenght];        // El request fc5 posee 8 bytes
    byte response[message_lenght];       // El buffer de respuesta posee 8 bytes
    short response_length;
    unsigned short value = 0;
    
    start -= 1;
    
    if (status)
      value = 0xFF;
    
    BuildMessage(address, (byte)5, start, value, message, message_lenght);
    
    SerialPort->write(message, message_lenght);
    GetResponse(response, response_length);
    
    if (CheckResponse(response, response_length))
    {
        #ifdef VERBOSE
        Serial.println("Read successful");
        #endif
        status = 1;//"Read successful";
        return true;
    }
    else
    {
        #ifdef VERBOSE
        Serial.println("Error CRC");
        #endif
        status = 0;// "CRC error";
        return false;
    }
}

boolean ModbusSerial::fc6(byte address, unsigned short start, unsigned short value)
{
    short message_lenght = 8;
    byte message[message_lenght];        // El Resquest fc6 posee 8 bytes
    byte response[message_lenght];       // El buffer Response posee 8 bytes
    short response_length;
    
    start -= 40001;
    
    message[4] = (byte)(value >> 8);
    message[5] = (byte)(value);
    BuildMessage(address, (byte)6, start, value, message, message_lenght);
    
    SerialPort->write(message, message_lenght);
    GetResponse(response, response_length);
    
    if (CheckResponse(response, response_length))
    {
        #ifdef VERBOSE
        Serial.println("Read successful");
        #endif
        status = 1;//"Read successful";
        return true;
    }
    else
    {
        #ifdef VERBOSE
        Serial.println("Error CRC");
        #endif
        status = 0;// "CRC error";
        return false;
    }
}

boolean ModbusSerial::fc15(byte address, unsigned short start, unsigned short coils, boolean values[])
{
    int numbytes = (int)ceil((float)((float)coils / 8.0));
    short message_lenght = 9 + numbytes;
    byte message[message_lenght];    
    
    short response_length = 8;
    byte response[response_length];       // El buffer Response posee 8 bytes
    
    start -= 1;
    
    message[6] = (byte)(numbytes);
    
    int totalBits = coils;
    int count = coils;
    int index = 7;
    for (int i = 0; i < numbytes; i++)
    {
        for (int j = 0; j < 8 && j + i * 8 < coils; j++)
        {
            message[i + index] = (byte)(message[i + index] | (byte)((values[j+i*8] & 1) << j));
        }
    }
    BuildMessage(address, (byte)15, start, coils, message, message_lenght); //Build modbus message
    
    SerialPort->write(message, message_lenght);
    GetResponse(response, response_length);
    
    if (CheckResponse(response, response_length))
    {
        #ifdef VERBOSE
        Serial.println("Read successful");
        #endif
        status = 1;//"Read successful";
        return true;
    }
    else
    {
        #ifdef VERBOSE
        Serial.println("Error CRC");
        #endif
        status = 0;// "CRC error";
        return false;
    }
}
    
// Function 16 - Write multiple registers
boolean ModbusSerial::fc16(byte address, unsigned short start, unsigned short registers, unsigned short values[])
{
    short message_lenght = 9 + 2 * registers;
    byte message[message_lenght];          // El Resquest fc6 posee
    byte response[8];                      // El buffer Response posee 8 bytes
    short response_length;
    
    start -= 40001;
    
    message[6] = (byte)(registers * 2);
    
    for (int i = 0; i < registers; i++) {
        message[7 + 2 * i] = (byte)(values[i] >> 8);
        message[8 + 2 * i] = (byte)(values[i]);
    }
    BuildMessage(address, (byte)16, start, registers, message, message_lenght);
    
    SerialPort->write(message, message_lenght);
    GetResponse(response, response_length);
    
    if (CheckResponse(response, response_length))
    {
        #ifdef VERBOSE
        Serial.println("Read successful");
        #endif
        status = 1;//"Write successful";
        return true;
    }
    else
    {
        #ifdef VERBOSE
        Serial.println("Error CRC");
        #endif
        status = 0;// "CRC error";
        return false;
    }
}
