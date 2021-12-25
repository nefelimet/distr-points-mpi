#ifndef UTILS_H
#define UTILS_H

void printPoint(double *a, int d);
void createPoint(double *a, int d);
double calculateDistance(double *a, double *b, int d);
void swap(double *a, double *b);
int partition(double *a, int left, int right, int pIndex);
double quickselect(double *nums, int left, int right, int k, int *index);
void print_2D(double **X, int x, int y);
void print_3D(double ***X, int x, int y, int z);
void get_1D_col(double ***X, int x, int y, int z, int point, int proc, double* result);
void get_2D_arr(double ***X, int x, int y, int z, int proc, double **result);
void fill_3D(double ***X, int x, int y, int z);
void set_1D_col(double ***X, int x, int y, int z, int point, int pid, double *col);
void swap_cols(double ***X, int x, int y, int z, int point1, int pid1, int point2,  int pid2);

#endif
