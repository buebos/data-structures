/**
 * @file quong.c
 * @author Gael Herrera
 * @brief This code is actually 2x trash than usual, I just wanted
 * to solve the problem, I'm kind of tired, it's late, read at your
 * own risk lol. I feel lucky to have 0 followers on gh lol.
 * @version 0.1
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS_QUONG_OPERATION 0

#define ERROR_QUONG_NULL 1
#define ERROR_SONG_NULL 2
#define ERROR_QUONG_NO_NEXT 3
#define ERROR_QUONG_NO_PREV 4
#define ERROR_QUONG_EMPTY 5
#define ERROR_QUONG_INDEX 6

#define OPERATION_QUONG_DELETE 0
#define OPERATION_QUONG_INSERT 1

#define QUONG_PLAY_MODE_LINEAR 0
#define QUONG_PLAY_MODE_LOOP 1

typedef struct {
    char title[256];
    char author[256];
    /** "0000-00-00" + "\0" */
    char release_date[11];
} Song;

typedef struct QuongNode {
    Song *song;
    size_t index;

    struct QuongNode *prev;
    struct QuongNode *next;
} QuongNode;

typedef struct {
    size_t _size;
    unsigned short _play_mode;

    QuongNode *start;

    QuongNode *front;
    QuongNode *rear;
} Quong;

QuongNode *quong_node_new(Song *song, size_t index) {
    QuongNode *quong_node = calloc(1, sizeof(QuongNode));
    quong_node->song = song;
    quong_node->index = index;

    return quong_node;
}

short quong_next(Quong *quong) {
    if (!quong) {
        return ERROR_QUONG_NULL;
    }
    if (!quong->front) {
        return ERROR_QUONG_EMPTY;
    }
    if (!quong->front->next) {
        return ERROR_QUONG_NO_NEXT;
    }

    quong->front = quong->front->next;

    return SUCCESS_QUONG_OPERATION;
}

short quong_prev(Quong *quong) {
    if (!quong) {
        return ERROR_QUONG_NULL;
    }
    if (!quong->front) {
        return ERROR_QUONG_EMPTY;
    }
    if (!quong->front->prev) {
        return ERROR_QUONG_NO_PREV;
    }

    quong->front = quong->front->prev;

    return SUCCESS_QUONG_OPERATION;
}

short quong_enqueue(Quong *quong, Song *song) {
    if (!quong) {
        return ERROR_QUONG_NULL;
    }
    if (!song) {
        return ERROR_SONG_NULL;
    }

    QuongNode *enqueuing_node = quong_node_new(song, quong->_size);
    QuongNode *current_node = quong->front;

    if (!current_node) {
        quong->front = enqueuing_node;
        quong->rear = enqueuing_node;
        quong->_size += 1;

        if (quong->_play_mode == QUONG_PLAY_MODE_LOOP) {
            quong->front->prev = quong->rear;
            quong->rear->next = quong->front;
        }

        return SUCCESS_QUONG_OPERATION;
    }

    while (current_node->next && current_node->next != quong->front) {
        current_node = current_node->next;
    }

    current_node->next = enqueuing_node;
    enqueuing_node->prev = current_node;

    quong->rear = enqueuing_node;

    if (quong->_play_mode == QUONG_PLAY_MODE_LOOP) {
        quong->front->prev = quong->rear;
        quong->rear->next = quong->front;
    }

    quong->_size += 1;

    return SUCCESS_QUONG_OPERATION;
}

short quong_dequeue(Quong *quong) {
    if (!quong) {
        return ERROR_QUONG_NULL;
    }
    if (!quong->front) {
        return ERROR_QUONG_EMPTY;
    }

    if (!quong->front->next) {
        free(quong->front);
        quong->front = NULL;
        quong->rear = NULL;
        quong->_size = 0;
        return SUCCESS_QUONG_OPERATION;
    }

    quong->front = quong->front->next;

    free(quong->front->prev);

    if (quong->_play_mode == QUONG_PLAY_MODE_LOOP) {
        quong->front->prev = quong->rear;
        quong->rear->next = quong->front;
    } else {
        quong->front->prev = NULL;
        quong->rear->next = NULL;
    }

    quong->_size -= 1;

    return SUCCESS_QUONG_OPERATION;
}

short quong_toggle_loop(Quong *quong) {
    if (!quong) {
        return ERROR_QUONG_NULL;
    }

    quong->_play_mode = quong->_play_mode == QUONG_PLAY_MODE_LOOP
                            ? QUONG_PLAY_MODE_LINEAR
                            : QUONG_PLAY_MODE_LOOP;

    if (!quong->front) {
        return SUCCESS_QUONG_OPERATION;
    }

    size_t initial_index = quong->front->index;
    for (int i = 0; i < initial_index; i++) {
        quong_prev(quong);
    }

    if (quong->_play_mode == QUONG_PLAY_MODE_LOOP) {
        quong->front->prev = quong->rear;
        quong->rear->next = quong->front;
    } else {
        quong->front->prev = NULL;
        quong->rear->next = NULL;
    }

    for (int i = 0; i < initial_index; i++) {
        quong_next(quong);
    }

    return SUCCESS_QUONG_OPERATION;
}

