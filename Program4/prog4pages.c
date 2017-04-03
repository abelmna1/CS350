#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int generator(int min, int max){
    int retVal;
    retVal = min + (max-min) * (double)rand() / (double)RAND_MAX + 0.5;
    //printf("%d\n", retVal);
    return retVal;
}


int shuffleArray(int * array, int n){
    if(n > 1){
       int i;
       for(i = 0; i < n-1; i++){
           int j = i + rand() / (RAND_MAX / (n-i) + 1);
           int t = array[j];
           array[j] = array[i];
           array[i] = t;
       }
    }
    return 0;
}


/*int cacheInitialize(int * array, int size){
    int i;
    array = malloc(sizeof(int) * size);
    for(i = 0; i < size; i++){
        array[i] = -1;
    }
    return 0;
}*/

int initializeLocality(int * array){
    int i;
    for(i = 0; i < 10000; i++){
        array[i] = generator(1, 100);
        //printf("%d\n", array[i]);
    }
    return 0;
}

int initializeNumbers(int * array){
    int i;
    for(i = 0; i < 8000; i++){
        array[i] = generator(1, 20);
    }
    for(i = 8000; i < 10000; i++){
        array[i] = generator(21, 100);
    }
    shuffleArray(array, 10000);
    return 0;
}

int initializeLoop(int * array){
    int i, index;
    int value = 1;
    for(i = 0; i < 10000; i++){
        if(value < 51){
           array[i] = value;
           value++;
        }
        if(value > 50){
           value = 1;
        }
    }
    return 0;
}



double randomPolicy(int * pageAccesses, int cacheSize, int numPages){
    int cache[cacheSize];
    int found, i, j, space, index, prevFound, hits;

    for(i = 0; i < cacheSize; i++){   //initialize cache
        cache[i] = -1;
    }

    found = hits = 0;
    index = -1;
    space = cacheSize;
    for(j = 0; j < numPages; j++){
        for(i = 0; i < cacheSize; i++){
            if(pageAccesses[j] == cache[i]){
                //printf("%s\n", "hit");
                hits++;
                found = 1;
 	    }
	}
        if(found == 0){
            //printf("%s\n", "miss");
            for(i = 0; i < cacheSize; i++){
                 if(cache[i] == -1){
                     index = i;
                     break;
                 }
            }
            if(index >= 0){   //if available slot is found
               cache[index] = pageAccesses[j];
            }
            else{            //slot needs to be evicted
                index = generator(0, cacheSize-1);
                //printf("random number: %d\n", index);
                cache[index] = pageAccesses[j];
            }
        }
        found = 0; index = -1;
    }
    return (double)hits / numPages;
}

double fifoPolicy(int * pageAccesses, int cacheSize, int numPages){
    int cache[cacheSize];
    int front, end, i, j, index, found, hits;

    for(i = 0; i < cacheSize; i++){
        cache[i] = -1;
    }

    found = front = hits = 0;
    index = -1;
    end = cacheSize-1;
    for(i = 0; i < numPages; i++){
        //printf("pageAccesses[i] = %d\n", pageAccesses[i]);
        for(j = 0; j < cacheSize; j++){
            if(pageAccesses[i] == cache[j]){
                //printf("hit\n");
		//printf("cache[%d] = %d\n", j, cache[j]);
                hits++;
                found = 1;
            }
        }
        if(found == 0){
            //printf("miss\n");
            for(j = cacheSize-1; j >= 0; j--){  //look for available slots at end of list
                if(cache[j] == -1){
                    index = j;
                    //printf(" cache index: %d\n", index);
                    break;
                }
            }
            if(index >= 0){  //if available slot is found
                cache[index] = pageAccesses[i];
               // printf("cache[%d] = %d\n", index, pageAccesses[i]);
            }
            else{          //find page for eviction
                for(j = cacheSize-1 ; j > 0; j--){
                   cache[j] = cache[j-1];
                }
                cache[0] = pageAccesses[i];
            }

        }
        found = 0;
        index = -1;
   }
   return (double)hits / numPages;
}

