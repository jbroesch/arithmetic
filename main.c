/**********************************************************************
*
* © James D. Broesch, 2011, 2021
* 
* Simple arithmetic demo.
* The tone stops when switch 1 is pressed.
* A timer interrupt provides background functions.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33FJ256GP506
* Compiler:        MPLAB® C30 v3.00 or higher
*
************************************************************************/

#include <xc.h>

#include <math.h>

#include "WM8510CODEC.h"
#include "timer.h"
#include "dscInit.h"
#include "dma.h"

int main(void)
{	
	signed short int x, y, z;
	signed long int a;

	// Intialize the board and the drivers
	DSCInit();

	// Initalize the CODEC.
	WM8510Init();
	// Start Audio input and output function
	WM8510Start();
		
	// Configure the CODEC for 8K operation	
	WM8510SampleRate8KConfig();

	// Use timer 1 as a real time clock.
	// 1 tick = 15,625 * 256 clock cycles = 100ms.
	timer1Init(15625);

	// Intialize and start the DMA channels.
	// Channel 0 moves data from the transmit buffers to the DCI.
	// Channel 1 moves data from the DCI to the recieve buffers.
	dmaInit();	
	
	// Integer arithmetic as we know it.

	x = 2; // Set your break point here.
	y = 3;
	z = x * y;

	x = -2;
	y = 3;
	z = x * y;

	x = -2;
	y = -3;
	z = x * y;

	x = 7000;
	y = -9000;
	z = x * y; // Bad result! We overflowed.

	// Try a long signed integer:
	a = x * y; // That did not work either.
	a *= (-1); // This makes it easier to interpret the results.
	// So, we use a cast on the variables:
	a = (long signed int)x * (long signed int)y; // That works!
	a *= (-1); // This makes it easier to interpret the results.
	// Message: rember to cast appropriately.


	// Fractional aritmetic examples.
	x = (0.25) * 0x7FFF;
	y = (0.50) * 0x7FFF;
	//(16 bits) * (16 bits) -> 32 bit result.
	a = (long signed int)x * (long signed int)y;
	a >>= 16; // Normalize the result to 16 bits.
	// The result should be:
	z = (0.25) * (0.50) * 0x7FFF;
	// They don't match because of the two sign bits!
	// Instead we try:
	a = ((long signed int)x * (long signed int)y) << 1;
	a >>= 16; // Normalize the result to 16 bits.
	// Shifting left gets rid of the redundant sign bit.
	// The result now matches what we would expect.
	// This the major computational difference between
	// 		fractional and integer arithmetic.

	// This also works for negative numbers:
	x = (0.25) * 0x7FFF;
	y = (-0.50) * 0x7FFF;
	//(16 bits) * (16 bits) -> 32 bit result.
	a = ((long signed int)x * (long signed int)y) << 1;
	a >>= 16; // Normalize the result to 16 bits.
 	// We can check the result:
	z = (0.25) * (-0.50) * 0x7FFF;
	// Note the sign extension in the result. 
	// Also, we get a slight rounding error: 0xF001 vs 0xF000.

	// End of the examples.

	// Main processing loop.
	// Quit if switch 1 is pressed.
	while(SWITCH_S1)
		{
		} 

	// Disable the DMA Channels.
	DMA0CONbits.CHEN = 0;
	DMA1CONbits.CHEN = 0;

	// Stop the CODEC interface.
	WM8510Stop();

return(0); // Go home.
}
