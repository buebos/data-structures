#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../util/clear.h"
#include "../queue-array.h"

#define ENV_PROD 0
#define ENV_DEV 1

typedef struct app {
    QUA *main_queue;

    size_t prev_circular_start;

    bool should_run;

    char current_menu;

    short unsigned int env;
} App;

typedef struct song {
    char name[32];
    char author[32];

    unsigned int release_date;
} Song;

Song SONGS[10] = {
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

void print_songs_queue_view();
void SongsQueueHeader() {
    QUA *queue = app.main_queue;
    Song *songs = (Song *)(queue->array);
    size_t last_index = (queue->rear_index) % queue->capacity;

    if (app.env == ENV_DEV) {
        printf("[DEBUG]: Front index = %zu\n", queue->front_index);
        printf("[DEBUG]: Last index = (queue->rear_index + 1) %% queue->capacity = (%zu + 1) %% %zu = %zu \n\n", queue->rear_index, queue->capacity, last_index);
    }

    if (!queue->length) {
        printf("| << No songs added to queue >> |\n\n");
        return;
    }

    printf("| ");

    print_songs_queue_view();

    printf("|\n\n");
}
void SongsQueueList() {
    QUA *queue = app.main_queue;

    printf("\n");
}

void Header() {
    printf("\t<--- QUETIFY --->\n\n");
    SongsQueueHeader();
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

void InvalidInputMenu() {
    char res_char = '\0';

    printf("[ERROR]: Invalid input, if you do not wish to continue type 7, press any other key to continue.\n\n");

    printf("[INPUT]: ");
    scanf(" %c", &res_char);

    app.should_run = res_char != '7';
}

void AddSongMenu() {
    size_t index = 0;

    for (size_t i = 0; i < SONGS_LENGTH; i++) {
        printf("%zu.\t%s - %s\n", i + 1, SONGS[i].name, SONGS[i].author);
    }

    printf("\n");

    printf("[INFO]: Type the index of the song to add.\n");
    printf("[INPUT]: ");
    scanf("%d", &index);

    if (index < 1 || index > SONGS_LENGTH) {
        clear();
        Header();
        InvalidInputMenu();
        return;
    }

    qua_enqueue(app.main_queue, &SONGS[index - 1]);
}

void RemoveSongMenu() {
    char res_char = '\0';

    SongsQueueList();

    printf("[INPUT] (Index of song to remove): ");
    scanf("%s", &res_char);
}

void print_song(Song *song) {
    if (!song->name[0]) {
        printf(" None ", song->name, song->author);
        return;
    }

    printf(" %s - %s ", song->name, song->author);
}

void print_songs_queue_view() {
    QUA *queue = app.main_queue;

    QUA helper = qua_new_static(queue->capacity, queue->element_size, CONFIG_ALLOC_ARRAY, queue->is_circular);

    Song *queue_songs = (Song *)queue->array;
    Song *helper_songs = (Song *)helper.array;

    size_t playing_song_index = queue->front_index;

    helper.front_index = playing_song_index;

    if (app.env == ENV_DEV) {
        printf("\n[DEBUG]: Initial Queue front index: %zu\n", queue->front_index);
        printf("[DEBUG]: Initial Helper front index: %zu\n\n", helper.front_index);
    }

    for (int i = 0; i < queue->capacity; i++) {
        qua_enqueue(&helper, &(queue_songs[queue->front_index]));
        qua_dequeue(queue);

        // /** Print the previous song */
        // if (queue->is_circular && playing_song_index == 0 && helper.front_index == queue->capacity - 1) {
        //     print_song(&(helper_songs[helper.front_index]));
        // } else if (helper.front_index == playing_song_index - 1) {
        //     print_song(&(helper_songs[helper.front_index]));
        // }
    }

    queue->front_index = playing_song_index;

    for (int i = 0; i < helper.capacity; i++) {
        /** Print the current song */
        print_song(&(helper_songs[helper.front_index]));

        qua_enqueue(queue, &(helper_songs[helper.front_index]));
        qua_dequeue(&helper);

        // /** Print the next song */
        // if (queue->is_circular && playing_song_index == queue->capacity - 1 && queue->front_index == 0) {
        //     print_song(&(queue_songs[queue->front_index]));
        // } else if (queue->front_index == playing_song_index + 1) {
        //     print_song(&(queue_songs[queue->front_index]));
        // }
    }

    if (app.env == ENV_DEV) {
        printf("\n[DEBUG]: Final Queue front index: %zu\n", queue->front_index);
        printf("[DEBUG]: Final Helper front index: %zu\n\n", helper.front_index);
    }

    qua_free(&helper);
}

int main(int argc, char *argv[]) {
    /** ENV_PROD = 0; ENV_DEV = 1 */
    app.env = argc > 1 && strcmp(argv[1], "-d") == 0;

    Song queue_array[10] = {0};
    QUA queue = qua_new_static(10, sizeof(Song), queue_array, CONFIG_LINEAR_QUEUE);

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
        MainMenu(); /** Modifies app.current_menu */

        clear();
        Header();

        switch (app.current_menu) {
            case '0':
                /** Toggle loop */
                if (queue.is_circular) {
                    app.prev_circular_start = queue.front_index;
                    qua_fifo_toggle_circular(&queue, 0);
                } else {
                    qua_fifo_toggle_circular(&queue, app.prev_circular_start);
                }
                break;
            case '1':
                /** Previous song */
                if (!queue.is_circular && queue.front_index == 0) {
                    /** Cannot go to previous song */
                    break;
                }

                if (queue.front_index > 0) {
                    queue.front_index = queue.front_index - 1;
                } else {
                    queue.front_index = queue.capacity - 1;
                }
                break;
            case '2':
                /** Next song */
                if (!queue.is_circular && queue.front_index == queue.capacity - 1) {
                    /** Cannot go to next song */
                    break;
                }

                qua_dequeue(&queue);
                break;
            case '3':
                /** Add song */
                AddSongMenu();
                break;
            case '4':
                /** Show queue list */
                SongsQueueList();
                break;
            case '5':
                /** Remove song */
                RemoveSongMenu();
                break;
            case '6':
                /** Restart queue */
                queue.front_index = 0;
                break;
            case '7':
                /** End program */
                app.should_run = false;
                break;
            default:
                InvalidInputMenu();
                break;
        }
    }

    clear();
    Header(&queue);

    printf("\n[BYE]: See you in space cowboy!\n");

    qua_free(&queue);

    return 0;
}