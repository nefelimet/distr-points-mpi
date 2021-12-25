#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "mpi.h"
#include "mpi_distr.h"

void distributeByMedian(int pid, int numTasks, MPI_Status mpistat, MPI_Request mpireq, int p_left, int p_right, double ***X, int N, int p, int d, double *arr){
    
    int a = N / p;
    int nProc = p_right - p_left + 1;
    int head = p_left;
    
    //Pivot is declared here, so that all processes allocate memory for it.
    double *pivot = (double *)malloc(d * sizeof(double));    
  
    //For head process (head process is always the first one)
    if (pid == head){
        //This print statement is put here because in main it was run many times
        printf("X array:\n");
        print_3D(X, d, a, p);
        
        //Calculate pivot = first point of head process
        get_1D_col(X, d, a, p, 0, head, pivot);
        //printf("pid %d calculated pivot : ", pid);
        //printPoint(pivot, d);
        //printf("\n");
        
        //Send pivot to all processes
        for (int i = p_left + 1; i <= p_right; i++){
            MPI_Send(pivot, d, MPI_DOUBLE, i, 55, MPI_COMM_WORLD);
            //printf("pid %d sent pivot: ", pid);
            //printPoint(pivot, d);
            //printf(" to pid %d.\n", i);
        }
    }
    
    //For all other processes
    else if (pid > p_left && pid <= p_right){
        //Receive pivot
        MPI_Recv(pivot, d, MPI_DOUBLE, head, 55, MPI_COMM_WORLD, &mpistat);
        //printf("pid %d received pivot: ", pid);
        //printPoint(pivot, d);
        //printf("\n");
    }
    
    
    
    double *T_1D = (double *)malloc(a * nProc * sizeof(double));
    double *t = (double *)malloc(a * sizeof(double));
    
    if (pid == head){        
        //Fill first slots in T_1D with head's distances from pivot
        double *t0 = (double *)malloc(a * sizeof(double));
        //Calculate distances
        for (int point = 0; point < a; point++){
            
            double *col = (double *)malloc(d * sizeof(double));
            get_1D_col(X, d, a, p, point, pid, col);
                
            double dist = calculateDistance(pivot, col, d);
            t[point] = dist;
            free(col);
        }
        //Store distances in the first slot of T_1D
        for (int i = 0; i < a; i++){
            T_1D[a * (pid-p_left) + i] = t[i];
        }
        
    }
    
    else if (pid > p_left && pid <= p_right){
        //For every point that a process has, calculate its distance from pivot
        for (int point = 0; point < a; point++){
            
            //Calculate distances from pivot
            double *col = (double *)malloc(d * sizeof(double));
            get_1D_col(X, d, a, p, point, pid, col);
                
            double dist = calculateDistance(pivot, col, d);
            t[point] = dist;
            free(col);
        }
        //printf("pid %d has t array: ", pid);
        //printPoint(t, a);
        //printf("\n");
        MPI_Send(t, a, MPI_DOUBLE, head, 65, MPI_COMM_WORLD);
    }
    if (pid == head){
        for (int pr = p_left + 1; pr <= p_right; pr++){
            MPI_Recv(t, a, MPI_DOUBLE, pr, 65, MPI_COMM_WORLD, &mpistat);
            for (int i = 0; i < a; i++){
                T_1D[a*(pr-p_left) + i] = t[i];
            }
        }
        printf("T_1D array:\n");
        printPoint(T_1D, a*nProc);
        printf("\n\n");
    }    

    //Calculate median distance and its index and send to all processes
    double medDist;
    int medDistIndex;
    
    if (pid == head){
        medDist = quickselect(T_1D, 0, a*nProc-1, a*nProc/2, &medDistIndex);
        //printf("medDist calculated: %.2f at index %d in process %d\n", medDist, medDistIndex, medDistIndex/a);
        for (int t = p_left + 1; t <= p_right; t++){
            MPI_Send(&medDist, 1, MPI_DOUBLE, t, 45, MPI_COMM_WORLD);
            //printf("pid %d sent distance %.2f to pid %d\n", pid, medDist, t);
        }
        
        //Send medDistIndex to all processes
        for (int t = p_left + 1; t <= p_right; t++){
            MPI_Send(&medDistIndex, 1, MPI_DOUBLE, t, 46, MPI_COMM_WORLD);
            //printf("pid %d sent medDist index %d to pid %d\n", pid, medDistIndex, t);
        }
    }
    else if (pid > p_left && pid <= p_right){
        MPI_Recv(&medDist, 1, MPI_DOUBLE, head, 45, MPI_COMM_WORLD, &mpistat);
        //printf("pid %d received distance %.2f\n", pid, medDist);
        MPI_Recv(&medDistIndex, 1, MPI_DOUBLE, head, 46, MPI_COMM_WORLD, &mpistat);
        //printf("pid %d received distance index %d\n", pid, medDistIndex);
    }
    
    
    //Find in which process medDist belongs
    int medDistProc = medDistIndex / a;
    
    //Swap medDistIndex with a*nProc/2 (so that medDist gets put in the middle)
    double send_buf, recv_buf;
    
    if (medDistProc != nProc/2){
        if(pid == medDistProc){
            send_buf = arr[medDistIndex];
            arr[a * nProc / 2] = arr[medDistIndex];
            MPI_Send(&send_buf, 1, MPI_DOUBLE, nProc/2, 75, MPI_COMM_WORLD);
            MPI_Recv(&recv_buf, 1, MPI_DOUBLE, nProc/2, 85, MPI_COMM_WORLD, &mpistat);
            arr[medDistIndex] = recv_buf;
            printf("pid %d swapped %d with %d\n", pid, medDistIndex, a * nProc/2);
        }
        else if (pid == nProc/2){
            arr[medDistIndex] = arr[a * nProc / 2];
            MPI_Recv(&send_buf, 1, MPI_DOUBLE, medDistProc, 75, MPI_COMM_WORLD, &mpistat);
            recv_buf = arr[a * nProc / 2];
            arr[a * nProc / 2] = send_buf;
            MPI_Send(&recv_buf, 1, MPI_DOUBLE, medDistProc, 85, MPI_COMM_WORLD);
            printf("pid %d swapped %d with %d\n", pid, medDistIndex, a * nProc/2);
        }
    }
    
    
    
    printf("pid %d arr after swap: ", pid);
    printPoint(arr, a*nProc);
    printf("\n");
    
    
    free(T_1D);
    
}

void distributeAll(double ***X, int N, int p, int d){
}
