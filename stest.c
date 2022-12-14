#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "stest.h"


unsigned long randvar = 1;
unsigned int
randint()
{
  randvar = randvar * 1664525 + 1013904223;
  return randvar;
}


void 
fill_vector_random(int *array, int size) 
{
  for(int i = 0; i<size; i++){
    array[i]  = randint()%1000;
  }
}


void 
swap(int *vector,int a,int b)
{
    int aux = vector[a];
    vector[a] = vector[b];
    vector[b] = aux;
}


void 
bubble_sort(int *vector, int size) {  // slow sort
  for (int i = size-1; i>0; i--) {
    for (int j = 0; j<i; j++) {
      if(vector[j]>vector[j+1]) {
       swap(vector,j,j+1);
      }
    }
  }
}


void 
exec_code()
{
  int size = 500000;
  int* array = (int*) malloc(sizeof(int) * size);
  fill_vector_random(array, size);
  bubble_sort(array, size);
}


int
main(int argc, char *argv[])
{
  for(int i=1; i<=TEST_PROCESSES; i++){
    if(fork(i*TEST_PROCESSES) == 0){
      stest(getpid());
      exec_code();
      exit();
    }
  }
  for(int i=1; i<=TEST_PROCESSES; i++){
    wait();
  }
  printf(1, "FINISHED\n");
  stest(-1);
  exit();
}