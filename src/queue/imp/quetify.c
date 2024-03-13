#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../util/clear.h"
#include "../queue-circular-array.h"

typedef struct app {
    QCA *main_queue;

    bool should_run;

    char current_menu;
} App;

typedef struct song {
    char name[32];
    char author[32];

    unsigned int release_date;
} Song;

const Song SONGS[10] = {
    {"homesick", "wave to earth", 202307},
    {"Ocean Eyes", "Billie Eilish", 201512},
    {"Sea of Love", "The National", 201303},
    {"Swim Good", "Frank Ocean", 201107},
    {"Oceans (Where Feet May Fail)", "Hillsong United", 201301},
    {"Island in the Sun", "Weezer", 200106},
    {"Swallowed in the Sea", "Coldplay", 200506},
    {"The Ocean", "Led Zeppelin", 197303},
    {"Under the Bridge", "Red Hot Chili Peppers", 199209},
    {"The Ship Song", "Nick Cave & The Bad Seeds", 199003}

};
const size_t SONGS_LENGTH = sizeof(SONGS) / sizeof(Song);

App app = {NULL};

void SongQueue() {
    QCA *queue = app.main_queue;
    Song *songs = (Song *)(queue->array);

    printf("|\t");

    for (size_t i = queue->front_index; i < queue->capacity; i++) {
        size_t index = (queue->front_index + i) % queue->capacity;
        Song *song = &(songs[index]);

        /** Prints only if not empty string (first char '\0') */
        if (song->name[0]) {
            printf("%s\t", song->name);
        }
    }

    printf("\t|\n\n");
}

void Header() {
    printf("\t<--- QUETIFY --->\n\n");
    SongQueue();
}
void MainMenu() {
    printf("[0] Toggle loop\n");
    printf("[1] Previous\n");
    printf("[2] Next\n");
    printf("[3] Add song\n");
    printf("[4] Show playlist\n");
    printf("[5] Remove song\n");
    printf("[6] Restart playlist\n");
    printf("[7] Close program\n\n");

    printf("[INPUT]: ");
    scanf(" %c", &app.current_menu);
}
void AllSongs() {
    char res_char = '\0';

    for (size_t i = 0; i < SONGS_LENGTH; i++) {
        printf("%zu.\t%s - %s\n", i + 1, SONGS[i].name, SONGS[i].author);
    }

    printf("\n");

    printf("[INFO]: If you do not wish to continue type 7, press any other key to continue.\n\n");

    printf("[INPUT]: ");
    scanf(" %c", &res_char);
}
void InvalidInput() {
    char res_char = '\0';

    printf("[ERROR]: Invalid input, if you do not wish to continue type 7, press any other key to continue.\n\n");

    printf("[INPUT]: ");
    scanf(" %c", &res_char);

    app.should_run = res_char != '7';
}

int main() {
    Song queue_array[10] = {0};
    QCA queue = qca_new_static(10, sizeof(Song), queue_array);

    app.main_queue = &queue;
    app.should_run = true;

    /**
     * All of the functions in pascal case render something to the
     * terminal and can modify the global app state. (yes, like React
     * components lmao iuk)
     */
    while (app.should_run) {
        clear();
        Header();

        MainMenu();

        clear();
        Header();

        switch (app.current_menu) {
            case '0':
                break;
            case '1':
                break;
            case '2':
                break;
            case '3':
                AllSongs();
                break;
            case '4':
                break;
            case '5':
                break;
            case '6':
                break;
            case '7':
                app.should_run = false;
                break;
            default:
                InvalidInput();
                break;
        }
    }

    clear();
    Header(&queue);

    printf("\n[BYE]: See you in space cowboy!\n");

    qca_free(&queue);

    return 0;
}