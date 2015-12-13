#include <avr/io.h>
#include <util/delay.h>


// set the pins
#define output(directions,pin) (directions |= pin)
#define input(directions,pin) (directions &= (~pin))

// set to highs and lows for output pins
#define high(port,pin) (port |= pin)
#define low(port,pin) (port &= (~pin))

// look into pins and bytes
#define pin_test(pins,pin) (pins & pin)
#define bit_test(byte,bit) (byte & (1 << bit))

// delays
#define bit_delay_time 103 //
#define bit_delay() _delay_us(bit_delay_time)
#define half_bit_delay() _delay_us(bit_delay_time/2)

// ports
#define serial_port PORTB
#define serial_direction DDRB
#define serial_pins PINB
#define led_port PORTA
#define led_direction DDRA


// pins
#define serial_pin_in (1<<PB1)
#define serial_pin_out (1<<PB2)
#define led0 (1 << PA3)
#define led1 (1 << PA2)
#define led2 (1 << PA1)
#define led3 (1 << PA0)


void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte){

  *rxbyte = 0;
  while (pin_test(*pins,pin))
  ; // wait for start bit

  half_bit_delay();
  bit_delay();

  if pin_test(*pins,pin)
    *rxbyte |= (1 << 0);
  else
    *rxbyte |= (0 << 0);

  bit_delay(); // wait for a bit

  if pin_test(*pins,pin)
    *rxbyte |= (1 << 1);
  else
    *rxbyte |= (0 << 1);

  bit_delay(); // wait for a bit

  if pin_test(*pins,pin)
    *rxbyte |= (1 << 2);
  else
    *rxbyte |= (0 << 2);

  bit_delay(); // wait for a bit

  if pin_test(*pins,pin)
    *rxbyte |= (1 << 3);
  else
    *rxbyte |= (0 << 3);

  bit_delay(); // wait for a bit

  if pin_test(*pins,pin)
    *rxbyte |= (1 << 4);
  else
    *rxbyte |= (0 << 4);

  bit_delay(); // wait for a bit

  if pin_test(*pins,pin)
    *rxbyte |= (1 << 5);
  else
    *rxbyte |= (0 << 5);

  bit_delay(); // wait for a bit

  if pin_test(*pins,pin)
    *rxbyte |= (1 << 6);
  else
    *rxbyte |= (0 << 6);

  bit_delay(); // wait for a bit

  if pin_test(*pins,pin)
    *rxbyte |= (1 << 7);
  else
    *rxbyte |= (0 << 7);

  // wait for stop bit
  bit_delay();
  half_bit_delay();
}

void put_char(volatile unsigned char *port, unsigned char pin, char txchar){

  low(*port,pin);
  bit_delay();

  if (bit_test(txchar,0))
    high(*port,pin);
  else
    low(*port,pin);

  bit_delay();

  if (bit_test(txchar,1))
    high(*port,pin);
  else
    low(*port,pin);

  bit_delay();

  if (bit_test(txchar,2))
    high(*port,pin);
  else
    low(*port,pin);

  bit_delay();
  if (bit_test(txchar,3))
    high(*port,pin);
  else
    low(*port,pin);

  bit_delay();
  if (bit_test(txchar,4))
    high(*port,pin);
  else
    low(*port,pin);

  bit_delay();
  if (bit_test(txchar,5))
    high(*port,pin);
  else
    low(*port,pin);

  bit_delay();
  if (bit_test(txchar,6))
    high(*port,pin);
  else
    low(*port,pin);

  bit_delay();
  if (bit_test(txchar,7))
    high(*port,pin);
  else
    low(*port,pin);

  bit_delay();

  // stop bit
  high(*port,pin);
  bit_delay();

  // char delay??
  bit_delay();

}

void leds_off(){
  low(led_port,led0);
  low(led_port,led1);
  low(led_port,led2);
  low(led_port,led3);
}

void leds_on(){
  high(led_port,led0);
  high(led_port,led1);
  high(led_port,led2);
  high(led_port,led3);
}

int main(void){
  static char chr;

  CLKPR = (1 << CLKPCE);
  CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

  // init output
  high(serial_port,serial_pin_out);
  output(serial_direction,serial_pin_out);

  // initialize leds
  output(led_direction,led0);
  output(led_direction,led1);
  output(led_direction,led2);
  output(led_direction,led3);

  leds_on();

  while(1){
    get_char(&serial_pins,serial_pin_in,&chr);

    switch(chr){
      case '0':
        leds_off();
        break;
      case '1':
        leds_on();
        break;
      case '2':
        leds_off();
        high(led_port,led1);
        high(led_port,led2);
        break;
      case '3':
        leds_off();
        high(led_port,led0);
        high(led_port,led3);
        break;
      default:
        leds_off();
    }

    put_char(&serial_port,serial_pin_out,chr);
  }
}
