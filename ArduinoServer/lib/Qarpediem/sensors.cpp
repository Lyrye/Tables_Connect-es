#include <DHT.h>
#include <ChainableLED.h>
#include <Qarpediem.h>
#include <Multiplexer.h>
#include <SensorServer.h>
#include "sensors.h"

DHT dhT(Qarpediem::C_TEMPHUM_PIN, DHT22);

extern Mux8 mux_com;
extern short int count_IR[6];
extern int mouvement;
extern ChainableLED status_led;
extern bool requestReceived;

float DISTANCE_SEUIL = 80; // Seuil de détection capteur distance
float INFRAR_SEUIL = 1;

// Data
float tableData[22] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int nb_places = 6;
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                           TEMPERATURE AMBIANTE/HUMIDITE
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
  capteur_temp() & capteur_hum(): Capteur température et humidité 101020088
   â€¢Utilise la bibliothÃ©que DHT pour lire la température (°C) et l'humidity (%) retourner par le capteur
     â€¢Comme les data sont transmise en valeur entiere , les fonction retourne de valeur entiÃ¨re
     â€¢si le capteur n'est pas connectÃ©, alors on retourne la valeur 0xFF moins la valeur d'ajout effectuer à la constitution de la trame
*/
void capteur_temp()
{
    tableData[14] = dhT.readTemperature();
}


