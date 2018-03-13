/**
 * Assignment 5: Life in Cilk
 * Team Member 1 Name: Yessenia Valencia
 * Team Member 2 Name: 
 *
 */

#include "life.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

int cellValue(int i, int j, int size, int *arr) {
  int val = arr[i*size+j]; 
  return val; 
}
void printArray(int *arr, int size) {
  int currentIndex = 0; 
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      std::cout << arr[currentIndex] << " "; 
      currentIndex++; 
    }
    std::cout << std::endl; 
  }
}

void genlife(int *a, unsigned int n)
{
  std::ofstream resultFile;
  resultFile.open("generatedInput");
  int currentIndex = 0;
  srand(time(NULL));
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      int randNumber = rand() % 2; 
      a[currentIndex] = randNumber; 
      resultFile << a[currentIndex] << " "; 
      currentIndex++; 
    }
    resultFile << "\n";
  }
  resultFile.close(); 
}

//Read the life matrix from a file
void readlife(int *a, unsigned int n, char *filename) {
  FILE * fp; 
  fp = fopen(filename, "r"); 
  if(fp == NULL) {
    fprintf(stderr, "error: cannot open file");
    exit(0); 
  }
  int currentIndex = 0;
  for(int i = 0; i < n	; i++) {
    for(int j = 0; j < n; j++) {
      int result = fscanf(fp, "%d", &a[currentIndex] );
      currentIndex++; 
    }
  }
  std::cout << std::endl;
  printArray(a, n); 
  std::cout << std::endl;
}
int numNeighbors(int i, int j, int size, int *arr) {
  int d1x = i-1; 
  if(d1x == -1)
    d1x += size;
  int d2x = i+1;
  if(d2x >= size)
    d2x = d2x % size;
  int d1y = j-1;
  if(d1y == -1)
    d1y += size;
  int d2y = j+1; 
  if(d2y >= size)							
    d2y = d2y % size; 
  int verCount = cellValue(d1x, j, size, arr) + cellValue(d2x, j, size, arr); 
  int horCount = cellValue(i, d1y, size, arr) + cellValue(i, d2y, size, arr); 
  int diagCount = cellValue(d1x, d1y, size, arr) + cellValue(d1x, d2y, size, arr) + cellValue(d2x, d1y, size,arr) + cellValue(d2x, d2y, size, arr);
  int count = verCount + horCount + diagCount; 
  return count;
}
void life(int *a, unsigned int n, unsigned int iter, int *livecount) {
  int debug_iteration = iter/10;
  int livecount_iter = 0;
  printArray(a, n); 
  for(int it = 1; it <= iter; it++) {
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
	int cell_state = a[i*n + j];
	int n_count = numNeighbors(i, j, n, a); 
	if(cell_state == 0) {
	  if(n_count == 3) {
	    a[i*n + j] = 1;
	    break;
	  }
	}
	else {
	  if(n_count < 2) {
	    a[i*n + j] = 0;
	    break;
	  }
	  else if(n_count == 2 || n_count == 3) {
	    a[i*n + j] = 1;
	    break;
	  }
	  else if(n_count > 3) {
	    a[i*n + j] = 0; 
	    break;
	  }
	}
      }
    }    
#if DEBUG == 1
    if(it % debug_iteration == 0) {
      int total_lives = countlive(a, n); 
      livecount[livecount_iter] = total_lives;
      livecount_iter++;
    }
#endif
  }
  for(int i = 0; i < 10; i++) 
    std::cout << "livecount[" << i << "] = " << livecount[i] << std::endl;
  
}
