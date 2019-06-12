#include <Arduino.h>
#include <ChainableLED.h>
#include <DHT.h>
#include <DuplexBase.h>
#include <Multiplexer.h>
#include <SensorAverage.h>
#include <Qarpediem.h>
#include <sensors.h>
using namespace Qarpediem;

SoftwareSerial softSerial(Qarpediem::RX_PIN, Qarpediem::TX_PIN);

SensorServer sensors_server(Qarpediem::SERIAL_COM);

void setup(){
	Serial.begin(9600);
	Serial.println();
	Serial.println(F("--------------------------------------------"));
	Serial.println(F("| QARPEDIEM - Connected Table Sensor Board |"));
	Serial.println(F("--------------------------------------------"));
	Serial.flush();

	sensors_server.init();

	Serial.println(F("Starting interqarpe interface..."));
}

void loop(){

	sensors_server.routine();
  
}
