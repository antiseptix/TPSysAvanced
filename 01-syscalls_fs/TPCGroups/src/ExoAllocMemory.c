#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int nbFois = atoi(argv[1]);
    
    for(int i = 0; i< nbFois; i++)
    {
        printf("Création du %d ème octé \n", i);
        void * var = malloc(1048576); //Alloue 1 Mo nbFois (50 fois)
        sleep(1);
    }  
}