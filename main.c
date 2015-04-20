#include <proc/p32mx320f128h.h>

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

int main(void) {
	unsigned int speed;
	unsigned int sweep = 0x1;
	
	/* PORTB.2 is analog pin with potentiometer*/
	AD1PCFG = ~(1 << 2);
	TRISBSET = (1 << 2);
	/* Use pin 2 for positive */
	AD1CHSbits.CH0SA = 0x2;
	/* Set negative reference*/
	AD1CHSbits.CH0NA = 0x0;
	
	/* Data format in uint32, 0 - 1024 */
	AD1CON1bits.FORM = 0x4;
	/* Manual sampling, auto conversion when sampling is done */
	AD1CON1bits.SSRC = 0x7;
	AD1CON1bits.CLRASAM = 0x0;
	AD1CON1bits.ASAM = 0x0;
	AD1CON2 = 0x0;
	AD1CON3bits.ADRC = 0x1;
	
	/* Set up output pins */
	ODCE = 0x0;
	TRISECLR = 0xFF;
	
	/* Turn on ADC */
	AD1CON1bits.ON = 0x1;
	
	for(;;) {
		/* Start sampling, wait until conversion is done */
		AD1CON1bits.SAMP = 0x1;
		while(!AD1CON1bits.SAMP);
		while(!AD1CON1bits.DONE);
		
		/* Get the value */
		speed = ADC1BUF0;
		
		PORTE = sweep;
		if((sweep <<= 1) >= 0x100)
			sweep = 1;
		delay(speed * 1000);
	}
	
	return 0;
}

