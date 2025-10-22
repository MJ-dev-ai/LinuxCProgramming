#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int count_event(float* vel, int len);
void create_event(float* vel, int len, int range);

int main(void) {
    srand(time(NULL));
    int len = 150;
    float speed_values[len];
    float speed_values_no_event[len];
    create_event(speed_values, len, 15);
    create_event(speed_values_no_event, len, 8);

    int event = count_event(speed_values, len);
    printf("%d번 급제동/급가속 발생!!\n", event);
    event = count_event(speed_values_no_event, len);
    printf("%d번 급제동/급가속 발생!!\n", event);
    return 0;
}

void create_event(float* vel, int len, int range) {
    vel[0] = 0.0;
    int count = 0;
    float sign = 1;
    for (int i = 1; i < len; i++) {
        // 속도 변화량 -range ~ range
        float accel = (float)((rand() % (2 * range + 1)) - range);
        // 최소 10번까지는 accel 부호 유지
        if (count < 10) {
            accel = abs(accel) * sign;
            count++;
            vel[i] = vel[i - 1] + accel;
            if (vel[i] < 0) vel[i] = 0;
        }
        // 10번 이후부터는 accel의 부호가 바뀌면 count 초기화
        else {
            vel[i] = vel[i - 1] + accel;
            if (vel[i] < 0) vel[i] = 0;
            if (sign * accel <= 0) {
                count = 0;
                sign = -1 * sign;
            }
        }
    }
}

int count_event(float* vel, int len) {
    int count = 0;
    int start = 0, stop = 0;
    float delta = 0;
    float delta_prev = 0;
    for (int i = 0; i < len - 1; i++) {
        delta_prev = delta;
        delta = vel[i + 1] - vel[i];
        printf("%d", (int)vel[i]);
        if (delta >= 10 && delta_prev < 10) {
            start += 1;
            count += 1;
            printf("↑");
        }
        else if (delta <= -10 && delta_prev > -10) {
            stop += 1;
            count += 1;
            printf("↓");
        }
        printf(" ");
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("%d\n", (int)vel[len - 1]);
    printf("급가속: %d번, 급정거: %d번\n", start, stop);
    return count;
}