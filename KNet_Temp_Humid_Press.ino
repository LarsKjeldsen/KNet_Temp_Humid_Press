/*
	Name:       KNet_Temp_Humid_Press.ino
	Created:	21-04-2021 16:45:09
	Author:     Lars S. Kjeldsen
*/
#include <ESPmDNS.h>
#include <Update.h>
#include <HttpsOTAUpdate.h>
#include <ArduinoOTA.h>
#include <WiFiClientSecure.h>
#include <ssl_client.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <PubSubClient.h>
#include <Adafruit_BME280.h>
#include <WiFiUdp.h>
#include <WiFiType.h>
#include <WiFiSTA.h>
#include <WiFiServer.h>
#include <WiFiScan.h>
#include <WiFiMulti.h>
#include <WiFiGeneric.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFi.h>
#include <ETH.h>
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>
#include <HW.h>
#include <Network.h>
#include <Reading.h>

#define DEEP_SLEEP_TIME 55
RTC_NOINIT_ATTR bool Maintanance_mode;

Reading* reading;

void setup()
{
	Wire.begin(SDA, SCL);
	Serial.begin(115200);

	HW_setup();
	if (!WiFi_Setup()) {
		goToLightSleep();
		ESP.restart();
	}

	if (!MQTT_Setup()) {
		goToLightSleep();
		ESP.restart();
	}

	Serial.println("Starter BME280");
	reading = new Reading();

	reading->Get_weather();
	Send_reading(reading);

	Serial.println("Initialise done");
}


void loop()
{
	unsigned long m = millis();
	reading->Get_weather();
	Send_reading(reading);

	if (!GetStatusCode())
		goToLightSleep();
	else
	{
		long i = millis() + (DEEP_SLEEP_TIME * 1000L);
		
		while (i > millis())
		{
			delay(100);
			ArduinoOTA.handle();
		}
	}
}


void goToLightSleep()
{
	Serial.println("Going to sleep...");
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);
	btStop();

	adc_power_off();
	esp_wifi_stop();
	esp_bt_controller_disable();

	// Configure the timer to wake us up!
	esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * 1000000L);
	esp_light_sleep_start();
	//    esp_deep_sleep_start();
}