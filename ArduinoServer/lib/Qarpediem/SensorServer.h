#ifndef __DUPLEX_SENSORS__
#define __DUPLEX_SENSORS__

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DuplexBase.h>
//#include <AlphaSense.h>

using namespace InterQarpe;

namespace Qarpediem {

class SensorServer : public DuplexBase {
	HardwareSerial *serial_port;
	uint32_t last_poll_average;


	int write_bytes(uint8_t* buffer, size_t buffer_length);

	int read_bytes(uint8_t* buffer, size_t buffer_length);

	size_t bytes_available();

	uint32_t now_ms(void);

	void configure_adc();

	void on_query(const char* query_str);
	void on_config(String config);
	void on_sensor(String sensor_name);
	void on_sensor_distance(char id);
	void send_table_status(char id);
	void send_raw_distance(char id);
void send_raw_tempIR(char id);
public:
	SensorServer(HardwareSerial* serial);

	void init(void);

	void routine(void);
};

};

#endif // __DUPLEX_SENSORS__