void capteur_hum()
{
  tableData[15] = dhT.readHumidity();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                        CAPTEUR DE TEMPERATURE INFRAROUGE
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
  capteur_IR(): capteur de TÂ° IR ZTP-115M

  La data concernant les capteurs de temperature IR est consituer de 1 octet, se sont les bits qui indiqueront si oui ou non, les capteurs indique une prÃ©sence, ainsi pour le capteur numÃ©ro 1 ( adresser grÃ¢ce a la fonction Mux)
    il correspondra au bit de poids faible ( pow(2,0) =1), le capteur 2, le bit suivant ( pow(2,1)=2) ect... jusqu'Ã  la lecture des 6 capteurs maximum ( le programme peut etre converti vers 8 )

    Il est important de calibré correctement les capteurs grâce au tableau en variable globale : calibration_IR
*/
void capteur_IR ()
{
  int nbr = 1 ;
  int i =0;
  float voltage, total ;

  while(nbr < (nb_places +1))
  {
    mux_com.select_output(nbr); //dirige l'entrée vers le capteur nbr
    delay(50);
    total = 0;

    for(i = 0; i < 20; i++)
    {
      voltage  = analogRead(Qarpediem::C_IR_PIN);
      total = voltage + total;
    }
    total = total /20;
    total = (total * 5.0)/1023.0;
    if (!isnan(total))
    {
      tableData[nbr + 5] = total;
    }else
    {
      tableData[nbr + 5] = 0;
    }
    nbr++;
  }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         CAPTEUR DE DISTANCE
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
  capteur_distance(unsigned int distance[]) : capteur de distance 101020042
    Le capteur a une prÃ©cision maximale de 80 cm, une courbe d'Ã©quation a Ã©tait Ã©tabli pour obtenir une relation entre la distance mesurÃ© et sa Tenssion en sortie (25.291 (U^-1.139))
    Le systÃ¨me pouvant gÃ©rer jusqu'Ã  6 places, ce capteur reserve 6 octets dans la Trame, 1 octets par capteur, ainsi, pour les capteurs non connectÃ©s, on met cet valeur Ã  255 par dÃ©faut (254 a cause de l'opÃ©ration effectuer dans la trame)

    Puis on  indique quel capteur on veut mesurer grÃ¢ce a la fonction Mux, et on fait la moyenne d'un certain nombre de mesures
    Comme le capteur a une prÃ©cision maximum de 80 cm, si la valeur retourner est supÃ©rieur Ã  80 cm, alors on peut concidÃ©rer que le capteur n'a pas d'objet devant lui ( ni chaise ni personne)
    C'est pourquoi lorsque le capteur retourne une valeur supÃ©rieur Ã  150 cm, on est largement au dessus de la limite du capteur, pour Ã©viter ainsi un conflit avec le "0xFF", ou mÃªme  de dÃ©passer la valeur d'un octet, on dÃ©fini 150 comme valeur maximum

    enfin on retourne le tableau ainsi constituer, ( distance[0] correspond au capteur 1, distance[1] au capteur 2, ect...)
*/
void capteur_distance()
{
  double voltage = 0, total = 0;
  short int nbr = 1;
  int i = 0, j = 0;

  for (i = 0; i < nb_places; i++) {
    tableData[i] = 254; //initialisation par default comme non connectÃ©
  }

  while (nbr < nb_places +1)
  {
    total = 0;
     //dirige l'entrée vers le capteur nbr
    mux_com.select_output(nbr);

    for (j = 0; j < 20; j++) {
      voltage = analogRead(Qarpediem::C_DIST_PIN) ; //conversion de la valeur lue en Volt
      total += voltage; //somme des 50 distances mesurées
    }
    total = total / 20; //moyenne des distances
    total = (total * 5.0)/1023.0;
    total= exp((1/-0.77) * log(total/12.71)); //Conversion en cm

    //if (total > 150) total = 150;

    if(!isnan(total) && total > 0) // If it's a finite number
    {
        tableData[nbr - 1] = total;
    }else{
        tableData[nbr - 1] = 0;
    }
      nbr ++;
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                         CAPTEUR DE MOUVEMENT
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
  capteur_mouvement() : detecteur de mouvement
    DÃ©fini sur un attachInterupt dans le setup() , movement sert Ã  changer la variable "mouvement" a chaque que le capteur de mouvement change d'Ã©tat ( 0 vers 1 ou 1 vers 0)
*/
void capteur_mouvement()
{
    float mvt = digitalRead(Qarpediem::C_MOUVEMENT_PIN);
    tableData[13]= mvt; //Ajout dans le tableau principale
}

//capteur de son (desactive)
void capteur_son() {
  float son;
  mux_com.select_output(0); // Choix Multiplexeur
  son = analogRead(Qarpediem::C_SOUND_PIN);
  tableData[12] = son;
}


void state (void)
{
 status_led.setColorRGB(0, 0, 255, 0); // Red

  for(int i = 0; i < nb_places ; i++)
  {
    for(int j = 6; j<nb_places +6 ; j++)
    {
      if( tableData[i] < DISTANCE_SEUIL) //Distance
      {
        if (tableData[j] > INFRAR_SEUIL) // IR
        {
          tableData[i+15] = 2 ; // Il y a quelqu'un
          status_led.setColorRGB(0, 0, 0, 255); //Blue

        }else
        {
          tableData[i+15] = 1 ; // Il y a une chaise
          status_led.setColorRGB(0, 255, 0, 0); // Red
        }
      }else
      {
        tableData[i+15] = 0; // pas de chaise
        status_led.setColorRGB(0, 128, 128, 128); // Grey
      }
    }
  }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                     ROUTINE ("MAIN FUNCTION executed in loop()")
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*void routine()
   Correspond à la routine du programme, la fonction est éxécuter en boucle .
   Elle consiste à lire les valeurs des capteurs, vérifier les message reçu, vérifier si il faut envoyer la nouvelle Trame, et si oui, la transmettre.
*/
void sensors_routine() {

    int places= 0;
    Serial.println(F("==Sensors routine=="));

    pinMode(Qarpediem::MUX_PIN1,OUTPUT);
    pinMode(Qarpediem::MUX_PIN2,OUTPUT);
    pinMode(Qarpediem::MUX_PIN3,OUTPUT);
    dhT.begin();
    status_led.init();

    //Initialisation des valeurs de chaque capteurs
    capteur_distance();
    capteur_IR();
    capteur_son();
    capteur_mouvement();
    capteur_hum();
    capteur_temp();

    for(int j = 0; j< 6; j++) // Compte du nombre de places maximum à la table
    {
      if (tableData[j] > 10 && tableData[j] < 1000 )
      {
        places ++;
      }
    }

    nb_places = places;

    Serial.println(nb_places); // On afiche le nombre de place
    state();


    int i = 0;
    while(i < 16) {

  // Choix d'affichage des capteurs actifs
      if ( i == nb_places) // Si on a afficher toutes les distances
      {
        i = 6 ; //On passe a l'affichage des IR
      }
      if ( i == nb_places+6) // Si on a afficher toutes les IR
      {
        i = 12; // On passse au mvt
      }

      switch(i){

        /*case 0:
        Serial.print("Distance 1: ");
        Serial.println(tableData[i]);
        Serial.print("Etat");
        Serial.println(tableData[16]);
        break;

        case 1:
        Serial.print("Distance 2: ");
        Serial.println(tableData[i]);
         Serial.print("Etat");
        Serial.println(tableData[17]);
        break;

        case 2:
        Serial.print("Distance 3: ");
        Serial.println(tableData[i]);
        //Serial.print("Etat");
        //Serial.println(tableState[i]);
        break;

        case 3:
        Serial.print("Distance 4: ");
        Serial.println(tableData[i]);
        //Serial.print("Etat");
        //Serial.println(tableState[i]);
        break;

        case 4:
        Serial.print("Distance 5: ");
        Serial.println(tableData[i]);
        //Serial.print("Etat");
        //Serial.println(tableState[i]);
        break;

        case 5:
        Serial.print("Distance 6: ");
        Serial.println(tableData[i]);
        //Serial.print("Etat");
        //Serial.println(tableState[i]);
        break;

        case 6:
        Serial.print("IR 1: ");
        Serial.println(tableData[i]);
        break;

        case 7:
        Serial.print("IR 2: ");
        Serial.println(tableData[i]);
        break;

        case 8:
        Serial.print("IR 3: ");
        Serial.println(tableData[i]);
        break;

        case 9:
        Serial.print("IR 4: ");
        Serial.println(tableData[i]);
        break;

        case 10:
        Serial.print("IR 5: ");
        Serial.println(tableData[i]);
        break;

        case 11:
        Serial.print("IR 6: ");
        Serial.println(tableData[i]);
        break;

        case 12:
        Serial.print("Bruit ");
        Serial.println(tableData[i]);
        break;

        case 13:
        Serial.print("Mouvement  ");
        Serial.println(tableData[i]);
        break;

        case 14:
        Serial.print("Temperature: ");
        Serial.println(tableData[i]);
        break;
*/
        case 15:
        Serial.print("Humidité: ");
        Serial.println(dhT.readHumidity());
        break;

      }
      i++;
   }
} // Fin de routine()
