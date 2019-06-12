#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <Arduino.h>



void capteur_temp(void);
void capteur_hum(void);
void capteur_IR(void);
void capteur_distance();
void capteur_mouvement(void);
void capteur_son(void);
unsigned int data_mvt();
void createDataTable(unsigned int data_mvmt, unsigned int bruit);
boolean event(void);
void color(unsigned int data_IR);
void get_table_status(unsigned int data_IR);
void sensors_routine();
void state (void);
#endif
