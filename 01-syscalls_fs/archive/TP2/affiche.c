#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
    if(argc > 1){
        printf("%s\n",argv[1]);
    }else{
        printf("No parameters entered\n");
    }
}