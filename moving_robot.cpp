#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <cmath>
using namespace std;

#define PI 3.1415926535

class InwheelMotor {
public:
    double wheel_size = 0; // m
    double torque = 0; // Nm
    double G = 0;
    InwheelMotor(double ws, double t, double g): wheel_size(ws / 2000.0), torque(t), G(g) {}
};

const double c = 0.018; // 구름저항계수
const double g = 9.81; // 중력가속도
const int MAP_WIDTH = 500;
const int MAP_HEIGHT = 500;

double gradeability(double m, double Tm, int Nm, double G,
                    double eta, double r, double c, double sf);

float* calculate_lidar_distances();
int* findobstacle(float* sensor);
void draw_map_from_sensor(float* sensor);
// --------------------------
// 메인 함수
// --------------------------
int main() {
    float* sensor = calculate_lidar_distances();
    int* angles = findobstacle(sensor);
    for (int i = 0; i < 3; i++) {
        printf("%d° ", angles[i]);
    }
    printf("\n");
    draw_map_from_sensor(sensor);
    return 0;
}

double gradeability(double m, double Tm, int Nm, double G,
                    double eta, double r, double c, double sf) {
    /*
    Inputs:
        총질량 m(kg), 모터 정격토크 Tm(N.m), 모터 수 Nm, 감속비 G, 구동효율 eta,
        바퀴 반지름 r(m), 구름마찰계수 c, 안전계수 sf
    Return:
        grade (double)
    */
    double T_total = Nm * Tm * G * eta;
    double F_avail = T_total / r;
    double theta = (F_avail / sf - c * m * g) / (m * g);
    if (theta < 0) theta = 0;
    else if (theta > 1) theta = 1;
    theta = asin(theta);
    double theta_deg = theta * 180.0 / PI;
    double grade = tan(theta) * 100.0;

    printf("Favail = %.2lf N\n", F_avail);
    printf("Theta = %.2lf\n", theta_deg);
    printf("Gradeability = %.2lf %%\n", grade);
    return grade;
}

int* findobstacle(float* sensor) {
    /*
    라이다 센서 값 중 최솟값 3개를 구해 출력하는 함수
    Input:
        sensor (float*): 0°에서 360°까지 총 360개의 라이다 센서 값
    Return:
        angles (int*): 최솟값 3개를 갖는 각도 값들의 배열
    */
    int epoch = 3;
    int* angles = (int*)malloc(epoch * sizeof(int));
    int min_angle = 0;
    float min_distance = sensor[min_angle];
    float cur_min_distance = 0;
    for (int n = 0; n < epoch; n++) {
        min_angle = 0;
        min_distance = sensor[min_angle];
        for (int i = 0; i < 360; i++) {
            if (min_distance > sensor[i] && sensor[i] > cur_min_distance) {
                min_angle = i;
                min_distance = sensor[i];
            }
        }
        angles[n] = min_angle;
        printf("%d° 방향 %.2fcm 거리에 가장 가까운 장애물이 있습니다.\n", min_angle, min_distance);
        cur_min_distance = min_distance;
    }
    return angles;
}

