#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 10

double* adc_to_voltage(int* adc, int N, double Vref);
double* voltage_to_distance(double* voltage, int N, double k);
void print_min_max(double* distance, int N);
int* voltage_to_adc(double* voltage, int N, double Vref);

int main(void) {
    int N = 5;
    while (1) {
        printf("N = ");
        scanf("%d", &N);
        if (N > MAX_N) {
            printf("N값은 %d보다 작아야 합니다.\n", MAX_N);
        }
        else break;
    }
    int adc[N];
    srand(time(NULL));
    for (int i = 0; i < N; i++) adc[i] = rand() % 1024;
    double Vref = 5.0;
    double k = 27.0;
    
    double* voltage = adc_to_voltage(adc, N, Vref);
    double* distance = voltage_to_distance(voltage, N, k);
    int* adc_restore = voltage_to_adc(voltage, N, Vref);

    printf("ADC\t\t→ ");
    for (int i = 0; i < N; i++) {
        printf("%d\t", adc[i]);
    }
    printf("\n");
    printf("전압(V)\t\t→ ");
    for (int i = 0; i < N; i++) {
        printf("%.2f\t", voltage[i]);
    }
    printf("\n");
    printf("거리(cm)\t→ ");
    for (int i = 0; i < N; i++) {
        printf("%.2f\t", distance[i]);
    }
    printf("\n");
    printf("ADC2\t\t→ ");
    for (int i = 0; i < N; i++) {
        printf("%d\t", adc_restore[i]);
    }
    printf("\n");
    print_min_max(distance, N);
    free(voltage);
    free(distance);
    free(adc_restore);
    return 0;
}

double* adc_to_voltage(int* adc, int N, double Vref) {
    double* voltage = (double*)malloc(N * sizeof(double));
    for (int i = 0; i < N; i++) {
        double v = Vref * (double)adc[i] / 1023.0;
        if (v < 0.01) v = 0.01;
        voltage[i] = v;
    }
    return voltage;
}

double* voltage_to_distance(double* voltage, int N, double k) {
    double* distance = (double*)malloc(N * sizeof(double));
    for (int i = 0; i < N; i++) {
        distance[i] = k / voltage[i];
    }
    return distance;
}

void print_min_max(double* distance, int N) {
    double min = distance[0], max = distance[0];
    for (int i = 0; i < N; i++) {
        if (distance[i] < min) min = distance[i];
        if (distance[i] > max) max = distance[i];
    }
    printf("거리 최솟값 %.2lf, 최댓값 %.2lf\n", min, max);
}

int* voltage_to_adc(double* voltage, int N, double Vref) {
    int* adc = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        adc[i] = (int)(voltage[i] * 1023.0 / Vref);
    }
    return adc;
}