#include "Reading.h"
#include "HW.h"



Reading::Reading()
{
	int c = 0;

	Serial.println("Starting BME");
	while ((!bme.begin(0x76, &Wire)) && (c < 10)) {
		Serial.println("Could not find BME280 sensor!");
		delay(1000);
		c++;                              
	}
	if (c < 10)
		Serial.println("BME280 sensor OK !");
	else
		Serial.println("BME280 ERROR starting device");

	bme.setSampling(
		Adafruit_BME280::MODE_NORMAL,
//		Adafruit_BME280::MODE_FORCED,
		Adafruit_BME280::SAMPLING_X1,
		Adafruit_BME280::SAMPLING_X1,
		Adafruit_BME280::SAMPLING_X1,
//		Adafruit_BME280::FILTER_X4,
		Adafruit_BME280::FILTER_OFF,
		Adafruit_BME280::STANDBY_MS_10);

	Serial.println("Completed Startup");
}


void Reading::Get_weather()
{
	bme.takeForcedMeasurement();

	Temp = bme.readTemperature();  // Read temp before pressure
	Humid = bme.readHumidity();

	Press = bme.readPressure();
	if (Press == NAN)
		Press = bme.readPressure();
	Press /= 100.0F;

	Serial.printf("T: %.2f -P: %.2f -H: %.2f\n", Temp, Press, Humid);
}

