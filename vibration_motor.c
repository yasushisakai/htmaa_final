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
#define bit_delay_time 100 //
#define bit_delay() _delay_us(bit_delay_time)
#define half_bit_delay() _delay_us(bit_delay_time/2)

#define PWM_on_delay() _delay_us(10) //
#define PWM_off_delay_slow() _delay_us(5) // slow
#define PWM_off_delay_fast() _delay_us(1) // fast

#define PWM_count 5000
#define cycle_count 15 //??

// ports
#define serial_port PORTA
#define serial_direction DDRA
#define serial_pins PINA
#define motor_port PORTB
#define motor_direction DDRB

// pins
// motors
#define motor1 (1<<PB2)
#define motor2 (1<<PB1)

//serial communication
#define mic_serial_in (1<<PA3)
#define mic_serial_out (1<<PA2)
#define led_serial_in (1<<PA1)
#define led_serial_out (1<<PA0)

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

int main(void){

	// set clock
	CLKPR = (1 << CLKPCE);
	CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

	//set motors for output
	low(motor_port,motor1);
	output(motor_direction,motor1);

	// counters
	static uint16_t count;
	static uint8_t cycle;
	static uint16_t frame;

	// while(1){
	// 		for(count=0;count<PWM_count;++count){
	// 			high(motor_port,motor1);
	// 			PWM_on_delay();
	// 			low(motor_port,motor1);
	// 			PWM_off_delay();
	// 		}
	// }

	// put_get char example
	output(serial_direction,led_serial_out);
	static char chr;
	frame = 0;

	// just make it high
	low(motor_port,motor1);

}
