#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ncurses.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>


#define MAX_QUEUE 100  

sem_t *semaphore = NULL;

int ram, hard, cores,activecores,cram;
int fd[2];
struct p_param {
    int pid;
    char filename[100];
    int r;
    int h;
};

int iterator = -1, front = -1;
struct p_param queue[MAX_QUEUE];
struct p_param *running=NULL;
struct p_param obj;



int empty()
{
   
   if(front==-1&&iterator==-1){
       return 1;
       }
  return 0;
   
}
void displayqueue(){
    printf("Current queue:\n");
    
    for(int i=front+1;i<=iterator;){
        if(queue[i].pid!=0){
        printf("%d,%s,%d,%d\n", queue[i].pid, queue[i].filename, queue[i].r, queue[i].h);
        i=(i+1)%MAX_QUEUE;
        if(i==front){break;}
      }
    }
}
void removequeue(int pid)
{
    for(int i=0;i<MAX_QUEUE;i++)
         {
           if(pid==queue[i].pid){
		queue[i].pid=0;
                break;
                    }
         }

 }
void destroyall(){
    pid_t pid;
    for(int i=0;i<cores;i++)
    {
        if(running[i].pid!=0){
           running[i].pid=0;
            int signal = SIGKILL;
    int result = kill(pid, signal);
    }
}
    for(int i=0;i<MAX_QUEUE;i++){

            if(queue[i].pid!=0){
                   queue[i].pid=0;
                   int signal = SIGKILL; 
                   int result = kill(pid, signal);
              }
    }
    front=-1;
    iterator=-1;
    activecores=cores;
    ram=cram;
  
}
int run = 0;
void activeprocesses(){
  printf("Active processes on Cores : \n");
   for(int i=0;i<cores;i++){
     if(running[i].pid!=0){
      printf("The process %s with process id %d\n",running[i].filename,running[i].pid);
      }
   }

 }
void compliefile(char* filename,int pid){
     char command[100];
        sprintf(command, "gcc -o %s%d.out %s", filename,pid, filename);
        if (system(command) != 0) {
            perror("gcc");
            exit(EXIT_FAILURE);
        }

}
void runfile(char* filename,int pid){
        char cmd[100];
        
        sprintf(cmd, "x-terminal-emulator -e './%s%d.out %d' &", filename,pid,pid);
        
        system(cmd);


}
int emptycore(){
    for(int i=0;i<cores;i++){
      if(running[i].pid==0){
        return i;
       }
    }
 }
