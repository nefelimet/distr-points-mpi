#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "utils.h"

//Prints a point (vector) a of size d
void printPoint(double *a, int d){
    printf("( ");
    for (int i = 0; i < d; i++){
        printf("%.2f ", a[i]);
    }
    printf(")");
}

//Fills a point (vector) a of dize d with random elements
void createPoint(double *a, int d){
    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < d; i++){
        a[i] = rand() % 50;
        double div = RAND_MAX / 50;
        a[i] = (double)rand() / div;
    }
}

//Takes two points (vectors) of size d and calculates their distance
double calculateDistance(double *a, double *b, int d){
    double sum = 0;
    for (int i = 0; i < d; i++){
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    double distance = sqrt(sum);
    return distance;
}

//Swaps two numbers in memory
void swap(double *a, double *b){
    double temp = *a;
    *a = *b;
    *b = temp;
}

//Partition function used in quickselect
int partition(double *a, int left, int right, int pIndex){
    //Pick pIndex as a pivot from the array
    double pivot = a[pIndex];
 
    //Move pivot to end
    swap(&a[pIndex], &a[right]);
 
    //Elements less than the pivot will be pushed to the left of pIndex;
    //Elements more than the pivot will be pushed to the right of pIndex;
    //Equal elements can go either way
    pIndex = left;
 
    //Each time we find an element less than or equal to the pivot, pIndex
    //is incremented, and that element would be placed before the pivot.
    for (int i = left; i < right; i++){
        if (a[i] <= pivot){
            swap(&a[i], &a[pIndex]);
            pIndex++;
        }
    }
 
    //Move pivot to its final place
    swap(&a[pIndex], &a[right]);
 
    //Return pIndex (index of the pivot element)
    return pIndex;
}
 
//Returns the k-th smallest element in the list within left…right
//Also modifies index variable so that it stores the index where k-th smallest is found
double quickselect(double *nums, int left, int right, int k, int* index){
    // If the array contains only one element, return that element
    if (left == right) {
        *index = left - 1;
        return nums[left];
    }
 
    //Select pIndex between left and right
    int pIndex = left + rand() % (right - left + 1);
 
    pIndex = partition(nums, left, right, pIndex);
 
    //The pivot is in its final sorted position
    if (k == pIndex) {
        *index = k - 1;
        return nums[k];
    }
 
    //If k is less than the pivot index
    else if (k < pIndex) {
        return quickselect(nums, left, pIndex - 1, k, index);
    }
 
    //If k is more than the pivot index
    else {
        return quickselect(nums, pIndex + 1, right, k, index);
    }
}

//Prints a 2D array of size x X y
void print_2D(double **X, int x, int y){
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            printf("%.2f  ", X[i][j]);
        }
        printf("\n");
    }
}

//Prints a 3D array, by printing z 2D arrays
void print_3D(double ***X, int x, int y, int z){
    for (int k = 0; k < z; k++){
        //Since we can't print 3D arrays, we print all 2D arrays (there are z of them)
        double **arr = (double**)malloc(x * sizeof(double*));
        for (int i = 0; i < x; i++){
            arr[i] = (double*)malloc(y * sizeof(double));
        }
        get_2D_arr(X, x, y, z, k, arr);
        print_2D(arr, x, y);
        printf("\n\n");
        
        //Free memory
        for (int i = 0; i < x; i++){
            free(arr[i]);
        }
        free(arr);
    }
}

//Returns a 1D column (or a point) from a 3D array. Writes result in the result vector
void get_1D_col(double ***X, int x, int y, int z, int point, int proc, double* result){
    for (int i = 0; i < x; i++){
        result[i] = X[i][point][proc];
    }
}

//Returns a 2D array from a 3D array. Writes result in the result array
void get_2D_arr(double ***X, int x, int y, int z, int proc, double **result){
    for (int i = 0; i < x; i++){
        for (int j = 0; j < y; j++){
            result[i][j] = X[i][j][proc];
        }
    }
}

//Fills a 3D array with random elements
void fill_3D(double ***X, int x, int y, int z){
    for(int i = 0; i < x; i++){
        for (int j = 0; j < y; j++){
            for (int k = 0; k < z; k++){
                double div = RAND_MAX / 50;
                X[i][j][k] = (double)rand() / div;
            }
        }
    }
}

//Sets a 1D column (or point) in a 3D array
void set_1D_col(double ***X, int x, int y, int z, int point, int pid, double *col){
        for (int i = 0; i < x; i++){
            X[i][point][pid] = col[i];
        }
}

//Swaps two 1D columns in a 3D array
void swap_cols(double ***X, int x, int y, int z, int point1, int pid1, int point2,  int pid2){
    double *col1 = (double *)malloc(x * sizeof(double));
    double *col2 = (double *)malloc(x * sizeof(double));
    double *temp = (double *)malloc(x * sizeof(double));
    get_1D_col(X, x, y, z, point1, pid1, col1);
    get_1D_col(X, x, y, z, point2, pid2, col2);
    
    //temp gets col1's contents
    for (int i = 0; i < x; i++){
        temp[i] = col1[i];
    } 
    //col1 gets col2's contents
    set_1D_col(X, x, y, z, point1, pid1, col2);
    //col2 gets temp's contents
    set_1D_col(X, x, y, z, point2, pid2, temp);
    
    free(col1);
    free(col2);
    free(temp);
}
