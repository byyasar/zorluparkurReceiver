#include <Arduino.h>
// Arduino nRF24Lo1 Joystick Shield Car
// Receiver Car Code
#include <nRF24L01.h>
#include <RF24.h> //https://github.com/nRF24/RF24
#include <RF24_config.h>
#include <SPI.h>
#include <printf.h>

#define CE_PIN 9
#define CSN_PIN 10

const int Enable_A = 3;
const int Enable_B = 6;
const int RightMotorF = 5; // in1
const int RightMotorB = 7; // in2
const int LeftMotorF = 2;  // in3
const int LeftMotorB = 4;  // in4

RF24 radio(CE_PIN, CSN_PIN);
const uint64_t pipe = 0xE8E8F0F0E1LL;

struct Data_Package
{
  byte hiz;
  byte solsag;
  byte ilerigeri;
  byte pot;
  byte anahtar;
};

Data_Package data; // Create a variable with the above structure
void HiziAyarla(byte pwmHizSol, byte pwmHizSag)
{
  analogWrite(Enable_A, pwmHizSag);
  analogWrite(Enable_B, pwmHizSol);
}

void Duraklat()
{
  HiziAyarla(0, 0);
  Serial.println("Duraklatıldı");
  delay(50);
  digitalWrite(RightMotorB, LOW);
  digitalWrite(LeftMotorB, LOW);
  digitalWrite(RightMotorF, LOW);
  digitalWrite(LeftMotorF, LOW);
}

void IleriGit(int hizSol, int hizSag)
{
  HiziAyarla(hizSol, hizSag);
  digitalWrite(RightMotorB, LOW);
  digitalWrite(LeftMotorB, LOW);
  digitalWrite(RightMotorF, HIGH);
  digitalWrite(LeftMotorF, HIGH);
  Serial.println("Motor forward");
}
void GeriGit(int hizSol, int hizSag)
{
  HiziAyarla(hizSol, hizSag);
  digitalWrite(RightMotorB, HIGH);
  digitalWrite(LeftMotorB, HIGH);
  digitalWrite(RightMotorF, LOW);
  digitalWrite(LeftMotorF, LOW);
  Serial.println("Motor Back");
}
void SagaHizliDon(int hizSol, int hizSag)
{
  HiziAyarla(hizSol, hizSag);
  digitalWrite(RightMotorB, HIGH);
  digitalWrite(LeftMotorB, LOW);
  digitalWrite(RightMotorF, LOW);
  digitalWrite(LeftMotorF, HIGH);
  Serial.println("hizli saga don");
}
void SolaHizliDon(int hizSol, int hizSag)
{
  HiziAyarla(hizSol, hizSag);
  digitalWrite(RightMotorB, LOW);
  digitalWrite(LeftMotorB, HIGH);
  digitalWrite(RightMotorF, HIGH);
  digitalWrite(LeftMotorF, LOW);
  Serial.println("hizli sola don");
}
void SagaDon(int hizSol, int hizSag)
{
  HiziAyarla(hizSol, hizSag);
  /*digitalWrite(RightMotorB, LOW);
  digitalWrite(LeftMotorB, LOW);
  digitalWrite(RightMotorF, LOW);
  digitalWrite(LeftMotorF, HIGH);*/
  digitalWrite(RightMotorB, LOW);
  digitalWrite(LeftMotorB, LOW);
  digitalWrite(RightMotorF, HIGH);
  digitalWrite(LeftMotorF, HIGH);
  Serial.println("yavas saga don");
}
void SolaDon(int hizSol, int hizSag)
{
  HiziAyarla(hizSol, hizSag);
  /*digitalWrite(RightMotorB, LOW);
  digitalWrite(LeftMotorB, LOW);
  digitalWrite(RightMotorF, HIGH);
  digitalWrite(LeftMotorF, LOW);*/
  digitalWrite(RightMotorB, LOW);
  digitalWrite(LeftMotorB, LOW);
  digitalWrite(RightMotorF, HIGH);
  digitalWrite(LeftMotorF, HIGH);
  Serial.println("yavas sola don");
}

void setup()
{
  pinMode(RightMotorF, OUTPUT);
  pinMode(RightMotorB, OUTPUT);
  pinMode(LeftMotorF, OUTPUT);
  pinMode(LeftMotorB, OUTPUT);
  pinMode(Enable_A, OUTPUT);
  pinMode(Enable_B, OUTPUT);

  Duraklat();
  Serial.begin(9600);
  if (!radio.begin())
  {
    Serial.println("nrf başlamadı");
    while (1)
    {
    }
  }
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  printf_begin();
  radio.printDetails();
}

void loop()
{

  if (radio.available())
  {
    radio.read(&data, sizeof(Data_Package));
    Serial.print(data.hiz);
    Serial.print("-");
    Serial.print(data.solsag);
    Serial.print("-");
    Serial.print(data.ilerigeri);
    Serial.print("-");
    Serial.print(data.pot);
    Serial.print("-");
    Serial.println(data.anahtar);
    if (data.solsag < 118)
    {

      data.ilerigeri > 140 ? SolaDon((data.solsag), (127 - data.solsag) * 2) : data.ilerigeri < 110 ? SagaDon((255 - data.solsag), (127 - data.solsag) * 2)
                                                                                                    : SolaHizliDon(data.hiz, data.hiz);
    }
    else if (data.solsag > 128)
    {
      data.ilerigeri > 140 ? SagaDon((data.solsag), (127 - data.solsag) * 2) : data.ilerigeri < 110 ? SolaDon((127 - data.solsag) * 2, (255 - data.solsag))
                                                                                                    : SagaHizliDon(data.hiz, data.hiz);
    }
    else
    {
      data.ilerigeri > 140 ? IleriGit(data.hiz, data.hiz) : data.ilerigeri < 110 ? GeriGit(data.hiz, data.hiz)
                                                                                 : Duraklat();
    }
  }

  delay(100);
}
