//----------------------------------------------------
// File:		NetworkCommunication.h
// Version:  	v1.0.7
// Change date:	12.09.2019
// Autor:    	4Source
// Homepage: 	github.com/4Source
//----------------------------------------------------
#ifndef NetworkCommunication_h
#define NetworkCommunication_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include <arduino.h>
#endif

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "yourSSID"
#define STAWPA2  "yourPW"
#endif

#ifndef APSSID
#define APSSID "ESP-AP"
#define APWPA2  ""
#endif

extern void setRequest_Handler();

ESP8266WebServer server(80);		//WebServer
int8_t apConnectedDevices;			//Access Point Connected Devices Counter

uint8_t getIP_fourth_octet() 
{	
	uint8_t fourth_octet;
	//ESP-01
	if(WiFi.macAddress() == "5C:CF:7F:74:6E:C4")
	{
		fourth_octet = 198;	
	}
	//ESP-02
	else if(WiFi.macAddress() == "5C:CF:7F:74:6D:C7")
	{
		fourth_octet = 199;	
	}
	//other
	else
	{
		fourth_octet = 197;	
	}
	return fourth_octet;		
}

uint8_t network_Start() 
{	
	uint8_t ret = 0;
	uint8_t conatt = 5;
	uint8_t trys = 0;
		
	IPAddress ip_ip(192, 168, 2, getIP_fourth_octet());			//IPAddress
	IPAddress ip_gw(192, 168, 2 , 1);	//Gatway 
	IPAddress ip_sm(255, 255, 255, 0);	//Subnetmask
		
	//Disconnect
	WiFi.disconnect();
	while (WiFi.status() != WL_DISCONNECTED && (trys++) < 10)
	{
		delay(500);
	}
	while(WiFi.softAPdisconnect(true))
	{
		delay(500);
	}
	
	
	//Station Mode
	trys = 0;
	while ((trys++) < conatt)
	{
		char ssid[32] = STASSID;
		char wpa2[64] = STAWPA2;
		
		WiFi.mode(WIFI_STA);
		WiFi.persistent(false);
		WiFi.begin(ssid, wpa2);
		WiFi.config(ip_ip, ip_gw, ip_sm);
		
		if(WiFi.status() == WL_CONNECTED)
		{
			apConnectedDevices = -1;
			ret = 1;
			digitalWrite(BUILTIN_LED, LOW);
			break;
		}
		
		delay(500);
		Serial.print(".");
	}
	
	//Access Point
	if(ret == 0)
	{
		trys = 0;	
		//Disconnect
		WiFi.disconnect();
		while (WiFi.status() != WL_DISCONNECTED && (trys++) < 10)
		{
			delay(500);
		}
		
		char ssid[32] = APSSID;
		char wpa2[64] = APWPA2;

		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(ip_ip, ip_gw, ip_sm);
		WiFi.softAP(ssid, wpa2);
		ret = 2;
		
		apConnectedDevices = 0;
	}
	
	//WebServer
	setRequest_Handler();		//Handler
    server.begin();				//Start Webserver
	return ret;
}

uint8_t network_Start(enum WiFiMode mode) 
{	
	uint8_t ret = 0;
	uint8_t conatt = 5;
	uint8_t trys = 0;
		
	IPAddress ip_ip(192, 168, 2, getIP_fourth_octet());			//IPAddress
	IPAddress ip_gw(192, 168, 2 , 1);	//Gatway 
	IPAddress ip_sm(255, 255, 255, 0);	//Subnetmask
	
	//Disconnect
	WiFi.disconnect();
	while (WiFi.status() != WL_DISCONNECTED && (trys++) < 10)
	{
		delay(500);
	}
	while(WiFi.softAPdisconnect(true))
	{
		delay(500);
	}
	
	trys = 0;
	WiFi.mode(mode);
	if(mode == WIFI_STA)
	{
		char ssid[32] = STASSID;
		char wpa2[64] = STAWPA2;
		
		WiFi.persistent(false);
		WiFi.config(ip_ip, ip_gw, ip_sm);
		
		while (WiFi.status() != WL_CONNECTED && (trys++) < 50) 
		{
			WiFi.begin(ssid, wpa2);
		}
		
		if(WiFi.status() == WL_CONNECTED)
		{
			ret = 1;
			apConnectedDevices = -1;
			digitalWrite(BUILTIN_LED, LOW);
		}
	}
	else if (mode == WIFI_AP)
	{
		char ssid[32] = APSSID;
		char wpa2[64] = APWPA2;
		
		WiFi.softAPConfig(ip_ip, ip_gw, ip_sm);
		
		WiFi.softAP(ssid, wpa2);
		
		ret = 2;
		apConnectedDevices = 0;
	}
	//WebServer
	setRequest_Handler();		//Handler
    server.begin();				//Start Webserver
	return ret;
}

uint8_t status_WiFi()
{
	return WiFi.status();
}

void SerialOut(uint8_t net_mode)
{
	if(net_mode == 0)
	{
		Serial.println("Error!");
	}
	else if(net_mode == 1)	//WiFi Station
	{
		Serial.println("WIFI Station");
		Serial.print("IP Address: "); Serial.println(WiFi.localIP()); 
	}
	else if(net_mode == 2)	//WiFi Accesspoint
	{
		Serial.println("WIFI Accesspoint");
		Serial.print("IP Address: "); Serial.println(WiFi.softAPIP()); 
	}
}

void handleClient()
{
	server.handleClient();
}

bool checkClients()
{
	if(apConnectedDevices >= 0 && WiFi.softAPgetStationNum() != apConnectedDevices)
	{
		apConnectedDevices = WiFi.softAPgetStationNum();
		return true;
	}
	return false;
}

uint8_t getCountDevices()
{
	return apConnectedDevices;
}
		
#endif