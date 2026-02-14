#include <stdio.h>
#include <unistd.h>

int main(void) {
  printf("Hi from hi.c (pid %d)!\n",getpid());
  return 0;
}