#include <LiquidCrystal_I2C.h>
//#include <LowPower.h>
#include <Sodaq_DS3231.h>
#include <DHT.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);  //creo el objeto

//-----------año-mes-dia-hora-min-seg-----------------//
char diasemana [][6] = {"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};

//------Digitales------//
#define UV 3
#define cooler 4
#define DHTPIN 5
#define leda 6
#define ledr 7
#define ledv 8
#define p1 10
#define p2 11
#define p3 12
#define electrovalvula 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); //inicializa el sensor DHT11

//-----Analogicas-----//
int PinSensorTierra = A0;

//-------Constantes-------//
// Hora a la que quiera que se riegue //
int ttHR1i = 14;     //hora inicial 1
int ttMIN1i = 30;    //min inicial 1
int ttSEG1i = 00;    //seg inicial 1
int UVHRapagado = 00;
int UVMINapagado = 00;
int cantHRUV = 8;
//-------Variables-------
int valorHtierra;
int lpulsador;
int contUV = 0;
int horav;
int tiempo = 10;
int cont = 0;
int contador = 0;

//-----------Otros-------------------

//DateTime dt(2022, 3, 25, 2, 22, 50, 5); //Para caragr hora y fecha al reloj



void setup() {
rtc.begin();
Wire.begin();
Serial.begin(9600);
//rtc.setDateTime(dt);  //descomentar si quiero setear hora y fecha en modulo reloj
delay(500);
pinMode(electrovalvula, OUTPUT);
pinMode(UV,OUTPUT);
pinMode(cooler,OUTPUT);
pinMode(leda,OUTPUT);
pinMode(ledr,OUTPUT);
pinMode(ledv,OUTPUT);
pinMode(p1,INPUT);
pinMode(p2,INPUT);
pinMode(p3,INPUT);
digitalWrite(p1, HIGH);
digitalWrite(p2, HIGH);
digitalWrite(p3, HIGH);
dht.begin();
lcd.init();
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("El comandante");
lcd.setCursor(0,1);
lcd.print("te saluda");
delay(2000);
attachInterrupt(digitalPinToInterrupt(2), RiegoManual, FALLING); //si la interrupcion se origina de 0 a 1 poner RISING
//LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_ON, 
//               SPI_OFF, USART0_OFF, TWI_OFF);
}




//---------------------------------------------------------LOOP--------------------------------------------------------//
void loop() {
  
//  lpulsador = digitalRead(pulsador);  //lectura estado del pulsador
  DateTime now = rtc.now();
  int pulsador1 = digitalRead(p1);
  int pulsador2 = digitalRead(p2);
  int pulsador3 = digitalRead(p3);
  int humedad = analogRead(PinSensorTierra);  //lectura sensor de humedad tierra
  valorHtierra = map(humedad,0,1023,100,0); //pasa a porcentajes la lectura de humedad tierra
  
  
  //----------Variables horas actuales-------------//
  int hora = now.hour();
  int minuto = now.minute();
  int seg = now.second();
  int dia = now.dayOfWeek(); ////////diasemana
  int mes = now.month();
  int anio = now.year();
  int fecha = now.date();

  //---------Variables de humedad y temperatura ambiente---------//
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //--------------Coolers se prenden y UV se apagan si la temperatura es alta---------//
  if (t>=35){
    digitalWrite(cooler, HIGH);
    //digitalWrite(UV, LOW);
  }
  else if(t<=33){
    digitalWrite(cooler, LOW);
    //digitalWrite(UV, HIGH);
  }

  //------------Prendido de electrovalvula--------------//
  if((ttHR1i == hora && ttMIN1i == minuto && seg == 0 && (dia == 1 || dia ==2))) {
    int segv = 0;
    if (contador < tiempo){  //establezco cuantos segundos quiero que riegue
      digitalWrite(electrovalvula, HIGH);
      digitalWrite(ledr, LOW);
      digitalWrite(ledv, HIGH);
      digitalWrite(leda, LOW);
      if (seg != segv){ //tener otra variable me permite regar por mas de 60s
        contador++;
      }
      segv = seg;
    }
  }
  else{
    digitalWrite(electrovalvula, LOW);
    digitalWrite(leda, HIGH);
    digitalWrite(ledr, LOW);
    digitalWrite(ledv, LOW);
    contador = 0;
  }
  //----------Apagado y pendido de UV-------------//
  if (UVHRapagado <= hora && UVMINapagado <= minuto){
    contUV = 0;
    if (horav != hora){
      contUV++;
    }
    if (contUV < cantHRUV){
      digitalWrite(UV, HIGH);
      digitalWrite(ledv,HIGH);
      digitalWrite(ledr,HIGH);
      digitalWrite(leda,LOW);
    }
    else{
      digitalWrite(UV, LOW);
      digitalWrite(ledv,LOW);
      digitalWrite(ledr,LOW);
      digitalWrite(leda,HIGH);
      contUV = 0;
    }
    horav = hora;
  }
  if ((seg >= 0 && seg <= 19 && pulsador2 == 1 && pulsador3 == 1 && cont == 0) || (pulsador3 == 1 && pulsador2 == 1 && pulsador1 == 0 && cont == 0)){
//    if (isnan(h) || isnan(t)){
//      digitalWrite(cooler, HIGH);
//      lcd.setCursor(0,0);
//      lcd.print("Error lectura en");
//      lcd.setCursor(6,1);
//      lcd.print("DHT11");
//      delay(500);
//    }
//    else if (isnan(valorHtierra)){
//      lcd.setCursor(0,0);
//      lcd.print("Error lectura en");
//      lcd.setCursor(6,1);
//      lcd.print("FC-28");
//      delay(500);
//    }
//    else{
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print(hora);
      lcd.setCursor(6,0);
      lcd.print(":");
      lcd.setCursor(7,0);
      lcd.print(minuto);
      lcd.setCursor(9,0);
      lcd.print(":");
      lcd.setCursor(10,0);
      lcd.print(seg);
      lcd.setCursor(1,1);
      lcd.print(fecha);
      lcd.setCursor(3,1);
      lcd.print("/");
      lcd.setCursor(4,1);
      lcd.print(mes);
      lcd.setCursor(6,1);
      lcd.print("/");
      lcd.setCursor(7,1);
      lcd.print(anio);
      lcd.setCursor(12,1);
      lcd.print(dia);
      delay(500);
    //}
  }
  else if ((seg >= 20 && seg <= 39 && pulsador1 == 1 && pulsador3 == 1 && cont == 0) || (pulsador3 == 1 && pulsador2 == 0 && pulsador1 == 1 && cont == 0)){
//    if (isnan(h) || isnan(t)){
//      digitalWrite(cooler, HIGH);
//      lcd.setCursor(0,0);
//      lcd.print("Error lectura en");
//      lcd.setCursor(6,1);
//      lcd.print("DHT11");
//      delay(500);
//    }
//    else if (isnan(valorHtierra)){
//      lcd.setCursor(0,0);
//      lcd.print("Error lectura en");
//      lcd.setCursor(6,1);
//      lcd.print("FC-28");
//      delay(500);
//    }
//    else{
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Temp= ");
      lcd.setCursor(10,0);
      lcd.print(t);
      lcd.setCursor(12,0);
      lcd.print("ºC");
      lcd.setCursor(3,1);
      lcd.print("Hum=  ");
      lcd.setCursor(10,1);
      lcd.print(h);
      lcd.setCursor(12,1);
      lcd.print("%");
      delay(500);
  //  }
  }
  else if ((seg >= 40 && seg <= 59 && pulsador1 == 1 && pulsador2 == 1 && cont == 0) || (pulsador3 == 0 && pulsador2 == 1 && pulsador1 == 1 && cont == 0)){
//    if (isnan(h) || isnan(t)){
//      digitalWrite(cooler, HIGH);
//      lcd.setCursor(0,0);
//      lcd.print("Error lectura en");
//      lcd.setCursor(6,1);
//      lcd.print("DHT11");
//      delay(500);
//    }
//    else if (isnan(valorHtierra)){
//      lcd.setCursor(0,0);
//      lcd.print("Error lectura en");
//      lcd.setCursor(6,1);
//      lcd.print("FC-28");
//      delay(500);
//    }
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("Humedad");
      lcd.setCursor(9,1);
      lcd.print(valorHtierra);
      lcd.setCursor(11,1);
      lcd.print("%");
      delay(500);
  }
    else if (pulsador1 == 0 && pulsador2 == 0 && pulsador3 == 0 && cont == 0){
      cont = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Saca la mano");
      lcd.setCursor(0,1);
      lcd.print("de ahi carajo");
      delay(3000);
      if (cont == 1){
        pulsador3 = 1;  
        while (pulsador3 == 1){
          pulsador1 = digitalRead(p1);
          pulsador2 = digitalRead(p2);
          pulsador3 = digitalRead(p3);
          lcd.clear();        
          lcd.setCursor(0,0);
          lcd.print("Tiempo de riego:");
          lcd.setCursor(7,1);
          lcd.print(tiempo);
          if (pulsador1 == 1 && pulsador2 == 1){
            delay(500);
          }
          else if (pulsador1 == 0){
            tiempo--;
            delay(200);
          }
          else if(pulsador2 == 0){
            tiempo++;
            delay(200);
          }
        }
        cont = 2;
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("Te vas a");
        lcd.setCursor(2,1);
        lcd.print("lectrocutar");
        delay(3000);
      }
      if(cont == 2){
        pulsador3 = 1;  
        while (pulsador3 == 1){
          pulsador1 = digitalRead(p1);
          pulsador2 = digitalRead(p2);
          pulsador3 = digitalRead(p3);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Hora de riego");
          lcd.setCursor(0,1);
          lcd.print("*");
          lcd.setCursor(1,1);
          lcd.print(ttHR1i);
          lcd.setCursor(3,1);
          lcd.print("*");
          lcd.setCursor(4,1);
          lcd.print(":");
          lcd.setCursor(5,1);
          lcd.print(ttMIN1i);
          lcd.setCursor(7,1);
          lcd.print(":");
          lcd.setCursor(9,1);
          lcd.print(ttSEG1i);
          if (pulsador1 == 1 && pulsador2 == 1){
            delay(500);
          }
          else if (pulsador1 == 0){
            ttHR1i--;
            delay(200);
          }
          else if(pulsador2 == 0){
            ttHR1i++;
            delay(200);
          }
        }
      cont = 3;
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Cortaste toda");
      lcd.setCursor(4,1);
      lcd.print("la loooz");
      delay(3000);
      }
      if (cont == 3){
        pulsador3 = 1;  
        while (pulsador3 == 1){
          pulsador1 = digitalRead(p1);
          pulsador2 = digitalRead(p2);
          pulsador3 = digitalRead(p3);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Hora de riego");
          lcd.setCursor(0,1);
          lcd.print(ttHR1i);
          lcd.setCursor(3,1);
          lcd.print(":");
          lcd.setCursor(4,1);
          lcd.print("*");
          lcd.setCursor(5,1);
          lcd.print(ttMIN1i);
          lcd.setCursor(7,1);
          lcd.print("*");
          lcd.setCursor(8,1);
          lcd.print(":");
          lcd.setCursor(9,1);
          lcd.print(ttSEG1i);
          if (pulsador1 == 1 && pulsador2 == 1){
            delay(500);
          }
          else if (pulsador1 == 0){
            ttMIN1i--;
            delay(200);
          }
          else if(pulsador2 == 0){
            ttMIN1i++;
            delay(200);
          }
        }
      cont = 4;
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("MAIAMEEEEE");
      delay(3000);
      }
      if (cont == 4){
        pulsador3 = 1;  
        while (pulsador3 == 1){
          pulsador1 = digitalRead(p1);
          pulsador2 = digitalRead(p2);
          pulsador3 = digitalRead(p3);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Hora de riego");
          lcd.setCursor(0,1);
          lcd.print(ttHR1i);
          lcd.setCursor(3,1);
          lcd.print(":");
          lcd.setCursor(4,1);
          lcd.print(ttMIN1i);
          lcd.setCursor(6,1);
          lcd.print(":");
          lcd.setCursor(7,1);
          lcd.print("*");
          lcd.setCursor(8,1);
          lcd.print(ttSEG1i);
          lcd.setCursor(10,1);
          lcd.print("*");
          if (pulsador1 == 1 && pulsador2 == 1){
            delay(500);
          }
          else if (pulsador1 == 0){
            ttSEG1i--;
            delay(200);
          }
          else if(pulsador2 == 0){
            ttSEG1i++;
            delay(200);
          }
        }
      cont = 5;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Yo no manejo el");
      lcd.setCursor(5,1);
      lcd.print("rating");
      delay(3000);
      }
     if (cont == 5){
        pulsador3 = 1;  
        while (pulsador3 == 1){
          pulsador1 = digitalRead(p1);
          pulsador2 = digitalRead(p2);
          pulsador3 = digitalRead(p3);
          lcd.clear();
          lcd.setCursor(4,0);
          lcd.print("Apagar UV");
          lcd.setCursor(0,1);
          lcd.print("*");
          lcd.setCursor(1,1);
          lcd.print(UVHRapagado);
          lcd.setCursor(4,1);
          lcd.print("*");
          lcd.setCursor(5,1);
          lcd.print(":");
          lcd.setCursor(6,1);
          lcd.print(UVMINapagado);
          lcd.setCursor(8,1);
          lcd.print(":");
          lcd.setCursor(9,1);
          lcd.print("00");
          if (pulsador1 == 1 && pulsador2 == 1){
            delay(500);
          }
          else if (pulsador1 == 0){
            UVHRapagado--;
            delay(200);
          }
          else if(pulsador2 == 0){
            UVHRapagado++;
            delay(200);
          }
        }
      cont = 6;
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Yo manejo un");
      lcd.setCursor(3,1);
      lcd.print("Rolls-Royce");
      delay(3000);
      }
      if (cont == 6){
        pulsador3 = 1;  
        while (pulsador3 == 1){
          pulsador1 = digitalRead(p1);
          pulsador2 = digitalRead(p2);
          pulsador3 = digitalRead(p3);
          lcd.clear();
          lcd.setCursor(4,0);
          lcd.print("Apagar UV");
          lcd.setCursor(0,1);
          lcd.print(UVHRapagado);
          lcd.setCursor(2,1);
          lcd.print(":");
          lcd.setCursor(3,1);
          lcd.print("*");
          lcd.setCursor(4,1);
          lcd.print(UVMINapagado);
          lcd.setCursor(8,1);
          lcd.print("*");
          lcd.setCursor(9,1);
          lcd.print(":");
          lcd.setCursor(10,1);
          lcd.print("00");
          if (pulsador1 == 1 && pulsador2 == 1){
            delay(500);
          }
          else if (pulsador1 == 0){
            UVMINapagado--;
            delay(200);
          }
          else if(pulsador2 == 0){
            UVMINapagado++;
            delay(200);
          }
        }
      cont = 7;
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Basta");
      lcd.setCursor(5,1);
      lcd.print("chicos");
      delay(3000);
      }
      if (cont == 7){
        pulsador3 = 1;  
        while (pulsador3 == 1){
          pulsador1 = digitalRead(p1);
          pulsador2 = digitalRead(p2);
          pulsador3 = digitalRead(p3);
          lcd.clear();
          lcd.setCursor(4,0);
          lcd.print("Apagar UV por");
          lcd.setCursor(6,1);
          lcd.print(cantHRUV);
          lcd.setCursor(8,1);
          lcd.print("Hs");
          if (pulsador1 == 1 && pulsador2 == 1){
            delay(500);
          }
          else if (pulsador1 == 0){
            cantHRUV--;
            delay(200);
          }
          else if(pulsador2 == 0){
            cantHRUV++;
            delay(200);
          }
        }
      cont = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("De algo hay que");
      lcd.setCursor(2,1);
      lcd.print("morir Ricardo");
      delay(3000);
      }
    }
//    else{
//      lcd.clear();
//      lcd.setCursor(0,0);
//      lcd.print(" Humedad Tierra ");
//      lcd.setCursor(7,1);
//      lcd.print(valorHtierra);
//      lcd.setCursor(9,1);
//      lcd.print("%");
//    }

//
//  Serial.print(now.year(), DEC);
//  Serial.print('/');
//  Serial.print(now.month(), DEC);
//  Serial.print('/');
//  Serial.print(now.date(), DEC);
//  Serial.print(' ');
//  Serial.print(now.hour(), DEC);
//  Serial.print(':');
//  Serial.print(now.minute(), DEC);
//  Serial.print(':');
//  Serial.print(now.second(), DEC);
//  Serial.print(' ');
//  Serial.print(diasemana[now.dayOfWeek()]);
//  Serial.println();
//  delay(1000);
//Serial.print("pulsador1 = ");
//  Serial.println(pulsador1);
// Serial.print("pulsador2 = ");
//  Serial.println(pulsador2);
//  Serial.print("pulsador3 = ");
//  Serial.println(pulsador3);
//  Serial.print("Humedad: ");
//  Serial.print(h);
//  Serial.print(" %\t");
//  Serial.print("Temperatura: ");
//  Serial.print(t);
//  Serial.print(" *C ");
}

void RiegoManual(){
  int contadorseg = 0;
  int humedad = analogRead(PinSensorTierra);
  valorHtierra = map(humedad,0,1023,100,0);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Regando Crack ");
  lcd.setCursor(0,1);
  lcd.print("Humedad ");
  lcd.setCursor(9,1);
  lcd.print(valorHtierra);
  lcd.setCursor(11,1);
  lcd.print("%");
  digitalWrite(leda, HIGH);
  digitalWrite(ledv, LOW);
  digitalWrite(ledr, HIGH);
  digitalWrite(electrovalvula, HIGH);
  delay(500);
}
