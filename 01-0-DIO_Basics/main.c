#include <avr/io.h>
#include <util/delay.h>

int getBit(int num, int position);
int setBit(int num, int position);
int clearBit(int num, int position);

int main(void)
{
	int i;
	DDRA = 0b00001111;
	PORTA = 0b00000000;
	while(1) //Super loop
	{
		for(i = 0; i < 7; ++i){
			PORTA = setBit(PORTA,i>3 ? 6-i:i);
			_delay_ms(250);
			PORTA = clearBit(PORTA,i>3 ? 6-i:i);
		}
	}
}

// Function to get the value of a specific bit at a given position
int getBit(int num, int position) {
    return (num >> position) & 1;
}

// Function to set a specific bit at a given position to 1
int setBit(int num, int position) {
    return num | (1 << position);
}

// Function to clear a specific bit at a given position to 0
int clearBit(int num, int position) {
    return num & ~(1 << position);
}
