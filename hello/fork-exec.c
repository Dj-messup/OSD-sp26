#include <stdio,h> 
#include <unistd,hm
#include

void child (){
    printf("Hello from Child Process (pid %d)")
}

void parent(){
    printf("Hello from Parent Process( parent pid %d, child pid %d)!\n", getpid(), child_pid);
}

int main() {
    int status = fork();
    if (status <0) {
        //fork failed
        return 1;
    }
    if (status == 0) {
        //child process
        child();
    }
    else {
        //parent proces
        parent(status);
    }
    return 0;
}