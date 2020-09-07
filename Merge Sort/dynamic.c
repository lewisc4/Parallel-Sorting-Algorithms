#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "mpi.h"


void mergeSort(int *arr, int *temp, int length);
void merge(int *arr, int *temp, int length);

int main(int argc, char **argv) {

  MPI_Init(&argc, &argv);
  int rank, numranks;
  MPI_Comm_size(MPI_COMM_WORLD, &numranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // --- Merge Sort Alogrithm --- //

  
  // --- This is main, where array will be initialized and initial Sort() call will be made --- //

  double startFull = omp_get_wtime();  
  
  // Size of array
  int fullSize = 20000000;
  int *arr, *temp, *sendCount, *displ, *myArr, *myTemp;
  int mySize = fullSize/numranks;
  int remainder = fullSize%numranks;
  

  if (rank == numranks-1) {
    mySize += remainder;
  }

  if (rank == 0) {
    temp = (int*) malloc(fullSize*sizeof(int));    
    arr = (int*) malloc(fullSize*sizeof(int));

    sendCount = (int*) malloc(numranks*sizeof(int));
    displ = (int*) malloc(numranks*sizeof(int));
    
    for (int i=0; i<fullSize; i++) {
      arr[i] = rand() % 50 + 1;
    }
 
    /*for (int i=0; i<fullSize; i++) {
      printf("Before sort: arr[%d] = %d\n", i, arr[i]);
    }*/

    int sum = 0;
    int shift = mySize;

    for (int i=0; i<numranks; i++) {

      if (i == numranks-1) {
	shift += remainder;
      }

      // Number of elements to send each arrau
      sendCount[i] = shift;

      // Offset for scatter for each rank
      displ[i] = sum;

      // Increment offset value
      sum += sendCount[i];
    }

  }

  myTemp = (int*) malloc(mySize*sizeof(int));
  myArr = (int*) malloc(mySize*sizeof(int));

  
  MPI_Scatterv(arr, sendCount, displ, MPI_INT, myArr, mySize, MPI_INT, 0, MPI_COMM_WORLD);

  mergeSort(myArr, myTemp, mySize); 

  MPI_Gatherv(myArr, mySize, MPI_INT, arr, sendCount, displ, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {

    mergeSort(arr, temp, fullSize); 
    
    /*printf("\n\n");
    for (int i=0; i<fullSize; i++) {      
      printf("After sort: arr[%d] = %d\n", i, arr[i]);
      if ((i+1)%(fullSize) == 0) {
	printf("\n");
      }
    }*/
  
    double endFull = omp_get_wtime();
  
    printf("Total time: %f\n", endFull-startFull);
  }

  MPI_Finalize();
  
  return 0;
}

void mergeSort(int *arr, int *temp, int currLength) {

  // There needs to be at least 2 elements to perform this algorithm
  if (currLength >= 2) {

    int leftHalf = currLength/2;
    int rightHalf = currLength - leftHalf;
    
    // Set number of threads
    omp_set_num_threads(12);

#pragma omp parallel sections
    {
#pragma omp section
      {
	// Sort the first half of the array
	mergeSort(arr, temp, leftHalf);	
      }

#pragma omp section
      {
	// Sort second half
	mergeSort(arr+(currLength/2), temp, rightHalf);
      }
    }
    
    // Merge two halves
    merge(arr, temp, currLength);
  }
}

void merge(int *arr, int *temp, int currLength) {

  // Index values for temp arrays and final array
  int iLeft = 0;
  int iRight = currLength/2;
  int iTotal = 0;
  
  
  while (iLeft < currLength/2 && iRight < currLength) {

    if (arr[iLeft] < arr[iRight]) {

      temp[iTotal] = arr[iLeft];
      iTotal++;
      iLeft++;
    } else {

      temp[iTotal] = arr[iRight];
      iTotal++;
      iRight++;
    }
  }

  while (iLeft < currLength/2) {

    temp[iTotal] = arr[iLeft];
    iTotal++;
    iLeft++;
  }

  while (iRight<currLength) {

    temp[iTotal] = arr[iRight];
    iTotal++;
    iRight++;
  }

  for (int i=0; i<currLength; i++) {
    arr[i] = temp[i];
  }
		      
}