void insert_to_queue(struct p_param obj){
    iterator = (iterator + 1) % MAX_QUEUE;
    queue[iterator].pid = obj.pid;
    strcpy(queue[iterator].filename, obj.filename);
    queue[iterator].r = obj.r;
    queue[iterator].h = obj.h;
}
void * threads(void *arg){ 

            sem_t *semaphore = (sem_t *) arg;
           sem_wait(semaphore);
           if(activecores>0){
           activecores--;
           int ch=front;
           int flag=0;
            if(empty()!=1){
            
                 front=(front+1)%MAX_QUEUE;
            
           while(ram<queue[front].r||hard<queue[front].h){
                     insert_to_queue(queue[front]);
                     return NULL;
                 }
            int x=emptycore();
            running[x].pid=queue[front].pid;
           running[x].r=queue[front].r;
	strcpy(running[x].filename,queue[front].filename);
            ram = ram -queue[front].r; // Update RAM
           
        hard = hard - queue[front].h; // Update Hard
      compliefile(queue[front].filename,queue[front].pid);
        runfile(queue[front].filename,queue[front].pid);
         if(front==iterator){
              front=-1;
              iterator=-1;
          }
        }
}
        else{
               front=(front+1)%MAX_QUEUE;
               insert_to_queue(queue[front]);
            }
        
       sem_post(semaphore);
}
void* idreturn(void *arg){
     while(1){

     int flag=0;
       int fd = open("mypipe", O_RDONLY);

    int x=0;
    read(fd, &x, sizeof(x));
    close(fd);
   int j=0;
    if(x!=0){
     flag=1;
    for(int i=0;i<cores;i++){
             if(running[i].pid==x){

                   running[i].pid=0;
                   ram=ram+running[i].r;
		
                   activecores++;
                  
                   break;
                }
       }
     
     }
if(!empty()){

 pthread_t p;
 pthread_create(&p,NULL,threads,semaphore);
 pthread_join(p,NULL);
}
  }


}
int create_process(char* filename, int r, int h) {
    
    pthread_t t;
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t p;
    p = fork();
    if (p == -1) {
        printf("Process not created !! %s\n", filename);
        return 0;
    }
    else if (p == 0) {
        int x = getpid();
     
         close(fd[0]); // close reading end of pipe
                    write(fd[1], &x, sizeof(x)); // write message to pipe
                    close(fd[1]); // clos

        exit(EXIT_SUCCESS);
    }
    else {
        close(fd[1]); // close writing end of pipe
        int id=0;
        read(fd[0], &id, sizeof(id)); // read message from pipe
        obj.pid=id;
        if ((iterator + 1) % MAX_QUEUE != front) {
            strcpy(obj.filename, filename);
            obj.r = r;
            obj.h = h;
            insert_to_queue(obj);
        }
        
        else {
            printf("Queue is full right now try again later : \n");
            return 0;
        }
    }
    
    pthread_create(&t,NULL,threads,semaphore); 
    pthread_join(t,NULL);

    return 0;
}
void intro()
{
    system("clear");
printf("\t\tWelcome to my operating system project!\n");
    printf("\t\tThank you for your interest in my project!\n");
sleep(5);
system("clear");
}
int opening() {

system("clear");
    // Initialize curses
    initscr();

    // Turn off cursor blinking
    curs_set(0);

    // Get the screen dimensions
    int X, Y;
    getmaxyx(stdscr, Y, X);

    // Calculate the position of the text
    int textX = X / 2 - 3;
    int textY = Y / 2;

    // Define the characters for the loading animation
    char chars[] = {'\\', '|', '/', '-'};

    // Print the text
    int i;
    for (i = 0; i < 20; i++) {
        mvprintw(textY, textX, "PAWA is opening...");
        printf("\t");mvprintw(textY + 1, textX, "[%c]", chars[i % 4]);
        refresh();
        usleep(500000); // Wait for 5seconds
        mvprintw(textY + 1, textX, "[%s]", " ");
        refresh();
    }

    endwin();

system("clear");
return 0;
}
int shuttingdown(){
system("clear");
    // Initialize curses
    initscr();

    // Turn off cursor blinking
    curs_set(0);
    int X, Y;
    getmaxyx(stdscr, Y, X);
    int textX = X / 2 - 3;
    int textY = Y / 2;
    char chars[] = {'\\', '|', '/', '-'};
    int i;
    for (i = 0; i < 20; i++) {
        mvprintw(textY, textX, "PAWA is shutting down...");
        printf("\t");mvprintw(textY + 1, textX, "[%c]", chars[i % 4]);
        refresh();
        usleep(500000); // Wait for 5seconds
        mvprintw(textY + 1, textX, "[%s]", " ");
        refresh();
    }

    endwin();
sleep(5);
system("clear");
return 0;
}

