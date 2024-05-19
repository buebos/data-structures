#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/clear.h"
#include "../util/log/colors.h"
#include "../util/log/log.h"
#include "hashtable.h"

#define MAX_MOVIE_NAME_LENGTH 128
#define APP_MAX_INPUT_LEN MAX_MOVIE_NAME_LENGTH
#define MAX_MOVIES 200

typedef struct {
    int id;
    char name[MAX_MOVIE_NAME_LENGTH];
    int year;
    float rating;
} Movie;

typedef struct {
    char input[APP_MAX_INPUT_LEN];
    int choice;

    bool should_run;

    HashTable_PrintFormat table_format;
} App;

void KeyToContinue();
void initialize_hash_tables(HashTable *table_id, HashTable *table_name) {
    Movie initial_movies[] = {
        {1, "The Shawshank Redemption", 1994, 9.3},
        {2, "The Godfather", 1972, 9.2},
        {3, "The Dark Knight", 2008, 9.0},
        {4, "The Lord of the Rings: The Return of the King", 2003, 8.9},
        {5, "Pulp Fiction", 1994, 8.9},
        {6, "Schindler's List", 1993, 8.9},
    };

    for (int i = 0; i < sizeof(initial_movies) / sizeof(Movie); i++) {
        Movie *movie_id_copy = calloc(1, sizeof(Movie));
        Movie *movie_name_copy = calloc(1, sizeof(Movie));

        memcpy(movie_id_copy, &initial_movies[i], sizeof(Movie));
        memcpy(movie_name_copy, &initial_movies[i], sizeof(Movie));

        hashtable_set(
            table_name,
            movie_name_copy->name,
            movie_name_copy

        );

        hashtable_set(
            table_id,
            &movie_id_copy->id,
            movie_id_copy

        );
    }
}

size_t hash_id(HashTable *table, HashTable_KeyRef key) {
    return *(int *)key;
}

size_t hash_name(HashTable *table, HashTable_KeyRef key) {
    size_t hash = 0;
    char *name = (char *)key;
    for (int i = 0; name[i] != '\0'; i++) {
        hash = (hash << 5) - hash + name[i];
    }
    return hash % table->capacity;
}

void print_movie(HashTable_ValRef value) {
    Movie *movie = (Movie *)value;
    printf("ID: %d, Name: %s, Year: %d, Rating: %.1f", movie->id, movie->name, movie->year, movie->rating);
}

void app_get_input(char *input) {
    fgets(input, APP_MAX_INPUT_LEN, stdin);

    while (*input != EOF && *input != '\n') {
        input += 1;
    }

    *input = '\0';
}

void KeyToContinue() {
    char holder = 0;

    printf("\n" AC_PURPLE "[INPUT]" AC_RESET ": Type enter to continue: ");

    while (holder != '\n') {
        holder = getchar();
    }
}
void TemplateReset() {
    clear();
    printf(AC_YELLOW "\n============================== Hashflix ==============================\n\n" AC_RESET);
}

