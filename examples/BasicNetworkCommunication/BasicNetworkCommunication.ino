//----------------------------------------------------
// File:		BasicNetworkCommunication.ino
// Version:  	v1.0.7
// Change date:	12.09.2019
// Autor:    	4Source
// Homepage: 	github.com/4Source
//----------------------------------------------------
#define STASSID "yourSSID"
#define STAWPA2  "yourPW"

#include <NetworkCommunication.h>

extern ESP8266WebServer server;

//Example Handler 
//>>>
void request_Handler(){
	server.send(200);
	Serial.println("request_Handler");
}
void request_HandlerNotFound() {
	server.send(404);
	Serial.println("request_HandlerNotFound");
}
//<<<
//Commit Handler
void setRequest_Handler()
{
	server.on("/", request_Handler);
	server.onNotFound(request_HandlerNotFound);
}

//Setup
void setup(void) {
	Serial.begin(115200);
	while(!Serial);
	Serial.println();
	
	char mode = network_Start();
	
	Serial.println();
	SerialOut(mode);
	Serial.println("Ready!");
}
//Loop
void loop() 
{
	handleClient();
	if(checkClients())
	{	
		Serial.print("Connected Device(s): ");
		Serial.println(getCountDevices());
	}
}