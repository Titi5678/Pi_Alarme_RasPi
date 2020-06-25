// For PIR and LEDs
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
 
// FOR lcd 
 
#include <stdlib.h>
# include <string.h>
# include <stdint.h>
# include <linux/spi/spidev.h> 
# include <sys/ioctl.h>


int delay(unsigned long millis);
int gpio_export(int pin);
int gpio_unexport(int pin);
int gpio_direction(int pin, int dir);
int gpio_read(int pin);
int gpio_write(int pin, int value);

//for LCD 
void LCD_command(uint8_t cmd);  //1
void LCD_init(); //2 
int LCD_cursor(uint8_t line, uint8_t col); //3 
void LCD_char(char c); // 4 
void LCD_string(char *str); //5 
void lcd_clear(void); //6 

void reverse(char s[]);
 

void itoa(int n, char s[]);
 