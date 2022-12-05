#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include <inttypes.h>

#define F_CPU 8000000UL
#include <util/delay.h>

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

void setup() {
  DDRC = _BV(PINC7);
  TCCR0B = _BV(CS02) | _BV(CS00);
  TIMSK0 = _BV(TOIE0);
  ADMUX = _BV(REFS1) | _BV(REFS0);
}

int main() {
  setup();
  for(;;)
    sleep_mode();
}
