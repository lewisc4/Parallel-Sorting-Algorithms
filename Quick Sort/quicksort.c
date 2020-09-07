#include <stdio.h>
#include <stdlib.h>
#include <omp.h>



/* ----- Quick Sort ----- */

//function to quicksort an array
void quicksort(int *arr, int l, int r);

//function to partition an array
int partition(int *arr, int l, int r);

int main(int argc, char **argv) {
    double start = omp_get_wtime();
    int n = 1000000;                                                    //size of array to be sorted
    int *arr = (int*)malloc(n * sizeof(int));                           //array to be sorted
    int l, r;                                                           //low and high limits

    omp_set_num_threads(12);
    
    //giving each element in the array a random value
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000000 + 1;
    }

    l = 0;                                                              //beginning of array
    r = n - 1;                                                          //end of array

    //calling quicksort
    quicksort(arr, l, r);

    //error checking
    for (int i = 0; i < n; i++) {
        //if (arr[i - 1] > arr[i]) {
            //printf("Messed up\t");
        //}
        //printf("%d\t", arr[i]);
    }

    double end = omp_get_wtime();

    printf("Time: %f\t\n", end - start);
}

//function to quicksort elements in array
void quicksort(int *arr, int l, int r) {

    //if it hasn't gone through all elements
    if (l < r) {
        int d = partition(arr, l, r);                                   //calling partition function on arr from arr[l] to arr[r]

        //start omp parallel
        //printf("num threads: %d", omp_get_num_threads());
        #pragma omp parallel sections 
        {
            //omp section for quick sort on left of partition point
            #pragma omp section
                quicksort(arr, l, d - 1);

            //omp section for quick sort on right of partition point
            #pragma omp section
                quicksort(arr, d + 1, r);

        }

    }
}

//function to partition elements in array
//returns point for use in next set of partitions
int partition(int *arr, int l, int r) {
    int p;                                                              //pivot variable
    int temp;                                                           //temp variable for swapping values
    int i;                                                              //counter to see how many variables are already sorted beginning at the left side
    int j = r;                                                              //counter to see how many variables are already sorted beginning at the right side

    //if there are still elements to sort
    if (l < r) {
        p = l;                                                          //p starts at left side
        i = l;                                                          //i starts at left side

        //while i is still less than j
        while (i < j) {
            //incrementing i until element is found
            while (arr[i] <= arr[p] && i < r) {
                i++;
            }
            //decrementing j until element is found
            while (arr[j] > arr[p]) {
                j--;
            }
            //switch arr[i] and arr[j] if i is still less than j
            if (i < j) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        //switching pivot element with high element
        temp = arr[p];
        arr[p] = arr[j];
        arr[j] = temp;

    }
    //return new point for next set of partitions
    return j;
}

