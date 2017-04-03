#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int parser(int* numC, int* numL, int* p, char* const* argv, int* argC, char ** ustr, int* sleep){
  //char * const * argv = args;
  int argc = *argC;
  int pFlag = 0;
  int sFlag = 0;
  char *str = *ustr;  
  int retVal = 0;
   int c;
  // char * str = "prog2tree [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time]";
   while((c = getopt(argc, argv, "uN:M:ps:")) != -1){
      switch(c) {
          case 'u':
            // uFlag = 1;
             fprintf(stderr, "%s\n", str); 
             exit(0);
          case 'N':
  	     *numL = atoi(optarg);
            // nFlag = 1;
             break;
          case 'M':
	     *numC = atoi(optarg);
 	   //  mValue = optarg;
             break;
          case 'p':
    	     *p = 1;
             pFlag = 1;
             break;
          case 's':
             sFlag = 1;
             *sleep = atoi(optarg);
             retVal = 1;
             break;
          case '?':
            fprintf(stderr, "%s\n", str);
 	    exit(0);
          default:
	     fprintf(stderr, "%s\n", str);
             exit(0);
      }
   }
   /*if(sFlag == 1 && pFlag == 1){
     fprintf(stderr, "%s\n", "both p and s flags assigned.");
     exit(0);
   }*/
  return retVal;
}   

