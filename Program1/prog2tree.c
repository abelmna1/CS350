#include <stdio.h>
#include <stdlib.h>
#include "parser.c"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int reduce(int num, char ** args){
  
   int i,j;
   char array[10];
   char * temp;
   for(i = 0; i < num; i++){
      if(strcmp("-N", args[i]) == 0){
	 j = atoi(args[i+1]);
         j--;
         sprintf(array, "%d", j);
         //  printf("%s\n", array);
         temp = array;
         //  printf("temp: %s\n", temp);
         memcpy(args[i+1], temp, strlen(temp)+1);
      }
   }
     return 0;
}




int main(int argc, char **argv){
// printf("argv[2]: %s\n", argv[2]);
 //printf("%s\n", argv[2]);

 int numC, numL, s, i, j;
 numC = 1; 
 numL = 1;
 int reached = 0;
 s  = 1;
 int p = 0;
 int r; 
 char * ustr;
 int count = 0;
 int value = 0;
 char numArray[10];
 int defaultLeaf = 0;
 char * newArgs[9];
 ustr = "prog2tree [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-tme]";
 r = parser(&numC, &numL, &p, argv, &argc, &ustr, &s);
 if(numL > 4 || numC > 3){
  fprintf(stderr, "%s\n", "number provided is out of range.");
  exit(0);
 }
 if(numL == 0) numL = 1; 
 if(p == 0 && r == 0){   //neither -s nor -p flags were called
  defaultLeaf = 1;
 } 
  //printf("p: %d, s: %d\n", p, r);
 if(p == 1 && r == 1){
    fprintf(stderr, "%s\n", "both -s and -p aren't allowed to be called.");  
    exit(0);
 }
 //printf("ALIVE: Level %d process with pid=%d, child of ppid=%d\n", numL-1, getpid(), getppid());
 printf("ALIVE: Level %d process with pid=%d, child of ppid=%d\n", numL-1, getpid(), getppid());
 if(numL == 0 || numL == 1){
   if(defaultLeaf){
      sleep(1);
   }
   else if(p){
     pause();
   }
   else{
    sleep(s);
   }
   //printf("EXITING: Level %d process with pid=%d, child of ppid=%d\n", numL-1, getpid(), getppid());
   reached = 1;
 }

 else if(numL > 1){
 //    for(i = 0; i < numL; i++){
    for(j = 0; j < numC; j++){
         pid_t pid;    
         pid = fork();
         if(pid < 0){
            fprintf(stderr, "%s\n", "fork failed.");
            exit(0);
         }
        else if(pid == 0){
           reduce(argc, argv);
           execvp("./prog2tree", argv); 
         }
    }
     for(i=0; i < numC; i++)
      wait(NULL);
 }
 printf("EXITING: Level %d process with pid=%d, child of ppid=%d\n", numL-1, getpid(), getppid());
 return 0;
}





