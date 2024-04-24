#ifndef __TREX_COMPONENTS_H__

#define __TREX_COMPONENTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/trex-parser.h"
#include "../core/trex/trex.h"

#define MAX_EXPR_LEN 256
#define MAX_VAR_LEN 32

typedef struct {
    Trex *trex;
    int current_menu;
    int should_run;

    OperatorArray *operators;
    TokStrArray *skips;
} App;

void MainMenu(App *app);

void Logout();

#endif  // !__TREX_COMPONENTS_H__