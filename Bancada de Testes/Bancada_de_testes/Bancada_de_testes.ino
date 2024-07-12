// Case: Bancada de Testes 
// Amanda Jury Nakamura "Sagu"
/*   Objetivos: 
              Simular os sinais gerados pelos sensores do carro (digitais e analógicos)
              Desenvolver uma forma de testar os sensores do carro e validá-los em bancada foco para a ECUV3 */

//#include <Arduino.h>
#include <Wire.h>
#include <esp_timer.h>

#define CONST_PI 3.14159
#define DIAMETRO 0.51916 // Diâmetro da roda em metros
#define COMPRIMENTO (CONST_PI * DIAMETRO)
#define V1 15
#define V2 20
#define V3 35

//define pino DAC

//#define TEMP_SENSOR_ADDRESS 0x5A // Endereço I2C do MLX90614
//#define ACCELEROMETER_ADDRESS 0x68 // Endereço I2C do MPU6050
//Wire.begin(0x5A);
//Wire.begin(0x68);
//1 - Anterior Direito; 2- Anterior Esquerdo; 3- Eixo traseiro;
//const int velPin1 = 16;  

const int velPin1 = 16;  
const int velPin2 = 17;
const int velPin3 = 18;

const int pinRPM = 20;
const int pinFreio = 21;
const int pinTemp = 22;
const int pinAcel = 23;
const int pinDAC = 25;
volatile static bool estadoOnda1 = false; 
volatile static bool estadoOnda2 = false;
volatile static bool estadoOnda3 = false;

volatile static bool estadoRPM = false;
volatile static bool estadoFreio = false;
volatile static bool estadoOndaDAC = false;

esp_timer_handle_t timer1;
esp_timer_handle_t timer2;
esp_timer_handle_t timer3;
esp_timer_handle_t timerRPM;
//hw_timer_t * timerTemp = NULL;
//hw_timer_t * timerAcel = NULL;
unsigned long int timerFreio = 0;

//Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // Instância do sensor MLX90614
//MPU6050 mpu; // Instância do acelerômetro MPU6050

// Função para alternar o estado da onda
void alterna1(void* arg) {   
  estadoOnda1 = !estadoOnda1;
  digitalWrite(velPin1, estadoOnda1);
}

void alterna2(void* arg) {   
  estadoOnda2 = !estadoOnda2;
  digitalWrite(velPin2, estadoOnda2);
}

void alterna3(void* arg) {   
  estadoOnda3 = !estadoOnda3;
  digitalWrite(velPin3, estadoOnda3);
}


void alternaRPM(void* arg) {   
  estadoRPM = !estadoRPM;
  digitalWrite(pinRPM, estadoRPM);
}

//void IRAM_ATTR alternaFreio() {   
  //estadoFreio = !estadoFreio;
  //digitalWrit


//
int auxDAC = 0; //valor de entrada do DAC
//esse valor varia de 0 a 255 e volta para 0
void ondaTriangular () {  
    dacWrite(pinDAC, auxDAC); 
    auxDAC++;

    if (auxDAC == 256){
      auxDAC = 0;
    }

}


