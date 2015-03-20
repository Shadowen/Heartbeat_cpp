#ifndef HEARTBEAT
#define HEARTBEAT

#include <Arduino.h>

// Returns the free ram space
static inline int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

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
	
	inline void writeShort(const short& data)
	{
		Serial.write(data >> 8);
		Serial.write(data);
	}
	
	void sendByte(const byte& id, const byte& data);
	void sendString(const byte& id, const String& data);
	
	void sendHeartbeat();
	void sendMonitor(String data);
};

extern Heartbeat_class Heartbeat;

#endif