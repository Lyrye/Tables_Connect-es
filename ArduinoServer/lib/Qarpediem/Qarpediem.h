#ifndef __QARPEDIEM_PIN__
#define __QARPEDIEM_PIN__

#include <Arduino.h>
#include "SensorServer.h"

namespace Qarpediem {

  //Pins de selection multiplexeur
  int const MUX_PIN1 = 4;
	int const MUX_PIN2 = 5;
	int const MUX_PIN3 = 6;

    // CAPTEURS
    //pin des capteurs

    // PIN A3
    int const C_TEMPHUM_PIN = A3; // Correct (A3)

    // PIN A1
    int const C_SOUND_PIN = A1;
    int const C_IR_PIN = A1;

    // PIN A0

    int const C_DIST_PIN = A0;

    int const C_MOUVEMENT_PIN = 2;
    int const C_INTERRUPT_PIN = 0;

	   // LED
	   int const LED_CLOCK_PIN = 7; //7
	   int const LED_DATA_PIN = 8; //8

     // Nombre de places/chaises
     int const NB_PLACES = 6; // Par défaut, une table comporte 6 chaises

    // SERIAL COMMUNICATION
    int const RX_PIN = 18;
    int const TX_PIN = 19;
	  HardwareSerial* const SERIAL_COM = &Serial1;
  };

#endif // __QARPEDIEM_PIN__
