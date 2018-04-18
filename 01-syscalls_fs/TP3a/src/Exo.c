#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include <unistd.h>

void sig_hand(int sig) {
    printf("signal reçu %d \n",sig);
}

int main(int argc, char **argv) {
    sigset_t sig_proc;
    struct sigaction action;
    sigemptyset(&sig_proc);


    // CHANGER LE TRAITEMENT 
    action.sa_mask = sig_proc;
    action.sa_flags = 0;
    action.sa_handler = sig_hand;
    sigaction(SIGINT, &action,0);

    // MASQUER SIGINT
    sigaddset(&sig_proc, SIGINT);
    sigprocmask(SIG_SETMASK, &sig_proc, NULL);

    //attendre le signal SIGINT
    sigfillset(&sig_proc);
    sigdelset(&sig_proc, SIGINT);
    sigsuspend(&sig_proc);
 
    return EXIT_SUCCESS;
}


