#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../linked-list/linked-list.h"
#include "../util/clear.h"
#include "../util/int.h"
#include "../util/log/colors.h"
#include "../util/log/log.h"
#include "hashlist.h"
#include "hashtable.h"

#define APP_MAX_STR_LEN 128
#define APP_MOVIES_INITIAL_LEN 105

#define APP_HASHTABLE_INITIAL_LEN_BASE 200

typedef struct {
    int id;
    char name[APP_MAX_STR_LEN];
    int year;
    float rating;
} Movie;

typedef struct {
    char input[APP_MAX_STR_LEN];
    int choice;

    bool should_run;

    HashTable_PrintFormat table_format;
} App;

void KeyToContinue();
void initialize_hash_tables(HashTable *table_id, HashList *table_name) {
    Movie initial_movies[] =
        {
            {1, "Blade Runner 2049\0", 2017, 80},
            {2, "Interstellar\0", 2014, 87},
            {3, "Mad Max: Fury Road\0", 2015, 81},
            {4, "Children of Men\0", 2006, 79},
            {5, "Inception\0", 2010, 88},
            {6, "Tenet\0", 2020, 73},
            {7, "Once Upon a Time... In Hollywood\0", 2019, 76},
            {8, "Inglourious Basterds\0", 2009, 84},
            {9, "The Lord of the Rings: The Return of the King\0", 2003, 90},
            {10, "The Wolf of Wall Street\0", 2013, 82},
            {11, "Disturbia\0", 2007, 68},
            {12, "Dunkirk\0", 2017, 78},
            {13, "Troy\0", 2004, 73},
            {14, "The Dark Knight\0", 2008, 90},
            {15, "Forrest Gump\0", 1994, 88},
            {16, "Fight Club\0", 1999, 88},
            {17, "Star Wars: Episode V - Empire Strikes Back\0", 1980, 87},
            {18, "The Silence of the Lamps\0", 1991, 86},
            {19, "Saving Private Ryan\0", 1998, 86},
            {20, "The Departed\0", 2006, 85},
            {21, "Whiplash\0", 2014, 85},
            {22, "Spider-Man: Into the Spider-Verse\0", 2018, 85},
            {23, "Avengers: Infinity War\0", 2018, 84},
            {24, "Django Unchained\0", 2012, 85},
            {25, "The Ministry of Ungentlemanly Warfare\0", 2024, 73},
            {26, "Godzilla x Kong: The New Empire\0", 2024, 65},
            {27, "Monkey Man\0", 2024, 71},
            {28, "Road House\0", 2024, 62},
            {29, "Rebel Moon - Part Two: The Scargiver\0", 2024, 52},
            {30, "Wish\0", 2023, 56},
            {31, "Anyone But You\0", 2023, 62},
            {32, "The Fall Guy\0", 2024, 74},
            {33, "The Bricklayer\0", 2023, 51},
            {34, "Anatomy of a Fall\0", 2023, 77},
            {35, "Hit man\0", 2023, 77},
            {36, "Glass\0", 2019, 66},
            {37, "Barbie\0", 2023, 68},
            {38, "Stolen\0", 2024, 56},
            {39, "Madame Web\0", 2024, 38},
            {40, "Joker\0", 2019, 84},
            {41, "The Shawshank Redemption\0", 1994, 93},
            {42, "Pulp Fiction\0", 1994, 92},
            {43, "Titanic\0", 1997, 89},
            {44, "The Matrix\0", 1999, 87},
            {45, "The Sixth Sense\0", 1999, 85},
            {46, "Gladiator\0", 2000, 86},
            {47, "Memento\0", 2000, 85},
            {48, "The Lord of the Rings: The Fellowship of the Ring\0", 2001, 88},
            {49, "A Beautiful Mind\0", 2001, 86},
            {50, "Spirited Away\0", 2001, 89},
            {51, "The Lord of the Rings: The Two Towers\0", 2002, 89},
            {52, "Finding Nemo\0", 2003, 90},
            {53, "The Incredibles\0", 2004, 89},
            {54, "Eternal Sunshine of the Spotless Mind\0", 2004, 88},
            {55, "Million Dollar Baby\0", 2004, 87},
            {56, "Brokeback Mountain\0", 2005, 87},
            {57, "Crash\0", 2004, 78},
            {58, "Pan's Labyrinth\0", 2006, 89},
            {59, "The Prestige\0", 2006, 88},
            {60, "No Country for Old Men\0", 2007, 90},
            {61, "There Will Be Blood\0", 2007, 89},
            {62, "WALL-E\0", 2008, 89},
            {63, "Slumdog Millionaire\0", 2008, 88},
            {64, "The Dark Knight\0", 2008, 90},
            {65, "Up\0", 2009, 88},
            {66, "Avatar\0", 2009, 83},
            {67, "Inception\0", 2010, 88},
            {68, "Toy Story 3\0", 2010, 87},
            {69, "Black Swan\0", 2010, 85},
            {70, "The Social Network\0", 2010, 88},
            {71, "The King's Speech\0", 2010, 87},
            {72, "The Artist\0", 2011, 88},
            {73, "Hugo\0", 2011, 87},
            {74, "The Help\0", 2011, 81},
            {75, "The Avengers\0", 2012, 85},
            {76, "Django Unchained\0", 2012, 87},
            {77, "Life of Pi\0", 2012, 86},
            {78, "Gravity\0", 2013, 87},
            {79, "12 Years a Slave\0", 2013, 88},
            {80, "Her\0", 2013, 89},
            {81, "Boyhood\0", 2014, 87},
            {82, "Birdman or (The Unexpected Virtue of Ignorance)\0", 2014, 88},
            {83, "The Grand Budapest Hotel\0", 2014, 87},
            {84, "Spotlight\0", 2015, 88},
            {85, "Mad Max: Fury Road\0", 2015, 90},
            {86, "The Revenant\0", 2015, 88},
            {87, "La La Land\0", 2016, 89},
            {88, "Arrival\0", 2016, 88},
            {89, "Moonlight\0", 2016, 88},
            {90, "Get Out\0", 2017, 87},
            {91, "Dunkirk\0", 2017, 89},
            {92, "The Shape of Water\0", 2017, 87},
            {93, "Three Billboards Outside Ebbing, Missouri\0", 2017, 88},
            {94, "Coco\0", 2017, 89},
            {95, "Call Me by Your Name\0", 2017, 88},
            {96, "Lady Bird\0", 2017, 87},
            {97, "Bohemian Rhapsody\0", 2018, 88},
            {98, "A Star is Born\0", 2018, 87},
            {99, "Black Panther\0", 2018, 88},
            {100, "Parasite\0", 2019, 89},
            {101, "Jojo Rabbit\0", 2019, 87},
            {102, "Joker\0", 2019, 86},
            {103, "Once Upon a Time... In Hollywood\0", 2019, 88},
            {104, "1917\0", 2019, 88},
            {105, "The Irishman\0", 2019, 87}

        };

    for (int i = 0; i < sizeof(initial_movies) / sizeof(Movie); i++) {
        Movie *movie_id_copy = calloc(1, sizeof(Movie));
        Movie *movie_name_copy = calloc(1, sizeof(Movie));

        memcpy(movie_id_copy, &initial_movies[i], sizeof(Movie));
        memcpy(movie_name_copy, &initial_movies[i], sizeof(Movie));

        hashlist_insert(
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
    fgets(input, APP_MAX_STR_LEN, stdin);

    while (*input != EOF && *input != '\n') {
        input += 1;
    }

    *input = '\0';
}

int integer_compare(void *a, void *b) {
    return *(int *)a - *(int *)b;
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

    LinkedList *table_available_ids = linked_list(sizeof(int));

    HashTable table_id = hashtable_new(
        APP_HASHTABLE_INITIAL_LEN_BASE,

        HASHTABLE_DEFAULT_RESIZE_FACTOR,
        1.0f,
        1.0f,

        NULL,
        print_movie,
        HASHTABLE_KEY_NUMBER,
        false,
        true

    );
    HashList table_name = hashlist_new(
        APP_HASHTABLE_INITIAL_LEN_BASE * 4,

        HASHTABLE_DEFAULT_RESIZE_FACTOR,
        0.25f,
        0.25f,

        NULL,
        print_movie,
        HASHTABLE_KEY_STRING,
        false,
        true

    );

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

                hashlist_print(&table_name, app.table_format);
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

                HashList_Entry *entry = hashlist_get(&table_name, app.input);

                if (!entry || !entry->head) {
                    printlog(LOG_WARN, "No movies found.");
                    KeyToContinue();
                    break;
                }

                HashList_EntryNode *node = entry->head;

                size_t count = 1;

                printlog(LOG_INFO, "Movies found: ");

                while (node) {
                    printf("\t[%d]: ", count);
                    print_movie(node->value);
                    printf("\n");
                    count += 1;
                    node = node->next;
                }

                KeyToContinue();

                break;
            }
            case 5: {
                TemplateReset();

                int id = table_available_ids->head
                             ? (*(int *)(table_available_ids->head->value_addr))
                             : table_id.size + 1;

                if (id > table_id.size + 1) {
                    delete_index(table_available_ids, 0, 0);
                    id = table_id.size + 1;
                }

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

                movie->id = id;

                memcpy(movie_copy, movie, sizeof(Movie));

                hashtable_set(&table_id, &movie->id, movie);
                hashlist_insert(&table_name, movie_copy->name, movie_copy);

                if (table_available_ids->length) {
                    delete_index(table_available_ids, 0, 0);
                }

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

                if (choice < 0 || choice > 1) {
                    printlog(LOG_ERROR, "Invalid choice.");
                    KeyToContinue();
                    break;
                }

                if (choice == 0) {
                    int id = 0;

                    printf("[INPUT]: Type the movie ID: ");

                    app_get_input(app.input);
                    sscanf(app.input, "%d", &id);

                    printf("\n");

                    Movie *movie = (Movie *)hashtable_get(&table_id, &id);

                    if (!movie) {
                        printlog(LOG_WARN, "Movie not found.");
                        KeyToContinue();
                        break;
                    }

                    HashList_Entry *entry = hashlist_get(&table_name, movie->name);

                    if (!entry->head || !entry->head->next) {
                        hashlist_delete(&table_name, movie->name);
                    } else {
                        HashList_EntryNode *prev = NULL;
                        HashList_EntryNode *node = entry->head;

                        while (node) {
                            if (((Movie *)node->value)->id == movie->id) {
                                if (!node->next) {
                                    entry->tail = prev;
                                }

                                if (!prev) {
                                    entry->head = node->next;
                                } else {
                                    prev->next = node->next;
                                }

                                table_name.size -= 1;
                                free(node->value);
                                free(node);
                                break;
                            }

                            prev = node;
                            node = node->next;
                        }

                        if (entry->head) {
                            entry->key = ((Movie *)entry->head)->name;
                        }
                    }

                    hashtable_delete(&table_id, &id);

                    if (id <= table_id.size) {
                        unshift(table_available_ids, integer(id));
                        sortll(table_available_ids, integer_compare);
                    }

                    printlog(LOG_INFO, "Movie removed successfully.");

                } else if (choice == 1) {
                    printf("[INPUT]: Type the movie name: ");
                    app_get_input(app.input);

                    printf("\n");

                    HashList_Entry *entry = hashlist_get(&table_name, app.input);

                    if (!entry) {
                        printlog(LOG_WARN, "No matching movies.");
                        KeyToContinue();
                        break;
                    }

                    int count = 0;
                    HashList_EntryNode *node = entry->head;

                    while (node) {
                        int node_movie_id = ((Movie *)(node->value))->id;

                        hashtable_delete(&table_id, &node_movie_id);

                        if (node_movie_id <= table_id.size) {
                            unshift(table_available_ids, integer(node_movie_id));
                            sortll(table_available_ids, integer_compare);
                        }

                        count += 1;
                        node = node->next;
                    }

                    /** This will delete the whole entry */
                    hashlist_delete(&table_name, app.input);

                    printlog(LOG_INFO, "Movies removed successfully: %d.", count);
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

    printlog(LOG_INFO, "Deleting all entries of both tables...");

    hashtable_empty(&table_id);
    printdev("Removed hashed ids table");

    hashlist_empty(&table_name);
    printdev("Removed hashed names table");

    printlog(LOG_WARN, "2,000,000 records affected. (just like in your deletes without where clause queries)");
    printf("\n");

    printlog(LOG_INFO, "See you in space cowboy!");

    return 0;
}