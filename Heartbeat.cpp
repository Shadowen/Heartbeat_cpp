#include "Heartbeat.h"

// Singleton object
Heartbeat_class Heartbeat;

Heartbeat_class::Heartbeat_class():
	serialState(WAIT_IDENTIFIER)
{
}

Heartbeat_class::~Heartbeat_class()
{
}

void Heartbeat_class::begin(void (*callback)(byte, byte, void*))
{
	Serial.begin(9600);
	parseData_callback = callback;
}

void serialEvent(){
	Heartbeat.checkSerial();
}

void Heartbeat_class::checkSerial(){
  if (Serial.available() > 0){
    switch(serialState){
    case WAIT_IDENTIFIER:
      // Recieving a new transmission
      identifier = Serial.read();
      sendMonitor("ID: " + String(identifier));
    case WAIT_LENGTH:
      bytesExpected = Serial.read();
      // Did not find bytesExpected
      if (bytesExpected == -1){
        serialState = WAIT_LENGTH;
        break;
      }
      sendMonitor("BytesExpected: " + String(bytesExpected));
      // Prepare for reading the message
      bytesRecieved = 0;
    case WAIT_MESSAGE:
      // Read the message
      bytesRecieved += Serial.readBytes(serialBuffer + bytesRecieved, bytesExpected - bytesRecieved);
      // Message is incomplete
      if (bytesRecieved != bytesExpected){
        serialState = WAIT_MESSAGE;
        break;
      }
      // Message is complete
	  if (parseData_callback != NULL)
	  {
		parseData_callback(identifier, bytesRecieved, serialBuffer);
      }
	  serialState = WAIT_IDENTIFIER;
      break;
    default:
      // Error
      sendMonitor("Invalid serial state!");
    }
  }
}

void Heartbeat_class::sendMonitor(String text){
  sendString(1, text);
}

void Heartbeat_class::sendHeartbeat(){
  Serial.write(0);
  Serial.write(0);
}

void Heartbeat_class::sendByte(const byte& id, const byte& data)
{
  Serial.write(id);
  Serial.write(1);
  Serial.write(data);	
}

void Heartbeat_class::sendString(const byte& id, const String& data){
  Serial.write(id);
  Serial.write(data.length());
  Serial.print(data.c_str());
}