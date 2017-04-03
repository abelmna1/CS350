#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int parser(int* argC, char * const * argv, char ** ustr){
    int argc = * argC;
    int i, p;
    if(argc == 2){
      p = atoi(argv[1]);
      if(p < 1 || p > 32){
        fprintf(stderr, "%s\n", "num_procs out of range");
        exit(0);
      }
      char c;
      for(c = 0; c < strlen(argv[1]); c++){
        if(!(isdigit(argv[1][c]))){
          fprintf(stderr, "%s\n", "value provided isn't an integer");
          exit(0);
        }
      }
    }
    else{
      fprintf(stderr, "%s\n", *ustr);
      exit(0);
    }
    return p;
} 
