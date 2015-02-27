#ifndef HEARTBEAT
#define HEARTBEAT

#include "Arduino.h"
#include <SoftwareSerial.h>

class Heartbeat_class
{
private:
	// Static variables
	static const byte WAIT_IDENTIFIER = 0;
	static const byte WAIT_LENGTH = 1;
	static const byte WAIT_MESSAGE = 2;
	// Regular variables
	byte serialState;
	byte identifier;
	byte bytesExpected;
	byte bytesRecieved;
	char serialBuffer[50];
	// The function to call when data received is complete
	void (*parseData_callback)(byte, byte, void*);
	
	// Functions
public:
	Heartbeat_class();
	~Heartbeat_class();
	
	void begin(void (*callback)(byte, byte, void*));
	
	void checkSerial();

	inline void write(const byte& data)
	{
		Serial.write(data);
	}

	inline void write(const short& data)
	{
		Serial.write(data << 8);
		Serial.write(data);
	}
	
	void sendByte(const byte& id, const byte& data);
	void sendString(const byte& id, const String& data);
	
	void sendHeartbeat();
	void sendMonitor(String data);
};

extern Heartbeat_class Heartbeat;

#endif