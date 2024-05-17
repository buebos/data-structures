#include <stdlib.h>
#include <stdio.h>

int *integer(int value) {
    int *addr = (int *)calloc(1, sizeof(int));
    *addr = value;
    return addr;
}

void integer_print(void *addr) {
    printf("%d", *((int *)addr));
}