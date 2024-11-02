#include <Arduino.h>

// Máscaras organizadas para representar los números del 0 al 9
// Cada número tiene dos máscaras: una para el Puerto D y otra para el Puerto B
/// DEL 2 AL 5 - BIT2- segmento D /BIT 3 - segmento C/ BIT 4 - segmento B / BIT 5- segmento A
const uint8_t puertoD_masks[] = {
  0b00111100, // 0 - segmentos A, B, C, D encendidos en D2-D5
  0b00011000, // 1 - segmentos B, C
  0b00101100, // 2 - segmentos A, B, D, G
  0b00111100, // 3 - segmentos A, B, C, D, G
  0b00011000, // 4 - segmentos B, C, F, G
  0b00110100, // 5 - segmentos A, C, D, F, G
  0b00110100, // 6 - segmentos A, C, D, E, F, G
  0b00011100, // 7 - segmentos A, B, C
  0b00111110, // 8 - segmentos A, B, C, D, E, F, G
  0b00111100  // 9 - segmentos A, B, C, D, F, G
};
/// DEL 4 AL 7 - BIT4 pinto decimal (gp) / BIT 5 - segmento F / BIT 6 - segmento G / BIT 7- segmento E
const uint8_t puertoB_masks[] = {
  0b00000101, // 0 - segmentos E, F
  0b00000000, // 1 - no hay segmentos en Puerto B
  0b00000011, // 2 - segmentos E, G
  0b00000010, // 3 - segmento G
  0b00000110, // 4 - segmentos F, G
  0b00000110, // 5 - segmentos F, G
  0b00000111, // 6 - segmentos E, F, G
  0b00000100, // 7 -segmento F
  0b00000111, // 8 - segmentos E, F, G
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