double lruPolicy(int * pageAccesses, int cacheSize, int numPages){
    int i, j, index, found, value, duplicate;
    int hits = 0;
    found = 0;
    int cache[cacheSize];
    for(i = 0; i < cacheSize; i++){
        cache[i] = -1;
    }
    index = duplicate = value = -1;
    for(i = 0; i < numPages; i++){
       // printf("pageAccesses[%d] = %d\n", i, pageAccesses[i]);
        for(j = 0; j < cacheSize; j++){
          // printf("cache[%d] == %d\n", j, cache[j]);
            if(pageAccesses[i] == cache[j]){
                //printf("hit\n");
                hits++;
                found = 1;
                value = cache[j];
            }
        }
        if(found == 1){   //update cache on hit
           //printf("enters found block\n");
           for(j = 0; j < cacheSize; j++){
		if(value == cache[j]){
		    duplicate = j;
                }
           }
           if(duplicate >= 1){    //duplicate found
               //printf("duplicate found\n");
               if(duplicate == cacheSize-1){   //last index is special case
		   for(j = cacheSize-1; j >= 1; j--){
		       cache[j] = cache[j-1];
                   }
                   cache[0] = value;
               }
               else{
	           for(j = duplicate; j > 0; j--){   //duplicate acts as pivot, shift all before it
		       cache[j] = cache[j-1];
                   }
                   cache[0] = value;
               }

           }
           else if(duplicate == 0){  //duplicate at first index
               cache[0] = value;
           }
           else{  //no duplicate
               for(j = cacheSize-1; j > 0; j--){
		    cache[j] = cache[j-1];
      	       }
               cache[0] = value;
           }
        }
        if(found == 0){
              //printf("miss\n");
              for(j = cacheSize-1; j >= 0; j--){
                  if(cache[j] == -1){
		      index = j;
                      break;
                  }
              }
              if(index >= 0){  //available slot in cache
                  cache[index] = pageAccesses[i];
              }
              else{    //eviction from cache
	         for(j = cacheSize-1; j >= 0; j--){
                     cache[j] = cache[j-1];
                 }
                 cache[0] = pageAccesses[i];
              }
        }
          index = -1; value = -1; found = 0; duplicate = -1;
          //printf("reaches here\n");
      }

     return (double)hits / numPages;
}

double clockPolicy(int * pageAccesses, int cacheSize, int numPages){
    typedef struct block{
        int num;
        int usageBit;
    } block;

    block cache[cacheSize];
    int i, j, index, found, value, clockHand;
    int flag = 0;
    clockHand = 0;  //generate clock hand
    index = value = -1; found = 0;
    int hits = 0;
    for(i = 0; i < cacheSize; i++){
        cache[i].num = -1;
        cache[i].usageBit = 0;
    }

    for(i = 0; i < numPages; i++){
        //printf("pagesAccesses[%d] = %d\n", i, pageAccesses[i]);
        for(j = 0; j < cacheSize; j++){
            //printf("cache[%d] = %d, %d\n", j, cache[j].num, cache[j].usageBit);
            if(pageAccesses[i] == cache[j].num){
                found = 1;
               // printf("hit\n");
                hits++;
                value = cache[j].num;
                if(found == 1){   //update usage bit
                    if(cache[j].usageBit == 0){
                        cache[j].usageBit = 1;
                    }
                }
            }
        }
        if(found == 0){  //not found
           //printf("miss\n");
           for(j = 0; j < cacheSize; j++){
               if(cache[j].num == -1){
                   index = j;
               }
               //break;
           }
           if(index >= 0){  //available slot in cache
               //printf("index: %d\n", index);
               cache[index].num = pageAccesses[i];
               cache[index].usageBit = 1;
           }
           else{   //slot needed for eviction
                while(flag != 1){
                    for(j = clockHand; j < cacheSize; j++){
                       if(cache[j].usageBit == 0){  //unused page found
                           cache[j].num = pageAccesses[i];
                           cache[j].usageBit = 1;
                           clockHand = j;
                           flag = 1;
                           break;
                       }
                       else{
                           cache[j].usageBit = 0;
                       }
                    }
                    if(flag != 1){ //still couldn't find anything
                        clockHand = 0;
                    }
                    /*if(clockHand == cache
Size-1 && flag == 0){  //end of array is a special case
                       if(cache[clockHand].usageBit == 0){  //unused page found
                           cache[clockHand].num = pageAccesses[i];
                           cache[clockHand].usageBit = 1;
                           //clockHand = 0;
                           flag = 1;
                       }
                       else{
                           cache[clockHand].usageBit = 0;
                           clockHand = 0;
                       }
                    } */
                }

           }


        }

        index = -1; value = -1; found = 0; flag = 0;
   }
        return (double)hits / numPages;
}

