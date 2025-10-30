#include <stdio.h>
#include <stdlib.h>

typedef struct list {
    int value;
    struct list* next;
} list;

list* create_from_array(int*,int);
void free_list(list*);
void print_list(list*);

int main(void) {
    int size = 10;
    int* arr = (int*)malloc(size*sizeof(int));
    for (int i = 0; i < size; i++) {
        arr[i] = (i + 1) * 40;
    }
    list* head = create_from_array(arr, size);
    print_list(head);
    free_list(head);
    free(arr);
    return 0;
}

list* create_from_array(int* arr, int size) {
    list* head = (list*)malloc(sizeof(list));
    head->value = arr[0];
    head->next = NULL;
    list* cur = head;
    for (int i = 1; i < size; i++) {
        list* temp = (list*)malloc(sizeof(list));
        temp->value = arr[i];
        temp->next = NULL;
        cur->next = temp;
        cur = cur->next;
    }
    return head;
}

void free_list(list* head) {
    list* cur = head;
    while (cur != NULL) {
        list* next_node = cur->next;
        free(cur);
        cur = next_node;
    }
}

void print_list(list* la) {
    list* cur = la;
    int count = 0;
    printf("[");
    while (cur != NULL && count <= 10) {
        printf("%d", cur->value);
        if (cur->next != NULL) printf(", ");
        cur = cur->next;
        count += 1;
    }
    printf("]\n");
}