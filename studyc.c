#include <stdio.h>
#include <string.h>
#include "sort.c"

int main(void) {
    char str[80];

    printf("문자열 입력: ");
    fgets(str, 80, stdin);
    puts("입력된 문자열: ");
    puts(str);

    int len = sizeof(str) / sizeof(str[0]);
    int len_str = strlen(str);
    printf("%d %d\n", len, len_str);
    return 0;
}