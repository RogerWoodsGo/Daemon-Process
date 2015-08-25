#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<limits.h>
#include<stdio.h>
#include<errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

static void daemonize(void) {
    int i;
#ifdef SIGTTOU
    signal(SIGTTOU, SIG_IGN);
#endif
#ifdef SIGTTIN
    signal(SIGTTIN, SIG_IGN);
#endif
#ifdef SIGTSTP
    signal(SIGTSTP, SIG_IGN);
#endif
    //fork subprocess
    if (0 != fork()) exit(0);

    //dispatch from parent
    if (-1 == setsid()) exit(0);

    //ignore sighup
    signal(SIGHUP, SIG_IGN);

    //prevent from openning a new console
    if (0 != fork()) exit(0);

    for(i = 0;i < NOFILE;++i)//关闭打开的文件描述符 
        close(i);
    //change to root to prevent umount
    if (0 != chdir("/")) exit(0);

    //change the default file permission
    umask(0);
    int error, out, in;
    error=open("/home/beyondwu/workspace/CPP/testDaemon/error.log",O_WRONLY|O_CREAT,0664);
    dup2(error,2);
    close(error);
    in=open("/home/beyondwu/workspace/CPP/testDaemon/in.log",O_RDONLY|O_CREAT,0664);
    if(dup2(in,0)==-1) perror("in");
    close(in);
    out=open("/home/beyondwu/workspace/CPP/testDaemon/out.log",O_WRONLY|O_CREAT,0644);
    if(dup2(out,1)==-1) perror("out");
    close(out);
}

char log_str[1024];
int main(){
    char time_str[20] = {'\0'};
    time_t time_type;
    time(&time_type);
    daemonize();
    while(1){
        strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", localtime(&time_type));
       memcpy(log_str, time_str, strlen(time_str)); 
       memcpy(log_str + strlen(time_str), ":produce message!", strlen(":produce message!")); 
       printf("%s\n", log_str);
       fflush(stdout);
       sleep(1);
   }
    return 0;
}


