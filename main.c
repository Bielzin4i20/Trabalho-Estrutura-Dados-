#include <stdio.h>
#include <stdlib.h>
#include "expressao.h"

int main(void) {
    const char *tests[] = {
        "3 4 + 5 *",
        "7 2 * 4 + 7 * 2 + 4",
        "8 5 2 4 + * + 8 +",
        "6 2 / 3 + 4 *",
        "9 5 2 8 * 4 + * + 9 +",
        "2 3 + log 5 /",
        "10 log 3 ^ 2 +",
        "45 60 + 30 cos *",
        "0.5 45 sen 2 ^ +",
        NULL
    };

    for (int i = 0; tests[i] != NULL; ++i) {
        char *in = getFormaInFixa((char*)tests[i]);
        float val = getValorPosFixa((char*)tests[i]);

        printf("Teste %d:\nPos-fixa: %s\n", i+1, tests[i]);

        if (in) {
            printf("Infixa : %s\n", in);
            free(in);
        } else {
            printf("Infixa : ERRO\n");
        }

        printf("Valor  : %.6f\n\n", val);
    }

    return 0;
}
