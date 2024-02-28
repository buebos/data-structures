#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../util/clear.h"
#include "../stack.h"

#define GENRE_YEAR_LENGTH 11
#define BOOK_LENGTH 44

typedef struct {
    char title[64];
    char genre[32];
    unsigned int year;
} Book;

Book BOOKS[BOOK_LENGTH] = {
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

const char GENRES[11][35] =
    {"Fantasy",
     "Historical novel",
     "Crime",
     "Historical fiction",
     "Mystery",
     "Thriller",
     "Science fiction",
     "Post-apocalyptic fiction",
     "Fiction",
     "Horror",
     "Adventure Story"};

const unsigned int YEARS[11] = {
    1997,
    1998,
    1999,
    2000,
    2001,
    2002,
    2003,
    2004,
    2005,
    2006,
    2007};

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
void print_book(void* addr) {
    const Book* book = as_book(addr);

    printf(" (%d), (%s), (%s) ", book->year, book->genre, book->title);
}

void stack_match_genre(Stack** genre_stacks, Book* book) {
    if (book == NULL) {
        return;
    }

    for (int i = 0; i < GENRE_YEAR_LENGTH; i++) {
        if (strcmp(GENRES[i], book->genre) == 0) {
            stack_push(genre_stacks[i], book);
        }
    }
}
void stack_match_year(Stack** year_stacks, Book* book) {
    if (book == NULL) {
        return;
    }

    for (int i = 0; i < GENRE_YEAR_LENGTH; i++) {
        if (YEARS[i] == book->year) {
            stack_push(year_stacks[i], book);
        }
    }
}

void header() {
    printf("--- BOOSTACK ---\n\n");
}
void menu_stack_by(Stack* box, char* strategy) {
    Stack* stacks[GENRE_YEAR_LENGTH] = {NULL};
    const bool is_genre_stacks = strcmp(strategy, "genre") == 0;

    for (int i = 0; i < GENRE_YEAR_LENGTH; i++) {
        stacks[i] = stack_new(BOOK_LENGTH);
        stacks[i]->should_free_node_data_address = false;
    }

    while (box->top) {
        if (is_genre_stacks) {
            stack_match_genre(stacks, as_book(box->top->data));
        } else {
            stack_match_year(stacks, as_book(box->top->data));
        }
        stack_pop(box);
    }

    while (true) {
        unsigned int option = 0;

        clear();
        header();

        if (is_genre_stacks) {
            printf("[INFO]: [1] Stack by genre\n\n");
        } else {
            printf("[INFO]: [2] Stack by year\n\n");
        }

        for (int i = 0; i < GENRE_YEAR_LENGTH; i++) {
            if (is_genre_stacks) {
                printf("[%d]: %s\n", i, GENRES[i]);
            } else {
                printf("[%d]: %d\n", i, YEARS[i]);
            }
        }
        printf("[11]: Back to main menu\n\n");

        printf("[INPUT]: Selection: ");
        scanf("%d", &option);

        if (option == 11) {
            break;
        }

        clear();
        header();
        if (is_genre_stacks) {
            printf("[MENU]: [1] Stack by genre\n\n");
            printf("[INFO]: Stack of genre: %s\n", GENRES[option]);
        } else {
            printf("[MENU]: [2] Stack by year\n\n");
            printf("[INFO]: Stack of year: %d\n", YEARS[option]);
        }

        stack_vertical_print(stacks[option], print_book);

        printf("\n");
        printf("[INPUT]: Type any key to continue: ");
        scanf(" %c", &option);

        printf("\n");
    }

    for (int i = 0; i < GENRE_YEAR_LENGTH; i++) {
        stack_free(stacks[i]);
    }
}

int main() {
    Stack* box = init_box();
    bool should_continue = true;
    char res_char = '\0';

    while (should_continue) {
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
                menu_stack_by(box, "genre");
                break;
            case 2:
                menu_stack_by(box, "year");
                break;
            default:
                break;
        }

        if (selected_option != 0) {
            stack_free(box);
            box = init_box();
        }
    }

    stack_free(box);

    clear();
    header();
    printf("[INFO]: See you space cowboy!");

    return 0;
}
