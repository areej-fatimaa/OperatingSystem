#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
int main(int argc, char *argv[])
{
    char str[100];
    int i, j;  
    printf("Enter a string: ");
    fgets(str, 100, stdin);
    str[strcspn(str, "\n")] = '\0';
    j = strlen(str) - 1;
    for (i = 0; i < j; i++, j--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
    printf("Reversed string: %s\n", str);

int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);
    return 0;
}
