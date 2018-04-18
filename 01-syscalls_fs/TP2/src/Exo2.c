#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<getopt.h>
#include <dirent.h> 
#include<fcntl.h>
#include <unistd.h>

#define STDOUT 1
#define STDERR 2

/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
  if(argc <= 1 )
  { 
      dprintf(STDERR, "Il faut au moins un argument\n");
      exit(EXIT_FAILURE);
  }
  else
  {
      dprintf(STDOUT, "Le premier argument saisi est : %s\n", argv[1]);   
  }

  // Business logic must be implemented at this point
  int pid; 
  int childExitStatus;
  int ffile; //n° file descriptor

  // Test si fils cree
  if ((pid = fork()) < 0)
  {
    dprintf(STDERR, "Erreur creation fork\n");
    return 1;
  }
  
  /**
   * COMMAND : ./Exo2 ps
   */ 

  //Fils
  if (pid == 0){

    int myPID = getpid();
    printf("Je suis le fils %d, mon père :%d\n",myPID,getppid()) ;
    
    // STDOUT -> contenu de "ps" dans le fichier, STDERR affiché sur la console
    //close(STDOUT); // Close STDOUT
    //dup2(1,2); // STDOUT vers STDERR
    //dprintf(STDERR, "Close(STDOUT)\n");

    // STDERR -> sortie STDERR dans le fichier, contenu de "ps" affiché sur la console
    close(STDERR); // Close STDERR
    dup2(2,1); // STDERR vers STDOUT
    dprintf(STDOUT, "Close(STDERR)\n");
        
    static char template[] = "/tmp/proc-exerciceXXXXXX";
    char fname[30];
    strcpy(fname,template);
    ffile = mkstemp(fname);
    
    // Send it in STDERR, Display if close STDOUT
    dprintf(STDERR,"Descripteur de Fichier %s : %d\n",fname,ffile);
    
    execlp(argv[1],argv[1],(char*)NULL);

    dprintf(STDERR,"Erreur creation fork\n");
    
    exit(1);
  }
  //Pere
  else {
    printf("Je suis le père %d, mon fils :%d\n",getpid(),pid) ;
    waitpid( pid, &childExitStatus, 0);
    printf("Message quelconque : Code Retour du Fils %d\n",  WEXITSTATUS(childExitStatus));
    printf("That's all folks !\n");
    exit(0) ;
  }

  return EXIT_SUCCESS;
}
