#include <stdio.h>
#include <stdlib.h>

float* create_signal(int n);
int* create_alert(float* signal, int n);

int main(void) {
    int n = 100;
    float* signal = create_signal(n);
    int* alert = create_alert(signal, n);

    for (int i = 0; i < n; i++) {
        printf("%.1f ",signal[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    for (int i = 0; i < n; i++) {
        printf("%d ",alert[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    free(signal);
    free(alert);
    return 0;
}

float* create_signal(int n) {
    float* signal = (float*)malloc(n * sizeof(float));
    signal[0] = 40;
    for (int i = 1; i < n; i++) {
        signal[i] = 32.5 + (float)(rand() % 21 - 10) / 2.0;
    }
    return signal;
}
/*
float* create_signal(int n) {
    float* signal = (float*)malloc(n * sizeof(float));
    signal[0] = 40;
    int count = 0;
    float sign = 1;
    int range = 5;
    for (int i = 1; i < n; i++) {
        float diff = (float)((rand() % (2 * range + 1)) - range);
        if (count < 10) {
            diff = abs(diff) * sign;
            count++;
            signal[i] = signal[i - 1] + diff;
            if (signal[i] < 10) signal[i] = 10.0;
            if (signal[i] > 45) signal[i] = 45.0;
        }
        else {
            signal[i] = signal[i - 1] + diff;
            if (signal[i] < 10) signal[i] = 10.0;
            if (signal[i] > 45) signal[i] = 45.0;
            if (sign * diff <= 0) {
                count = 0;
                sign = -1 * sign;
            }
        }
    }
    return signal;
}
*/
int* create_alert(float* signal, int n) {
    float on_thres = 30.0, off_thres = 35.0;
    int* alert = (int*)malloc(n * sizeof(int));
    alert[0] = 0;
    for (int i = 1; i < n; i++) {
        if (alert[i - 1] == 0) {
            if (signal[i] < on_thres) alert[i] = 1;
            else alert[i] = 0;
        }
        if (alert[i - 1] == 1) {
            if (signal[i] > off_thres) alert[i] = 0;
            else alert[i] = 1;
        }
    }
    return alert;
}