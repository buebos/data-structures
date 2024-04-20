#include "components.h"

#include "../util/system/clear.h"

void ClearTerminal() {
    clear();
}

void KeyToContinue(App *app) {
    printf("[INPUT]: Type any key to continue: ");
    scanf("%d", &app->current_menu);

    app->current_menu = 0;
}

void PrintHeader() {
    printf("     Expression Parser\n");
    printf("===========================\n\n");
}

void PrintExpression(Trex *trex) {
    if (!trex || !trex->expression) {
        printf("[EXP]: No expression set.\n\n");
        return;
    }

    printf("[EXP]: Current expression:\n");

    printf("  [ORIGINAL]: %s\n", trex->expression);

    printf("  [PARSED]: ");
    trex_print_expression(trex->_root);
    printf("\n");
}

void PrintVariables(Trex *trex) {
    if (trex == NULL || !trex->_var_map._head) {
        return;
    }

    TrexVarMapPair *node = trex->_var_map._head;
    size_t count = 1;

    printf("[VARS]: Variables:\n");

    while (node != NULL) {
        printf("  [%d] %s = %.2f\n", count++, node->key, node->value);
        node = node->next;
    }

    printf("\n");
}

void SetExpression(App *app) {
    ClearTerminal();
    PrintHeader();
    PrintExpression(app->trex);
    PrintVariables(app->trex);

    char *expression = calloc(MAX_EXPR_LEN, sizeof(char));

    printf("[MENU]: Enter the expression: ");
    fgets(expression, MAX_EXPR_LEN, stdin);

    if (!expression || expression[0] == '\n') {
        printf("[ERROR]: Please type a valid expression.\n");

        KeyToContinue(app);
        return;
    }

    expression[strcspn(expression, "\n")] = '\0';

    if (app->trex != NULL) {
        trex_free(app->trex);
    }

    app->trex = trex_parser_parse_from(expression, app->skips, app->operators);

    if (app->trex) {
        return;
    }

    printf("[ERROR]: Please type a valid expression.\n");

    KeyToContinue(app);
}

void SetVariables(App *app) {
    char var_name[MAX_VAR_LEN];
    float var_value;

    while (1) {
        ClearTerminal();
        PrintHeader();

        PrintExpression(app->trex);
        PrintVariables(app->trex);

        printf("[MENU]: Set variables:\n\n");

        if (!app->trex || !app->trex->_root) {
            printf("[ERROR]: No expression set.\n");

            KeyToContinue(app);

            return;
        }

        printf("[INPUT]: Enter variable name (or '\\q' to quit): ");
        scanf("%s", var_name);

        if (strcmp(var_name, "\\q") == 0) {
            break;
        }

        if (!trex_varmap_has_key(&app->trex->_var_map, var_name)) {
            printf("[ERROR]: Invalid variable key.\n\n");

            KeyToContinue(app);

            continue;
        }

        printf("[INPUT]: Enter variable value: ");

        if (scanf("%f", &var_value) != 1) {
            printf("[ERROR]: Invalid input for variable value.\n");
            continue;
        }

        trex_varmap_set(&app->trex->_var_map, var_name, var_value);
    }
}

void GetResult(App *app) {
    ClearTerminal();
    PrintHeader();
    PrintExpression(app->trex);
    PrintVariables(app->trex);

    if (!app->trex || !app->trex->_root) {
        printf("[ERROR]: No expression set.\n");
    } else {
        float result = trex_get_result(app->trex);

        printf("[INFO]: Result: ");

        trex_print_expression(app->trex->_root);

        printf(" = %f\n", result);
    }

    printf("\n");

    KeyToContinue(app);
}

void MainMenu(App *app) {
    int choice = 0;

    while (app->should_run) {
        ClearTerminal();
        PrintHeader();

        PrintExpression(app->trex);
        PrintVariables(app->trex);

        printf("[INFO]: Choose an option:\n\n");

        printf("[1]. Set expression\n");
        printf("[2]. Set variables\n");
        printf("[3]. Get result\n");
        printf("[4]. Exit\n\n");

        printf("[INPUT]: ");
        scanf("%d", &choice);

        getchar();  // Consume newline character

        switch (choice) {
            case 1:
                SetExpression(app);
                break;
            case 2:
                SetVariables(app);
                break;
            case 3:
                GetResult(app);
                break;
            case 4:
                app->should_run = 0;
                break;
            default:
                printf("[ERROR]: Invalid choice.\n");
                break;
        }
    }

    ClearTerminal();
    PrintHeader();

    printf("[BYE]: See you in space cowboy!\n");
}