float* calculate_lidar_distances() {
    /*
    3개의 장애물을 가진 MAP에서 라이다 센서 값을 구해 저장하는 함수.
    Return:
        distances (float*): 0°에서 360°까지의 각도별 센서의 거리 값 배열 (cm)
    */
    // 1. 환경 및 수학 상수 설정
    const float SENSOR_X = (float)MAP_WIDTH / 2.0f; // 250.0f
    const float SENSOR_Y = (float)MAP_HEIGHT / 2.0f; // 250.0f
    const int NUM_DATA = 360; // 360도

    // --- 2. 고정된 3개의 장애물 정보 ---
    // 장애물 1 (왼쪽 상단)
    const float obs1_cx = 100.0f;
    const float obs1_cy = 400.0f;
    const float obs1_r = 70.0f;

    // 장애물 2 (오른쪽 중간)
    const float obs2_cx = 400.0f;
    const float obs2_cy = 200.0f;
    const float obs2_r = 50.0f;
    
    // 장애물 3 (왼쪽 하단 근처)
    const float obs3_cx = 150.0f;
    const float obs3_cy = 100.0f;
    const float obs3_r = 30.0f;
    // -------------------------------------
    
    // 3. 동적 메모리 할당
    float *distances = (float*)malloc(NUM_DATA * sizeof(float));
    if (distances == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        return NULL;
    }

    // 4. 0도부터 359도까지 거리 계산
    for (int angle = 0; angle < NUM_DATA; angle++) {
        float angle_rad = (float)angle * PI / 180.0f;
        float dir_x = cosf(angle_rad);
        float dir_y = sinf(angle_rad);

        // 맵의 최대 거리로 초기화
        float min_dist = sqrtf((float)MAP_WIDTH * MAP_WIDTH + (float)MAP_HEIGHT * MAP_HEIGHT); 

        // A. 벽과의 거리 계산 (X=0, X=500, Y=0, Y=500)
        if (dir_x < 0.0f) { float t = (0.0f - SENSOR_X) / dir_x; if (t > 0.0f) min_dist = fminf(min_dist, t); }
        if (dir_x > 0.0f) { float t = ((float)MAP_WIDTH - SENSOR_X) / dir_x; if (t > 0.0f) min_dist = fminf(min_dist, t); }
        if (dir_y < 0.0f) { float t = (0.0f - SENSOR_Y) / dir_y; if (t > 0.0f) min_dist = fminf(min_dist, t); }
        if (dir_y > 0.0f) { float t = ((float)MAP_HEIGHT - SENSOR_Y) / dir_y; if (t > 0.0f) min_dist = fminf(min_dist, t); }

        // B. 원형 장애물 표면과의 거리 계산 함수 (재사용 가능한 블록)
        auto calculate_circle_distance = [&](float obs_cx, float obs_cy, float obs_r) {
            float px = obs_cx - SENSOR_X; float py = obs_cy - SENSOR_Y;
            float r_sq = obs_r * obs_r;
            
            // 2차 방정식 계수: a*t^2 + b*t + c = 0 (a=1)
            float b = 2.0f * (px * dir_x + py * dir_y);
            float c = (px * px + py * py) - r_sq;

            float discriminant = b * b - 4.0f * c;

            if (discriminant >= 0.0f) {
                float d_sqrt = sqrtf(discriminant);
                float t1 = (-b - d_sqrt) / 2.0f; // 가까운 교점
                float t2 = (-b + d_sqrt) / 2.0f; // 먼 교점

                if (t1 > 0.0f) return t1;
                else if (t2 > 0.0f) return t2;
            }
            return INFINITY;
        };

        // C. 3개 장애물과의 거리 계산 적용
        min_dist = fminf(min_dist, calculate_circle_distance(obs1_cx, obs1_cy, obs1_r));
        min_dist = fminf(min_dist, calculate_circle_distance(obs2_cx, obs2_cy, obs2_r));
        min_dist = fminf(min_dist, calculate_circle_distance(obs3_cx, obs3_cy, obs3_r));


        // 5. 최종 거리 저장
        distances[angle] = min_dist;
    }

    return distances;
}