short quong_fifo_index_operation(unsigned short operation, Quong *quong, Song *song, size_t index) {
    if (!quong) {
        return ERROR_QUONG_NULL;
    }
    if (operation == OPERATION_QUONG_INSERT && !song) {
        return ERROR_SONG_NULL;
    }
    if (!quong->front && index > 0) {
        return ERROR_QUONG_EMPTY;
    }

    if (operation == OPERATION_QUONG_INSERT && (index < 0 || index > quong->_size)) {
        return ERROR_QUONG_INDEX;
    }
    if (operation == OPERATION_QUONG_DELETE && (index < 0 || index >= quong->_size)) {
        return ERROR_QUONG_INDEX;
    }

    if (operation == OPERATION_QUONG_INSERT && index == quong->_size) {
        return quong_enqueue(quong, song);
    }

    Quong helper = {0};
    size_t initial_index = quong->front->index;

    bool should_retoggle_loop = quong->_play_mode == QUONG_PLAY_MODE_LOOP;

    for (int i = 0; i < initial_index; i++) {
        quong_prev(quong);
    }
    if (should_retoggle_loop) {
        quong_toggle_loop(quong);
    }

    while (quong->front) {
        size_t current_index = quong->front->index;

        if (index == current_index && operation == OPERATION_QUONG_INSERT) {
            quong_enqueue(&helper, song);

            if (current_index <= initial_index) {
                initial_index += 1;
            }
        }

        if (index != current_index || (index == current_index && operation != OPERATION_QUONG_DELETE)) {
            quong_enqueue(&helper, quong->front->song);
        }

        quong_dequeue(quong);
    }

    while (helper.front) {
        quong_enqueue(quong, helper.front->song);
        quong_dequeue(&helper);
    }

    if (should_retoggle_loop) {
        quong_toggle_loop(quong);
    }
    for (int i = 0; i < initial_index; i++) {
        quong_next(quong);
    }

    return SUCCESS_QUONG_OPERATION;
}

short quong_fifo_for_each(Quong *quong, void(callback)(size_t, QuongNode *, Quong *)) {
    if (!quong) {
        return ERROR_QUONG_NULL;
    }
    if (!quong->front) {
        return ERROR_QUONG_EMPTY;
    }

    Quong helper = {0};

    size_t initial_index = quong->front->index;

    bool should_retoggle_loop = quong->_play_mode == QUONG_PLAY_MODE_LOOP;

    for (int i = 0; i < initial_index; i++) {
        quong_prev(quong);
    }
    if (should_retoggle_loop) {
        quong_toggle_loop(quong);
    }

    while (quong->front) {
        callback(initial_index, quong->front, quong);

        quong_enqueue(&helper, quong->front->song);
        quong_dequeue(quong);
    }

    while (helper.front) {
        quong_enqueue(quong, helper.front->song);
        quong_dequeue(&helper);
    }

    if (should_retoggle_loop) {
        quong_toggle_loop(quong);
    }
    for (int i = 0; i < initial_index; i++) {
        quong_next(quong);
    }

    return SUCCESS_QUONG_OPERATION;
}

short quong_restart(Quong *quong) {
    if (!quong) {
        return ERROR_QUONG_NULL;
    }
    if (!quong->front) {
        return ERROR_QUONG_EMPTY;
    }

    size_t initial_index = quong->front->index;

    for (int i = 0; i < initial_index; i++) {
        quong_prev(quong);
    }

    return SUCCESS_QUONG_OPERATION;
}

short quong_dequeue_all(Quong *quong) {
    if (!quong) {
        return ERROR_QUONG_NULL;
    }

    if (!quong->front) {
        return SUCCESS_QUONG_OPERATION;
    }

    size_t initial_index = quong->front->index;

    for (int i = 0; i < initial_index; i++) {
        quong_prev(quong);
    }
    if (quong->_play_mode == QUONG_PLAY_MODE_LOOP) {
        quong_toggle_loop(quong);
    }

    while (quong->front) {
        quong_dequeue(quong);
    }

    return SUCCESS_QUONG_OPERATION;
}

const char *quong_get_operation_status_label(short status) {
    switch (status) {
        case SUCCESS_QUONG_OPERATION:
            return "Success quong operation";
        case ERROR_QUONG_EMPTY:
            return "Quong is empty";
        case ERROR_QUONG_INDEX:
            return "Quong index out of range";
        case ERROR_QUONG_NO_NEXT:
            return "No next song";
        case ERROR_QUONG_NO_PREV:
            return "No previous song";
        case ERROR_QUONG_NULL:
            return "Quong is null";
        case ERROR_SONG_NULL:
            return "Song is null";
        default:
            return "Unknown error";
    }
}

const char *quong_get_play_mode_label(short play_mode) {
    switch (play_mode) {
        case QUONG_PLAY_MODE_LINEAR:
            return "Linear";
        case QUONG_PLAY_MODE_LOOP:
            return "Loop";
        default:
            return "Unknown";
    }
}
