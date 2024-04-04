//Transmitter Code
#include <nRF24L01.h>
#include <RF24.h>  //https://github.com/nRF24/RF24
#include <SPI.h>
#include <printf.h>


#define CE_PIN 7
#define CSN_PIN 8

int myDelay = 50;

#define HIZ A0
#define SOLSAG A2
#define ILERIGERI A3
#define POT A6
#define ANAHTAR A5


struct Data_Package {
  byte hiz;
  byte solsag;
  byte ilerigeri;
  byte pot;
  byte anahtar;
};




Data_Package data;  //Create a variable with the above structure
int buttons[] = { ANAHTAR };


const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(CE_PIN, CSN_PIN);


void ResetData() {
  data.hiz = 0;
  data.solsag = 127;
  data.ilerigeri = 127;
  data.pot = 127;
  data.anahtar = 0;
}


void setup() {
  for (int i; i < 7; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    digitalWrite(buttons[i], HIGH);
  }
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  printf_begin();
  radio.printDetails();
  ResetData();
  delay(1000);
}


void loop() {
 
  data.hiz = map(analogRead(HIZ), 0, 1023, 0, 255);
  data.solsag = map(analogRead(SOLSAG), 0, 1023, 0, 255);
  data.ilerigeri = map(analogRead(ILERIGERI), 0, 1023, 0, 255);
  data.pot = map(analogRead(POT), 0, 1023, 0, 255);
  data.anahtar = !digitalRead(ANAHTAR);
  delay(myDelay);
  radio.write(&data, sizeof(Data_Package));  // Send the whole data from the structure to the receiver


  Serial.print(data.hiz);
  Serial.print("-");
  Serial.print(data.solsag);
  Serial.print("-");
  Serial.print(data.ilerigeri);
  Serial.print("-");
  Serial.print(data.pot);
  Serial.print("-");
  Serial.println(data.anahtar);
 
}


