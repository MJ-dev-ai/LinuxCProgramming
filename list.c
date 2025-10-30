#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void swap_ptr(char** ppa, char** ppb);
void swap(double* a, double* b);
void line_up(double*, double*, double*);
void print_array(int* pa, int size);
void print_str(char** ary, int size);
void func(int (*fp)(int, int));
int sum(int a, int b) { return (a + b); }
int max(int a, int b) { return (a > b) ? a : b; }
int mul(int a, int b) { return (a * b); }

typedef struct Node {
    int value;
    struct Node *next; 
} Node;

typedef struct Array {
    Node *head;
} Array;

Node* create_node(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    
    if (new_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    new_node->value = value;
    new_node->next = NULL;
    
    return new_node;
}

Node* create_from_array(int* arr, int size) {
    if (size <= 0) {
        return NULL;
    }
    
    Node* head = create_node(arr[0]);
    Node* cur = head;
    
    for (int i = 1; i < size; i++) {
        Node* temp = create_node(arr[i]);
        
        cur->next = temp;
        cur = cur->next;
    }
    
    return head;
}

void newprint(Array arr) {
    Node* cur = arr.head;
    printf("[");
    
    while (cur->next != NULL) {
        printf("%d, ", cur->value);
        cur = cur->next; // 다음 노드로 이동
    }
    printf("%d]\n", cur->value);
}

void free_list(Array arr) {
    Node *current = arr.head;
    Node *next_node;
    
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

int main(void) {
    int arr[3] = {0, 3, 5};
    Array newarr;
    newarr.head = create_from_array(arr, 3);
    newprint(newarr);
    free_list(newarr);

    return 0;
}

void func(int (*fp)(int, int)) {
    int a, b;
    printf("두 정수를 입력하세요: ");
    scanf("%d%d", &a, &b);
    int res = fp(a, b);
    printf("결과값: %d\n", res);
}

void swap_ptr(char** ppa, char** ppb) {
    char* temp_pt = *ppa;
    *ppa = *ppb;
    *ppb = temp_pt;
}

void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}
void line_up(double* maxp, double* midp, double* minp) {
    if (*minp > *midp) swap(minp, midp);
    if (*minp > *maxp) swap(minp, maxp);
    if (*midp > *maxp) swap(midp, maxp);
}
void print_array(int* arr, int size) {
    printf("[");
    for (int i = 0; i < size - 1; i++) {
        printf("%d ", arr[i]);
    }
    printf("%d]\n", arr[size - 1]);
}

void print_str(char** ary, int size) {
    printf("[");
    for (int i = 0; i < size - 1; i++) {
        printf("\"%s\", ", ary[i]);
    }
    printf("\"%s\"]\n", ary[size - 1]);
}