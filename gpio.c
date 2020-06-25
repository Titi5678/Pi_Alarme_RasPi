#include "gpio.h" 


// For PIR and LEDs
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// FOR lcd 

# include <string.h>
# include <stdint.h>
# include <linux/spi/spidev.h> 
# include <sys/ioctl.h>

#define GPIO_RS 17
# define GPIO_CSB 27
# define GPIO_MOSI 10
# define GPIO_CLK 11
# define RESET 4
# define led 22
#include <stdlib.h>
# include <string.h>

//for Buzzer 

#include <pthread.h>
#include <sched.h>
#include <sys/time.h>







/* Symbolische Namen fuer die Datenrichtung und die Daten  */
#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

/* Datenpuffer fuer die GPIO-Funktionen */
#define MAXBUFFER 100

/*
 * Delay (warten), Zeitangabe in Millisekunden
 */
int delay(unsigned long millis)
  {
  struct timespec ts;
  int err;

  ts.tv_sec = millis / 1000;
  ts.tv_nsec = (millis % 1000) * 1000000L;
  err = nanosleep(&ts, (struct timespec *)NULL);
  return (err);
  }

/* GPIO-Pin aktivieren
 * Schreiben der Pinnummer nach /sys/class/gpio/export
 * Ergebnis: 0 = O.K., -1 = Fehler
 */
int gpio_export(int pin)
  {
  char buffer[MAXBUFFER];    /* Output Buffer   */
  ssize_t bytes;             /* Datensatzlaenge */
  int fd;                    /* Filedescriptor  */
  int res;                   /* Ergebnis von write */

  fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd < 0)
    {
    perror("Kann nicht auf export schreiben!\n");
    return(-1);
    }
  bytes = snprintf(buffer, MAXBUFFER, "%d", pin);
  res = write(fd, buffer, bytes);
  if (res < 0)
    {
    perror("Kann Pin nicht aktivieren (write)!\n");
    return(-1);
    }
  close(fd);
  delay(100);
  return(0);
  }
  
  


/* GPIO-Pin deaktivieren
 * Schreiben der Pinnummer nach /sys/class/gpio/unexport
 * Ergebnis: 0 = O.K., -1 = Fehler
 */
int gpio_unexport(int pin)
  {
  char buffer[MAXBUFFER];    /* Output Buffer   */
  ssize_t bytes;             /* Datensatzlaenge */
  int fd;                    /* Filedescriptor  */
  int res;                   /* Ergebnis von write */

  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if (fd < 0)
    {
    perror("Kann nicht auf unexport schreiben!\n");
    return(-1);
    }
  bytes = snprintf(buffer, MAXBUFFER, "%d", pin);
  res = write(fd, buffer, bytes);
  if (res < 0)
    {
    perror("Kann Pin nicht deaktivieren (write)!\n");
    return(-1);
    }
  close(fd);
  return(0);
  }
  
  

/* Datenrichtung GPIO-Pin festlegen
 * Schreiben Pinnummer nach /sys/class/gpioXX/direction
 * Richtung dir: 0 = Lesen, 1 = Schreiben
 * Ergebnis: 0 = O.K., -1 = Fehler
 */
int gpio_direction(int pin, int dir)
  {
  char path[MAXBUFFER];      /* Buffer fuer Pfad   */
  int fd;                    /* Filedescriptor     */
  int res;                   /* Ergebnis von write */

  snprintf(path, MAXBUFFER, "/sys/class/gpio/gpio%d/direction", pin);
  fd = open(path, O_WRONLY);
  if (fd < 0)
    {
    perror("Kann Datenrichtung nicht setzen (open)!\n");
    return(-1);
    }
  switch (dir)
    {
    case IN : res = write(fd,"in",2); break;
    case OUT: res = write(fd,"out",3); break;
    }
  if (res < 0)
    {
    perror("Kann Datenrichtung nicht setzen (write)!\n");
    return(-1);
    }
  close(fd);
  return(0);
  }
   


/* vom GPIO-Pin lesen
 * Ergebnis: -1 = Fehler, 0/1 = Portstatus
 */
