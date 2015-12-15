#include <avr/io.h>
#include <util/delay.h>


// set the pins
#define output(directions,pin) (directions |= pin)
#define input(directions,pin) (directions &= (~pin))

// set to highs and lows for output pins
#define high(port,pin) (port |= pin)
#define low(port,pin) (port &= (~pin))

// delays
#define bit_delay_time 100 //
#define bit_delay() _delay_us(bit_delay_time)
#define half_bit_delay() _delay_us(bit_delay_time/2)


// ports
#define motor_port PORTB
#define motor_direction DDRB

// pins
// motors
#define motor1 (1<<PB2)
#define motor2 (1<<PB1)


int main(void){
  // setting the clock
  CLKPR = (1 << CLKPCE);
	CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

  // set motor pins
  low(motor_port,motor1);
  output(motor_direction,motor1);
  low(motor_port,motor2);
  output(motor_direction,motor2);


  low(motor_port,motor1);
  low(motor_port,motor2);

}
