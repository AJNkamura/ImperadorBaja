#include <Arduino.h>
#include <esp_timer.h>

// Define o pino onde o LED está conectado
const int ledPin = 2;

// Handle para o timer (controle remoto par o timer)
esp_timer_handle_t timer; 
/*esp_timer_handle_t é um tipo definido na biblioteca esp_timer.h, 
que representa um ponteiro para uma estrutura interna que o sistema 
usa para gerenciar o timer. Quando você cria um timer, o sistema 
devolve um controle remoto (handle) que você pode usar para controlar 
esse timer específico. */


// Função para alternar o estado do LED
void toggleLED(void* arg) {
  static bool ledState = false;
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
}


void setup() {
  Serial.begin(115200);
  
  // Configura o pino do LED como saída
  pinMode(ledPin, OUTPUT);

  // Configuração do timer
      /*A estrutura esp_timer_create_args_t é usada para definir os parâmetros
     necessários para criar um timer no ESP32. Essa estrutura inclui vários campos 
     que configuram como o timer deve funcionar. */
  const esp_timer_create_args_t timer_args = {
    .callback = &toggleLED,  // Função de callback chamada toda vez que o timer disparar
    .arg = NULL, // Argumento para a função de callback (não utilizado aqui)
    .dispatch_method = ESP_TIMER_TASK, // Método de despacho (tarefa do timer)
    .name = "toggleLED" // Nome do timer 
  };


//verificacao de erro
   // Cria o timer e armazena o handle na variável 'timer'
  esp_err_t err = esp_timer_create(&timer_args, &timer); //Função esp_timer_create - usada para criar um novo timer com os parâmetros especificados.
  if (err != ESP_OK) {
    Serial.println("Erro ao criar o timer");
    return;
  }


  // Inicia o timer com intervalo de 500ms (2Hz)
  err = esp_timer_start_periodic(timer, 5000000); // 500000 microsegundos = 500 ms
  if (err != ESP_OK) {
    Serial.println("Erro ao iniciar o timer");
  } else {
    Serial.println("Início do Timer com sucesso");
  }
}

void loop() {
  // Nada a fazer aqui, o LED é controlado pela interrupção do timer
}


