#include <Sodaq_DS3231.h>

unsigned long duracion;
unsigned long distancia;

#include <Wire.h>
#include <LowPower.h>
//-----------año-mes-dia-hora-min-seg-----------------//
char diasemana [][6] = {"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};
//-----------------digitales-------------------//
#define bomba 2
//#define trig 12
//#define echo 13
#define nivelagua 4
#define rojo 11
#define tiras 3
//--------------------------------------------------------
// hora 1 //
int ttHR1i = 20;     //hora inicial 1
int ttMIN1i = 00;    //min inicial 1
int ttHR1f = 20;     //hora final 1
int ttMIN1f = 01;    //min final 1
// hora 2 //
int ttHR2i = 20;     //hora inicial 2
int ttMIN2i = 05;    //min inicial 2
int ttHR2f = 20;     //hora final 2
int ttMIN2f = 06;    //min final 2

int estado=0;
//---------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
rtc.begin();
Wire.begin();
delay(2000);
//Serial.begin(9600);      (si es que quiero medir)
pinMode(bomba, OUTPUT);
pinMode(rojo, OUTPUT);    // (indicador de cantidad de H2O)
pinMode(tiras, OUTPUT);   // (tira de luces led)
//pinMode(trig, OUTPUT);
//pinMode(echo, INPUT);
pinMode(nivelagua, INPUT);
LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_ON, 
               SPI_OFF, USART0_OFF, TWI_OFF);
}
//---------------------------------------------------------LOOP--------------------------------------------------------//
void loop() {
  int ValorLDR = analogRead(A7);
  //a continuacion puedo cambiar la cantidad de luz min para que se prendan los leds
  if (ValorLDR < 500){
    digitalWrite(tiras, HIGH);
  }
  if(ValorLDR > 600){
    digitalWrite(tiras, LOW);
   }
  DateTime now = rtc.now();
//----------Bomba de agua-------------//
int hora = now.hour();
int minuto = now.minute();
int seg = now.second();

if((ttHR1i <= hora && ttMIN1i <= minuto && ttHR1f >= hora && ttMIN1f > minuto) || (ttHR2i <= hora && ttMIN2i <= minuto && ttHR2f >= hora && ttMIN2f > minuto)){

estado = digitalRead(nivelagua);


//  digitalWrite(trig, LOW);
//  delayMicroseconds(4);
//  digitalWrite(trig, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trig, LOW);
//
//  duracion = pulseIn(echo, HIGH);
//  duracion = duracion /2;
//  distancia = duracion /29;
  // a continuacion puedo cambiar cantidad de agua en el tanque
    if(nivelagua == HIGH){
      digitalWrite(bomba, HIGH);
      digitalWrite(rojo, LOW);
    }
    if(nivelagua == LOW){
      digitalWrite(bomba, LOW);
      digitalWrite(rojo, HIGH);
    }
 } 
else{
  digitalWrite(bomba, LOW);   
}
//Serial.print("sensor ultrasonico:  ");
//Serial.print(distancia);                      (medir distancia de sensor a H2O)
//Serial.println("LDR");
//Serial.print(ValorLDR);                       (medir luz)
}  
