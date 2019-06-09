//----------------------------------------------------
// File:		NetworkCommunication.h
// Version:  	v1.0.1
// Change date:	09.06.2019
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

extern void setRequest_Handler();

ESP8266WebServer server(80);		//WebServer
int8_t apConnectedDevices;			//Access Point Connected Devices Counter

//ssid WIFI name, wpa2 WIFI password, ip_ip IP of the Controller, ip_gw Gateway of the Controller, ip_sm Subnetmask of the Controller
uint8_t network_Start(const char* ssid_sta, const char* wpa2_sta, const char* ssid_ap, const char* wpa2_ap, IPAddress ip_ip, IPAddress ip_gw, IPAddress ip_sm) 
{	
	uint8_t ret = 0;
	uint8_t conatt = 5;
	uint8_t trys = 0;
	//Disconnect
	WiFi.disconnect();
	while (WiFi.status() != WL_DISCONNECTED && (trys++) < 10)
	{
		delay(100);
	}
	trys = 0;
	// WiFi.setAutoConnect(false);
	
	//Station Mode
	while (WiFi.status() != WL_CONNECTED && (trys++) < conatt)
	{
		
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid_sta, wpa2_sta);
		WiFi.config(ip_ip, ip_gw, ip_sm);
		
		int counter = 0;
		while (WiFi.status() != WL_CONNECTED && (counter++) < 10)
		{
			delay(50);
		}
	}
	if(WiFi.status() == WL_CONNECTED)
	{
		apConnectedDevices = -1;
		ret = 1;
	}
	//Access Point
	else	
	{
		apConnectedDevices=0;
		
		WiFi.disconnect(); delay(1000);
		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(ip_ip, ip_gw, ip_sm);
		WiFi.softAP(ssid_ap, wpa2_ap);
		ret = 2;
	}
	
	//WebServer
	setRequest_Handler();		//Handler
    server.begin();				//Start Webserver
	return ret;
}
//ssid WIFI name, wpa2 WIFI password
uint8_t network_Start(const char* ssid_ap, const char* wpa2_ap) 
{	
	uint8_t ret = 0;
	// uint8_t conatt = 5;
	uint8_t trys = 0;
	//Disconnect
	WiFi.disconnect();
	while (WiFi.status() != WL_DISCONNECTED && (trys++) < 10)
	{
		delay(100);
	}
	// trys = 0;
	// WiFi.setAutoConnect(false);
	
	/* //Station Mode
	while (WiFi.status() != WL_CONNECTED && (trys++) < conatt)
	{
		
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid_sta, wpa2_sta);
		// WiFi.config(ip_ip, ip_gw, ip_sm);
		
		int counter = 0;
		while (WiFi.status() != WL_CONNECTED && (counter++) < 10)
		{
			delay(50);
		}
	}
	if(WiFi.status() == WL_CONNECTED)
	{
		apConnectedDevices = -1;
		ret = 1;
	} */
	//Access Point
	// else	
	// {
		apConnectedDevices=0;
		
		// WiFi.disconnect(); delay(1000);
		WiFi.mode(WIFI_AP);
		// WiFi.softAPConfig(ip_ip, ip_gw, ip_sm);
		WiFi.softAP(ssid_ap, wpa2_ap);
		ret = 2;
	// }
	
	//WebServer
	setRequest_Handler();		//Handler
    server.begin();				//Start Webserver
	return ret;
}
		
uint8_t status_WiFi()
{
	return WiFi.status();
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