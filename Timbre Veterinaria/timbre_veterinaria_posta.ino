#include <LowPower.h>

#define rele 10
#define trig 12
#define echo 13
#define azul 8

unsigned long duracion;
unsigned long distancia;

void setup() {
  // put your setup code here, to run once:
delay(1000);
pinMode(echo, INPUT);
pinMode(trig, OUTPUT);
pinMode(azul, OUTPUT);
pinMode(rele, OUTPUT);
LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_ON, 
                SPI_OFF, USART0_OFF, TWI_OFF);
//Serial.begin(9600);
}

void loop() {

  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duracion = pulseIn(echo, HIGH);
  duracion = duracion /2;
  distancia = duracion /29;

  //Serial.println(distancia);

  if((3 <= distancia && distancia <= 60)|| (150 <= distancia && distancia <= 160)||(distancia > 300)){
    digitalWrite(rele, LOW);
    digitalWrite(azul, HIGH);
    delay(100);
    digitalWrite(rele, HIGH);
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  else{
    digitalWrite(rele, HIGH);
    digitalWrite(azul, LOW);
    delay(30);
  }
 
}
