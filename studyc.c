#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void swap_ptr(char** ppa, char** ppb);
void swap(double* a, double* b);
void line_up(double*, double*, double*);
void print_array(int* pa, int size);
void print_str(char** ary, int size);
void func(int (*fp)(int, int));
int sum(int a, int b) { return (a + b); }
int max(int a, int b) { return (a > b) ? a : b; }
int mul(int a, int b) { return (a * b); }

int main(void) {
    int select;
    int (*fp[4])(int, int) = {NULL, sum, mul, max};
    printf("1 두 정수의 합\n2 두 정수의 곱\n3 두 정수중 큰 값 계산\n");
    printf("원하는 연산을 선택하세요: ");
    scanf("%d", &select);
    // switch (select) {
    //     case 1: func(sum); break;
    //     case 2: func(mul); break;
    //     case 3: func(max); break;
    // }
    if (select > 0 && select < 4) func(fp[select]);

    return 0;
}

void func(int (*fp)(int, int)) {
    int a, b;
    printf("두 정수를 입력하세요: ");
    scanf("%d%d", &a, &b);
    int res = fp(a, b);
    printf("결과값: %d\n", res);
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