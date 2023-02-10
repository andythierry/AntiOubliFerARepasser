/*//==============================================================================//
  Anti oubli pour fer a repasser
  DE ANDY Thierry
  basé sur:
 * Vibration Sensor interfacing with Arduino

 * Date: - 15-04-2019

 * Author:- Sourav Gupta

 * For:- circuitdigest.com

 */ //=============================================================================//

#include <Arduino.h>

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#define ON 1

#define OFF 0

#define LONGUEUR_TEMPO 180000UL //Longueur de la tempo en millisecondes
#define LONGUEUR_PREAVIS 175000UL //Longueur du preavis d'extinction la tempo en millisecondes
#define FREQUENCE_BUZZER 1000 //Longueur de la tempo en millisecondes
#define LONGUEUR_BUZZ_CONTINU 182000UL //Longueur du preavis d'extinction la tempo en millisecondes

/*

 * Pin Description

 */
float longtemp=(float)LONGUEUR_TEMPO;
unsigned long longbuzzconti=LONGUEUR_BUZZ_CONTINU;
unsigned long longpreav=LONGUEUR_PREAVIS;
unsigned long tempo=0;
int vibration_Sensor = 2;//pin D2 c'est 5
int buzz = 3;//pin D3 c'est 6
int LED = 13;
int LED_pin=13;
float reload = 0xF424; 
volatile int count;

static bool preavfuse=LOW;
static boolean output = HIGH;
/*

 * Programme flow Description

 */




/*

 * Pin mode setup

 */

void setup() {
Serial.begin(115200);
pinMode(vibration_Sensor, INPUT);
Serial.println("Programme de rappel d'oubli d'extinction de fer à repasser");
Serial.println("*****************ANDY Thierry*****************************");
Serial.println("*********************2022*********************************");
delay(500);

pinMode(LED, OUTPUT);
pinMode(buzz, OUTPUT);
tempo=millis();
attachInterrupt(digitalPinToInterrupt(vibration_Sensor), change, CHANGE);


 //Timer register setup
cli();
TCCR1A = 0;
TCCR1B = 0; 
OCR1A = (int)reload;
TCCR1B = (1<<WGM12) | (1<<CS12); 
TIMSK1 = (1<<OCIE1A); 
sei(); 
Serial.println("TIMER1 Setup Finished.");
}
/*

 * Led blink

 */

void led_blink(void);


void change(){
tempo=millis();
Serial.print("interruption\n");
longtemp=(float)LONGUEUR_TEMPO;
longbuzzconti=LONGUEUR_BUZZ_CONTINU;
longpreav=LONGUEUR_PREAVIS;
digitalWrite(buzz, LOW); 
}
void preavis(){

  //reload=((100+LONGUEUR_TEMPO-(millis()-tempo))/10000)*0xF424;
    reload=(1-((millis()-tempo)/longtemp))*0xF424;
    reload+=3000;
 Serial.println(reload);
}

void loop() {

Serial.println(longpreav-(millis()-tempo));
Serial.print('\n');

if (millis()-tempo>longpreav&&millis()-tempo<longtemp){

  Serial.print("Préavis d'extinction dépassé::");
  Serial.println((millis()-tempo));
  preavfuse=HIGH;
  preavis();

}else if(millis()-tempo<longpreav)
{
  
  preavfuse=LOW;

}


if ((millis()-tempo>longtemp)&&(millis()-tempo<longbuzzconti)){

  Serial.print("Temps Ecoulé\n");
  Serial.println((millis()-tempo));
  //reload = 0UL;
  //tempo=millis();
  preavfuse=LOW;
   digitalWrite(buzz, HIGH); 
}
if (millis()-tempo>longbuzzconti){
  preavfuse=LOW;
  Serial.print("Fin du buzz continu\n");
  Serial.println((millis()-tempo));
  digitalWrite(buzz, LOW); 
   reload = 0xF424;
   longtemp=longtemp+(float)LONGUEUR_TEMPO;
   longbuzzconti=longbuzzconti+LONGUEUR_BUZZ_CONTINU;
   longpreav=longtemp;

}
}



void led_blink() {

}


ISR(TIMER1_COMPA_vect)
{
count++;
flash();
}


void flash()
{


digitalWrite(LED_pin, output);
if(preavfuse==HIGH){
digitalWrite(buzz, output);
}else{
  //digitalWrite(buzz, LOW);
}

output = !output;
OCR1A=(int)reload;

}
