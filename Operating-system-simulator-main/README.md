# Operating-system-simulator
This is my semester project of operating system. In this project I have implemented operating system simulator in c.
It takes RAM , HARD and number of cores from user and run processes accordingly.
I have used threads, processes, semaphores and pipes(named,simple).
In this project the basic purpose was to run multiple files togather so happening.
It first compile the file then run it on a new terminal using x-terminal-emulator and our main terminal cotinue execution.
After another file completes its execution it sends back its id to main fiile(project1.c) and then it manages the ram and active cores accordingly.
It also have a waiting queue and a queue which shows active processes(ready queue).
Made on ubuntu and can be complied with  gcc  -pthread -o obj project1.c -lncurses and run its object file by ./obj
I am providing a zip file in which all necessary files are included to run this along with same files seprately
