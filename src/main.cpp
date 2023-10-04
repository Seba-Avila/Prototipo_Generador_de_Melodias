#include <Arduino.h>
#include "arduinoFFT.h"
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
arduinoFFT FFT;

const uint16_t samples = 512; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 5000; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[samples];
double vImag[samples];

char buffer[5]; //declaro como vector porque la funcion readBytesUntil() solo toma de argumento un vector de tipo char o byte
byte cache;     //declaro como byte la variable donde guardar los datos devueltos por la funcion readBytesUntil(), que solamente corresponde con la cantidad de bytes leidos

//Declaraciones Modos de Operacion
void Melodias(); //funcion correspondiente a la pantalla del Generador de Melodias. Aqui se espera la eleccion de una nota mientras no se vuelva a la pantalla inicial
void Detector(); //funcion correspondiente a la pantalla del Detector de Notas. Aqui se espera la confirmacion del inicio del muestreo y reproduccion de la señal del microfono



//Declaraciones Accion

byte muestras[50] PROGMEM={127,
142,
158,
173,
188,
201,
213,
224,
234,
241,
247,
251,
253,
253,
251,
247,
241,
234,
224,
213,
201,
188,
173,
158,
142,
127,
111,
95,
80,
65,
52,
40,
29,
19,
12,
6,
2,
0,
0,
2,
6,
12,
19,
29,
40,
52,
65,
80,
95,
111}; 

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

void Microfono(); //funcion que ejecuta la accion del Detector de notas.

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32"); //Nombre del dispositivo Bluetooth
  Serial.println("The device started, now you can pair it with bluetooth!"); //mensaje de aviso

  /*Más aclaraciones en archivo README*/
    
  ledcSetup(0, 10000, 8); //Aqui configuro la frecuencia y resolucion del canal PWM que voy a utilizar
  ledcAttachPin(32 , 0);   //Aqui asigno el canal configurados a un pin de la placa
  
  ledcWrite(0, 255);


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
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(599.06);
        }
    }
    ledcWrite(0, 255);                    //al romperse el bucle, la salida se pone a 1 (estado inactivo para el optoacoplador).
}

void SenoidalDb(){
  Serial.println("Senoidal c");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(564.43);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalD(){
  Serial.println("Senoidal D");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(533.54);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalEb(){
  Serial.println("Senoidal d");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(505.82);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalE(){
  Serial.println("Senoidal E");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(480.8);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalF(){
  Serial.println("Senoidal F");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(447.54);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalGb(){
  Serial.println("Senoidal f");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(427.78);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalG(){
  Serial.println("Senoidal G");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(401.16);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalAb(){
  Serial.println("Senoidal g");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(377.61);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalA(){
  Serial.println("Senoidal A");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(356.64);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalBb(){
  Serial.println("Senoidal a");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(337.83);
        }
    }
    ledcWrite(0, 255);
}

void SenoidalB(){
  Serial.println("Senoidal B");
    while(!SerialBT.available()){
        for (int j = 0; j < 50; j++) {
            ledcWrite(0, muestras[j]);
            delayMicroseconds(315.58);
        }
    }
    ledcWrite(0, 255);
}




void Microfono(){
    sampling_period_us = round(1000000*(1.0/samplingFrequency));


    while(buffer[0] != 'P' && buffer[0] != 'V'){  //mientras los datos recibidos no correspondan al boton de Parada o Volver
        while(!SerialBT.available()){             //ejecuto la siguiente acción mientras no se reciban datos
            microseconds = micros();
            for(int i=0; i<samples; i++)
            {
                vReal[i] = analogRead(36);
                vImag[i] = 0;
                while(micros() - microseconds < sampling_period_us){}
                microseconds += sampling_period_us;
            }
            FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency);
            FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
            FFT.Compute(FFT_FORWARD); /* Compute FFT */
            FFT.ComplexToMagnitude(); /* Compute magnitudes */
            double x = FFT.MajorPeak();
            Serial.println(x, 6);
            int n = (int) x;
            Serial.println(n);
            SerialBT.print(n);

            
            delay(1000); 
        }
        cache=SerialBT.readBytesUntil('\n', buffer, 3); //leo el dato que rompió el bucle. Si es V, romperá tambien el bucle principal de Detector. Si es P, romperá solo este bucle y volverá a esperar que se presione boton de Comienzo
    }
}
