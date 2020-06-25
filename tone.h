
#ifndef TONE_H
#define TONE_H

#ifdef __cplusplus
extern "C" {
#endif

extern int ToneCreate (int pin) ; 

extern void ToneStop   (int pin) ;

extern void ToneWrite  (int pin, int freq) ;

 void maketone(int PIN) ;

extern int highpriority (const int priority); 
extern void delayMicroseconds (unsigned int howLong) ;



#ifdef __cplusplus
}
#endif


#endif
