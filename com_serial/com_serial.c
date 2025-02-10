/* Unidade 4 Capítulo 6
 *    
 * RESIDENTE: Jackson da Silva Carneiro
 * 
 *        Introdução às Interfaces de Comunicação Serial com RP2040 ART, SPI e I2C
*/


#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#define I2C_PORT i2c1 
#define I2C_SDA 14 //pinos do displsy SSD
#define I2C_SCL 15
#define endereco 0x3C
#define LED_PIN_G 11
#define LED_PIN_B 12 
#define LED_PIN_R 13
#define BUTTON_A 5
#define BUTTON_B 6
#define MAT_5X5 7

void configura_GPIO(){ //CONFIGURAÇÃO DOS PINOS GPIO

  gpio_init(LED_PIN_R);          // Inicializa o pino do LED VERMELHO
  gpio_set_dir(LED_PIN_R, GPIO_OUT);   // Configura como saída

  gpio_init(LED_PIN_B);          // Inicializa o pino do LED AMARELO
  gpio_set_dir(LED_PIN_B, GPIO_OUT);   // Configura como saída

  gpio_init(LED_PIN_G);        // Inicializa o pino do LED VERDE
  gpio_set_dir(LED_PIN_G, GPIO_OUT);   // Configura como saída

  gpio_init(BUTTON_A);                //Inicializa o pino do Push BUtton A
  gpio_set_dir(BUTTON_A, GPIO_IN);   //Configura como entrada
  gpio_pull_up(BUTTON_A);           //Habilita pull up interno

  gpio_init(BUTTON_B);                //Inicializa o pino do Push BUtton B
  gpio_set_dir(BUTTON_B, GPIO_IN);   //Configura como entrada
  gpio_pull_up(BUTTON_B);           //Habilita pull up interno

 
}

volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;

static void gpio_irq_handler(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A) {
        button_a_pressed = true;  // Define a flag quando o botão A é pressionado
    } else if (gpio == BUTTON_B) {
        button_b_pressed = true;  // Define a flag quando o botão B é pressionado
    }
}
int main()
{
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);
  stdio_init_all();
  configura_GPIO();

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  ssd1306_t ssd; // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  bool cor = true;
  while (true)
  {
    cor = !cor;
    // Atualiza o conteúdo do display com animações
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    if (button_a_pressed) {
      gpio_put(LED_PIN_R, 0); // Liga o LED verde e desliga os LEDs azul e vermelho
      gpio_put(LED_PIN_B, 0);
      gpio_put(LED_PIN_G, 1);
      ssd1306_draw_string(&ssd, "Pressionado PUSH BUTTON A", 20, 30); // Desenha uma string
      ssd1306_send_data(&ssd); // Envia os dados para o display
      button_a_pressed = false;  // Reseta a flag
  }

  if (button_b_pressed) {
      gpio_put(LED_PIN_R, 0); // Liga o LED azul e desliga os LEDs vermelho e verde
      gpio_put(LED_PIN_B, 1);
      gpio_put(LED_PIN_G, 0);
      ssd1306_draw_string(&ssd, "Pressionado PUSH BUTTON B", 20, 30); // Desenha uma string
      ssd1306_send_data(&ssd); // Envia os dados para o display
      button_b_pressed = false;  // Reseta a flag
  }

       
    ssd1306_send_data(&ssd); // Atualiza o display

    sleep_ms(1000);
  }
}