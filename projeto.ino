#include <math.h>
#include <Wire.h>
#include <INA3221.h>
#include <Servo.h>

// Endereço do INA3221 usado
#define ADDRESS_INA3221 0x40
#define BAUD_RATE 115200
INA3221 INA_1(ADDRESS_INA3221);

const int termopilha = A0; // Porta da termopilha

Servo meuServo;  // Cria um objeto servo para controlar o microservo
int posicao = 0; // Variável para armazenar a posição do servo
int amplitude = 22; // Angulo que max que o microservo pode fazer
int velocidade = 20; // Tempo entre as mudaças do angulo do servo

float tensao_lampada, corrente_lampada; //medidas que o INA le
float tensao_termopilha = 0; //Tensao amplificada na termopilha 
const int led_medindo = 4; //Porta 4 do Arduino

void setup() {
  // Iniciando a comunicação Serial
  Serial.begin(BAUD_RATE);
  Serial.println("Iniciar scan por dispositivos I2C:\n");
  Serial.println("Escaniando...\n");

  // Iniciando a comunicação I2C
  Wire.begin();
  Wire.beginTransmission(ADDRESS_INA3221);

  if (INA_1.begin() == false) {
      Serial.println("Comunicação não estabelecida.");
  } else {
      Serial.println("Comunicação estabelecida");
      Serial.println("Endereço:");
      Serial.println(INA_1.getAddress());
  }

  // Configurações do módulo INA3221
  INA_1.reset();
  INA_1.disableChannel(1);
  INA_1.disableChannel(2);

  // Definindo limite para aviso crítico
  Wire.beginTransmission(ADDRESS_INA3221);

  meuServo.attach(2); // Conecta o servo ao pino 2 do Arduino
  pinMode(led_medindo, OUTPUT);
}

void loop() {
  tensao_lampada = INA_1.getBusVoltage(0);
  corrente_lampada = INA_1.getCurrent_mA(0);
  abrir_termopilha();
  digitalWrite(led_medindo, HIGH);
  tensao_termopilha = valor_termopilha();
  digitalWrite(led_medindo, LOW);
  fechar_termopilha();
  printar_valores();
}

void printar_valores(){
  Serial.print(tensao_lampada, 5);
  Serial.print(",");
  Serial.print(corrente_lampada, 5);
  Serial.print(",");
  Serial.println(tensao_termopilha, 5);
  delay(10000);
}

float valor_termopilha() {
  float tensao_media = 0;
  long int tensao = 0;
  int i = 0;
  int quantidade = 1000;
  for(int i = 0; i < quantidade; i++){
    tensao_media = analogRead(termopilha);
  }
  tensao = (5.0 / 1023.0) * tensao_media / quantidade;
  return tensao;
}

// Mover o servo de 0 a amplitude
void abrir_termopilha(){
  meuServo.attach(2);
  meuServo.write(22);
  delay(500);
  meuServo.attach(3);
}

// Mover o servo da amplitude a 0 graus
void fechar_termopilha(){
  meuServo.attach(2);
  meuServo.write(0);
  delay(500);
  meuServo.attach(3);
}