#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **_data;
    size_t _length;
} TokStrArray;

typedef struct {
    char *_data;
    size_t _cursor;
    size_t _size;
} TokReadable;

typedef char *(*TokenizerGetNext)(void);

typedef struct Tokenizer {
    size_t token_alloc_char_size;

    TokReadable _input;

    TokStrArray _delimiters;
    TokStrArray _skips;

    TokReadable _token_current;
} Tokenizer;

Tokenizer tok_new(char *input, TokStrArray delimiters, TokStrArray skips, size_t token_alloc_chunk_size);
char *tok_match_incoming_str_from(Tokenizer *tokenizer, TokStrArray str_arr);
char *tok_set_current_token(Tokenizer *tokenizer, char *token_next);
void tok_reset_current_token(Tokenizer *tokenizer);
char *tok_next(Tokenizer *tokenizer);

#endif  // __TOKENIZER_H__