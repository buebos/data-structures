#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../util/clear.h"
#include "./quong.h"

#define MENU_LABEL_LENGTH 32

typedef struct {
    Quong main_queue;
    bool should_run;
    unsigned short current_menu;
    short current_operation_status;
} App;

static Song SONGS[] = {
    {"La Incondicional", "Luis Miguel", "1989-03-13"},
    {"Amor Eterno", "Juan Gabriel", "1984-07-20"},
    {"Por Debajo de la Mesa", "Luis Miguel", "1996-04-29"},
    {"Lo Pasado, Pasado", "José José", "1980-09-19"},
    {"Simplemente Amigos", "Ana Gabriel", "1991-02-18"},
    {"Azul", "Cristian Castro", "2001-04-24"},
    {"Ya Lo Sé Que Tú Te Vas", "Juan Gabriel", "1980-09-19"},
    {"Amarte Es un Placer", "Luis Miguel", "1999-09-14"},
    {"Amor Mío, ¿Qué Me Has Hecho?", "Rocío Jurado", "1992-10-20"},
    {"Volver a Amar", "Cristian Castro", "2004-05-04"},
    {"No Me Platiques Más", "Luis Miguel", "1991-06-25"},
    {"Lo Dudo", "José José", "1978-07-17"},
    {"Mi Historia Entre Tus Dedos", "Gianluca Grignani", "1995-02-21"},
    {"De Mí Enamórate", "Daniela Romo", "1983-01-31"},
    {"Amor Prohibido", "Selena", "1994-03-22"},
    {"La Diferencia", "Vicente Fernández", "1976-03-22"},
    {"Alma Rebelde", "Luis Miguel", "1993-05-25"},
    {"El Triste", "José José", "1970-09-20"},
    {"Desesperado", "Cristian Castro", "1992-10-12"},
    {"Costumbres", "Juan Gabriel", "1987-08-24"},
};

const char MENUS[][MENU_LABEL_LENGTH] = {
    "Main menu",
    "Toggle loop",
    "Previous song",
    "Next song",
    "Add a song to queue",
    "Display queue",
    "Remove a song from queue",
    "Restart queue",
    "Exit program",
};

const size_t SONGS_LENGTH = sizeof(SONGS) / sizeof(Song);
const size_t MENUS_LENGTH = sizeof(MENUS) / MENU_LABEL_LENGTH;

void print_song(size_t index, Song* song) {
    printf("[%zu]: %s - %s | %s", index, song->title, song->author, song->release_date);
}
void print_enqueued_song(size_t current_song_index, QuongNode* enqueued, Quong* quong) {
    if (enqueued->index == current_song_index) {
        printf("> ");
    }

    print_song(enqueued->index + 1, enqueued->song);

    if (enqueued->index == current_song_index) {
        printf(" <");
    }

    printf("\n");
}

void SongQueueCurrent(App* app) {
    Quong* quong = &app->main_queue;

    if (!quong || !quong->front) {
        printf("|\tQuong empty\t|\n");

        return;
    }

    printf("|");
    printf("\t%s\t<-", quong->front->prev ? quong->front->prev->song->title : "None");

    printf("\t%s\t->", quong->front->song->title);

    printf("\t%s\t|\n", quong->front->next ? quong->front->next->song->title : "None");
}
void AppHeader(App* app) {
    printf("|\t\t<------\tQUETIFY\t------>\t\t|\n");

    printf(
        "|\t%s\t-\tPlay mode: %s |\n\n",

        MENUS[app->current_menu],
        quong_get_play_mode_label(app->main_queue._play_mode) /***/
    );

    SongQueueCurrent(app);

    printf("\n");
}

void MenuAddQueueSong(App* app) {
    size_t song_target_id = -1;
    size_t queue_target_index = -1;

    AppHeader(app);

    for (size_t i = 0; i < SONGS_LENGTH; i++) {
        Song current_song = SONGS[i];
        print_song(i + 1, &current_song);
        printf("\n");
    }

    printf("\n");

    printf("[INPUT]: Type the ID of the song to add: ");
    scanf("%zu", &song_target_id);

    printf("[INPUT]: Type the position on the queue (>=1): ");
    scanf("%zu", &queue_target_index);

    app->current_operation_status = quong_fifo_index_operation(
        OPERATION_QUONG_INSERT,
        &app->main_queue,
        &SONGS[song_target_id - 1],
        queue_target_index - 1

    );
}

void MenuSongQueueList(App* app) {
    AppHeader(app);

    if (app->main_queue._size == 0) {
        printf("[INFO]: Queue is empty\n");
    } else {
        quong_fifo_for_each(&app->main_queue, print_enqueued_song);
    }

    printf("\n");

    printf("[INPUT]: Type any key to continue: ");
    scanf("%hd", &app->current_operation_status);

    app->current_operation_status = 0;
}

void MenuDeleteQueueSong(App* app) {
    size_t queue_target_index = -1;

    AppHeader(app);

    quong_fifo_for_each(&app->main_queue, print_enqueued_song);

    printf("\n");

    printf("[INPUT]: Type the ID of the song to delete: ");
    scanf("%zu", &queue_target_index);

    app->current_operation_status = quong_fifo_index_operation(
        OPERATION_QUONG_DELETE,
        &app->main_queue,
        NULL,
        queue_target_index - 1

    );
}

void MenuInvalidInput(App* app) {
    app->current_menu = 0;

    AppHeader(app);

    printf("[ERROR]: Please provide an input as indicated in the menu\n\n");

    printf("[INPUT]: Type any number to continue: ");
    scanf("%hd", &app->current_menu);

    app->current_menu = 0;
}

void MenuQuongOperationError(App* app) {
    clear();

    AppHeader(app);

    printf("[ERROR]: %s\n\n", quong_get_operation_status_label(app->current_operation_status));

    printf("[INPUT]: Type any key to continue: ");
    scanf("%hd", app->current_menu);
}

void MenuMain(App* app) {
    AppHeader(app);

    for (int i = 1; i < MENUS_LENGTH; i++) {
        printf("[%d]: %s\n", i, MENUS[i]);
    }

    printf("\n");

    printf("[INPUT]: Select an option: ");
    scanf("%hd", &app->current_menu);
}

int main() {
    App app = {0};

    app.should_run = true;

    while (app.should_run) {
        app.current_menu = 0;
        app.current_operation_status = SUCCESS_QUONG_OPERATION;

        clear();
        MenuMain(&app);

        clear();

        switch (app.current_menu) {
            case 1:
                app.current_operation_status = quong_toggle_loop(&app.main_queue);
                break;
            case 2:
                app.current_operation_status = quong_prev(&app.main_queue);
                break;
            case 3:
                app.current_operation_status = quong_next(&app.main_queue);
                break;
            case 4:
                MenuAddQueueSong(&app);
                break;
            case 5:
                MenuSongQueueList(&app);
                break;
            case 6:
                MenuDeleteQueueSong(&app);
                break;
            case 7:
                quong_restart(&app.main_queue);
                break;
            case 8:
                app.should_run = false;
                break;
            default:
                MenuInvalidInput(&app);
                break;
        }

        if (app.current_operation_status != SUCCESS_QUONG_OPERATION) {
            MenuQuongOperationError(&app);
        }
    }

    quong_dequeue_all(&app.main_queue);

    AppHeader(&app);
    printf("[BYE]: See you in space cowboy!\n");

    return 0;
}