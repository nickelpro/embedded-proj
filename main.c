#include <stdio.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include <inttypes.h>

#define F_CPU 8000000UL
#include <util/delay.h>

#define BAUD 9600
#include <util/setbaud.h>

void uart_init(void) {
  UBRR1H = UBRRH_VALUE;
  UBRR1L = UBRRL_VALUE;

#if USE_2X
  UCSR1A |= _BV(U2X1);
#else
  UCSR1A &= ~(_BV(U2X1));
#endif

  UCSR1C = _BV(UCSZ11) | _BV(UCSZ10); /* 8-bit data */
  UCSR1B = _BV(RXEN1) | _BV(TXEN1);   /* Enable RX and TX */
}

int uart_putchar(char c, FILE* stream) {
  if(c == '\n') {
    uart_putchar('\r', stream);
  }
  loop_until_bit_is_set(UCSR1A, UDRE1);
  UDR1 = c;
  return c;
}

int uart_getchar(FILE* stream) {
  loop_until_bit_is_set(UCSR1A, RXC1);
  return UDR1;
}

ISR(TIMER0_OVF_vect) {
  static uint8_t trigger = 0;
  ++trigger;
  if(!(trigger %= 30))
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIE);
}

ISR(ADC_vect) {
  static uint16_t arr[7];
  static uint8_t idx = 0;
  uint8_t lo = ADCL;
  uint8_t hi = ADCH;
  arr[idx] = hi << 8 | lo;
  ++idx;
  idx %= 7;

  // Figure out if the last 7ish seconds has enough variations to be considered
  // still breathing
  uint8_t breathing = 0;


  if(breathing)
    PORTC = 0;
  else
    PORTC = _BV(PINC7);
}

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void setup() {
  DDRC = _BV(PINC7);
  TCCR0B = _BV(CS02) | _BV(CS00);
  TIMSK0 = _BV(TOIE0);
  ADMUX = _BV(REFS1) | _BV(REFS0);

  stdout = &uart_output;
  stdin = &uart_input;
}

int main() {
  setup();
  for(;;) {
    _delay_ms(1000);
    printf("Hello World\n");
  }
}
