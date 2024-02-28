#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../util/clear.h"
#include "../stack.h"

int *integer(int value) {
    int *addr = (int *)calloc(1, sizeof(int));
    *addr = value;
    return addr;
}

void print_number(void *addr) {
    printf("%d", *((int *)addr));
}

void print_header_stack(Stack *stack) {
    printf("[INFO]: Current stack:\n\n");
    stack_vertical_print(stack, print_number);
    printf("\n");
}

int main() {
    bool should_continue = true;
    size_t stack_capacity = 0;

    clear();
    print_header_stack(NULL);

    printf("[INPUT]: Type a number for the stack capacity: ");
    scanf("%zu", &stack_capacity);

    Stack *stack = stack_new(stack_capacity);

    while (should_continue) {
        StackNode *result = NULL;
        char res_char = '\0';
        int value = 0;

        clear();
        print_header_stack(stack);

        printf("[INFO]: Select an option\n");
        printf("[0]: Push an element to the stack\n");
        printf("[1]: Pop an element from the stack\n\n");
        printf("[INPUT]: Selection: ");
        scanf("%d", &value);

        clear();
        print_header_stack(stack);

        switch (value) {
            case 0:
                printf("[INPUT]: Type the value for element [%zu] of the stack: ", stack->length);
                scanf("%d", &value);

                result = stack_push(stack, integer(value));

                if (result == NULL) {
                    printf("[ERROR]: Overflow error, the max stack capacity is: %d\n\n", stack->capacity);
                }

                break;
            case 1:
                if (stack->length < 1) {
                    printf("[ERROR]: Underflow error, the min elements for pop should be 1\n\n");
                    break;
                }

                result = stack_pop(stack);
                /**
                 * The operation pop was done but left the stack empty with a
                 * NULL pointer as top
                 */
                if (result == NULL) {
                    result = (void *)!NULL;
                }

                break;
            default:
                break;
        }

        if (result != NULL) {
            clear();
            print_header_stack(stack);
        }

        printf("[INPUT]: Type 'y' to continue: ");
        scanf(" %c", &res_char);

        should_continue = res_char == 'y';
    }

    stack_free(stack);
    printf("[INFO]: See you space cowboy!");

    return 0;
}