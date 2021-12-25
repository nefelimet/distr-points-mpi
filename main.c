#include <stdio.h>
#include <stdlib.h>
#include "read_bin.h"
#include "mpi.h"
#include "mpi_distr.h"
#include "utils.h"

int main(int argc, char** argv){
    
    //p, N and d are taken as command arguments. They can be changed in Makefile
    int p = atoi(argv[1]); //p = NPROC = 4
    int N = atoi(argv[2]); //N = 16
    int d = atoi(argv[3]); //d = 3
    
    int a = N / p;
    
    double ***X; 
    X = (double ***)malloc(d * sizeof(double**));
    for (int i = 0; i < d; i++){
        X[i] = (double **)malloc(a * sizeof(double*));
        for(int j = 0; j < a; j++){
            X[i][j] = (double *)malloc(p * sizeof(double));
        }
    }
    
    fill_3D(X, d, a, p);
    //print_3D(X, d, a, p);
    
    
    //------Test createPoint and printPoint------
    /*
    double *test1 = (double *)malloc(d * sizeof(double));
    createPoint(test1, d);
    printf("point test1: ");
    printPoint(test1, d);
    printf("\n");
    free(test1);
    */
    
    //------Test get_1D_col------
    /*
    double *test2 = (double *)malloc(d * sizeof(double));
    get_1D_col(X, d, a, p, 0, 0, test2);
    printf("point test2: ");
    printPoint(test2, d);
    printf("\n");
    free(test2);
    */
    
    //------Test get_2D_arr------
    /*
    double** test3 = (double **)malloc(d * sizeof(double*));
    for (int i = 0; i < d; i++){
        test3[i] = (double *)malloc(a * sizeof(double));
    }
    get_2D_arr(X, d, a, p, 0, test3);
    printf("array test3: ");
    print_2D(test3, d, a);
    printf("\n");
    for(int i = 0; i < d; i++){
        free(test3[i]);
    }
    free(test3);
    */
    
    //------Test quickselect and partition------
    /*
    double *test4 = createPoint(d);
    double test4_qs = quickselect(test4, 0, d-1, d/2);
    printf("test4_qs: %f", test4_qs);
    free(test4);
    */
    
    //------Test swap_cols------
    /*
    printf("X before swap:\n");
    print_3D(X, d, a, p);
    
    swap_cols(X, d, a, p, 0, 0, 1, 1);
    printf("X after swap:\n");
    print_3D(X, d, a, p);
    */
    
    double *arr = (double *)malloc(a * p * sizeof(double));
    for (int i = 0; i < a*p; i++){
        arr[i] = (double)i;
    }
    int pid, numTasks;
    MPI_Status mpistat;
    MPI_Request mpireq;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    
    
    //printf("arr: ");
    //printPoint(arr, a*p);
    //printf("\n");
    
    distributeByMedian(pid, numTasks, mpistat, mpireq, 0, p-1, X, N, p, d, arr);
    
    MPI_Finalize();    
    

    //Free X
    for (int i = 0; i < d; i++){
        for(int j = 0; j < a; j++){
            free(X[i][j]);
        }
        free(X[i]);
    }
    free(X);
    
    return 0;
}
