#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../util/clear.h"
#include "../stack.h"

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

Stack* init_box() {
    Stack* box = stack_new(BOOK_LENGTH);

    box->should_free_node_data_address = false;

    for (int i = 0; i < BOOK_LENGTH; i++) {
        stack_push(box, &BOOKS[i]);
    }

    return box;
}

Book* as_book(void* addr) {
    return (Book*)addr;
}

void header() {
    printf("--- BOOSTACK (%zu books) ---\n\n", BOOK_LENGTH);
}
void print_book(void* addr) {
    const Book* book = as_book(addr);

    printf(" (%d), (%s), (%s) ", book->year, book->genre, book->title);
}

Stack** stacks_match_one_or_create(Stack** stacks, size_t* stacks_length, Book* book, char* strategy) {
    if (book == NULL) {
        return stacks;
    }

    bool is_genre_match = strcmp(strategy, "genre") == 0;

    for (int i = 0; i < *stacks_length; i++) {
        Book* top_book = as_book(stacks[i]->top->data);

        if (!top_book) {
            continue;
        }

        if (
            (is_genre_match && strcmp(top_book->genre, book->genre) == 0) ||
            (!is_genre_match && (top_book->year - book->year == 0))) {
            stack_push(stacks[i], book);
            return stacks;
        }
    }

    if (stacks == NULL) {
        stacks = calloc(1, sizeof(Stack));
    } else {
        stacks = realloc(stacks, ((*stacks_length) + 1) * sizeof(Stack));
    }

    /** Capacity to the max books the stack can contain */
    stacks[*stacks_length] = stack_new(BOOK_LENGTH);
    /** Since the books are not dynamically allocated */
    stacks[*stacks_length]->should_free_node_data_address = false;

    stack_push(stacks[*stacks_length], book);

    *stacks_length += 1;

    return stacks;
}

Stack** menu_stack_by(Stack* box, size_t* stacks_length_dir, char* strategy) {
    Stack** stacks = NULL;
    size_t stacks_length = *stacks_length_dir;

    const bool is_genre_stacks = strcmp(strategy, "genre") == 0;

    while (box->top) {
        stacks = stacks_match_one_or_create(stacks, stacks_length_dir, box->top->data, strategy);

        stack_pop(box);
    }
    stacks_length = *stacks_length_dir;

    while (true) {
        unsigned int option = 0;

        clear();
        header();

        if (is_genre_stacks) {
            printf("[INFO]: [1] Stack by genre\n\n");
        } else {
            printf("[INFO]: [2] Stack by year\n\n");
        }

        for (int i = 0; i < stacks_length; i++) {
            Book* current_stack_top_book = as_book(stacks[i]->top->data);

            if (current_stack_top_book == NULL) {
                continue;
            }

            if (is_genre_stacks) {
                printf("[%d]: %s\n", i, current_stack_top_book->genre);
            } else {
                printf("[%d]: %d\n", i, current_stack_top_book->year);
            }
        }

        printf("\n[%d]: Back to main menu\n\n", stacks_length);

        printf("[INPUT]: Selection: ");
        scanf("%d", &option);

        if (option == stacks_length) {
            break;
        }

        clear();
        header();
        if (is_genre_stacks) {
            printf("[MENU]: [1] Stack by genre\n\n");
            printf("[INFO]: Stack of genre: %s\n", as_book(stacks[option]->top->data)->genre);
        } else {
            printf("[MENU]: [2] Stack by year\n\n");
            printf("[INFO]: Stack of year: %d\n", as_book(stacks[option]->top->data)->year);
        }

        stack_vertical_print(stacks[option], print_book);

        printf("\n");
        printf("[INPUT]: Type any letter to continue: ");
        scanf(" %c", (char*)&option);

        printf("\n");
    }

    return stacks;
}

int main() {
    Stack* box = init_box();
    bool should_continue = true;
    char res_char = '\0';

    while (should_continue) {
        Stack** stacks = NULL;
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
                stacks = menu_stack_by(box, &stacks_length, "genre");
                break;
            case 2:
                stacks = menu_stack_by(box, &stacks_length, "year");
                break;
            default:
                break;
        }

        if (stacks == NULL) {
            continue;
        }

        for (int i = 0; i < stacks_length; i++) {
            while (stacks[i]->top) {
                stack_push(box, stacks[i]->top->data);
                stack_pop(stacks[i]);
            }

            stack_free(stacks[i]);
        }

        free(stacks);
    }

    stack_free(box);

    clear();
    header();
    printf("[INFO]: See you space cowboy!");

    return 0;
}
