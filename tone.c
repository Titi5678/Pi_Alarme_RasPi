



#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include "tone.h"
#include "gpio.h"
#include <sys/time.h>

#define UNU  __attribute__((unused)) 
#define PI_THREAD(X) void *X (UNU void *dummy)


#define	MAX_PINS	26

#define	PULSE_TIME	100

static int freqs[MAX_PINS] ;
static pthread_t threads[MAX_PINS] ;

static int newPin = -1 ;


static PI_THREAD (softToneThread)  //this set a threads to do pwm output 
{
  int pin, freq, halfPeriod ;
  struct sched_param param ;

  param.sched_priority = sched_get_priority_max (SCHED_RR) ;
  pthread_setschedparam (pthread_self (), SCHED_RR, &param) ;

  pin    = newPin ;
  newPin = -1 ;

  highpriority(50) ;

 gpio_export(pin);
 gpio_direction( pin,  1);

  for (;;)
  {
    freq = freqs[pin] ;
    if (freq == 0)
      delay(1) ;
    else
    {
      halfPeriod = 500000 / freq ;

      gpio_write(pin, 1) ;
      delayMicroseconds(halfPeriod) ;

      gpio_write(pin, 0) ;
      delayMicroseconds(halfPeriod) ;
    }
  }

  return NULL;
}


void ToneWrite (int pin, int freq)  // to write frequency value to given pin 
{
  pin &= 63 ;

  /**/ if (freq < 0)
    freq = 0 ;
  else if (freq > 5000)	// Max 5KHz
    freq = 5000 ;

  freqs[pin] = freq ;
}



int ToneCreate (int pin)  //create a new tone thread 
{
  int res ;
  pthread_t myThread ;

  gpio_export(pin);
  gpio_direction( pin,  1);
  
  gpio_write(pin, 0) ;

  if (threads [pin] != 0)
    return -1 ;

  freqs[pin] = 0;

  newPin = pin ;
  res    = pthread_create (&myThread, NULL, softToneThread, NULL);

  while (newPin != -1)
    delay(1) ;

  threads [pin] = myThread ;

  return res ;
}



void ToneStop (int pin) //stop tone 
{

 gpio_export(pin);
 gpio_direction( pin,  1);

  if (threads [pin] != 0)
  {
    pthread_cancel (threads [pin]) ;
    pthread_join   (threads [pin], NULL) ;
    threads [pin] = 0 ;
    gpio_write(pin, 0);
  }
}


//buzzer funktion 


void  maketone(int PIN) 

{	
  
        ToneCreate(PIN);
        ToneWrite (PIN,1500) ;
       
}



  int highpriority (const int priority) 
{
	
	 struct sched_param sched ; //this struct used to change the sheldung priority of threads in process //
	 memset (&sched, 0, sizeof(sched)) ; //this funktion will set n-"0" to the sched struct //
	 
	 
	 if (priority > sched_get_priority_max (SCHED_RR))
     sched.sched_priority = sched_get_priority_max (SCHED_RR) ;
     else
     sched.sched_priority = priority ;

     return sched_setscheduler (0, SCHED_RR, &sched) ;
}
 /* this block will set the schelduing priority of max
  * sched prioryty will recive tha max between ( gegeben priority and sched priority max ) */



  void delayMicrosecondsHard (unsigned int howLong)
{
  struct timeval tNow, tLong, tEnd ;

  gettimeofday (&tNow, NULL) ;
  tLong.tv_sec  = howLong / 1000000 ;
  tLong.tv_usec = howLong % 1000000 ;
  timeradd (&tNow, &tLong, &tEnd) ;

  while (timercmp (&tNow, &tEnd, <))
    gettimeofday (&tNow, NULL) ;
}


void delayMicroseconds (unsigned int howLong)
{
  struct timespec sleeper ;
  unsigned int uSecs = howLong % 1000000 ;
  unsigned int wSecs = howLong / 1000000 ;

  /**/ if (howLong ==   0)
    return ;
  else if (howLong  < 100)
    delayMicrosecondsHard (howLong) ;
  else
  {
    sleeper.tv_sec  = wSecs ;
    sleeper.tv_nsec = (long)(uSecs * 1000L) ;
    nanosleep (&sleeper, NULL) ;
  }
}
  
  














