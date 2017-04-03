#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include "parser.c"
#include <string.h>

int numProcs, level;

void signal_handler(int sig){
 // printf("Child reached signal handler. pid: %d, signal: %d\n", getpid(), sig);
  printf("EXITING: Level %d process with pid=%d, child of ppid=%d\n", level+1, getpid(), getppid()); 
  exit(0);
}




int main(int argc, char ** argv){
    int i;
    int pfds[2];
    char buf[30];
    int fd;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_RDONLY | O_NONBLOCK);
    char * ustr = "./prog3ipc <num-procs>";
    int r;
    int * ptr;
    const char *name = "abelmna1";
    numProcs = parser(&argc, argv, &ustr);
    printf("%s, num_procs = %d\n", "parsed successfully", numProcs);
    int shm_fd;
    const int SIZEABEL = numProcs*(sizeof(int));
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if(shm_fd == -1){
      fprintf(stderr, "%s\n", "shared memory object not created.");
      exit(0);
    }
    ftruncate(shm_fd, SIZEABEL);  //reduce size of shm 
    
    //memory map the shm
    ptr = mmap(0, SIZEABEL, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //sprintf(ptr, "%s", "hello dude\n");
    int parentPid = getpid();
    memcpy(ptr, &parentPid, sizeof(parentPid));    
   // ptr += sizeof(int);
    //printf("parent process: %d\n", *ptr); 
    ptr++;
    level = numProcs; 
    //unnamed pipe


    for(i = 0; i < numProcs-1; i++){
      int pfds[2];
      char buf[30];
      pid_t pid;
      if(pipe(pfds) == -1){
        perror("pipe");
        exit(1);
      }
     
      pid = fork();
      level--;
      if(pid < 0){  
        perror("process created unsuccessfully\n");  
        exit(1);
      }
      //else{  //either child or parent
           

      else if(pid == 0){  //child 
          printf("ALIVE: Level %d process with pid=%d, child of ppid=%d\n", numProcs-i-1, getpid(), getppid());
          int pid2 = getpid();
          memcpy(ptr, &pid2, sizeof(pid2));
          ptr++;  
           close(pfds[1]); //close write end
           if(read(pfds[0], buf, 10) <= 0){
             perror("child didn't receive message\n");
             exit(1);
           }  
           
           //printf("level received: %s\n", buf);
           //printf("level received: %s\n", buf); 
           if(atoi(buf) == 1){  //leaf node reached
              //printf("leaf node reached\n");
             // int fd2;  
             // char * myfifo2 = "/tmp/myfifo";
             // mkfifo(myfifo2, 0666);
             // fd2 = open(myfifo2, O_WRONLY);
              level = 0;             
              fd = open(myfifo, O_WRONLY);
	      write(fd, "leaf", sizeof("leaf"));
              close(fd);
              //unlink(myfifo);
              signal(SIGUSR1, signal_handler);
	      pause();
           }
           unlink(myfifo);
         //child_function();  
       }
       else{   //parent
           //printf("level %d\n", level);
           
             close(pfds[0]); //close read end
             char buffer[10];
             snprintf(buffer, 10, "%d", level);
             if(write(pfds[1], buffer, 10) <= 0){
               perror("can't write message\n"); 
               exit(1);
             }
             /*int read_fd = open(myfifo, O_RDONLY);
             char buffer2[10];           
             read(read_fd, buffer2, 10);
             //printf("Received: %s\n", buffer2);
             close(read_fd);
        
             unlink(myfifo);
             if(strcmp(buffer2, "leaf") != 0){
 	        pause();
             }
             else printf("leaf\n"); */
              break;
             //break;
        }
     //}
   }
  
   if(parentPid != getpid()){
     signal(SIGUSR1, signal_handler);
     pause();
   }
   else{   //root node reached
      //wait(NULL);
      sleep(1);
     // printf("reaches here\n");
     //printf("ORIGINAL PID: %d\n", getpid());
     //int read_fd = open(myfifo, O_RDONLY); 
     char buffer[10];
     
     if(read(fd, buffer, 4) == -1){
        perror("named pipe failed\n");
        exit(1);
     }
     /*else{
       printf("leaf\n");
     }*/
     //printf("%s\n", buffer);
     close(fd);
     unlink(myfifo);
     ptr--;
     for(i = 0; i < numProcs; i++){
     	printf("%d\n", ptr[i]);   
     }
     
     for(i= 1; i < numProcs; i++){
       kill(ptr[i], SIGUSR1);
     }
      if(shm_unlink(name) == -1){
        fprintf(stderr, "%s\n", "shared mem object not created.");
        exit(0);    
       }
     /*for(i = 0; i < numProcs-1; i++){
        if(i == 0){
           printf("process: %d, parent: %d\n", *ptr, *(ptr-1));
        }
        else printf("process: %d, parent: %d\n", ptr[i], ptr[i]-1); 
     }*/
      //printf("reaches here\n");
      printf("EXITING: Level %d process with pid=%d, child of ppid=%d", numProcs, getpid(), getppid());


   }
   

   //unlink(myfifo);
  
 
    return 0;
}
