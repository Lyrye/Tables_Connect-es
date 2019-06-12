#include "Multiplexer.h"

Mux8::Mux8(int pin_0, int pin_1, int pin_2){
	pin_select[0] = pin_0;
	pin_select[1] = pin_1;
	pin_select[2] = pin_2;

	digitalWrite(pin_0, LOW);
	digitalWrite(pin_1, LOW);
	digitalWrite(pin_2, LOW);
}

void Mux8::select_output(const int output_id){

	if ( output_id == 1 ||output_id == 3 || output_id == 5 || output_id == 7)
	{
		digitalWrite(pin_select[0], HIGH); //LSB
	}
	else {
		digitalWrite(pin_select[0], LOW);
	}

  if (output_id == 2 || output_id == 3 || output_id == 6 || output_id == 7) {
		digitalWrite(pin_select[1], HIGH);
	}
	else{
		digitalWrite(pin_select[1], LOW);
	}

  if (output_id == 4 || output_id == 5 || output_id == 6 || output_id == 7) {
		digitalWrite(pin_select[2], HIGH); //MSB
	}else {
		digitalWrite(pin_select[2], LOW);
	}

}
