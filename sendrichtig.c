#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> //exec() family
#include <errno.h> //error
#include <sys/types.h> //fork()
#include <sys/wait.h> //wait()
#include"sendrichtig.h"





pid_t create_process(void)

{

    pid_t pid ; 

    do {

    pid = fork();

    } while ((pid == -1) && (errno == EAGAIN));

    return pid;

}




void sonprocessremovepik(char *tab2[] )

{

wait(NULL);

char *arguments3[] = { "rm", "image.jpg", NULL };

pid_t pid3 = create_process() ;
switch(pid3)
{

case 0 :
execv("/bin/rm",arguments3);
break ;

}

wait(NULL);
exit(EXIT_SUCCESS);
}




void sonprocessSendemail(char *tab[])

{
wait(NULL);

pid_t pid2 = create_process();

switch (pid2)

{


case 0 :
execlp("/bin/sh","sh","-c","echo \"Warnning die Rollentreppe ist voll  bestzt gucken sie bitte anhang datei fuer bild\" | mutt karimbenmoussa41@gmail.com -s \"raspi mail\" -a \"image.jpg\"",NULL);
break ;

default :
sonprocessremovepik(tab) ;
 break ;

}

wait(NULL);
exit(EXIT_SUCCESS);
}



void fatherprocessTakepic(char *tab[] )

{

pid_t pid1 = create_process() ;

char *arguments1[] = { "raspistill", "-o", "/home/root/image.jpg","-q", "100", NULL };

switch(pid1)  

{ 

case 0: 

execv("/usr/bin/raspistill"  ,arguments1 );

break ; 

default : 

sonprocessSendemail(tab) ; 

break ; 

}

exit(EXIT_SUCCESS);
}





void sendEmail(char *tab[]) 

{


pid_t pid = fork() ; 

if (( pid == 0 ) && (i == 0))


{
fatherprocessTakepic(tab); 
}



}







/*int main (int argc , char* argv[]) 


{
printf("\n \n hallo world1 \n \n" );
sendEmail(argv);
printf("\n \n hallo world2\n \n" );
return 0 ;
 
} */





