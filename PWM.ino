#include <Wire.h>
#include <INA3221.h>

const int pwmPin = 9;   // Pino PWM para o MOSFET

#define ADDRESS_INA3221 0x40
#define BAUD_RATE 115200
INA3221 INA_1(ADDRESS_INA3221);

float tensao_lampada;
float corrente_lampada;
float tensao_lampada2;

void setup() {
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
  INA_1.enableChannel(0);
  INA_1.enableChannel(1);
  //INA_1.disableChannel(1);
  INA_1.disableChannel(2);

  // Definindo limite para aviso crítico
  Wire.beginTransmission(ADDRESS_INA3221);
  // Configurar o pino PWM como saída
  pinMode(pwmPin, OUTPUT);

}



void loop() {
  int pwmValue = 0;

  // Loop para aumentar a largura de pulso de 0 a 255
  for (pwmValue = 0; pwmValue <= 255; pwmValue++) {
    // Definir o valor PWM para o MOSFET
    pwmValue = pwmValue + 9;
    analogWrite(pwmPin, pwmValue);
    tensao_lampada = INA_1.getBusVoltage(0);
    corrente_lampada = INA_1.getCurrent_mA(0);
    tensao_lampada2 = INA_1.getBusVoltage(1);
    Serial.print(tensao_lampada, 5);
    Serial.print(",");
    Serial.print(corrente_lampada, 5);
    Serial.print(",");
    Serial.println(tensao_lampada2, 5);
    // Atraso para suavizar a variação
    delay(1000);
  }

  // Loop para diminuir a largura de pulso de 255 a 0
  for (pwmValue = 255; pwmValue >= 0; pwmValue--) {
    // Definir o valor PWM para o MOSFET
    //pwmValue = 0;
    analogWrite(pwmPin, pwmValue);

    // Atraso para suavizar a variação
    delay(5000);
  }
  
  delay(1000);
}

/*const int pwmPin = 9;   // Pino PWM para o MOSFET

void setup() {
  // Configurar o pino PWM como saída
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  // Loop para aumentar a largura de pulso de 0 a 255
  for (int pwmValue = 0; pwmValue <= 255; pwmValue++) {
    // Definir o valor PWM para o MOSFET
    analogWrite(pwmPin, pwmValue);

    // Atraso para suavizar a variação
    delay(10);
  }

  // Loop para diminuir a largura de pulso de 255 a 0
  for (int pwmValue = 255; pwmValue >= 0; pwmValue--) {
    // Definir o valor PWM para o MOSFET
    analogWrite(pwmPin, pwmValue);

    // Atraso para suavizar a variação
    delay(10);
  }
}*/