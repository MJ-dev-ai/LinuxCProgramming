#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define IPV6_ADDR_MAX_LEN 40
#define LOG_MAX_LEN 512

void print_log(char* filename, char* string);
typedef struct log {
    int day, month, year;
    int hour, minute, second, ms;
    char protocol[20];
    char source_ip[IPV6_ADDR_MAX_LEN];
    char dest_ip[IPV6_ADDR_MAX_LEN];
} LOG;

LOG* get_info_from_log(char* filename, int* log_num);

int main(void) {
    int log_num;
    LOG* log_list = get_info_from_log("log.txt", &log_num);
    if (log_list == NULL) return 1;
    printf("%d\n", log_num);
    free(log_list);
    return 0;
}

LOG* get_info_from_log(char* filename, int* log_num) {
    if (filename == NULL) filename = "log.txt";
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("파일이 존재하지 않습니다.\n");
        return 0;
    }
    char row[LOG_MAX_LEN];
    char protocol[20] = {0};
    char source_ip[IPV6_ADDR_MAX_LEN] = {0};
    char dest_ip[IPV6_ADDR_MAX_LEN] = {0};
    int count = 0;
    int day, month, year;
    int hour, minute, second, ms;
    int log_capacity = 4;
    LOG* log_list = (LOG*)malloc(log_capacity * sizeof(LOG));
    
    while (fgets(row, LOG_MAX_LEN, fp) != NULL) {
        if (strstr(row, "IPv6") == NULL) continue;
        //06/17/2025-03:17:41.530994
        int result = sscanf(row, "%02d/%02d/%04d-%02d:%02d:%02d.%06d%*[^{]{%19[^}]}%s -> %s", &day, &month, &year, &hour, &minute, &second, &ms, protocol, source_ip, dest_ip);
        char* port = strrchr(source_ip, ':');
        if (port != NULL) *port = '\0';
        port = strrchr(dest_ip, ':');
        if (port != NULL) *port = '\0';
        if (count >= log_capacity) {
            log_capacity *= 2;
            log_list = (LOG*)realloc(log_list, log_capacity * sizeof(LOG));
        }
        log_list[count].day = day; log_list[count].month = month; log_list[count].year = year;
        log_list[count].hour = hour; log_list[count].minute = minute; log_list[count].second = second; log_list[count].ms = ms;
        strcpy(log_list[count].protocol, protocol);
        strcpy(log_list[count].source_ip, source_ip);
        strcpy(log_list[count].dest_ip, dest_ip);
        printf("%3d [%02d/%02d/%04d-%02d:%02d:%02d.%06d] ", ++count, day, month, year, hour, minute, second, ms);
        printf("{%s} %s -> %s\n", protocol, source_ip, dest_ip);
    }
    fclose(fp);
    printf("Log %d/%d saved. (%ld Bytes)\n", count, log_capacity, sizeof(LOG) * count);
    *(log_num) = count;
    return log_list;
}

void print_log(char* filename, char* string) {
    FILE *fp;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;
    int day = tm.tm_mday;
    int hour = tm.tm_hour;
    int min = tm.tm_min;
    int sec = tm.tm_sec;
    char* username = getenv("USER");
    // USER 환경 변수가 없을 경우 대비하여 기본값 설정
    if (username == NULL) {
        username = "unknown_user";
    }
    char* current_path = getcwd(NULL, 0);
    if (current_path == NULL) current_path = "~";
    fp = fopen(filename, "a");
    fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] %s@ubuntu:%s$ %s", year, month, day, hour, min, sec, username, current_path, string);
    fclose(fp);
}