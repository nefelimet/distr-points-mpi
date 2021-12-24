#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "utils.h"

void printPoint(double *a, int d){
    printf("( ");
    for (int i = 0; i < d; i++){
        printf("%.2f ", a[i]);
    }
    printf(")");
}

void createPoint(double *a, int d){
    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < d; i++){
        a[i] = rand() % 50;
        double div = RAND_MAX / 50;
        a[i] = (double)rand() / div;
    }
}

double calculateDistance(double *a, double *b, int d){
    double sum = 0;
    for (int i = 0; i < d; i++){
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    double distance = sqrt(sum);
    return distance;
}

void swap(double *a, double *b){
    double temp = *a;
    *a = *b;
    *b = temp;
}

// Partition using Lomuto partition scheme
int partition(double *a, int left, int right, int pIndex){
    // pick `pIndex` as a pivot from the array
    double pivot = a[pIndex];
 
    // Move pivot to end
    swap(&a[pIndex], &a[right]);
 
    // elements less than the pivot will be pushed to the left of `pIndex`;
    // elements more than the pivot will be pushed to the right of `pIndex`;
    // equal elements can go either way
    pIndex = left;
 
    // each time we find an element less than or equal to the pivot, `pIndex`
    // is incremented, and that element would be placed before the pivot.
    for (int i = left; i < right; i++){
        if (a[i] <= pivot){
            swap(&a[i], &a[pIndex]);
            pIndex++;
        }
    }
 
    // move pivot to its final place
    swap(&a[pIndex], &a[right]);
 
    // return pIndex (index of the pivot element)
    return pIndex;
}
 
// Returns the k'th smallest element in the list within `left…right`
// (i.e., left <= k <= right). The search space within the array is
// changing for each round – but the list is still the same size.
// Thus, `k` does not need to be updated with each round.
double quickselect(double *nums, int left, int right, int k){
    // If the array contains only one element, return that element
    if (left == right) {
        return nums[left];
    }
 
    // select `pIndex` between left and right
    int pIndex = left + rand() % (right - left + 1);
 
    pIndex = partition(nums, left, right, pIndex);
 
    // The pivot is in its final sorted position
    if (k == pIndex) {
        return nums[k];
    }
 
    // if `k` is less than the pivot index
    else if (k < pIndex) {
        return quickselect(nums, left, pIndex - 1, k);
    }
 
    // if `k` is more than the pivot index
    else {
        return quickselect(nums, pIndex + 1, right, k);
    }
}

void print_2D(double **X, int x, int y){
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            printf("%.2f  ", X[i][j]);
        }
        printf("\n");
    }
}

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

void get_1D_col(double ***X, int x, int y, int z, int point, int proc, double* result){
    for (int i = 0; i < x; i++){
        result[i] = X[i][point][proc];
    }
}

void get_2D_arr(double ***X, int x, int y, int z, int proc, double **result){
    for (int i = 0; i < x; i++){
        for (int j = 0; j < y; j++){
            result[i][j] = X[i][j][proc];
        }
    }
}

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

void set_1D_col(double ***X, int x, int y, int z, int point, int pid, double *col){
        for (int i = 0; i < x; i++){
            X[i][point][pid] = col[i];
        }
}

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