int gpio_read(int pin)
  {
  char path[MAXBUFFER];         /* Buffer fuer Pfad     */
  int fd;                       /* Filedescriptor       */
  char result[MAXBUFFER] = {0}; /* Buffer fuer Ergebnis */

  snprintf(path, MAXBUFFER, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(path, O_RDONLY);
  if (fd < 0)
    {
    perror("Kann vom GPIO nicht lesen (open)!\n");
    return(-1);
    }
  if (read(fd, result, 3) < 0)
    {
    perror("Kann vom GPIO nicht lesen (read)!\n");
    return(-1);
    }
  close(fd);
  return(atoi(result));
  }
  
  
  

/* auf GPIO schreiben
 * Ergebnis: -1 = Fehler, 0 = O.K.
 */
int gpio_write(int pin, int value)
  {
  char path[MAXBUFFER];      /* Buffer fuer Pfad   */
  int fd;                    /* Filedescriptor     */
  int res;                   /* Ergebnis von write */

  snprintf(path, MAXBUFFER, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(path, O_WRONLY);
  if (fd < 0)
    {
    perror("Kann auf GPIO nicht schreiben (open)!\n");
    return(-1);
    }
  switch (value)
    {
    case LOW : res = write(fd,"0",1); break;
    case HIGH: res = write(fd,"1",1); break;
    }
  if (res < 0)
    {
    perror("Kann auf GPIO nicht schreiben (write)!\n");
    return(-1);
    }
  close(fd);
  return(0);
  }
  
  //// LCD Funktionen
  
  //////////////////////// SPI //////////////////////////////////

int spi_fd; 

void LCD_command(uint8_t cmd) // 1
{
        gpio_write( GPIO_CSB, 0); // CSB low
        gpio_write( GPIO_RS, 0 ); // RS low
        static struct spi_ioc_transfer tr;
        uint8_t rx[1];
        uint8_t tx[1];
        tr.tx_buf = (unsigned long)tx;
        tr.rx_buf = (unsigned long)rx;
        tr.len = 1;
        tr.speed_hz = 1000000;
        tr.bits_per_word = 8;
        tx[0] = cmd;
        int rc = ioctl( spi_fd, SPI_IOC_MESSAGE(1), &tr );
        if(rc < 0 ) {
                perror("send message");
                return;
        }
        usleep(1000);
        gpio_write( GPIO_CSB, 1 );
}


void LCD_init() // 2
 {
        gpio_export(GPIO_RS);
        gpio_export(GPIO_CSB);
        gpio_direction( GPIO_RS, OUT );
        gpio_direction( GPIO_CSB, OUT );
        int rc;
        spi_fd = open("/dev/spidev0.0", O_RDWR);
        if(spi_fd < 0 ) {
                perror("open spi device");
                return;
        }
        uint8_t mode = SPI_CPOL | SPI_CPHA;
        rc = ioctl( spi_fd, SPI_IOC_WR_MODE, &mode );
        if(rc < 0 ) {
                perror("send message");
                return;
        }
        usleep(250000);
        LCD_command(0x39); //Function Set - instruction table 1
        LCD_command(0x14); //Bias Set
        LCD_command(0x55); //Power Control
        LCD_command(0x6D); //Follower Control
        LCD_command(0x78); //Contrast Set
        LCD_command(0x38); //Function Set - switch back to instruction table 0
        LCD_command(0x0F); //Display ON/OFF
        LCD_command(0x01); //Clear Display
        usleep(26300);
        LCD_command(0x06); //Entry Mode Set - cursor auto-increment
}


int LCD_cursor(uint8_t line, uint8_t col) // 3
{
        if( (line > 1) || (col>15) )
                return -1; // error
        // Set DDRAM address command
        LCD_command(0x80 + 0x40*line + col);
        return 0; // OK
}
///////////////////////// Ausgabe ////////////////////////////////////
 void LCD_char(char c) // 4 
 
 {
        gpio_write( GPIO_CSB, 0 ); // CSB low
        gpio_write( GPIO_RS, 1 ); // RS high
        static struct spi_ioc_transfer tr;
        uint8_t rx[1];
        uint8_t tx[1];
        tr.tx_buf = (unsigned long)tx;
        tr.rx_buf = (unsigned long)rx;
        tr.len = 1;
        tr.speed_hz = 1000000;
        tr.bits_per_word = 8;
        tx[0] = c;
        int rc = ioctl( spi_fd, SPI_IOC_MESSAGE(1), &tr );
        if(rc < 0 ) {
                perror("send message");
                return;
        }
        gpio_write( GPIO_CSB, 1 ); // CSB high
}
void LCD_string(char *str) // 5

{
        while(*str)
        {
                LCD_char(*str++); // zeichen am LC-Display ausgeben
        }
}

void lcd_clear(void) //6

{
	
	usleep(108000);
        LCD_command(0x01); //Display lĂ¶schen
        usleep(26.3); //auf Display warten
        LCD_command(0x06); //Entry Mode set - Display wartet auf Eingabe
        usleep(26.3); //auf Display warten
}

  
   /* reverse:  reverse string s in place */
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }


void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
 
  
  
  
  
  
  
  
  
  
  
  