int main() {
    App app = {
        .table_format = HASHTABLE_PRINT_FLAT,
        .should_run = true,
        .choice = 0,
    };

    HashTable table_id = hashtable_new(MAX_MOVIES, NULL, print_movie, HASHTABLE_KEY_NUMBER, false, true);
    HashTable table_name = hashtable_new(MAX_MOVIES * 4, NULL, print_movie, HASHTABLE_KEY_STRING, false, true);

    initialize_hash_tables(&table_id, &table_name);

    while (app.should_run) {
        TemplateReset();

        printlog(
            LOG_INFO,
            "Name: { size: %zu, capacity: %zu } | ID: { size: %zu, capacity: %zu }",
            table_name.size, table_name.capacity,
            table_id.size, table_id.capacity

        );
        printlog(LOG_INFO, "Printing format: %s", app.table_format == HASHTABLE_PRINT_BUCKETS ? "Buckets" : "Flat");

        printf("\n");

        printlog(LOG_INFO, "Choose an option:\n");
        printf("[1] Show hash ID\n");
        printf("[2] Show hash name\n");
        printf("[3] Search by ID\n");
        printf("[4] Search by name\n");
        printf("[5] Add movie\n");
        printf("[6] Remove movie\n");
        printf("[7] Toggle table printing format\n");
        printf("[8] Exit\n\n");

        printf(AC_PURPLE "[INPUT]" AC_RESET ": Type your choice: ");

        app_get_input(app.input);
        sscanf(app.input, "%d", &app.choice);

        switch (app.choice) {
            case 1:
                TemplateReset();

                printlog(LOG_INFO, "Showing hash table by ID:");
                printf("\n");

                hashtable_print(&table_id, app.table_format);
                KeyToContinue();

                break;
            case 2:
                TemplateReset();

                printlog(LOG_INFO, "Showing hash table by name:");
                printf("\n");

                hashtable_print(&table_name, app.table_format);
                KeyToContinue();

                break;
            case 3: {
                TemplateReset();

                int id = 0;

                printf("[INPUT]: Type the movie ID: ");

                app_get_input(app.input);
                sscanf(app.input, "%d", &id);

                printf("\n");

                Movie *movie = (Movie *)hashtable_get(&table_id, &id);

                if (movie) {
                    printlog(LOG_INFO, "Movie found:");
                    print_movie(movie);
                    printf("\n");
                } else {
                    printlog(LOG_WARN, "Movie not found.");
                }

                KeyToContinue();

                break;
            }
            case 4: {
                TemplateReset();

                printf("[INPUT]: Type the movie name: ");
                app_get_input(app.input);
                printf("\n");

                Movie *movie = (Movie *)hashtable_get(&table_name, app.input);

                if (movie) {
                    printlog(LOG_INFO, "Movie found:");
                    print_movie(movie);
                    printf("\n");
                } else {
                    printlog(LOG_WARN, "Movie not found.");
                }

                KeyToContinue();

                break;
            }
            case 5: {
                TemplateReset();

                static int id = 6;

                Movie *movie = calloc(1, sizeof(Movie));
                Movie *movie_copy = calloc(1, sizeof(Movie));

                printf("[INPUT]: Type the movie name: ");
                app_get_input(movie->name);

                printf("[INPUT]: Type the movie year: ");
                app_get_input(app.input);
                sscanf(app.input, "%d", &movie->year);

                printf("[INPUT]: Type the movie rating: ");
                app_get_input(app.input);
                sscanf(app.input, "%f", &movie->rating);

                movie->id = ++id;

                memcpy(movie_copy, movie, sizeof(Movie));

                hashtable_set(&table_id, &movie->id, movie);
                hashtable_set(&table_name, movie_copy->name, movie_copy);

                printlog(LOG_INFO, "Movie added successfully.");

                KeyToContinue();

                break;
            }
            case 6: {
                TemplateReset();

                int choice = 0;

                printf("[INPUT]: Remove by [0] ID or [1] Name: ");

                app_get_input(app.input);
                sscanf(app.input, "%d", &choice);

                printdev("Choice is: %d", choice);

                if (choice == 0) {
                    int id = 0;

                    printf("[INPUT]: Type the movie ID: ");

                    app_get_input(app.input);
                    sscanf(app.input, "%d", &id);

                    printf("\n");

                    Movie *movie = (Movie *)hashtable_get(&table_id, &id);

                    if (movie) {
                        hashtable_delete(&table_id, &id);
                        hashtable_delete(&table_name, movie->name);
                        printlog(LOG_INFO, "Movie removed successfully.");
                    } else {
                        printlog(LOG_WARN, "Movie not found.");
                    }
                } else if (choice == 1) {
                    printf("[INPUT]: Type the movie name: ");
                    app_get_input(app.input);

                    printf("\n");

                    Movie *movie = (Movie *)hashtable_get(&table_name, app.input);

                    if (movie) {
                        hashtable_delete(&table_id, &movie->id);
                        hashtable_delete(&table_name, app.input);
                        printlog(LOG_INFO, "Movie removed successfully.");
                    } else {
                        printlog(LOG_WARN, "Movie not found.");
                    }

                } else {
                    printlog(LOG_ERROR, "Invalid choice.");
                }

                KeyToContinue();

                break;
            }
            case 7:
                app.table_format = !app.table_format;
                break;
            case 8:
                app.should_run = false;
                break;
            default:
                TemplateReset();

                printlog(LOG_ERROR, "Invalid choice.");

                KeyToContinue();
                break;
        }
    }

    TemplateReset();

    hashtable_empty(&table_id);
    hashtable_empty(&table_name);

    printlog(LOG_INFO, "See you in space cowboy!");

    return 0;
}