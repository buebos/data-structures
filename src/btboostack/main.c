/**
 * @file main.c
 * @author buebos
 * @brief This code is trash
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../binary-tree/binary-tree.h"
#include "../stack/stack.h"
#include "../util/clear.h"

typedef struct {
    char title[64];
    char genre[32];
    unsigned int year;
} Book;

Book BOOKS[] = {
    {"Cell", "Horror", 2006},
    {"The Street Lawyer", "Thriller", 1998},
    {"Nineteen Minutes", "Thriller", 2007},
    {"Harry Potter y la piedra filosofal", "Fantasy", 1997},
    {"Armageddon", "Post-apocalyptic fiction", 2003},
    {"The Jester", "Historical fiction", 2003},
    {"Harry Potter y el prisionero de Azkaban", "Fantasy", 1999},
    {"Star Wars: Episode I The Phantom Menace", "Science fiction", 1999},
    {"Dreamcatcher", "Fantasy", 2001},
    {"Rainbow Six", "Thriller", 1998},
    {"Hannibal", "Horror", 1999},
    {"Twelve Sharp", "Crime", 2006},
    {"The Bear and the Dragon", "Thriller", 2000},
    {"The Testament", "Adventure Story", 1999},
    {"Red Rabbit", "Historical fiction", 2002},
    {"Anansi Boys", "Fantasy", 2005},
    {"Airframe", "Thriller", 1997},
    {"Harry Potter y la camara secreta", "Fantasy", 1998},
    {"The King of Torts", "Thriller", 2003},
    {"The Da Vinci Code", "Mystery", 2003},
    {"Harry Potter y la Orden del Fenix", "Fantasy", 2003},
    {"Mary, Mary", "Crime", 2005},
    {"The Summons", "Thriller", 2002},
    {"Bag of Bones", "Horror", 1998},
    {"The 5th Horseman", "Thriller", 2006},
    {"Step on a Crack", "Thriller", 2007},
    {"Winter's Heart", "Fantasy", 2000},
    {"Plum Island", "Fiction", 1997},
    {"Eleven on Top", "Crime", 2005},
    {"Harry Potter y el misterio del principe", "Fantasy", 2005},
    {"The Closers", "Crime", 2005},
    {"Glorious Appearing", "Post-apocalyptic fiction", 2004},
    {"Prey ", "Science fiction", 2002},
    {"The Last Juror", "Thriller", 2004},
    {"'S' Is for Silence", "Mystery", 2005},
    {"4th of July", "Mystery", 2005},
    {"Harry Potter y las reliquias de la Muerte", "Fantasy", 2007},
    {"The Shelters of Stone", "Historical fiction", 2002},
    {"Harry Potter y el caliz de fuego", "Fantasy", 2000},
    {"The Dark Tower VII: The Dark Tower", "Fantasy", 2004},
    {"Book of the Dead ", "Crime", 2007},
    {"The Dark Tower VI: Song of Susannah", "Fantasy", 2004},
    {"Cold Mountain", "Historical novel", 1997},
    {"A Feast for Crows", "Fantasy", 2005},
};
size_t BOOK_LENGTH = sizeof(BOOKS) / sizeof(Book);

Stack *init_box() {
    Stack *box = stack_new(BOOK_LENGTH);

    box->should_free_node_data_address = false;

    for (int i = 0; i < BOOK_LENGTH; i++) {
        stack_push(box, &BOOKS[i]);
    }

    return box;
}

Book *as_book(void *addr) { return (Book *)addr; }
void header() { printf("--- BTBOSTACK (%zu books) ---\n\n", BOOK_LENGTH); }
void print_book(void *addr) {
    const Book *book = as_book(addr);

    printf(" (%d), (%s), (%s) ", book->year, book->genre, book->title);
}

void stack_print(void *data) {
    Stack *stack = (Stack *)data;

    stack_horizontal_print(stack, print_book);
}

int get_book_genre_weight_comparison(void *data_a, void *data_b) {
    return strcmp(as_book(((Stack *)data_a)->top->data)->genre, as_book(((Stack *)data_b)->top->data)->genre);
}

unsigned int get_book_genre_weight(Book *book) {
    const char *genre = book->genre;
    unsigned int acum_weight = 0;

    for (int i = 0; genre[i]; i++) {
        char current_char = tolower(genre[i]);
        acum_weight += (current_char - 'a' + 1);
    }

    return acum_weight;
}
unsigned int get_stack_data_weight_genre(void *data) {
    Stack *stack = (Stack *)data;
    Book *top_book = as_book(stack->top->data);

    if (!top_book) {
        return -1;
    }

    return get_book_genre_weight(top_book);
};
unsigned int get_stack_data_weight_year(void *data) {
    Stack *stack = (Stack *)data;
    Book *top_book = as_book(stack->top->data);

    if (!top_book) {
        return -1;
    }

    return top_book->year;
};

void print_genre_of_stack(size_t i, void *data) {
    Book *current_stack_top_book = as_book(((Stack *)data)->top->data);

    if (current_stack_top_book == NULL) {
        return;
    }

    printf("[%zu]: %s\n", i, current_stack_top_book->genre);
}

void print_year_of_stack(size_t i, void *data) {
    Book *current_stack_top_book = as_book(((Stack *)data)->top->data);

    if (current_stack_top_book == NULL) {
        return;
    }

    printf("[%zu]: %d\n", i, current_stack_top_book->year);
}

void stacks_match_one_or_insert(BT *btstacks, Book *book, char *strategy) {
    if (book == NULL) {
        return;
    }

    Stack *stack_matching = NULL;

    if (strcmp(strategy, "genre") == 0) {
        Stack *temp = stack_new(BOOK_LENGTH);
        stack_push(temp, book);

        stack_matching = bt_get_data_by_weight_comparison(btstacks, temp);
    } else {
        stack_matching = bt_get_data_by_weight(btstacks, book->year);
    }

    if (stack_matching) {
        stack_push((Stack *)stack_matching, book);
        return;
    }

    Stack *stack = stack_new(BOOK_LENGTH);
    stack_push(stack, book);
    bt_insert_iterative(btstacks, stack);
}

void menu_stack_by(BT *btstacks, Stack *box, char *strategy) {
    const bool is_genre_stacks = strcmp(strategy, "genre") == 0;

    if (is_genre_stacks) {
        btstacks->get_node_data_weight = get_stack_data_weight_genre;
        btstacks->get_data_weight_comparison = get_book_genre_weight_comparison;
    } else {
        btstacks->get_node_data_weight = get_stack_data_weight_year;
        btstacks->get_data_weight_comparison = NULL;
    }

    while (box->top) {
        stacks_match_one_or_insert(btstacks, as_book(box->top->data), strategy);

        stack_pop(box);
    }

    while (true) {
        size_t option = 0;

        clear();
        header();

        if (is_genre_stacks) {
            printf("[INFO]: [1] Stack by genre\n\n");
            bt_foreach_recursive(btstacks, print_genre_of_stack);
        } else {
            printf("[INFO]: [2] Stack by year\n\n");
            bt_foreach_recursive(btstacks, print_year_of_stack);
        }

        printf("\n[%zu]: Back to main menu\n\n", btstacks->_length);

        printf("[INPUT]: Selection: ");
        scanf("%zu", &option);

        if (option == btstacks->_length) {
            break;
        }

        Stack *stack = bt_get_index_recursive(btstacks, option);

        clear();
        header();
        if (is_genre_stacks) {
            printf("[MENU]: [1] Stack by genre\n\n");
            printf("[INFO]: Stack of genre: %s\n", as_book(stack->top->data)->genre);
        } else {
            printf("[MENU]: [2] Stack by year\n\n");
            printf("[INFO]: Stack of year: %d\n", as_book(stack->top->data)->year);
        }

        stack_vertical_print((Stack *)bt_get_index_recursive(btstacks, option),
                             print_book);

        printf("\n");
        printf("[INPUT]: Type any letter to continue: ");
        scanf(" %c", (char *)&option);

        printf("\n");
    }
}

int main() {
    Stack *box = init_box();
    bool should_continue = true;
    char res_char = '\0';

    while (should_continue) {
        BT *btstacks = bt_new(stack_print, get_stack_data_weight_genre);
        size_t stacks_length = 0;

        unsigned int selected_option = 0;

        clear();
        header();
        printf("[MENU]: Main menu\n\n");

        printf("[0]: Show box contents\n");
        printf("[1]: Stack by genre\n");
        printf("[2]: Stack by year\n");
        printf("[3]: Exit\n\n");

        printf("[INPUT]: Selection: ");
        scanf("%d", &selected_option);

        /** Like this to mantain code readibility */
        if (selected_option == 3) {
            break;
        }

        clear();
        header();

        switch (selected_option) {
            case 0:
                printf("[MENU]: [0] Show box contents\n\n");

                stack_vertical_print(box, print_book);

                printf("\n[INPUT]: Type 'y' to continue: ");
                scanf(" %c", &res_char);

                should_continue = res_char == 'y';
                break;
            case 1:
                menu_stack_by(btstacks, box, "genre");
                break;
            case 2:
                menu_stack_by(btstacks, box, "year");
                break;
            default:
                break;
        }

        for (int i = 0; i < btstacks->_length; i++) {
            Stack *stack = bt_get_index_recursive(btstacks, i);
            int initial_stack_length = stack->length;

            stack->should_free_node_data_address = false;

            for (int j = 0; j < initial_stack_length; j++) {
                stack_push(box, stack->top->data);
                stack_pop(stack);
            }
        }

        bt_free_recursive(btstacks);
    }

    stack_free(box);

    clear();
    header();
    printf("[INFO]: See you space cowboy!");

    return 0;
}
