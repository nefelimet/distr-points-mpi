#ifndef MPI_DISTR_H
#define MPI_DISTR_H

void distributeByMedian(int pid, int numTasks, MPI_Status mpistat, MPI_Request mpireq, int p_left, int p_right, double ***X, int N, int p, int d, double *arr);
void distributeAll(double ***X, int N, int p, int d, int p_left, int p_right);

#endif
