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
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>

void printMatrix(int *a, unsigned int n) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      std::cout << a[i*n + j] << " ";  
    }
    std::cout << std::endl;
  }
}
void printLivecount(int *livecount) {
  for(int i = 0; i < 10; i++) {
    std::cout << livecount[i] << " "; 
  }
  std::cout << std::endl;
}
void genlife(int *a, unsigned int n)
{
  //std::ofstream resultFile;
  //resultFile.open("generatedInput");
  int currentIndex = 0;
  srand(time(NULL));
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      a[currentIndex] = rand() % 2;  
      //resultFile << a[currentIndex] << " "; 
      currentIndex++; 
    }
    //resultFile << "\n";
  }
  //printMatrix(a,n); 
  //resultFile.close(); 
}

void readlife(int *a, unsigned int n, char *filename) {
  FILE * fp; 
  fp = fopen(filename, "r"); 
  if(fp == NULL) {
    fprintf(stderr, "error: cannot open file");
    exit(0); 
  }
  int currentIndex = 0;
  cilk_for(int i = 0; i < n	; i++) {
    for(int j = 0; j < n; j++) {
      int result = fscanf(fp, "%d", &a[currentIndex] );
      currentIndex++; 
    }
  }
}


void life(int *a, unsigned int n, unsigned int iter, int *livecount) {
  
  // for result file
  // std::ofstream results; 
  // results.open("results"); 
  int counter = 0; 
  int* b = (int *)malloc(sizeof(int)*n*n); 
  
  for(int it = 0; it < iter; it++) {
    cilk_for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
	int index = i*n + j;
	
	int dx1 = ((i - 1 + n) % n) * n;
	int dx2 = ((i + 1 + n) % n) * n;
	int dy1 = (j - 1 + n) % n;
	int dy2 = (j + 1 + n) % n;
	
	int nw = dx1 + dy1;
	int north = dx1 + j;
	int ne = dx1 + dy2;
	int west = i*n + dy1;
	int east = i*n + dy2;
	int sw = dx2 + dy1;
	int south = dx2 + j;
	int se = dx2 + dy2;
	
	if(a[nw] == 1) b[index]++; 
	if(a[north] == 1) b[index]++; 
	if(a[ne] == 1) b[index]++; 
	if(a[west] == 1) b[index]++; 
	if(a[east] == 1) b[index]++; 
	if(a[sw] == 1) b[index]++; 
	if(a[south] == 1) b[index]++; 
	if(a[se] == 1) b[index]++;
      }
    }
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
	int index = i*n + j; 
	if(a[index] == 1) { 
	  // If an occupied square has fewer than 2 occupied neighbors, it dies
	  if(b[index] < 2) 
	    a[index] = 0; 
	  // If an occupied square has more than three occupied neighbors, it dies
	  if(b[index] > 3) 
	    a[index] = 0; 
	  // If an occupied square has exactly two or three neighbors, it stays
	  if(b[index] == 2 || b[index] == 3)
	    a[index] = 1; 
	}
	else 
	  // If an empty square has exactly three occupied neighbors, it is born
	  if(b[index] == 3) 
	    a[index] = 1; 
	
	b[index] = 0; 
      }
    }
    
#if DEBUG == 1
    if((it+1)%(iter/10) == 0) { 
      livecount[counter] = countlive(a,n);
      counter++; 
    }
#endif    
  }
  free(b); 
  
  // Write to file
  //for(int i = 0; i < 10; i++) 
  // results << livecount[i] << " "; 
  
}

