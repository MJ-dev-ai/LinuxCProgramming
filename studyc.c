#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void swap_ptr(char** ppa, char** ppb);
void swap(double* a, double* b);
void line_up(double*, double*, double*);
void print_array(int* pa, int size);
void print_str(char** ary, int size);

int main(void) {


    return 0;
}

void swap_ptr(char** ppa, char** ppb) {
    char* temp_pt = *ppa;
    *ppa = *ppb;
    *ppb = temp_pt;
}

void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}
void line_up(double* maxp, double* midp, double* minp) {
    if (*minp > *midp) swap(minp, midp);
    if (*minp > *maxp) swap(minp, maxp);
    if (*midp > *maxp) swap(midp, maxp);
}
void print_array(int* arr, int size) {
    printf("[");
    for (int i = 0; i < size - 1; i++) {
        printf("%d ", arr[i]);
    }
    printf("%d]\n", arr[size - 1]);
}

void print_str(char** ary, int size) {
    printf("[");
    for (int i = 0; i < size - 1; i++) {
        printf("\"%s\", ", ary[i]);
    }
    printf("\"%s\"]\n", ary[size - 1]);
}