void setup() {
  Serial.begin(115200);
  pinMode(velPin1, OUTPUT); 
  pinMode(velPin2, OUTPUT);
  pinMode(velPin3, OUTPUT);

  Serial.println("T1");

  int t1 = (COMPRIMENTO * (1000 / 7.0) * 3.6) / V1; 
  const esp_timer_create_args_t timer_args = {
    .callback = &alterna1,  // Função de callback chamada toda vez que o timer disparar
    .arg = NULL, // Argumento para a função de callback (não utilizado aqui)
    .dispatch_method = ESP_TIMER_TASK, // Método de despacho (tarefa do timer)
    .name = "timer1" // Nome do timer 
  };
  esp_err_t err =  esp_timer_create(&timer_args, &timer1);
  if (err != ESP_OK) {
    Serial.println("Erro ao criar o timer");
    return;
  }
  err = esp_timer_start_periodic(timer1, t1/2*1000); // 500000 microsegundos = 500 ms
  if (err != ESP_OK) {
    Serial.println("Erro ao iniciar o timer");
  } else {
    Serial.println("Início do Timer com sucesso");
  }

  int t2 = (COMPRIMENTO * (1000 / 7.0) * 3.6) / V2; 
  const esp_timer_create_args_t timer_args2 = {
    .callback = &alterna2,  // Função de callback chamada toda vez que o timer disparar
    .arg = NULL, // Argumento para a função de callback (não utilizado aqui)
    .dispatch_method = ESP_TIMER_TASK, // Método de despacho (tarefa do timer)
    .name = "timer2" // Nome do timer 
  };
  err =  esp_timer_create(&timer_args2, &timer2);
  if (err != ESP_OK) {
    Serial.println("Erro ao criar o timer");
    return;
  }
  err = esp_timer_start_periodic(timer2, t2/2*1000); // 500000 microsegundos = 500 ms
  if (err != ESP_OK) {
    Serial.println("Erro ao iniciar o timer");
  } else {
    Serial.println("Início do Timer com sucesso");
  }

  Serial.println("T3");

  int t3 = (COMPRIMENTO * (1000 / 7.0) * 3.6) / V3; 
  const esp_timer_create_args_t timer_args3 = {
    .callback = &alterna3,  // Função de callback chamada toda vez que o timer disparar
    .arg = NULL, // Argumento para a função de callback (não utilizado aqui)
    .dispatch_method = ESP_TIMER_TASK, // Método de despacho (tarefa do timer)
    .name = "timer3" // Nome do timer 
  };
  err =  esp_timer_create(&timer_args3, &timer3);
  if (err != ESP_OK) {
    Serial.println("Erro ao criar o timer");
    return;
  }
  err = esp_timer_start_periodic(timer3, t3/2*1000); // 500000 microsegundos = 500 ms
  if (err != ESP_OK) {
    Serial.println("Erro ao iniciar o timer");
  } else {
    Serial.println("Início do Timer com sucesso");
  }
 
  Serial.println("T4");
  
  //para RPM
  pinMode(pinRPM, OUTPUT);
  int rpm = 3000;
  int periodoRPM = 60000/rpm;
  const esp_timer_create_args_t timer_args4 = {
    .callback = &alternaRPM,  // Função de callback chamada toda vez que o timer disparar
    .arg = NULL, // Argumento para a função de callback (não utilizado aqui)
    .dispatch_method = ESP_TIMER_TASK, // Método de despacho (tarefa do timer)
    .name = "timerRPM" // Nome do timer 
  };
  err =  esp_timer_create(&timer_args4, &timerRPM);
  if (err != ESP_OK) {
    Serial.println("Erro ao criar o timer");
    return;
  }
  err = esp_timer_start_periodic(timerRPM, periodoRPM/2*1000); // 500000 microsegundos = 500 ms
  if (err != ESP_OK) {
    Serial.println("Erro ao iniciar o timer");
  } else {
    Serial.println("Início do Timer com sucesso");
  } 

 
}

void loop() {
    //pressao freio -> afere acada 5s
    pinMode(pinFreio, OUTPUT);
    //timerFreio = tempo em ms da ultima execucao
    //varacao de temp > 5s
    if (millis()- timerFreio > 5000){
        digitalWrite(pinFreio, random(0, 1));
        timerFreio = millis();
    }

    ondaTriangular();
    delay (50);

  //Para o I2C, solicitacao da leitura de dados. ECU solicita para I2C.
  //Se o endereço da solicitacao condizer com o endereco do meu sensor -> simular o sinal e enviar para ECU.
 /*if(input)
        simulaTemp


  IF (input)
      simulaAceleracao   */


}
