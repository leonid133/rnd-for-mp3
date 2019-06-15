#define F_CPU 1200000UL
#include <tiny13a.h>

#include <delay.h>

#define NEXT PORTB.0            // Next track button
//#define RANDOM_ON PORTB.2	// on/off random
#define MIC 2			// Microphone ADC INPUT
#define LOUD_THRESHHOLD 200     // silence between two tracks

#define ADC_VREF_TYPE 0x00

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
	ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
	// Delay needed for the stabilization of the ADC input voltage
	delay_us(10);
	// Start the AD conversion
	ADCSRA|=0x40;
	// Wait for the AD conversion to complete
	while ((ADCSRA & 0x10)==0);
	ADCSRA|=0x10;
	return ADCW;
}

// Declare your global variables here

void skip_random_number_of_tracks(void)
{
	unsigned char num = 0;
	for(i=0; i<20; i++)
	{
		num = num + read_adc(MIC);
	}
	
	for(i=0; i<num; i++)
	{
		NEXT = 0;
		delay_ms(5);
		NEXT = 1;
		delay_ms(5);
	}
}

int main(void)
{
// Declare your local variables here
	int i=0
	unsigned int flagreset=0;
	float music_detector=0;
	if(MCUSR==0x02)
	{
	    flagreset=1;
	}
	MCUSR = 0x00;
// Crystal Oscillator division factor: 8
	#pragma optsize-
	CLKPR=0x80;
	CLKPR=0x03;
	#ifdef _OPTIMIZE_SIZE_
	#pragma optsize+
	#endif

// Input/Output Ports initialization
// Port B initialization
// Func5=In Func4=In Func3=In Func2=In Func1=Out Func0=Out 
// State5=T State4=T State3=T State2=T State1=0 State0=0 
	PORTB=0x00;
	DDRB=0x03;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=FFh
// OC0A output: Disconnected
// OC0B output: Disconnected
	TCCR0A=0x00;
	TCCR0B=0x00;
	TCNT0=0x00;
	OCR0A=0x00;
	OCR0B=0x00;

// External Interrupt(s) initialization
// INT0: Off
// Interrupt on any change on pins PCINT0-5: Off
	GIMSK=0x00;
	MCUCR=0x00;

// Timer/Counter 0 Interrupt(s) initialization
	TIMSK0=0x04;

// Analog Comparator initialization
// Analog Comparator: Off
	ACSR=0x80;
	ADCSRB=0x00;

// ADC initialization
// ADC Clock frequency: 75,000 kHz
// ADC Bandgap Voltage Reference: Off
// ADC Auto Trigger Source: Free Running
// Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
	DIDR0&=0x03;
	DIDR0|=0x00;
	ADMUX=ADC_VREF_TYPE & 0xff;
	ADCSRA=0xA1;
	ADCSRB&=0xF8;

// Global enable interrupts
	#asm("sei")
    NEXT = 1;
    delay_ms(1000);
	
    while (1)
    {   
         if(flagreset == 1)
         {     
	    //rebooted after reset button
            for(i=0; i<5; i++)
            {
                delay_ms(1000);   
            }        
            flagreset=0;
         }
         
	 music_detector = 0;
	 for(i=0; i<200; i++)
	 {
		
		music_detector = music_detector + (read_adc(MIC) - music_detector)*0.005;
		delay_ms(10);
	 }
	    
         if(music_detector < LOUD_THRESHHOLD) // && RANDOM_ON)
	 {
		skip_random_number_of_tracks();
		for(i=0; i<50; i++)
		{
			delay_ms(1000);   
		} 
	 }  
    };
}
