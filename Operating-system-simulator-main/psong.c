#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
int main(int argc, char *argv[]) {
   printf("Going to Beep 100 times \n");
   while(100)
    printf("\a");
 int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);
    return 0;
}
