#include	<pic.h>
#include        <string.h>
#include	<ctype.h>
#include	<math.h>
#include	<stdlib.h>
#include	<stdio.h>

//---------------------------------------------------------------------
// Variable declarations
//---------------------------------------------------------------------

struct {
	unsigned	recv1:1;
	unsigned 	recv2:1;
	unsigned  	n1:1;
	unsigned	n2:1;
	unsigned	n3:1;
	unsigned	n4:1;
	unsigned	:2;
	} stat;

union INTCHAR
{
unsigned int ui;
unsigned char ub[2];
};

bank1 union INTCHAR
capt_temp1,
capt_temp2;

bank1 unsigned int
capt_new1,
capt_new2,
capt_old1,
capt_old2;

// Function Prototypes-------------------------------------------------

interrupt void isr( void);
void Setup(void);

//---------------------------------------------------------------------
// isr()
//---------------------------------------------------------------------

interrupt void isr(void)
{

di ();

if (CCP1IF=1)							
	{
    	stat.recv1 = 0;

	capt_temp1.ub[0] = CCPR1L;
	capt_temp1.ub[1] = CCPR1H;

	capt_new1 = capt_temp1.ui - capt_old1;
	capt_old1 = capt_temp1.ui;

	if ((capt_new1 < 356) && (capt_new1 > 280))	// 312
		{
		if (stat.n1 && stat.n2)
			{
			RB0 = 1;
			RB1 = 1;
			}	

		stat.n1 = 1;
		stat.n2 = 1;

	 	}
	
	if ((capt_new1 < 594) && (capt_new1 > 486)) // 540
		{
		if (!stat.n1 && stat.n2)
			{
			RB0 = 0;
			RB1 = 1;
			}	

		stat.n1 = 0;
		stat.n2 = 1;

	 	}	

     	if ((capt_new1 < 1067) && (capt_new1 > 873))// 970
		{
		if (stat.n1 && !stat.n2)
			{
			RB0 = 1;
			RB1 = 0;
			}	

		stat.n1 = 1;
		stat.n2 = 0;

	 	}

     	if ((capt_new1 < 7442) && (capt_new1 > 6089)) // 6766
		{
		if (!stat.n1 && !stat.n2)
			{
			RB0 = 0;
			RB1 = 0;
			}	

		stat.n1 = 0;
		stat.n2 = 0;
	 	}

	CCP1IF = 0;
 	}

if (CCP2IF=1)							
	{
	stat.recv2 = 0;

	capt_temp2.ub[0] = CCPR2L;							
	capt_temp2.ub[1] = CCPR2H;

	capt_new2 = capt_temp2.ui - capt_old2;
	capt_old2 = capt_temp2.ui;

     if ((capt_new2 < 356) && (capt_new2 > 291))  // 324
		{
		if (stat.n3 && stat.n4)
			{
			RB2 = 1;
			RB3 = 1;
			}	

		stat.n3 = 1;
		stat.n4 = 1;
	 	}

     	if ((capt_new2 < 616) && (capt_new2 > 504)) //  560
		{
		if (!stat.n3 && stat.n4)
			{
			RB2 = 0;
			RB3 = 1;
			}	

		stat.n3 = 0;
		stat.n4 = 1;
	 	}

    	if ((capt_new2 < 1089) && (capt_new2 > 891))// 990
		{
		if (stat.n3 && !stat.n4)
			{
			RB2 = 1;
			RB3 = 0;
			}	

		stat.n3 = 1;
		stat.n4 = 0;
	 	}

     	if ((capt_new2 < 7441) && (capt_new2 > 6088)) // 6765
         {
		if (!stat.n3 && !stat.n4)
			{
			RB2 = 0;
			RB3 = 0;
			}	

		stat.n3 = 0;
		stat.n4 = 0;
	 	}
	CCP2IF = 0;
 	}

ei ();
}

//---------------------------------------------------------------------
//	Setup() initializes program variables and peripheral registers
//---------------------------------------------------------------------

void Setup(void)
{
OPTION = 0x86;			//RBPU disabled, TMR0 int clk, prescaler 128

PIE1 = 0;			//CCP1 INT disabled
PIR1 = 0;			// clear INT flags 

PIE2 = 0;			//CCP2INT disabled
PIR2 = 0;			// clear INT flags

PCON = 0x03;			//POR flags clear

TRISA = 0;					
TRISB = 0;
TRISC = 0xff;			// PORTC all inputs
TRISD = 0;
TRISE = 0;

PORTA = 0;
PORTB = 0;
PORTC = 0;			// Clear PORTC
PORTD = 0;			// Clear PORTD
PORTE = 0;

TMR0 = 0;
T1CON = 0x21;			// enable timer1 1:4 prescaler , int clock
TMR1H = 0;						//
TMR1L = 0;
T2CON = 0x00;			// timer2 off
					
CCP1CON = 0x06;			//capture on every 4th rising edge
CCPR1L = 0x00;
CCPR1H = 0x00;
CCP2CON = 0x06;
CCPR2L = 0x00;
CCPR2H = 0x00;

RCSTA = 0;			//USART disable
ADCON0 = 0;			//ADC disable

TMR2IF = 0;						//
TMR2IE = 0;						

INTCON = 0x00;       		// 
CCP1IE = 1;
CCP2IE = 1;
PEIE = 1;						// Enable interrupts.
GIE = 1;							//
}

//---------------------------------------------------------------------
// main()
//---------------------------------------------------------------------

main()
{
Setup();

while(1)                    						
   {
   CLRWDT ();
    }
}
