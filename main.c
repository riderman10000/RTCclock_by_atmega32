/*

	proj. for RAAC by :- 
	                  Rhimesh 
					  sagar

	all positive connected to the PORT B and each segments ground connected to PORTD 
	used i2c communication with ds3231m and //still the data capture system is not advance
	i.e.
	_____________________________________________________________________________________________
	| address | bit 7 msb | bit 6| bit 5| bit 4| bit 3| bit 2 | bit 1| bit 0 | function | range |
	---------------------------------------------------------------------------------------------
	| 0x00    |  0        | 10seconds          |    seconds                  | seconds  | 00-59 |
	---------------------------------------------------------------------------------------------
   
   this sample is from data sheet i have access all of these and divided 
   bug :- i donot know but at some poits i still see some hex data 
	data transfer rate 100KHZ (standard mode)

*/
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

#define F_SCL 100000L
#define F_CPU 1000000L

//_delay_ms(100) -> 1seconds

int B[8] = {PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7};
int D[8] = {PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7};

////////////////////////////////////////////////////////////
/////////// I2C CODES ///////////////////////////////////////
/////////////////////////////////////////////////////////////
void init_i2c(void)
{
	TWSR = 0x00;
	TWBR = 1;
	TWCR = (1<<TWEN);
}

unsigned char read_i2c(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
	return TWDR;
}
void write_i2c(unsigned char ch)
{
	TWDR = ch;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR&(1<<TWINT)));
}
void start(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR&(1<<TWINT))==0);
}

void stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	_delay_ms(0.1); //10
}
void rtc_write(char dev_addr,char dev_loc,char dev_data)
{
	start();
	write_i2c(dev_addr);
	write_i2c(dev_loc);
	write_i2c(dev_data);
	stop();
	_delay_ms(0.1); //10
}

unsigned char rtc_read(char dev_addr,char dev_loc)
{
	char ch;
	start();
	
	write_i2c(dev_addr);
	write_i2c(dev_loc);
	
	start();
	
	write_i2c(dev_addr|0x01);
	ch= read_i2c();
	stop();
	return ch;
}
/////////////////////////////////////////////////////////////////////////
////////////////////I2C CODES ENDS //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



void led_no(int  x,int  y)  //////////////////FUNCTION FOR CONTROLLING LED
{
PORTB = 0x00;
PORTD = 0xFF;

PORTB |= (1<<B[x]);
PORTD &= ~(1<<D[y]);

_delay_ms(0.1);

PORTB = 0x00;
PORTD = 0xFF;
}

//MADE MY OWN LIB. FOR RECEIVED DATA TO SHOW IN THE 7 SEGMENT DIS.
void print(int p, int n)
{
	#define f 1
///p -> positive
///n -> negative
	char i = 0;
	if(p==0)
	{
		while(i!=f)
		{
			led_no(0,n);
			led_no(1,n);
			led_no(2,n);
			led_no(3,n);
			led_no(4,n);
			led_no(5,n);
			//_delay_ms(0.1);
			i++;
		}
	}
	
	else if(p==1)
	{
		while(i!=f)
		{
			led_no(1,n);
			led_no(2,n);
			//_delay_ms(0.1);
			i++;
		}
	}
	
	else if (p==2)
	{
		while(i!=f)
		{
			led_no(0,n);
			led_no(1,n);
			led_no(3,n);
			led_no(4,n);
			led_no(6,n);
			//_delay_ms(0.1);
			i++;
		}
	}
	
	else if (p==3)
	{
		while(i!=f)
		{
			led_no(0,n);
			led_no(1,n);
			led_no(2,n);
			led_no(3,n);
			led_no(6,n);
			//_delay_ms(0.1);
			i++;
		}
	}
	else if (p==4)
	{
		while(i!=f)
		{
			led_no(1,n);
			led_no(2,n);
			led_no(5,n);
			led_no(6,n);
			//_delay_ms(0.1);
			i++;
		}
	}
	
	else if (p==5)
	{
		while(i!=f)
		{
			led_no(0,n);
			led_no(2,n);
			led_no(3,n);
			led_no(5,n);
			led_no(6,n);
			//_delay_ms(0.1);
			i++;
		}
	}
	else if (p==6)
	{
		while(i!=f)
		{
			led_no(0,n);
			led_no(2,n);
			led_no(3,n);
			led_no(4,n);
			led_no(5,n);
			led_no(6,n);
			//_delay_ms(0.1);
			i++;
		}
	}
	else if (p==7)
	{
		while(i!=f)
		{
			led_no(0,n);
			led_no(1,n);
			led_no(2,n);
			//_delay_ms(0.1);
			i++;
		}
	}
	else if (p==8)
	{
		while(i!=f)
		{
			led_no(0,n);
			led_no(1,n);
			led_no(2,n);
			led_no(3,n);
			led_no(4,n);
			led_no(5,n);
			//_delay_ms(0.1);
			i++;
		}
	}
	else if (p==9)
	{
		while(i!=f)
		{
			led_no(0,n);
			led_no(1,n);
			led_no(2,n);
			led_no(3,n);
			led_no(5,n);
			led_no(6,n);
			//_delay_ms(0.1);
			i++;
		}
	}

}



////FUNCTION TO SEPARATE RECEIVED DATA 
void div_show(int h, int m, int s)
{
	int count=0;
	int h_first 	= h/10;
	int h_last		= h%10;
	int h_comn_1	= 0;
	int h_comn_2	= 1;
	int m_first 	= m/10;
	int m_last		= m%10;
	int m_comn_1	= 2;
	int m_comn_2	= 3;
	int s_first	= s/10;
	int s_last		= s%10;
	int s_comn_1	= 4;
	int s_comn_2	= 5;
	//show in here or use print function
	for(count= 0;count<10;count++)
	{
		print(h_first,h_comn_1);
		print(h_last,h_comn_2);
		print(m_first,m_comn_1);
		print(m_last,m_comn_2);
		print(s_first,s_comn_1);
		print(s_last,s_comn_2);
		
	}
		
}


//AS PER THE NAME SUGGESTS
void get_time(void)
{
	char h,m,s;
	//_delay_ms(1);
	h=rtc_read(0xD0,0x02);
	m=rtc_read(0xD0,0x01);
	s=rtc_read(0xD0,0x00);
	div_show(h,m,s);	
}


///THIS IS THE MAIN CODE

int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	init_i2c();
	PORTA = 0xFF;
	PORTB = 0xFF;
	PORTC = 0xFF;
	PORTD = 0x00;
	_delay_ms(100);
	PORTA = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	while(1)
	{
		get_time();
	}
}