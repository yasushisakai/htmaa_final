#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


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

#define PWM_on_delay() _delay_us(5500) //
#define PWM_off_delay_slow() _delay_us(2500) // slow
#define PWM_off_delay_fast() _delay_ms(1) // fast
#define PWM_unit_delay() _delay_us(10)

#define PWM_cycle 50  // 8 milliseconds * 50cycles = 400milliseconds
#define cycle_count 8 //??

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

void put_string(volatile unsigned char *port,unsigned char pin, char *str){
  put_char(&serial_port,led_serial_out,0);
  int i=0;
  do{
    put_char(port,pin,str[i]);
  }while(str[++i]!=0);
}

int main(void){
  // setting the clock
  CLKPR = (1 << CLKPCE);
	CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

  char lo,hi;
  char b1,b2,b3,b4;
  unsigned int mic_value;
  char str_value[10];
  b4 = 0;

  //set motor pins
  low(motor_port,motor1);
  output(motor_direction,motor1);
  low(motor_port,motor2);
  output(motor_direction,motor2);

  // set serial pins
  high(serial_port,led_serial_out);
  output(serial_direction,led_serial_out);
  high(serial_port,mic_serial_in);
  input(serial_direction,mic_serial_in);

  while(1){

      
      // wait for frame
      while(1){
      b1 = b2;
      b2 = b3;
      b3 = b4;

      get_char(&serial_pins,mic_serial_in,&b4);

      if(b1==1 && b2==2 && b3==3 && b4==4){
        //put_char(&serial_port,led_serial_out,'B');
        break;
      }
    }
      get_char(&serial_pins,mic_serial_in,&lo);
      get_char(&serial_pins,mic_serial_in,&hi);

      // for debugging
      mic_value = hi * 256 + lo;
      //sprintf(str_value,"%d",mic_value);
      //put_string(&serial_port,led_serial_out,str_value);

      if(mic_value < 10){
        put_char(&serial_port,led_serial_out,'0');
        high(motor_port,motor1);
        high(motor_port,motor2);
      }else if (mic_value < 50){
        put_char(&serial_port,led_serial_out,'2');
        low(motor_port,motor1);
        low(motor_port,motor2);
      }else if (mic_value < 200){
        put_char(&serial_port,led_serial_out,'3');
        low(motor_port,motor1);
        low(motor_port,motor2);
      }else{
        put_char(&serial_port,led_serial_out,'1');
        low(motor_port,motor1);
        low(motor_port,motor2);
      }


        _delay_ms(50);
      //put_char(&serial_port,led_serial_out,lo);
      //put_char(&serial_port,led_serial_out,hi);
  }
}
