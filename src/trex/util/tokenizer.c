/**
 * @file tokenizer.c
 * @author Gael Herrera (https://github.com/buebos)
 * @brief Minimal tokenizer implementation. This is my favorite file from the
 * project.
 * @version 0.1
 * @date 2024-04-18
 *
 *
 * @todo
 * 1. Get the current index of the token like: tokenizer._input._cursor - strlen(token)
 * it is kind of buggy and strange when previous ignore chars are before the current
 * token or delimiter
 *
 * 2. Add option to skip over the delimiters as tokens. Maybe it would be better to
 * skip them by default and add the opposite option to include them as tok_next results
 */

#include "tokenizer.h"

Tokenizer tok_new(char *input, TokStrArray delimiters, TokStrArray skips, size_t token_alloc_char_size) {
    Tokenizer tokenizer = {0};

    tokenizer._input._data = input;
    tokenizer._input._size = strlen(input);
    tokenizer._input._cursor = 0;

    tokenizer._delimiters = delimiters;

    tokenizer._skips = skips;

    tokenizer.token_alloc_char_size = token_alloc_char_size;

    tokenizer._token_current._data = calloc(tokenizer.token_alloc_char_size, sizeof(char));
    tokenizer._token_current._size = token_alloc_char_size;

    return tokenizer;
}

char *tok_match_incoming_str_from(Tokenizer *tokenizer, TokStrArray str_arr) {
    char *str_best_match = NULL;
    size_t str_best_match_len = 0;

    size_t tokenizer_input_remaining_chars = (tokenizer->_input._size - tokenizer->_input._cursor) / sizeof(char);

    char *tok_input_cursor_ptr = &tokenizer->_input._data[tokenizer->_input._cursor];

    for (size_t i = 0; i < str_arr._length; i++) {
        char *str = str_arr._data[i];
        size_t str_len = strlen(str);

        if (tokenizer_input_remaining_chars < str_len) {
            continue;
        }

        char *delimiter_start_ptr = strstr(tok_input_cursor_ptr, str);

        if (
            /** The str has to begin right at the current index */
            delimiter_start_ptr == tok_input_cursor_ptr &&
            /**
             * This ensures the str with the max length is
             * match if there are two similar delimiters.
             */
            str_best_match_len <= str_len

        ) {
            str_best_match = str;
            str_best_match_len = str_len;
        }
    }

    return str_best_match;
}

char *tok_set_current_token(Tokenizer *tokenizer, char *token_next) {
    TokReadable *input = &tokenizer->_input;
    TokStrArray *delimiters = &tokenizer->_delimiters;

    TokReadable *token = &tokenizer->_token_current;

    token->_size = strlen(token_next);
    token->_data = realloc(token->_data, token->_size * sizeof(char));
    token->_cursor = 0;

    strcpy(token->_data, token_next);

    input->_cursor += token->_size;

    return token->_data;
}

void tok_reset_current_token(Tokenizer *tokenizer) {
    TokReadable *token = &tokenizer->_token_current;

    token->_data = realloc(token->_data, tokenizer->token_alloc_char_size * sizeof(char));
    memset(token->_data, 0, tokenizer->token_alloc_char_size);

    token->_size = 0;
    token->_cursor = 0;
}

char *tok_next(Tokenizer *tokenizer) {
    char inchar = 0;
    TokReadable *input = &tokenizer->_input;
    TokStrArray *delimiters = &tokenizer->_delimiters;

    TokReadable *token = &tokenizer->_token_current;

    if (input->_size == input->_cursor) {
        if (token->_data) {
            free(token->_data);
            token->_data = NULL;
            token->_cursor = -1;
            token->_size = 0;
        }

        return NULL;
    }

    tok_reset_current_token(tokenizer);

    while ((inchar = input->_data[input->_cursor])) {
        char *skippable_incoming_match = tok_match_incoming_str_from(tokenizer, tokenizer->_skips);

        if (skippable_incoming_match) {
            input->_cursor += strlen(skippable_incoming_match);

            if (input->_cursor == input->_size) {
                return token->_data;
            }

            continue;
        }

        char *delimiter_incoming_match = tok_match_incoming_str_from(tokenizer, tokenizer->_delimiters);

        if ((delimiter_incoming_match && token->_size)) {
            return token->_data;
        }
        if (delimiter_incoming_match && !token->_size) {
            return tok_set_current_token(tokenizer, delimiter_incoming_match);
        }

        token->_data[token->_cursor] = inchar;
        token->_size += 1;
        token->_cursor += 1;
        input->_cursor += 1;

        if (input->_cursor == input->_size) {
            return token->_data;
        }
    }

    return NULL;
}
