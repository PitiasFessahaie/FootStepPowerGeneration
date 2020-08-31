//Creater: Pitias Fessahaie
//Email :  selamptsegu@gmail.com
//Sep 2008

#include <htc.h>

#define data PORTC
#define RS RB6
#define EN RB7
#define D0 RC0
#define D1 RC1
#define D2 RC2
#define D3 RC3
#define D4 RC4
#define D5 RC5
#define D6 RC6
#define D7 RC7
#include "lcd.h"

#define PORTBIT(adr, bit)	((unsigned)(&adr)*8+(bit))

static bit SW1  @ PORTBIT(PORTB, 0);
static bit CHG  @ PORTBIT(PORTB, 1);

float valch0,valch1;

void delay_ms(int n )
{
    TMR1H=0xEC; 
 TMR1L=0x77;
 T1CKPS1=0;
 T1CKPS0=0;
 TMR1CS=0;
 TMR1IF=0;
 TMR1ON=1;
    while(n>0)
    {
  while(!TMR1IF);
  TMR1IF=0;
  TMR1H=0xEC; 
  TMR1L=0x77;
  n--;
 }
}

float select_adc(unsigned char chnl){    // init_adc(channel name);
    CHS2=CHS1=0;
    CHS0=chnl;
    delay_ms(10);
    ADON=1;
    ADCON0=0x45;
    while((ADCON0 & 4)!= 0);
    return (((ADRESH*256+ADRESL)*5.0/1023)*10);
}




void LCD_Write(unsigned char values,int rs){
 data = values;
 RS = rs;    // rs=0 command and rs=1 data
 EN = 1;
 delay_ms(1);
 EN = 0;
}

void LCD_clear(void){
 LCD_Write(0x01,0);  //this clears LCD
}

void LCD_goto(unsigned char row,unsigned char column){
 if(row==1){
  LCD_Write(0x80+column,0);
 }
 else if(row==2){
  LCD_Write(0xC0+column,0);
 }
}

void LCD_num(int n){

  LCD_Write((n/100)+48,1);
  LCD_Write(((n%100)/10)+48,1);
  LCD_Write((n%10)+48,1);
}
void delay(unsigned int x)
	    {unsigned char a;
	       for(a=0;a<x;a++)
	       {__delay_ms(1000);}};


void initLCD(void){ unsigned i;

 TRISC=0X00;  ///out put port C
 TRISD=0x00;//as output
 TRISB = 0x01;
 delay_ms(100);


 LCD_Write(0x30, 0); //8 - bit display
 EN=1; EN = 0;
 LCD_Write(0x38,0);    //2 lines mode



 LCD_Write(0x0C,0);    //dispaly on , cursor blinking
 delay_ms(1);

 LCD_clear();
 LCD_goto(1,0);
 delay_ms(1);
 LCD_goto(1,2);
   Lcd8_Write_String("FOOTSTEP POWER GENERATION");
 for(i=0;i<15;i++)
 {__delay_ms(500);
  Lcd8_Shift_Left(); 
    } for(i=0;i<15;i++)
 {__delay_ms(500);
  Lcd8_Shift_Right(); 
    } 
 LCD_goto(2,11);
 LCD_Write('v',1);delay_ms(100);
 LCD_goto(2,7);
 LCD_Write('.',1);delay_ms(100);

}

void main(void){
 PCFG3=PCFG2=PCFG1=PCFG0=0;
 ADFM=1;
 ADCS1=1;
 ADCS0=0;
 TRISA=0xFF;
 initLCD();

    while(1){

        valch0=select_adc(0);
       
       LCD_goto(2,1);
       LCD_Write('+',1);
       __delay_ms(5);
        LCD_goto(2,4);
        LCD_num(valch0);
        LCD_goto(2,8);
        LCD_num((valch0-(int)valch0)*1000);
       
       if(SW1==1){
	  LCD_goto(2,13);
 LCD_Write('C',1);delay_ms(100);
	  LCD_goto(2,14);
 LCD_Write('H',1);delay_ms(100);
	  LCD_goto(2,15);
 LCD_Write('G',1);delay_ms(100);
	  if(SW1=!0)
	  {CHG=1;
	  _delay(100);}}
	  
	 
	  if(SW1==0)
	  { LCD_goto(2,13);
 LCD_Write('N',1);delay_ms(100);
	  LCD_goto(2,14);
 LCD_Write('C',1);delay_ms(100);
	  LCD_goto(2,15);
 LCD_Write('H',1);delay_ms(100);
	     CHG=0; _delay(100);}

    }
}