int cloc() {
    time_t currentTime;
    struct tm *localTime;

    // Get the current time
    currentTime = time(NULL);

    // Convert the current time to local time
    localTime = localtime(&currentTime);

    // Display the time in hours:minutes:seconds format
    printf("The current time is %02d:%02d:%02d\n",
            localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

    return 0;
}
void* kernelmode(void* arg){
                   
int choice;
                       
                     
                       printf("press 1 to Destroy specific running processes : \n");
                       printf("press 2 to destroy all running processes : \n");
                       printf("press 3 to destroy all  processes : \n");
                       printf("Enter choice : ");
                       scanf("%d",&choice);
                      
                       if(choice==1){
                           activeprocesses();
                           int id;
                            printf("Enter process id : ");
                            scanf("%d",&id);
                            for(int i=0;i<cores;i++)
                            {
                               if(running[i].pid==id){
                                   char filename[200];
                                   sprintf(filename,"pkill %s%d", running[i].filename, running[i].pid);
                                    system(filename);
                                     
                                    ram=ram+running[i].r;
                                    running[i].pid=0;
                                    activecores++;
                                 
                                 }
                            }
                        }
                      
                       else if(choice==2){
                          for(int i=0;i<cores;i++)
                            {
                               if(running[i].pid!=0){
                                   char filename[200];
                                   sprintf(filename, "pkill %s%d", running[i].filename, running[i].pid);
                                    system(filename);
                       
                                    ram=ram+running[i].r;
                                    running[i].pid=0;
                                    activecores++;
                                    if(!empty()){
                                     pthread_t t;
                    pthread_create(&t,NULL,threads,semaphore); 
                    pthread_join(t,NULL);
}

                                  }
                                 }
                                
                            }
                       else if(choice==3){
                              for(int i=0;i<cores;i++)
                            {
                               if(running[i].pid!=0){
                                   char filename[200];
                                   sprintf(filename, "pkill %s%d", running[i].filename, running[i].pid);
                                    system(filename);
                                
                                    ram=ram+running[i].r;
                                    running[i].pid=0;
                                    activecores++;
                                    
                                 }
                             destroyall();
                            
    
                        }

                       }
                       else{
                              printf("Invalid choice !!!\n");
                               }
                    if(!empty()){
                    pthread_t t;
                    pthread_create(&t,NULL,threads,semaphore); 
                    pthread_join(t,NULL);
}                      
           

return NULL;


}

int main() {
printf("\033[1;37m"); // Set the text color to bold white
printf("\033[48;5;236m"); // Set the background color to dark gray



    int option;
  intro();
opening(); 
   pthread_t checkid,checkready;
   pthread_create(&checkid,NULL,idreturn,NULL);
    printf("   Enter the RAM(MBs) HARD(MBs) cores : ");
    scanf("%d", &ram);
    scanf("%d", &hard);
    scanf("%d", &cores);
    cram=ram;
    getchar(); // consume newline character

    running = (struct p_param *) malloc(cores * sizeof(struct p_param)); // allocate memory for n integers
    for(int i=0;i<cores;i++){
             running[i].pid=0;

        }
    activecores=cores;
    char semaphore_name[100];
    sprintf(semaphore_name, "semaphore_%d", getpid());

    semaphore = sem_open(semaphore_name, O_CREAT | O_EXCL, 0644, cores);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    do {
	cloc();
        printf("Press 0 to exit\n");
        printf("Press 1 for Notepad\n");
        printf("Press 2 for Calculator\n");
        printf("Press 3 to create file\n");
        printf("Press 4 to Copy file\n");
        printf("Press 5 to Move file\n");
        printf("Press 6 to Delete file\n");
        printf("Press 7 for Tic TAC TOE file\n");
        printf("Press 8 for Temprature calculator\n");
        printf("Press 9 for Reverse String\n");
        printf("Press 10 for letter count\n");
        printf("Press 11 for Age Calculator\n");
        printf("Press 12 for Play Song\n");
        printf("Press 13 for Number Gueessing\n");
        printf("Press 14 for Gender Guessing\n");
        printf("Press 15 for Kernel MODE\n");
	printf("Press 16 for Display Ram\n");
  	printf("Press 17 for Waiting Queue\n");
	printf("Press 18 for Active Processes\n");
        printf("Enter choice: ");
        scanf("%d", &option);

        switch (option) {
            case 0:  
	 if(option==0)	
	{shuttingdown();} 
               system("kill -1 -1");
                break;
            case 1: {
                
                
                create_process("notepad.c",10,20);
                
                break;
            }
           case 2:
                {
                     create_process("calc.c",5,10);
                     break; 
                }
          case 3:
                {
                     
                     create_process("create.c",2,5);
                     
                     break;
		}
           case 4:
                {
                    
                     create_process("copy.c",2,5);
                     break;
 		}
	  case 5:
 		{
                   
                     create_process("move.c",2,5);
                     break;
		}
          case 6:
		{
                  
                     create_process("delete.c",2,0);
                     break;
		}
	  case 7:
		{
                      
                     create_process("ttt.c",20,30);
                     break;
		}		
 	  case 8:
		{
 		  
                     create_process("tempcal.c",5,8);
                     break;
		}
          case 9:
		{
 			
                     create_process("revstr.c",2,3);
                     break;
		}
         case 10:
		{
			
                     create_process("lcount.c",2,30);
                     break;
		}
          case 11:
		{
                     create_process("agecal.c",2,15);
                     break;
		}
          case 12:
		{
			
                     create_process("psong.c",2,30);
                     break;
		}
         case 13:
		{
			
                     create_process("numguess.c",6,12);
                     break;
		}
        case 14:
		{
			
                     create_process("gender.c",6,12);
                     break;
		}
      case 15:
		{
		   pthread_t km;
                   pthread_create(&km,NULL,kernelmode,NULL);
                   pthread_join(km,NULL);
                  
                   
                   break;
		}
case 16:{
	printf(" RAM  %d \n",ram);
	break;}
case 17:
{
displayqueue();
break;
}
case 18:
{
activeprocesses();
break;
}
            default:
                printf("Invalid option selected\n");
                break;
        }

    } while(option!=0);
 
     pthread_join(checkid,NULL);
    if (sem_close(semaphore) < 0) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }
printf("\033[0m"); // Reset the text color to the default
printf("\033[0m"); // Reset the text color and background color to the default
    return 0;
}


