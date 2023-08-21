#include <Arduino.h>
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

char buffer[5]; //declaro como vector porque la funcion readBytesUntil() solo toma de argumento un vector de tipo char o byte
byte cache;     //declaro como byte la variable donde guardar los datos devueltos por la funcion readBytesUntil(), que solamente corresponde con la cantidad de bytes leidos

//Declaraciones Modos de Operacion
void Melodias(); //funcion correspondiente a la pantalla del Generador de Melodias. Aqui se espera la eleccion de una nota mientras no se vuelva a la pantalla inicial
void Detector(); //funcion correspondiente a la pantalla del Detector de Notas. Aqui se espera la confirmacion del inicio del muestreo y reproduccion de la señal del microfono



//Declaraciones Accion

byte muestras[200] PROGMEM={127,
130,
134,
138,
142,
146,
150,
154,
158,
162,
166,
170,
173,
177,
181,
184,
188,
191,
195,
198,
201,
204,
207,
210,
213,
216,
219,
222,
224,
227,
229,
232,
234,
236,
238,
240,
241,
243,
245,
246,
247,
248,
250,
250,
251,
252,
252,
253,
253,
253,
254,
253,
253,
253,
252,
252,
251,
250,
250,
248,
247,
246,
245,
243,
241,
240,
238,
236,
234,
232,
229,
227,
224,
222,
219,
216,
213,
210,
207,
204,
201,
198,
195,
191,
188,
184,
181,
177,
173,
170,
166,
162,
158,
154,
150,
146,
142,
138,
134,
130,
126,
123,
119,
115,
111,
107,
103,
99,
95,
91,
87,
83,
80,
76,
72,
69,
65,
62,
58,
55,
52,
49,
46,
43,
40,
37,
34,
31,
29,
26,
24,
21,
19,
17,
15,
13,
12,
10,
8,
7,
6,
5,
3,
3,
2,
1,
1,
0,
0,
0,
0,
0,
0,
0,
1,
1,
2,
3,
3,
5,
6,
7,
8,
10,
12,
13,
15,
17,
19,
21,
24,
26,
29,
31,
34,
37,
40,
43,
46,
49,
52,
55,
58,
62,
65,
69,
72,
76,
80,
83,
87,
91,
95,
99,
103,
107,
111,
115,
119,
123}; 

/*Las siguientes son las funciones donde se ejecutara cada PWM en base a las muestras antes definidas*/
void SenoidalC();
void SenoidalDb();
void SenoidalD();
void SenoidalEb();
void SenoidalE();
void SenoidalF();
void SenoidalGb();
void SenoidalG();
void SenoidalAb();
void SenoidalA();
void SenoidalBb();
void SenoidalB();

void Microfono(); //funcion que ejecuta la accion prototipo del Detector de notas.

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32"); //Nombre del dispositivo Bluetooth
  Serial.println("The device started, now you can pair it with bluetooth!"); //mensaje de aviso

  /*Más aclaraciones en archivo README*/
    
  ledcSetup(0, 200000, 8); //Aqui configuro la frecuencia y resolucion del canal PWM que voy a utilizar
  ledcAttachPin(32 , 0);   //Aqui asigno los canales configurados a pines de la placa

  /*Aqui configuro y asigno a pin el canal de la PWM para el detector de nota. Posee una resolucion de 8
  bits a diferencia de la resolucion de 12 del ADC ya que permite que la diferencia de bits entre el umbral y los
  picos captados sea mas notoria en la salida al LED debido a la mayor distancia en voltaje entre muestra y muestra 
  para una menor resolucion*/
  ledcSetup(1, 200000, 8);  
  ledcAttachPin(15, 1);


  Serial.println("\n\nInicio");  
  while(!SerialBT.available()){                   // mientras no haya datos en el buffer serial (es decir, mientras que por Bluetooth no se reciba nada) espera
    Serial.println("Esperando...");
    delay(3000);
  }
  cache=SerialBT.readBytesUntil('\n', buffer, 3); //Aclaraciones sobre readBytesUntil() en el archivo README
}


/*En el void loop aparece el switch principal, donde se determinara, en base a lo decidido
por el usuario, el ingreso a uno u otro modo de operacion*/

void loop() {
  switch(buffer[0]){
    case 'G':
      Melodias();
      break;
    case 'T':
      Detector();
      break;
  }
    
  /*Cuando se rompa el bucle principal de algun modo de operacion, volverá al loop, 
  saldra del switch y esperara de vuelta la elección de modo*/
    
  Serial.println("\n\nVuelta a inicio");
  while(!SerialBT.available()){                   
    Serial.println("Esperando...");
    delay(3000);
    }
    cache=SerialBT.readBytesUntil('\n', buffer, 3);
}



