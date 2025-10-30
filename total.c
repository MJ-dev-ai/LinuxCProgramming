#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

const float R = 0.05; // 예비율
const float eta = 0.8; // 시스템 효율
const float c = 0.02; // 구름계수
const float SF = 1.5; // 안전계수
const float k = 27.0; // 센서 거리 변환 상수
const float g = 9.81; // 중력 가속도
const float PI = 3.141592;

void compute_battery_usage(float, float);
void compute_gradeability(float, float);
void compute_drift_distance(float W, float r);
void count_abrupt_events();
void battery_histeresis_alert();
void adc_to_voltage_distance();

int main() {
    srand(time(NULL));
    printf("=== 이동로봇 사양 입력 ===\n");
    float W = 0.4, r = 0.0535, m = 40;
    float I_load = 8, v = 6.4;
    printf("바퀴 간 거리 [m]: "); scanf("%f", &W);
    printf("바퀴 반지름 [m]: "); scanf("%f", &r);
    printf("로봇 질량 [kg]: "); scanf("%f", &m);
    printf("평균 전류 [A]: "); scanf("%f", &I_load);
    printf("평균 속도 [km/h]: "); scanf("%f", &v);

    while (1) {
        printf("\n=== 이동로봇 종합 계산기 ===\n");
        int menu;
        printf("1. 이동로봇의 배터리 용량 선정\n");
        printf("2. 최대 견인력과 등판 가능 경사각 계산\n");
        printf("3. 이동로봇의 직진 편차 추정\n");
        printf("4. 로그에서 급가속/급제동 이벤트 카운트\n");
        printf("5. 배터리 잔량 히스테리시스 경보 시스템\n");
        printf("6. ADC원시값에서 전압/거리 계산기\n");
        printf("0. 종료\n");
        printf("메뉴 선택: "); scanf("%d", &menu);

        switch(menu) {
            case 0: break;
            case 1: compute_battery_usage(I_load, v); break;
            case 2: compute_gradeability(m, r); break;
            case 3: compute_drift_distance(W, r); break;
            case 4: count_abrupt_events(); break;
            case 5: battery_histeresis_alert(); break;
            case 6: adc_to_voltage_distance(); break;
            default: printf("잘못 입력하셨습니다.\n"); break;
        }
        if (menu == 0) break;
    }
    return 0;
}

