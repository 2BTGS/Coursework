#include "algorithm.h"
#include <stdlib.h>
#include <string.h>

char *algorithm(char *input)
{
    char *result;
    int len;
    int i;
    char c;

    if (input == NULL) {
        return NULL;
    }

    len = strlen(input);
    result = malloc((len + 1) * sizeof(char));

    if (result == NULL) {
        return NULL;
    }

    for (i = 0; i < len; i++) {
        c = input[i];

        if (c == ' ') {
            c = 'h';
        } else if (c == 'h') {
            c = ' ';
        } else if (c == ',') {
            c = 'i';
        } else if (c >= '0' && c <= '9') {
            c = c + 58;
        } else if (c >= 'a' && c <= 'z') {
            if (c <= 'm') {
                c = c + 19;
            } else {
                c = c - 19;
            }
        }

        result[i] = c;
    }

    result[len] = '\0';
    return result;
}
