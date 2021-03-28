#include <stdio.h>
#include <stdlib.h>

#include "onegin.h"

int main() {
    struct string** key = NULL;
    struct string** orig = NULL;
    int bufsize = 0, nlines = 0;
    char* buf = NULL;

    countSize(&bufsize);
    buf = (char*) calloc (bufsize + 2, sizeof(char));
    if (buf == NULL) {
        printf("Calloc error!\n");
        return EXIT_FAILURE;
    }

    fillBuf(buf, bufsize);
    countLines(buf, &nlines, bufsize);
    key = (struct string**) calloc(nlines + 2, sizeof(struct string*));
    if (key == NULL) {
        printf("Calloc error!\n");
        return EXIT_FAILURE;
    }

    fillStringStructWithBuf(buf, nlines, key, bufsize);
    sortThemAndOutputThem(key, nlines);
    clearMemory(key, buf, nlines);

    return 0;
}