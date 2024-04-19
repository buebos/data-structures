#ifndef __TREX_PARSER_H__

#define __TREX_PARSER_H__

#include "../util/tokenizer.h"
#include "trex/trex.h"

Trex *trex_parse_from(char *expression, TokStrArray *skip_strings, OperatorArray *operators);

#endif  // !__TREX_PARSER_H__