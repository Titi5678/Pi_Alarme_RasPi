#include "gpio.h"
#include "tone.h"
#include"sendrichtig.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define redLED 22 
#define greenLED 9   
#define PIR 26
#define GPIO_RS 17
# define GPIO_CSB 27
# define GPIO_MOSI 10
# define GPIO_CLK 11
# define RESET 4
# define led 22
# define TASTER 5
# define BUZZER 2


/* Symbolische Namen fuer die Datenrichtung und die Daten  */
#define IN  0
#define OUT 1
 
#define LOW  0
#define HIGH 1
 
/* Datenpuffer fuer die GPIO-Funktionen */
#define MAXBUFFER 100


int main(int argc , char *argv[])
  {
	while(1)
  {
	gpio_export (RESET);
	gpio_direction(RESET, OUT);
	gpio_write(RESET,1);
	delay(100);
	gpio_write(RESET,0);
	delay(100);
	gpio_write(RESET,1);
	delay(100);
	LCD_init();
	LCD_cursor(0,0);
	lcd_clear();

   // copy
gpio_unexport(RESET);

	gpio_export (RESET);
	gpio_direction(RESET, OUT);
	gpio_write(RESET,1);
	delay(100);
	gpio_write(RESET,0);
	delay(100);
	gpio_write(RESET,1);
	delay(100);
	LCD_init();
	LCD_cursor(0,0);
	lcd_clear();

   //
    LCD_cursor(0,0);
    LCD_string("Herzlich");
    LCD_cursor(1,0);
    LCD_string("wilkommen");

  
    gpio_export(redLED) ;
    gpio_direction(redLED,1);
    gpio_export(greenLED) ;
    gpio_direction(greenLED,1);
    gpio_export( PIR) ;
    gpio_direction( PIR,0);
	
	
     gpio_export(TASTER) ;
    gpio_direction(TASTER,0);
    delay(4000) ; 


    gpio_write(redLED, 1);
    gpio_write(greenLED, 1);
    delay(500);
    gpio_write(greenLED, 0);
    gpio_write(redLED, 0);
    delay(500);
    gpio_write(redLED, 1);
    gpio_write(greenLED, 1);
    delay(500);
    gpio_write(greenLED, 0);
    gpio_write(redLED, 0);

	int Pep =0 ; 
	int maxPep = 5;
	int Timer [1000];
	int maxTime = 22; // 20 sec 
	int restPep ;
	char buf [20];


	lcd_clear();
    LCD_cursor(0,0);
    itoa(maxPep, buf);
	LCD_string(buf);
    LCD_cursor(0,3);
    LCD_string("Personen");
	LCD_cursor(1,0);
    LCD_string("duerfen rein");
    gpio_write(redLED, 0);
    gpio_write(greenLED, 1);
    usleep(500000); //delay(500);
    gpio_write(greenLED, 0);
    usleep(500000); //	delay(500) ;
    gpio_write(greenLED, 1);


while(1)
  {
	 ToneStop (BUZZER); 
	  if (gpio_read(TASTER) ==  0 ){break;}
 
	if (Pep >= maxPep && gpio_read(PIR) == 1) {
		
			Pep = Pep + 1 ; 
			Timer [Pep-1]= maxTime;
			gpio_write(greenLED, 0);
			gpio_write(redLED, 1);
			maketone(BUZZER);
			usleep(500000);	//		delay(500); 
  			gpio_write(redLED, 0);
			ToneStop (BUZZER);
  			usleep(500000);  //delay(500) ; 
			maketone(BUZZER);
			sendEmail(argv);
			//sendemail() ;
 			gpio_write(redLED, 1);
			
			lcd_clear();
			LCD_cursor(0,0);
			LCD_string("Es ist ueberlastet");
			LCD_cursor(1,0);
			LCD_string("Bitte warten Sie");
			
			for (int i=0; i <Pep ; i++  )
				{
					if (Timer [i] > 0)
					{
						Timer[i] = Timer[i]-2;// - 2Sec  
					}
					else if (Timer [i] <= 0)
					{
						for (int j=0; j <Pep; j++  )
						{
							Timer[j] = Timer[j+1];
						}	
						Pep = Pep-1;
					}
				}
					
		
		 }
	else if (Pep < maxPep && gpio_read(PIR) == 1)
			{
				
			Pep = Pep + 1 ; 
			Timer [Pep-1]= maxTime;

			if (maxPep == Pep){lcd_clear();
            					LCD_cursor(0,0);
            					LCD_string("Es ist geschlossen");
								LCD_cursor(1,0);
								LCD_string("Bitte warten Sie");
								gpio_write(greenLED, 0);
								maketone(BUZZER);
                       			gpio_write(redLED, 1);
                        		usleep(500000); //delay(500);
                       			gpio_write(redLED, 0);
								ToneStop (BUZZER);
                        		usleep(500000); //	delay(500) ;
                         		gpio_write(redLED, 1);
								maketone(BUZZER);

						}
			else 	{
			
				restPep = maxPep - Pep ;
			
				lcd_clear();
				LCD_cursor(0,0);
				itoa(restPep, buf);
				LCD_string(buf);
				LCD_cursor(0,3);
				LCD_string("Personen");		
				LCD_cursor(1,0);
				LCD_string("duerfen rein");
				gpio_write(redLED, 0);
				ToneStop (BUZZER);
                       		gpio_write(greenLED, 1);
                        	usleep(500000); //delay(500);
                       		gpio_write(greenLED, 0);
                        	usleep(500000); //	delay(500) ;
                        	gpio_write(greenLED, 1);	
				}
			
		
			
			
		
			
			for (int i=0; i <Pep ; i++  )
				{
					if (Timer [i] > 0)
					{
						Timer[i] = Timer[i]-2;// - 2Sec  eine wegen 2 usleep in deise For eine wegen ein Usleep am Ende
					}
					else if (Timer [i] <= 0)
					{
						for (int j=0; j <Pep; j++  )
						{
							Timer[j] = Timer[j+1];
						}	
						Pep = Pep-1;
					}
				}	
			
		
			}
			
	else 
		{
			for (int i=0; i <Pep ; i++  )
				{
					if (Timer [i] > 0)
					{
						Timer[i] = Timer[i]-1;// - 1Sec  da kein delay in diese funktion breucht mann nur eine wegen  wegen ein Usleep am Ende
					}
					else if (Timer [i] <= 0)
					{
						for (int j=0; j <Pep; j++  )
						{
							Timer[j] = Timer[j+1];
						}	
						
						Pep = Pep-1;
						
						if (maxPep <= Pep){	lcd_clear();
											LCD_cursor(0,0);
											LCD_string("Es ist geschlossen");
											LCD_cursor(1,0);
											LCD_string("Bitte warten Sie");
											gpio_write(greenLED, 0);
											gpio_write(redLED, 1);
										  }
						else 	{
									restPep = maxPep - Pep ;
									gpio_write(redLED, 0);
									gpio_write(greenLED, 1);
									lcd_clear();
									LCD_cursor(0,0);
									itoa(restPep, buf);
									LCD_string(buf);
									LCD_cursor(0,3);
									LCD_string("Personen");		
									LCD_cursor(1,0);
									LCD_string("duerfen rein");			
								}	
						
					}
				}
					
					
					
		}
				 
 
  	usleep(1000000);	//	delay(1000); muss mit usleep versuchen
  
  }
  }
  
  
  gpio_unexport(GPIO_RS);
        gpio_unexport(GPIO_CSB);

  return(0);
  }
