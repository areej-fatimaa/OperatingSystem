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
    char filename[100];

    printf("Enter the name of the file to delete: ");
    scanf("%s", filename);

    int result = remove(filename);

    if (result == 0) {
        printf("File deleted successfully.\n");
    } else {
        printf("Error deleting file.\n");
    }
 int x = atoi(argv[1]);
 mkfifo("mypipe", 0666);

     int fd = open("mypipe", O_WRONLY);
      write(fd, &x, sizeof(x));
    close(fd);
    return 0;
}
