#include <avr/io.h>
#include <util/delay.h>

//
// macros
//

// set pin to output
#define output(directions,pin) (directions |= pin)
// set pin to input
#define input(directions,pin) (directions &= (~pin))
#define set(port,pin) (port |= pin)
#define clear(port,pin) (port &= (~pin)) // same as input??
#define pin_test(byte,bit) (byte & (1 << bit)) // see the bit?

// delays
#define PWM_delay() _delay_us(25) // 25 microsecond = 40kHz
#define bit_delay_time 104 // bit delay for 9600
#define bit_delay() _delay_us(bit_delay_time) // 9615.3846 Hz
#define half_bit_delay() _delay_us(bit_delay_time/2)
#define char_delay() _delay_ms(10) // char delay

// ports and direction register
#define led_port PORTA
#define led_direction DDRA
#define serial_port PORTB
#define serial_direction DDRB
#define serial_pins PINB

//
// pin distribution

// LED
#define led1 (1 << PA3) //pin 10
#define led2 (1 << PA2) //pin 11
#define led3 (1 << PA1) //pin 12
#define led4 (1 << PA0) //pin 13

// serial port
#define serial_pin_out (1<<PB2) // Rx
#define serial_pin_in (1<<PB1) // Tx


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

void initialize_leds(){
  set(led_direction,led1);
  set(led_direction,led2);
  set(led_direction,led3);
  set(led_direction,led4);
  output(led_port,led1);
  output(led_port,led2);
  output(led_port,led3);
  output(led_port,led4);
}

void clear_all_leds(){
  clear(led_port,led1);
  clear(led_port,led2);
  clear(led_port,led3);
  clear(led_port,led4);
}

void turn_on_leds(){
  set(led_port,led1);
  set(led_port,led2);
  set(led_port,led3);
  set(led_port,led4);
}

int main(void){

  static char chr;

  // clock
  CLKPR = (1 << CLKPCE);
  CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

  // set led pins to output
    // set serial in
  set(serial_direction,serial_pin_in);
  input(serial_port,serial_pin_in);

  initialize_leds();


  while(1){
    // set it?
    get_char(&serial_pins,serial_pin_in,&chr);

    if(chr == 0){
    turn_on_leds(); 
    }else{
      clear_all_leds();
    }


  }

  return 1;
}