// 라이다 센서 값으로부터 맵 이미지를 생성해 이미지로 저장하는 함수
void write_int(FILE *fp, int value) {
    fputc(value & 0xff, fp);
    fputc((value >> 8) & 0xff, fp);
    fputc((value >> 16) & 0xff, fp);
    fputc((value >> 24) & 0xff, fp);
}
void save_bitmap_file(char map_data[MAP_HEIGHT][MAP_WIDTH]) {
    const char* filename = "lidar_map.bmp";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "오류: 비트맵 파일을 열 수 없습니다: %s\n", filename);
        return;
    }

    // BMP 헤더 정의
    const int BMP_HEADER_SIZE = 14;
    const int DIB_HEADER_SIZE = 40;
    const int PIXEL_DATA_SIZE = MAP_WIDTH * MAP_HEIGHT * 3; // 24-bit (R, G, B)
    const int FILE_SIZE = BMP_HEADER_SIZE + DIB_HEADER_SIZE + PIXEL_DATA_SIZE;

    // --- BMP File Header (14 bytes) ---
    fputc('B', fp); fputc('M', fp); // Signature
    write_int(fp, FILE_SIZE);       // File size
    write_int(fp, 0);               // Reserved (0)
    write_int(fp, BMP_HEADER_SIZE + DIB_HEADER_SIZE); // Pixel data offset

    // --- DIB Header (40 bytes) ---
    write_int(fp, DIB_HEADER_SIZE); // DIB header size
    write_int(fp, MAP_WIDTH);       // Width
    write_int(fp, MAP_HEIGHT);      // Height (양수: bottom-up)
    fputc(1, fp); fputc(0, fp);      // Color planes (1)
    fputc(24, fp); fputc(0, fp);     // Bits per pixel (24)
    write_int(fp, 0);               // Compression method (0=None)
    write_int(fp, PIXEL_DATA_SIZE); // Image size (raw bitmap data size)
    write_int(fp, 2835);            // Horizontal resolution (72 DPI = 2835 ppm)
    write_int(fp, 2835);            // Vertical resolution
    write_int(fp, 0);               // Number of colors in the palette (0)
    write_int(fp, 0);               // Important colors (0)

    // --- Pixel Data ---
    // BMP는 보통 bottom-up (Y=0이 파일의 마지막 줄)으로 저장되지만,
    // 여기서는 Y=0을 첫 줄로 간주하고 Y축을 뒤집어 저장합니다 (Top-down)
    for (int y = MAP_HEIGHT - 1; y >= 0; y--) { // Y축을 뒤집어 bottom-up으로 저장
        for (int x = 0; x < MAP_WIDTH; x++) {
            char intensity = map_data[y][x];
            // Grayscale (R=G=B)
            fputc(intensity, fp); // Blue
            fputc(intensity, fp); // Green
            fputc(intensity, fp); // Red
        }
        // 4바이트 경계를 맞추기 위한 Padding (100*3 = 300은 4의 배수이므로 패딩 없음)
    }

    fclose(fp);
    printf("성공: 라이다 맵이 '%s' 파일로 저장되었습니다.\n", filename);
}
void draw_map_from_sensor(float* sensor) {
    int sensor_x = MAP_WIDTH / 2, sensor_y = MAP_HEIGHT / 2;
    char sensor_img[MAP_HEIGHT][MAP_WIDTH];
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            sensor_img[i][j] = 0;
        }
    }
    sensor_img[sensor_y][sensor_x] = 100;
    for (int i = 0; i < 360; i++) {
        int x = int(sensor[i] * cosf(i * PI / 180.0)), y = int(sensor[i] * sinf(i * PI / 180.0));
        x += sensor_x; y += sensor_y;
        if (x < 0) x = 0; if (x > MAP_WIDTH - 1) x = MAP_WIDTH - 1;
        if (y < 0) y = 0; if (y > MAP_HEIGHT - 1) y = MAP_HEIGHT - 1;
        y = MAP_HEIGHT - 1 - y;
        sensor_img[y][x] = 255;
        if (y + 1 < MAP_HEIGHT) sensor_img[y + 1][x] = 255;
        if (y - 1 >= 0) sensor_img[y - 1][x] = 255;
        if (x + 1 < MAP_WIDTH) sensor_img[y][x + 1] = 255;
        if (x - 1 >= 0) sensor_img[y][x - 1] = 255;
    }
    save_bitmap_file(sensor_img);
}

