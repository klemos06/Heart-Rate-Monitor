#include <EFM8LB1.h>
#include <stdio.h>

#define SYSCLK      72000000L  // SYSCLK frequency in Hz
#define BAUDRATE      115200L  // Baud rate of UART in bps
#define BUZZER          P2_2   // Pin for buzzer 
#define LCD_RS P1_7
#define LCD_E  P2_0
#define LCD_D4 P1_3
#define LCD_D5 P1_2
#define LCD_D6 P1_1
#define LCD_D7 P1_0
#define CHARS_PER_LINE 16

xdata float period_buffer[16]={0};

int buffcount = 0; 

unsigned char overflow_count;


char _c51_external_startup (void)
{
	// Disable Watchdog with key sequence
    SFRPAGE = 0x00;
    WDTCN = 0xDE; 
    WDTCN = 0xAD; 
    VDM0CN |= 0x80;
    RSTSRC = 0x02;

    SFRPAGE = 0x10;
    PFE0CN  = 0x20; // SYSCLK < 75 MHz
    SFRPAGE = 0x00;

    CLKSEL = 0x00;
    while ((CLKSEL & 0x80) == 0);
    CLKSEL = 0x03;
    while ((CLKSEL & 0x80) == 0);
    
    P0MDOUT |= 0x10; 
    XBR0     = 0x01; 
    XBR2     = 0x40; 

    SCON0 = 0x10;
    CKCON0 |= 0b_0000_0000; 
    TH1 = 0x100-((SYSCLK/BAUDRATE)/(2L*12L));
    TL1 = TH1;      
    TMOD &= ~0xf0;  
    TMOD |=  0x20;                       
    TR1 = 1; 
    TI = 1;  
    return 0;
}

void Timer3us(unsigned char us)
{
    unsigned char i;
    CKCON0|=0b_0100_0000;
    TMR3RL = (-(SYSCLK)/1000000L);
    TMR3 = TMR3RL;
    TMR3CN0 = 0x04;
    for (i = 0; i < us; i++)
    {
        while (!(TMR3CN0 & 0x80));
        TMR3CN0 &= ~(0x80);
    }
    TMR3CN0 = 0;
}

void waitms (unsigned int ms)
{
    unsigned int j;
    for(j=ms; j!=0; j--) { Timer3us(249); Timer3us(249); Timer3us(249); Timer3us(250); }
}

void TIMER0_Init(void)
{
    TMOD&=0b_1111_0000; 
    TMOD|=0b_0000_0001; 
    TR0=0; 
}


void LCD_pulse (void)
{ 
	LCD_E=1; 
	Timer3us(40); 
	LCD_E=0; 
}

void LCD_byte (unsigned char x)
{
    ACC=x;
    LCD_D7=ACC_7; 
    LCD_D6=ACC_6; 
    LCD_D5=ACC_5; 
    LCD_D4=ACC_4;
    LCD_pulse();
    Timer3us(40);
    ACC=x;
    LCD_D7=ACC_3; 
    LCD_D6=ACC_2; 
    LCD_D5=ACC_1; 
    LCD_D4=ACC_0;
    LCD_pulse();
}

void WriteData (unsigned char x) 
{ 
	LCD_RS=1; 
	LCD_byte(x); 
	waitms(2); 
}

void WriteCommand (unsigned char x) 
{
	LCD_RS=0; 
	LCD_byte(x); 
	waitms(5); 
}

void LCD_4BIT (void)
{
    LCD_E=0; // Resting state of LCD's enable is zero 
    // LCD_RW=0; // We are only writing to the LCD in this program
    waitms(20);
    // First make sure the LCD is in 8-bit mode and change to 4-bit mode
    WriteCommand(0x33);
    WriteCommand(0x33);
    WriteCommand(0x32);
    
    // Configure the LCD
    WriteCommand(0x28);
    WriteCommand(0x0c);
    WriteCommand(0x01); // Clear screen command (takes some time)
    waitms(20); // Wait for clear screen command to finish
}

void LCDprint(char * string, unsigned char line, bit clear)
{
    int j;
    
    WriteCommand(line==2?0xc0:0x80);
    waitms(5);
    for(j=0; string[j]!=0; j++) WriteData(string[j]); // Write the message
    if(clear) for(; j<CHARS_PER_LINE; j++) WriteData(' '); // Clear the rest of the line
}

int getsn (char * buff, int len)
{
	int j;
	char c;
	
	for(j=0; j<(len-1); j++)
	{
		c=getchar();
		if ( (c=='\n') || (c=='\r') )
		{
			buff[j]=0;
			return j;
		}
		else
		{
			buff[j]=c;
		}
	}
	buff[j]=0;
	return len;
}

float decimator (float xyz)
{
    if(buffcount == 16){
    	return -1;
    }
    
    if(xyz > 150 || xyz < 40){
    	return 0;
    } 
    
    
    period_buffer[buffcount]= xyz;
    buffcount++;
    
    return 0;
}

// check if the buffer has space - return -1 if full 

// adds the reading to 16 space array 



// main


void main (void) 
{
    xdata char buff1[17];
    xdata char buff2[17];
    float period;
    float bpm;
    float sum; 
    float avg;
    int i;
    int ret;


	BUZZER = 0;
	
    TIMER0_Init();
    LCD_4BIT();
    

    waitms(500); 
    while (1)
    {
        TL0=0; 
        TH0=0; 
        TF0=0; 
        overflow_count=0;
        
        // waiting for pulse start to line it
        while(P0_1!=0); 
        printf("HB:0\r\n"); // makes sure that the pulse starts at the bottom --> waits till the while loop where p0_1 =1 is over 
        while(P0_1!=1); // Wait for signal to be one
        TR0=1;             
        BUZZER = 1;
		printf("HB:1\r\n"); // set gui high --> now the hb has started
        while(P0_1!=0)
        {
            if(TF0==1) 
            { 
            	TF0=0;
            	overflow_count++; 
            }
        }
        
        BUZZER = 0;
        printf("HB:0\r\n"); // gui to low --> hb ended

        while(P0_1!=1)
        {
            if(TF0==1) 
            { 
            	TF0=0; 
            	overflow_count++; 
            }
        }
        
        TR0=0; // Stop timer 
        
        period = (overflow_count*65536.0+TH0*256.0+TL0)*(12.0/SYSCLK);  
        
        
        printf("Period:%f\r\n", period); // send the period to gui --> giving raw period to gui to conserve accuracy 
    
        bpm = (60.0 / period);
        
        sprintf(buff1, "Inst BPM: %.1f", bpm);
        
        LCDprint(buff1, 1, 1);
        
        ret = decimator(bpm);
        
        if (ret == -1){ 
        	sum = 0.0;
        	avg = 0.0;
           for(i = 0; i < 16; i++){ 
             	sum += period_buffer[i]; 
                }
            avg = sum / 16.0;   
            sprintf(buff2, "Avg BPM: %.1f", avg);           
            LCDprint(buff2, 2, 1);         
            buffcount = 0;
        }
        
        //buff1 = instantaneous
        //buff2 = avg
    }
}