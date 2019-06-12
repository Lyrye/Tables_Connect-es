#ifndef __MULTIPLEXER__
#define __MULTIPLEXER__

#include <Arduino.h>

class Mux8 {
	int pin_select[3];
public:

	Mux8(int pin_0, int pin_1, int pin_2);
	/**
	 * Select output
	 * @param enforcing output as the output of the mux
	 */
	void Mux8::select_output(const int output_id);
};
#endif // __MULTIPLEXER__
