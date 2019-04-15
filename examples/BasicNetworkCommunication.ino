//----------------------------------------------------
// File:		BasicNetworkCommunication.ino
// Version:  	v1.0
// Change date:	15.04.2019
// Autor:    	4Source
// Homepage: 	github.com/4Source
//----------------------------------------------------

#include <NetworkCommunication.h>

extern ESP8266WebServer server;

const char* ssid = "your-ssid";				//change this
const char* wpa2 = "your-password";			//change this

IPAddress ip_static(192, 168, 2, 199);		//change this
IPAddress ip_gateway(192, 168, 2, 199);		//change this
IPAddress ip_submask(255, 255, 255, 0);		//change this

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
	
	char mode = network_Start(ssid, wpa2, "ESP-WLAN", "", ip_static, ip_gateway, ip_submask);
	if(mode == 1)
	{
		Serial.println("WIFI Station");
	}
	else if(mode == 2)
	{
		Serial.println("WIFI Accesspoint");
	}
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