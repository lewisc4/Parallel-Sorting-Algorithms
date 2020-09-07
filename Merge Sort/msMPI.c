#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

void mergeSort(int *arr, int b, int e);
void merge(int *arr, int b, int m, int e);

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int rank, numranks;
  MPI_Comm_size(MPI_COMM_WORLD, &numranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  double startTime = MPI_Wtime();  

  // Size of array
  int fullSize = 50000000;
  int *arr, *sendCount, *displ, *myArr, *sorted;

  int mySize = fullSize/numranks;
  int remainder = fullSize%numranks;

  if (rank == numranks-1) {
    mySize += remainder;
  }
  
  if (rank == 0) {    
    // Allocate/initialize arrays
    arr = (int*) malloc(fullSize*sizeof(int));
    //    sorted = (int*) malloc(fullSize*sizeof(int));

    sendCount = (int*) malloc(numranks*sizeof(int));
    displ = (int*) malloc(numranks*sizeof(int));
    
    for (int i=0; i<fullSize; i++) {
      arr[i] = rand() % 50 + 1;
    }
    
    //Print master array before sort
    /*for (int i=0; i<fullSize; i++) {
      printf("Before sort: arr[%d] = %d\n", i, arr[i]);
    }*/
    
    int sum = 0;
    
    for (int i=0; i<numranks; i++) {
      // Number of elements to send each arrau
      sendCount[i] = mySize;

      // Offset for scatter for each rank
      displ[i] = sum;

      // Increment offset value
      sum += sendCount[i];
    }
  }

  myArr = (int*) malloc(mySize*sizeof(int));
  
  // Scatter master array to each rank based
  MPI_Scatterv(arr, sendCount, displ,
	       MPI_INT, myArr, mySize,
	       MPI_INT, 0, MPI_COMM_WORLD);
  
  int b = 0; // Beginning
  int e = fullSize-1; // End
  int mye = mySize-1;
  
  mergeSort(myArr, b, mye);

  if (rank == 0) {
    memset(arr, 0, sizeof(arr));
  }
  
  MPI_Gatherv(myArr, mySize, MPI_INT,
	      arr, sendCount, displ,
	      MPI_INT, 0, MPI_COMM_WORLD);
   
  if (rank == 0) {

    mergeSort(arr, b, e);

    double endTime = MPI_Wtime();
    
    /*printf("\n\n");
    
    for (int i=0; i<fullSize; i++) {      
      printf("After sort: arr[%d] = %d\n", i, arr[i]);

      if ((i+1)%(mySize) == 0) {
	printf("\n");
      }
      }*/

    printf("Full time: %f\n", endTime-startTime);
    
    //  free(arr);  
    }
   MPI_Finalize();
  return 0;
}

void mergeSort(int *myArr, int b, int e) {

  // If the beginning is less than the first, there is more to sort
  if (b < e) {

    // Get middle value
    int m = b+(e-b)/2;
    // Last value in first half is the middle
    int bm = m;
    // First value in second half is middle+1
    int em = bm+1;

    //#pragma omp parallel sections
    //{
      //#pragma omp section
      //{
	// Sort first half
    mergeSort(myArr, b, bm);
	//}

      //#pragma omp section
      //{
	// Sort second half
    mergeSort(myArr, em, e);
	//}
      // }
      
    // Merge two halves
    merge(myArr, b, m, e);
  }
}


void merge(int *myArr, int b, int m, int e) {

  // Number of elements in the beginning half
  int bNumele = m-b+1;
  // Number of elements in ending half
  int eNumele = e-m;

  int ib, ie, iFinal;
  
  // Temp array for beginning half
  int *bTemp = (int*) malloc(bNumele*sizeof(int));
  // Temp array for ending half
  int *eTemp = (int*) malloc(eNumele*sizeof(int));

  // Assign temp array value from master array for first half
  for (ib=0; ib<bNumele; ib++) {
    bTemp[ib] = myArr[b+ib];
  }
  
  // Assign temp array value from master array for second half
  for(ie=0; ie<eNumele; ie++) {
    eTemp[ie] = myArr[m+1+ie];
  }

  // Index values for temp arrays and final array
  ib = 0; ie = 0; iFinal = b; 

  // Compare values and assign to final array
  while (ib < bNumele && ie < eNumele) {

    // Whichever half has the smaller value, assign to final array
    if (bTemp[ib] <= eTemp[ie]) {
      
      myArr[iFinal] = bTemp[ib];     
      ib++;
    } else {
      
      myArr[iFinal] = eTemp[ie];
      ie++;
    }
    iFinal++;
  }

  // Deal with potential leftover values in each array
  while (ib < bNumele) {
    myArr[iFinal] = bTemp[ib];
    ib++;
    iFinal++;
  }
  while (ie < eNumele) {
    myArr[iFinal] = eTemp[ie];    
    ie++;
    iFinal++;
  } 
}