void compute_battery_usage(float I_load, float vel) {
    printf("\n=== 이동로봇의 배터리 용량 선정 ===\n");
    printf("\n--- 배터리 사양 입력 ---\n");
    float V, capacity;
    printf("공칭전압 [V]: "); scanf("%f", &V);
    printf("용량 [Ah]: "); scanf("%f", &capacity);
    float E_usable, t, d;
    E_usable = V * capacity * eta * (1 - R);
    float P = V * I_load / eta;
    t = E_usable / P;
    d = vel * t;
    printf("사용 가능 에너지: %.2f Wh\n", E_usable);
    printf("런타임: %.2f h (%.1f min)\n", t, t * 60.0);
    printf("사용 가능 에너지: %.2f km\n", d);
    printf("계산 가정: 전압강하/온도영향 무시, 평균전류 일정, 전류는 부하측 기준)\n");
}
void compute_gradeability(float m, float r) {
    printf("\n=== 최대 견인력과 등판 가능 경사각 계산 ===\n");
    printf("\n--- 모터 사양 입력 ---\n");
    float Tm, G;
    int Nm;
    printf("모터 정격토크 [N·m]: "); scanf("%f", &Tm);
    printf("구동 모터 수 [개]: "); scanf("%d", &Nm);
    printf("감속비: "); scanf("%f", &G);
    float T_total = (float)Nm * Tm * G * eta;
    float F_avail = T_total / r;
    float theta = (F_avail / SF - c * m * g) / (m * g);
    if (theta < 0) theta = 0;
    else if (theta > 1) theta = 1;
    theta = asin(theta);
    float theta_deg = theta * 180.0 / PI;
    float grade = tan(theta) * 100.0;
    printf("가용 견인력: %.2lf N\n", F_avail);
    printf("등판 가능 경사각: %.2lf°\n", theta_deg);
    printf("등판율: %.2lf %%\n", grade);
}
void compute_drift_distance(float W, float r) {
    printf("\n=== 이동로봇의 직진 편차 추정 ===\n");
    printf("\n--- 로봇 사양 입력 ---\n");
    int er, el;
    float L;
    printf("좌/우 바퀴 오차율 [%%]: "); scanf("%d%d", &el, &er);
    printf("목표 주행 거리 [m]: "); scanf("%f", &L);
    float rl, rr;
    rl = r * (float)(100 + el) / 100.0;
    rr = r * (float)(100 + er) / 100.0;
    float Rc = W / 2.0 * (rl + rr) / (rr - rl);
    float delta = L / Rc;
    float delta_degree = delta / PI * 180.0;
    float ddrift = Rc * (1 - cos(delta));
    printf("좌/우 바퀴 반경: %.4f m / %.4f m\n", rl, rr);
    printf("곡률반경: %.2f m\n", Rc);
    printf("편류각: %.2f°\n", delta_degree);
    printf("편류거리: %.2f m\n", ddrift);
}
void count_abrupt_events() {
    printf("\n=== 로그에서 급가속/급제동 이벤트 카운트 ===\n");
    int len = 100;
    
    float* vel = (float*)malloc(len * sizeof(float));
    int range = 15;
    printf("생성할 랜덤 속도 값의 갯수: "); scanf("%d", &len);
    printf("가속도 범위 [m/s²]: "); scanf("%d", &range);
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
    count = 0;
    int start = 0, stop = 0;
    float delta = 0;
    float delta_prev = 0;
    printf("속도 값(급가속 ↑/ 급정거 ↓)\n");
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
    free(vel);
}
void battery_histeresis_alert() {
    printf("\n=== 배터리 잔량 히스테리시스 경보 시스템 ===\n");
    int n = 20;
    printf("생성할 배터리 로그 갯수: "); scanf("%d", &n);
    float* signal = (float*)malloc(n * sizeof(float));
    printf("생성된 배터리 로그\n");
    signal[0] = 40;
    printf("%3.1f ", signal[0]);
    for (int i = 1; i < n; i++) {
        signal[i] = 32.5 + (float)(rand() % 21 - 10) / 2.0;
        printf("%3.1f ", signal[i]);
        if (i % 10 == 9 || i == n - 1) printf("\n");
    }
    float on_thres = 30.0, off_thres = 35.0;
    int* alert = (int*)malloc(n * sizeof(int));
    printf("경고등 상태 (켜짐: 1, 꺼짐: 0)\n");
    alert[0] = 0;
    printf("%4d ", alert[0]);
    for (int i = 1; i < n; i++) {
        if (alert[i - 1] == 0) {
            if (signal[i] < on_thres) alert[i] = 1;
            else alert[i] = 0;
        }
        if (alert[i - 1] == 1) {
            if (signal[i] > off_thres) alert[i] = 0;
            else alert[i] = 1;
        }
        printf("%4d ", alert[i]);
        if (i % 10 == 9 || i == n - 1) printf("\n");
    }
    free(signal); free(alert);
}
void adc_to_voltage_distance() {
    int N = 5;
    int MAX_N = 10;
    printf("\n=== ADC원시값에서 전압/거리 계산기 ===\n");
    printf("디지털 센서 갯수: "); scanf("%d", &N);
    while (N > MAX_N) {
        printf("센서 갯수는 %d보다 작아야 합니다\n", MAX_N);
        printf("디지털 센서 갯수: "); scanf("%d", &N);
    }
    int* adc = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) adc[i] = rand() % 1024;
    float Vref = 5.0;
    float* voltage = (float*)malloc(N * sizeof(float));
    float* distance = (float*)malloc(N * sizeof(float));
    for (int i = 0; i < N; i++) {
        float v = Vref * (float)adc[i] / 1023.0;
        if (v < 0.01) v = 0.01;
        voltage[i] = v;
    }
    for (int i = 0; i < N; i++) {
        distance[i] = k / voltage[i];
    }
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
    free(adc); free(voltage); free(distance);
}