//Modos de Operacion


void Melodias(){
    Serial.println("\n\n\nMelodias");

    /*El siguiente bucle se va a ejecutar mientras la lectura no sea V, caracter que se 
    envía al presionar el boton de retorno para volver a la pantalla principal en la app*/
    
    while(buffer[0]!='V'){
        while(!SerialBT.available()){                          //Aqui espero la llegada de datos desde la app
            Serial.println("Esperan2...");
            delay(2000);
        }
        cache=SerialBT.readBytesUntil('\n', buffer, 3);
    
        /*En base a lo recibido, ingreso al switch del Generador para determinar qué PWM reproduzco a la salida*/
        
        switch(buffer[0]){
            case 'C':
                SenoidalC();
                break;
            case 'c':
                SenoidalDb();
                break;
            case 'D':
                SenoidalD();
                break;
            case 'd':
                SenoidalEb();
                break;
            case 'E':
                SenoidalE();
                break;
            case 'F':
                SenoidalF();
                break;
            case 'f':
                SenoidalGb();
                break;
            case 'G':
                SenoidalG();
                break;
            case 'g':
                SenoidalAb();
                break;
            case 'A':
                SenoidalA();
                break;
            case 'a':
                SenoidalBb();
                break;
            case 'B':
                SenoidalB();
                break;
        }
    }
}

void Detector(){
    while(buffer[0]!='V'){                              //mientras no se presione el botón de volver
        Serial.println("\n\n\nDetector");
        Serial.println("Enter para iniciar");
        while(!SerialBT.available()){                   //se espera un dato
            Serial.println("Esperan3...");
            delay(2000);
        }
        cache=SerialBT.readBytesUntil('\n', buffer, 3);
        if(buffer[0]=='E'){                             //si es E, es decir, si se presiona el boton "Comienzo"
            Microfono();                                //ejecuto la funcion Microfono
        }
    }
}


//ACCION

/*Aclaraciones en archivo README*/

void SenoidalC(){
    Serial.println("Senoidal C");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(144.51);
        }
    }
    ledcWrite(0, 0);                    //al romperse el bucle, la salida se pone a 0.
}

void SenoidalDb(){
  Serial.println("Senoidal c");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(135.85);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalD(){
  Serial.println("Senoidal D");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(128.13);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalEb(){
  Serial.println("Senoidal d");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(121.2);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalE(){
  Serial.println("Senoidal E");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(114.95);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalF(){
  Serial.println("Senoidal F");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(106.63);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalGb(){
  Serial.println("Senoidal f");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(101.69);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalG(){
  Serial.println("Senoidal G");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(95.04);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalAb(){
  Serial.println("Senoidal g");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(89.15);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalA(){
  Serial.println("Senoidal A");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(83.9);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalBb(){
  Serial.println("Senoidal a");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(79.2);
        }
    }
    ledcWrite(0, 0);
}

void SenoidalB(){
  Serial.println("Senoidal B");
    while(!SerialBT.available()){
        for (int j = 0; j < 200; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(73.64);
        }
    }
    ledcWrite(0, 0);
}




void Microfono(){
    int Res;                                      //declaro una variable en la que guardar la diferencia entre umbral y picos captados
    Serial.println("Entre al bucle mic");

    while(buffer[0] != 'P' && buffer[0] != 'V'){  //mientras los datos recibidos no correspondan al boton de Parada o Volver
        while(!SerialBT.available()){             //ejecuto la siguiente acción mientras no se reciban datos
            Res = 2000 - analogRead(36);          //habiendo configurado y medido el umbral, establecemos una diferencia entre ese valor y el valor leído
            if(abs(Res)>150){                     //como el valor del umbral no es completamente estable, suelen haber variaciones minimas, pero si la diferencia supera los 100 bits
                ledcWrite(0, abs(Res));           //enciendo el LED con un ciclo de trabajo correspondiente a la diferencia. El valor esta tomado como un absoluto porque los picos aparecen con valores tanto mayores como menores al umbral
                delay(1);
            }else{
                ledcWrite(0, 0);                  //si esa diferencia no se cumple, apago la salida.
            }              
            Serial.println(analogRead(36));
            delay(1);
        }
        cache=SerialBT.readBytesUntil('\n', buffer, 3); //leo el dato que rompió el bucle. Si es V, romperá tambien el bucle principal de Detector. Si es P, romperá solo este bucle y volverá a esperar que se presione boton de Comienzo
    }
    ledcWrite(0, 0);                                   //apago la salida
}