double optimalPolicy(int * pageAccesses, int cacheSize, int numPages){
    int i, j, k, index, index2, value, found, pageNumber, found2;
    int hits = 0;
    int cache[cacheSize];
    int cacheStates[cacheSize];
    for(i = 0; i < cacheSize; i++){
        cache[i] = -1;
        cacheStates[i] = -1;
    }



    index = value = index2 = -1;
    found = pageNumber = found2 = 0;

    for(i = 0; i < numPages; i++){
       //printf("pageAccesses[%d]: %d\n", i, pageAccesses[i]);
        for(j = 0; j < cacheSize; j++){
            //printf("cache[%d]: %d\n", j, cache[j]);
            if(pageAccesses[i] == cache[j]){
                //printf("hit\n");
                hits++;
                found = 1;
            }
        }

        if(found == 0){   //page not found
            //printf("miss\n");
            for(j = 0; j < cacheSize; j++){
                if(cache[j] == -1){
                    index = j;
                    break;
                }
            }
            if(index >= 0){   //deals with cold start misses
                cache[index] = pageAccesses[i];
            }
            else{             //page needed for eviction
                 pageNumber = i; //current page in sequence
                 for(j = 0; j < cacheSize; j++){
                     for(k = pageNumber; k < numPages; k++){  //k = pageNumber
                         if(cache[j] == pageAccesses[k]){
                                 found2 = 1;
                                 cacheStates[j] = k;  // cacheStates[j] = 1
                         }
                     }
 		     if(found2 == 0){  //item in cache never used again
 			// printf("never used again: %d\n", cache[j]);
                         cache[j] = pageAccesses[i];
                         break;
                     }
                 }
                 if(found2 != 0){   //every item in cache appears again
                         //NEW**********
                     //max = cacheStates[0];                
                     for(j = 0; j < cacheSize; j++){

                         if(cacheStates[j] == 1){
                             if(index2 < j){
                                 index2 = j;
                             }
                         }

                     }
                    // printf("selected for eviction: %d\n", cache[index2]);
                     cache[index2] = pageAccesses[i];
                 }
            }
        }
        for(j = 0; j < cacheSize; j++){    //reset cacheStates
            cacheStates[j] = -1;
        }

        found = 0; index = -1; value = -1; found2 = 0; index2 = -1;
     }


    return (double)hits / numPages;
}

