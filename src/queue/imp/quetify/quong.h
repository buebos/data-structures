#ifndef QUONG_H
#define QUONG_H

#include <stdbool.h>
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

QuongNode *quong_node_new(Song *song);
short quong_enqueue(Quong *quong, Song *song);
short quong_dequeue(Quong *quong);
short quong_toggle_loop(Quong *quong);
short quong_next(Quong *quong);
short quong_prev(Quong *quong);
short quong_fifo_index_operation(unsigned short operation, Quong *quong, Song *song, size_t index);
short quong_fifo_for_each(Quong *quong, void(callback)(size_t i, QuongNode *, Quong *));
short quong_restart(Quong *quong);

short quong_dequeue_all(Quong *quong);

const char *quong_get_play_mode_label(short play_mode);
const char *quong_get_operation_status_label(short status);

#endif /* QUONG_H */
