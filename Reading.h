// Reading.h

#ifndef _READING_h
#define _READING_h

#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>


class Reading
{
public:
	void Get_weather();

	float Temp;
	float Humid;
	float Press;

private:
	Adafruit_BME280 bme;

public: 
	Reading();
};
#endif

