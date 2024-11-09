#include <Arduino.h>


// conecciones a los pines del arduino nano:

// D2 -A
// D3 -B
// D4 -C
// D5 -D

// D8 -E
// D9 -F
// D10 -G 
// D11 -DP

// Máscaras organizadas para representar los números del 0 al 9
// Cada número tiene dos máscaras: una para el Puerto D y otra para el Puerto B
// Del 2 al 5 - BIT5 - segmento D  / BIT 4 - segmento C / BIT 3 - segmento B  / BIT 2  segmento A
const uint8_t puertoD_masks[] = {
  0b00111100, // 0 - segmentos A, B, C, D encendidos en D2-D5
  0b00011000, // 1 - segmentos B, C
  0b00101100, // 2 - segmentos A, B, D, G
  0b00111100, // 3 - segmentos A, B, C, D, G
  0b00011000, // 4 - segmentos B, C, F, G
  0b00110100, // 5 - segmentos A, C, D, F, G
  0b00110100, // 6 - segmentos A, C, D, E, F, G
  0b00011100, // 7 - segmentos A, B, C
  0b00111100, // 8 - segmentos A, B, C, D, E, F, G
  0b00111100  // 9 - segmentos A, B, C, D, F, G
};

// Del 4 al 7 - BIT3 segmento DP / BIT2 segmento G  / BIT1 segmento F  / BIT 0 segmento E
const uint8_t puertoB_masks[] = {
  0b00001011, // 0 - segmentos E, F
  0b00000000, // 1 - no hay segmentos en Puerto B
  0b00000101, // 2 - segmentos E, G
  0b00000100, // 3 - segmento F, G
  0b00000110, // 4 - segmentos F, G
  0b00000110, // 5 - segmentos E, F, G
  0b00000111, // 6 - segmentos E, F, G
  0b00000010, // 7 - segmento F
  0b00000111, // 8 - segmentos E, F, G, DP apagado
  0b00000110  // 9 - segmentos F, G
};

void setup() {
  // Configurar pines D2-D5 como salida en el Puerto D
  asm volatile (
    "ldi r16, 0b00111100 \n"   // Configura D2-D5 como salida en DDRD
    "out %[ddr_d], r16 \n"
    :
    : [ddr_d] "I" (_SFR_IO_ADDR(DDRD))  // Dirección de DDRD para configurar como salida
    : "r16"
  );

  // Configurar pines D8-D11 como salida en el Puerto B
  asm volatile (
    "ldi r16, 0b00001111 \n"   // Configura D8-D11 como salida en DDRB
    "out %[ddr_b], r16 \n"
    :
    : [ddr_b] "I" (_SFR_IO_ADDR(DDRB))  // Dirección de DDRB para configurar como salida
    : "r16"
  );
}

void mostrarNumero(uint8_t numero) {
  // Asegurarse de que el número está en el rango 0-9
  if (numero > 9) return;

  // Máscaras para el número seleccionado
  uint8_t d_mask = puertoD_masks[numero];
  uint8_t b_mask = puertoB_masks[numero];

  // Aplicar las máscaras al Puerto D y Puerto B
  asm volatile (
    // Configurar el valor del Puerto D
    "out %[port_d], %[d_mask] \n"

    // Configurar el valor del Puerto B
    "out %[port_b], %[b_mask] \n"
    :
    : [port_d] "I" (_SFR_IO_ADDR(PORTD)),
      [port_b] "I" (_SFR_IO_ADDR(PORTB)),
      [d_mask] "r" (d_mask),
      [b_mask] "r" (b_mask)
  );
}

void loop() {
  // Cuenta regresiva del 9 al 0
  for (int i = 9; i >= 0; i--) {
    mostrarNumero(i);   // Llama a la función para mostrar cada número
    delay(1000);        // Mantener el número visible por 1 segundo
  }
}
