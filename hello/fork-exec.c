#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void child1(void) {
  printf("Hello from Child #1 (pid %d)!\n", getpid());
  _exit(0);
}

static void child2(void) {
  printf("Hello from Child #2 (pid %d)! About to exec ./hi\n", getpid());

  // This line is from class , execl("./hi", "hi", (char*)NULL);
  // It execs the program built from hi.c (the "exec target").
  int status = execl("./hi", "hi", (char *)NULL);

  // If exec succeeds, it NEVER returns. So reaching here means exec failed.
  if (status == -1) {
    perror("execl failed");
    exit(EXIT_FAILURE);
  }

  _exit(0);
}

int main(void) {
  pid_t p1 = fork();
  if (p1 < 0) {
    perror("fork child1");
    return 1;
  }

  if (p1 == 0) {
    child1();
  }

  // Parent makes a SECOND child process
  pid_t p2 = fork();
  if (p2 < 0) {
    perror("fork child2");
    return 1;
  }

  if (p2 == 0) {
    child2();
  }

  printf("Hello from Parent (pid %d)! child1=%d child2=%d\n", getpid(), p1, p2);

  // Parent waits so there are no weird stuff going
  waitpid(p1, NULL, 0);
  waitpid(p2, NULL, 0);

  printf("Parent (pid %d) exiting.\n", getpid());
  return 0;
}