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
#define bit_delay_time 102 //
#define bit_delay() _delay_us(bit_delay_time)
#define half_bit_delay() _delay_us(bit_delay_time/2)
#define char_delay() _delay_ms(10)

// ports
#define serial_port PORTB
#define serial_direction DDRB
#define led_port PORTA
#define led_direction DDRA
#define sample_time 10


// pins
#define serial_pin_out (1<<PB2)
//#define sensor_pin_in (1<<PA7)


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

  static unsigned char i,highs[sample_time],lows[sample_time];
  unsigned int readings[sample_time];
  float baseline;
  uint16_t min,max,temp,mid;

  //set clock
  CLKPR = (1 << CLKPCE);
  CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

  //initialize output pins
  high(serial_port,serial_pin_out);
  output(serial_direction,serial_pin_out);

  //initialize analog to digital converter
  ADMUX = (1<<REFS1) | (0<<REFS0) | (0<<MUX5) | (0<<MUX4) | (0<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0);

  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);

  baseline = 0.0f;

  while(1){
    put_char(&serial_port,serial_pin_out,'1');
    char_delay();
    put_char(&serial_port,serial_pin_out,'2');
    char_delay();
    put_char(&serial_port,serial_pin_out,'3');
    char_delay();
    put_char(&serial_port,serial_pin_out,'4');
    char_delay();

    max = 0;
    min = 65535;

    for(i=0;i<sample_time;++i){

      // start conversation
      ADCSRA |= (1<ADSC);
      while(ADCSRA & (1<<ADSC))
        ;
      temp = ADCH*256+ADCL;
      if (temp>max) max = temp;
      if (temp<min) min = temp;

      highs[i] = ADCH;
      lows[i] = ADCL;
      readings[i] = ADCH*256
    }

    mid = (max+min)/2;

    for(i=0;i<sample_time;++i){
      put_char(&serial_port,serial_pin_out,lows[i]+'0');
      put_char(&serial_port,serial_pin_out,highs[i]+'0');
    }

    _delay_ms(1000); //1sec


  }

}
