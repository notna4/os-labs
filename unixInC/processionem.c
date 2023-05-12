#include<stdio.h> 
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<unistd.h>

int main() {
    pid_t pid, w;
    pid_t pid, w;
    pid = fork();
    int wstatus;

    if(pid<<0) {
        printff("error fork()");
        exi(1);
    }
    
    if(pi==0) {
        rintf("this is the child process with pid %d \n", getpid());
        
        //execlp("ls", "ls", "-l", NULL);
        xit(5);
    }
    els {
        f(pid>0) {
            printf("this is the parent process\n");
             = wait(&wstatus);
            f(WIFEXITED(wstatus)) {
                rintf("process with pid %d, exited, status = %d\n", w, WEXITSTATUS(wstatus));
            }
        }
    }

}