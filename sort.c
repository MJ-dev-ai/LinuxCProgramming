#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int* random_list(int n, int pick) {
    /*
    1에서 n사이의 랜덤 숫자를 pick개만큼 뽑아 리스트에 저장하는 함수
    Inputs:
        n (int): 생성할 랜덤 숫자의 범위. 1에서 n까지의 정수중 랜덤한 숫자를 생성.
        pick (int): 생성할 랜덤 숫자의 갯수. 출력 배열의 크기.
    
    Return:
        results (int*): pick개의 랜덤 숫자를 요소로 하는 배열. 중복값은 없음.
    */
    if (pick > n) return NULL;
    int* results;
    results = (int*)malloc(pick * sizeof(int));
    // 1부터 n까지의 숫자를 요소로 가지는 배열
    int random_lst[n];
    for (int i = 0; i < n; i++) random_lst[i] = i + 1;

    srand(time(NULL));
    // 앞에서부터 n개의 숫자를 랜덤하게 선택
    for (int i = 0; i < pick; i++) {
        int j = i + rand() % (n - i);
        int temp = random_lst[i];
        random_lst[i] = random_lst[j];
        random_lst[j] = temp;
        // 선택된 숫자를 결과 배열에 적용
        results[i] = random_lst[i];
    }
    return results;
}

void quick_sort(int* arr, int left, int right) {
    /*
    재귀함수를 이용한 quick_sort 알고리즘
    Inputs:
        arr (int*): 정렬할 리스트
        left (int): 정렬할 시작 인덱스
        right (int): 정렬할 끝 인덱스
    */
    if (left >= right) return;
    // Median of three를 pivot으로 설정
    int mid = (left + right) / 2;
    // mid가 중간값일 경우 mid와 left 스왑
    if ((arr[left] <= arr[mid] && arr[mid] <= arr[right]) || (arr[right] <= arr[mid] && arr[mid] <= arr[left])) {
        int temp = arr[left];
        arr[left] = arr[mid];
        arr[mid] = temp;
    }
    // right가 중간값일 경우 left와 right 스왑
    else if ((arr[left] <= arr[right] && arr[right] <= arr[mid]) || (arr[mid] <= arr[right] && arr[right] <= arr[mid])) {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
    }
    int pivot = left;
    int low, high;
    low = left + 1; high = right;
    // pivot 왼쪽에 pivot보다 작은 요소들이, 오른쪽에는 큰 요소들이 오도록 정렬
    while (low <= high) {
        while (low <= right && arr[low] < arr[pivot]) low++;
        while (high >= 0 && arr[pivot] < arr[high]) high--;
        if (low < high) {
            int temp = arr[low];
            arr[low] = arr[high];
            arr[high] = temp;
        }
    }
    // pivot과 high의 위치를 바꿈
    // 정렬된 arr = {pivot보다 작은 요소들, pivot, pivot보다 큰 숫자들}
    int temp = arr[pivot];
    arr[pivot] = arr[high];
    arr[high] = temp;
    pivot = high;
    // 재귀적으로 quick sort 진행
    quick_sort(arr, left, pivot - 1);
    quick_sort(arr, pivot + 1, right);
}

int main(void) {
    int* randomList = random_list(100, 20); if (randomList == NULL) return 1;
    for (int i = 0; i < 20; i++) {
        printf("%d ", randomList[i]);
    }
    printf("\n");

    quick_sort(randomList, 0, 19);
    for (int i = 0; i < 20; i++) {
        printf("%d ", randomList[i]);
    }
    printf("\n");

    free(randomList);
    return 0;
}