int main(int argc, char ** argv){
    FILE *fp1;
    FILE *fp2;
    FILE *fp3;
    fp1 = fopen("no_locality.csv", "w");
    fp2 = fopen("80-20.csv", "w");
    fp3 = fopen("looping.csv", "w");
    int pageAccesses[10000];
    /*int pages[11];
    pages[0] = 0;
    pages[1] = 1;
    pages[2] = 2;
    pages[3] = 0;
    pages[4] = 1;
    pages[5] = 3;
    pages[6] = 0;
    pages[7] = 3;
    pages[8] = 1;
    pages[9] = 2;
    pages[10] = 1;*/

    /*int pages2[9];
    pages2[0] = 1;
    pages2[1] = 2;
    pages2[2] = 3;
    pages2[3] = 4;
    pages2[4] = 5;
    pages2[5] = 6;
    pages2[6] = 1;
    pages2[7] = 2;
    pages2[8] = 3;*/
    unsigned long seed;
    seed = time(NULL);
    srand(seed);
    int i, j, k;

    //******************* no locality *******************************
    initializeLocality(pageAccesses);
    if(fprintf(fp1, "%s, %s, %s, %s, %s, %s\n", "#cache", "OPT", "LRU", "FIFO", "RAND", "CLOCK") == -1){
        fprintf(stderr, "%s\n", "information can't be written file");
        exit(1);
    }
    for(i = 5; i <= 100; i+=5){
       double random = randomPolicy(pageAccesses, i, 10000);
       double fifo = fifoPolicy(pageAccesses, i, 10000);
       double lru = lruPolicy(pageAccesses, i, 10000);
       double clock = clockPolicy(pageAccesses, i, 10000);
       double optimal = optimalPolicy(pageAccesses, i, 10000);
       if(fprintf(fp1, "%d, %.2f, %.2f, %.2f, %.2f, %.2f\n", i, optimal, lru, fifo, random, clock) == -1){
           fprintf(stderr, "%s\n", "information can't be written file");
           exit(1);
       }
    }
    fclose(fp1);

    //******************* eight-twenty ******************************

    initializeNumbers(pageAccesses);
    if(fprintf(fp2, "%s, %s, %s, %s, %s, %s\n", "#cache", "OPT", "LRU", "FIFO", "RAND", "CLOCK") == -1){
        fprintf(stderr, "%s\n", "information can't be written file");
        exit(1);
    }
    for(i = 5; i <= 100; i+=5){
       double random = randomPolicy(pageAccesses, i, 10000);
       double fifo = fifoPolicy(pageAccesses, i, 10000);
       double lru = lruPolicy(pageAccesses, i, 10000);
       double clock = clockPolicy(pageAccesses, i, 10000);
       double optimal = optimalPolicy(pageAccesses, i, 10000);
       if(fprintf(fp2, "%d, %.2f, %.2f, %.2f, %.2f, %.2f\n", i, optimal, lru, fifo, random, clock) == -1){
           fprintf(stderr, "%s\n", "information can't be written file");
           exit(1);
       }
    }
    fclose(fp2);

    //******************** looping ********************************
    initializeLoop(pageAccesses);
    if(fprintf(fp3, "%s, %s, %s, %s, %s, %s\n", "#cache", "OPT", "LRU", "FIFO", "RAND", "CLOCK") == -1){
        fprintf(stderr, "%s\n", "information can't be written file");
        exit(1);
    }
    for(i = 5; i <= 100; i+=5){
       double random = randomPolicy(pageAccesses, i, 10000);
       double fifo = fifoPolicy(pageAccesses, i, 10000);
       double lru = lruPolicy(pageAccesses, i, 10000);
       double clock = clockPolicy(pageAccesses, i, 10000);
       double optimal = optimalPolicy(pageAccesses, i, 10000);
       if(fprintf(fp3, "%d, %.2f, %.2f, %.2f, %.2f, %.2f\n", i, optimal, lru, fifo, random, clock) == -1){
           fprintf(stderr, "%s\n", "information can't be written file");
           exit(1);
       }
    }
    fclose(fp3);

    //initializeLocality(pageAccesses);  //generate pages without locality
    //initializeNumbers(pageAccesses);   //generate pages with 80-20 principle
    //initializeLoop(pageAccesses);
    //randomPolicy(pageAccesses, 5, 10000);
    //fifoPolicy(pageAccesses, 5, 10000);
    //lruPolicy(pageAccesses, 5, 10000);
    //clockPolicy(pageAccesses, 5, 10000);
    //optimalPolicy(pageAccesses, 5, 10000);

    /*initializeNumbers(pageAccesses);
    double ret = randomPolicy(pageAccesses, 10, 10000);
    printf("%.2f\n", ret);*/

    return 0;
}
