#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student {
    int id;
    char name[10];
    int korean;
    int english;
    int math;
} Student;

int total_result(Student*);
void sort_by_total(Student*, int);
void print_students(Student*, int);
void sort_by_value(Student* s, int size, int index);

int main(void) {
    // 학생들의 정보를 담을 리스트
    int size = 10;
    Student* s = (Student*)malloc(size * sizeof(Student));
    // 사용자의 입력을 받을 변수들
    int id;
    char name[10];
    int korean, english, math;
    // 사용자 입력을 리스트에 저장
    // for (int i = 0; i < size; i++) {
    //     printf("학번: ");
    //     scanf("%d", &id); s[i].id = id;
    //     printf("이름: ");
    //     scanf("%s", name); strcpy(s[i].name, name);
    //     printf("국어, 영어, 수학 점수: ");
    //     scanf("%d%d%d", &korean, &english, &math);
    //     s[i].korean = korean; s[i].english = english; s[i].math = math;
    // }

    // 미리 입력받은 학생 정보들
    Student s2[10] = {
        {315, "홍길동", 80, 75, 90},
        {316, "이순신", 88, 92, 100},
        {317, "서하윤", 95, 99, 98},
        {318, "유관순", 84, 70, 72},
        {319, "박신혜", 60, 65, 40},
        {320, "서예화", 72, 77, 70},
        {321, "이범준", 100, 99, 40},
        {322, "소연화", 24, 100, 22},
        {323, "김지유", 80, 99, 68},
        {324, "장진호", 66, 88, 99}
    };
    
    // 정렬 전과 정렬 후 데이터 출력
    printf("# 정렬 전 데이터...\n");
    print_students(s2, size);
    sort_by_value(s2, size, 1);
    printf("# 정렬 후 데이터...\n");
    print_students(s2, size);

    free(s);
    return 0;
}

int total_result(Student* s) {
    // 학생의 정보를 받아와 국영수 평균을 반환하는 함수
    int sum = s->korean + s->english + s->math;
    return sum;
}

void sort_by_total(Student* s, int size) {
    // 국영수 총점을 기준으로 학생 리스트 정렬
    for (int i = 0; i < size - 1; i++) {
        int max_j = i + 1;
        int total_max = total_result(s + max_j);
        for (int j = i + 2; j < size; j++) {
            int total_j = total_result(s + j);
            if (total_max < total_j) {
                max_j = j;
                total_max = total_j;
            }
        }
        int total_i = total_result(s + i);
        if (total_i < total_max) {
            Student temp = s[i];
            s[i] = s[max_j];
            s[max_j] = temp;
        }
    }
}

void sort_by_value(Student* s, int size, int index) {
    if (index >= 5 || index < 0) sort_by_total(s, size);
    else if (index != 1) {
        for (int i = 0; i < size - 1; i++) {
            int max_j = i + 1;
            int total_max;
            int total_i;
            switch (index) {
                case 0: total_i = s[i].id; break;
                case 2: total_i = s[i].korean; break;
                case 3: total_i = s[i].english; break;
                case 4: total_i = s[i].math; break;
            }
            for (int j = i + 2; j < size; j++) {
                int total_j;
                switch (index) {
                case 0: total_max = s[max_j].id; total_j = s[j].id; break;
                case 2: total_max = s[max_j].korean; total_j = s[j].korean; break;
                case 3: total_max = s[max_j].english; total_j = s[j].english; break;
                case 4: total_max = s[max_j].math; total_j = s[j].math; break;
                }
                if (total_max < total_j) max_j = j;
            }
            if (total_i < total_max) {
                Student temp = s[i];
                s[i] = s[max_j];
                s[max_j] = temp;
            }
        }
    }
    else {
        for (int i = 0; i < size - 1; i++) {
            int max_j = i + 1;
            for (int j = i + 2; j < size; j++) {
                if (strcmp(s[max_j].name, s[j].name) > 0) {
                    max_j = j;
                }
            }
            if (strcmp(s[i].name, s[max_j].name) > 0) {
                Student temp = s[i];
                s[i] = s[max_j];
                s[max_j] = temp; 
            }
        }
    }
}

void print_students(Student* s, int size) {
    // 학생들의 정보와 총점, 평균, 학점을 출력하는 함수
    for (int i = 0; i < size; i++) {
        Student cur = s[i];
        int total = total_result(s + i); // 총점 계산
        double avg = (double)total / 3.0; // 총점으로부터 평균 계산
        char grade; // 평균을 기준으로 학점 산정
        if (avg >= 90) grade = 'A';
        else if (avg >= 80) grade = 'B';
        else if (avg >= 70) grade = 'C';
        else grade = 'F';
        printf("%d %s\t%d\t%d\t%d\t%d\t%.1lf\t%c\n", cur.id, cur.name, cur.korean, cur.english, cur.math, total, avg, grade);
    }
}