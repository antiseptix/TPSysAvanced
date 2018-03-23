/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
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

#define MAX_PATH_LENGTH 4096

/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
  // Business logic must be implemented at this point
  int pid;
  // Test si fils cree
  if ((pid = fork()) < 0)
  {
    dprintf(STDERR, "Erreur creation fork\n");
    return 1;
  }

  if (pid == 0){
    int myPID = getpid();
    printf("Je suis le fils %d, mon père :%d\n",myPID,getppid()) ;
    exit(myPID % 10);
  }
  else {
    printf("Je suis le père %d, mon fils :%d\n",getpid(),pid) ;
    int childExitStatus = 0;
    waitpid( pid, &childExitStatus, 0);
    printf("Code Retour du Fils %d\n",  WEXITSTATUS(childExitStatus));
    exit(0) ;
  }
  return EXIT_SUCCESS;
}
