  sigaction(SIGINT,&action,0);

  sigaddset(&sig_proc,SIGINT);
  sigprocmask(SIG_SETMASK,&sig_proc,NULL);

  sigfillset(&sig_proc);
  sigdelset(&sig_proc,SIGINT);
  sigsuspend(&sig_proc);