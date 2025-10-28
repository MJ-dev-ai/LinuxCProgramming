#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_N 16

int** create_sensor_values(int n, int t);
float* classify_by_threshold(int** sensor, int n, int t, int threshold);

int main(void) {
    int n = 8;
    int t = 32;
    if (n > MAX_N) {
        printf("n must be smaller than %d", MAX_N);
        return 1;
    }
    int** sensor = create_sensor_values(n, t);
    //int sensor[] = {320, 340, 980, 1010, 990, 310, 305, 1000};
    float* avg_index = classify_by_threshold(sensor, n, t, 500);
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", sensor[i][j]);
        }
        printf("| %.1f\n", avg_index[i]);
    }
    printf("\n");

    free(avg_index);
    for (int i = 0; i < t; i++) free(sensor[i]);
    free(sensor);
    return 0;
}

int** create_sensor_values(int n, int t) {
    srand(time(NULL));
    int** sensor = (int**)malloc(t * sizeof(int*));
    for (int i = 0; i < t; i++) {
        sensor[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) sensor[i][j] = rand() % 1024;
    }
    return sensor;
}

float* classify_by_threshold(int** sensor, int n, int t, int threshold) {
    float* results = (float*)malloc(t * sizeof(float));
    int black_count = 0;
    float index_avg = 0;
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < n; j++) {
            float detect = (sensor[i][j] <= threshold) ? 1.0 : 0.0;
            index_avg += (float)j * detect;
            black_count += (int)detect;
        }
        if (black_count == 0) results[i] = -1;
        else results[i] = index_avg / (float)black_count;
    }
    return results;
}

