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
  if(val == 0)
    return 0;
  else
    return 1;
}
void setCellValue(int i, int j, int size, int *arr, int value) {
  arr[i*size + j] = value;
}
void isOccupied(int i, int j,int val) {
  std::cout << "(" << i << ", " << j << ")"  << std::endl;
  if(val == 1) 
    std::cout << "This square is occupied." << std::endl;
  else
    std::cout << "This square is empty." << std::endl;
}
void printArray(int *arr, int size) {
  for(int i = 0; i < size; i++) 
    for(int j = 0; j < size; j++) 
      std::cout << cellValue(i,j,size, arr) << " "; 
}
void printLiveCount(int *livecount, int size) {
  std::cout << "livecount [ ";
  for(int i = 0; i < size; i++) 
    std::cout << livecount[i] << " ";
  std::cout << "]" << std::endl;
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
}
int numNeighbors(int i, int j, int size, int *arr) {
  int d1x = i-1; 
  if(d1x == -1) 
    d1x += size;
  int d1y = j-1;
  if(d1y == -1) 
    d1y += size;
  int d2x = i+1;
  d2x = d2x % size;
  int d2y = j+1; 
  d2y = d2y % size; 
  
  int v1 = cellValue(d1x, j, size, arr); // v1 = (x-1,y)
  int v2 = cellValue(d2x, j, size, arr); // v2 = (x+1,y)
  int h1 = cellValue(i, d1y, size, arr); // h1 = (x, y-1)
  int h2 = cellValue(i, d2y, size, arr); // h2 = (x, y+1)
  int d1 = cellValue(d1x, d1y, size, arr); 
  int d2 = cellValue(d1x, d2y, size, arr); 
  int d3 = cellValue(d2x, d1y, size, arr); 
  int d4 = cellValue(d2x, d2y, size, arr); 
  
  int verCount = v1 + v2;
  int horCount = h1 + h2;
  int diagCount = d1 + d2 + d3 + d4;
  
  std::cout << "v1 = (" << d1x << ", " << j << ")" << " = " << v1 << std::endl;
  std::cout << "v2 = (" << d2x << ", " << j << ")" << " = " << v2 << std::endl;
  std::cout << "h1 = (" << i << ", " << d1y << ")" << " = " << h1 << std::endl;
  std::cout << "h2 = (" << i << ", " << d2y << ")" << " = " << h2 << std::endl;  
  std::cout << "d1 = (" << d1x << ", " << d1y << ")" << " = " << d1 << std::endl;
  std::cout << "d2 = (" << d1x << ", " << d2y << ")" << " = " << d2 << std::endl;
  std::cout << "d3 = (" << d2x << ", " << d1y << ")" << " = " << d3 << std::endl;
  std::cout << "d4 = (" << d2x << ", " << d2y << ")" << " = " << d4 << std::endl;
  std::cout << verCount << " " << horCount << " " << diagCount << std::endl;
  
  int count = verCount + horCount + diagCount; 
  return count;
}
int updateLiveCount(int *a, int n,int *livecount, int iter, int debug_iteration, int livecount_iter) {
#if DEBUG == 1
  livecount[livecount_iter] = countlive(a,n);
  livecount_iter++; 
  printLiveCount(livecount, 10); 
#endif
  return livecount_iter;
}
void life(int *a, unsigned int n, unsigned int iter, int *livecount) {
  std::ofstream results; 
  results.open("results"); 
  int debug_iteration = iter/10;
  int livecount_iter = 0;
  for(int it = 0; it < iter; it++) {
    
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
	int cell_state = a[i*n + j];
	isOccupied(i,j,cell_state);
	int neighbors = numNeighbors(i, j, n, a); 
	if(neighbors == 3) {
	  setCellValue(i,j,n,a,1);
	}
	else if(cell_state == 1 && neighbors == 2) {
	  setCellValue(i,j,n,a,1);
	}
	else if(cell_state == 1 && neighbors < 2) {
	  setCellValue(i,j,n,a,0);
	  std::cout << "Not enough neighbors.This square is now empty" << std::endl;
	}
	else if(cell_state == 1 && neighbors > 3) {
	  setCellValue(i,j,n,a,0);
	  std::cout << "Overcrowding: square is now empty" << std::endl;
	}
      }
    }
    if(it % debug_iteration == 0) 
      livecount_iter = updateLiveCount(a,n,livecount,it, debug_iteration, livecount_iter); 
  }
  for(int i = 0; i < n; i++) {
    results << livecount[i] << " "; 
  }
}
