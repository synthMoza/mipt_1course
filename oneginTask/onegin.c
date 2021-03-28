#include "onegin.h"

void countSize(int * bufsize) {
    assert(bufsize != NULL);

    FILE* my_file = fopen(INPUT, "rb");

    assert(my_file != NULL);

    fseek(my_file, 0L, SEEK_END);
    *bufsize = (int) ftell(my_file);
    fclose(my_file);
}

void fillBuf(char* buf, int bufsize) {
    assert(buf != NULL);
    assert(bufsize != 0);

    int my_file = open(INPUT, O_RDONLY);
    read(my_file, buf, bufsize);
    close(my_file);
}

void countLines(char* buf, int* nlines, int bufsize) {
    assert(buf != NULL);
    assert(nlines != 0);
    assert(bufsize != 0);

    char* temp_buf = buf;
    int n_lines = 0;
    int bsize = bufsize;
    char* sign_ptr = NULL;
    while ((sign_ptr = (char*) memchr( temp_buf, '\n', bsize)) != NULL) {
        n_lines++;
        bsize += (int) (temp_buf - sign_ptr);
        temp_buf = sign_ptr + 1;
    }
    *nlines = n_lines;
}

void fillStringStructWithBuf(char* buf, int nlines, struct string* key[], int bufsize) {
    assert(buf != NULL);
    assert(nlines != 0);
    assert(key != NULL);
    assert(bufsize != 0);

    int struct_counter = 0, bsize = bufsize;
    char* sign_ptr = buf;

    sign_ptr = (char*) memchr(buf, '\n', bsize);
    key[0] = (struct string*) calloc(1, sizeof(struct string));
    key[0]->value = buf;

    sign_ptr = buf;
    for (struct_counter = 1; struct_counter < nlines; struct_counter++) {
        key[struct_counter] = (struct string*) calloc(1, sizeof(struct string)); //allocate memory for a string

        sign_ptr = (char*) memchr(sign_ptr, '\n', bsize); //find lines
        *sign_ptr = '\0';

        key[struct_counter]->value = sign_ptr + 1; //point to the next line
        key[struct_counter - 1]->length = (int) (sign_ptr - key[struct_counter - 1]->value);
        bsize = bsize - key[struct_counter - 1]->length;
    }
    if ((sign_ptr = (char*) memchr(sign_ptr, '\n', bsize)) == NULL)
        sign_ptr = (char*) memchr(sign_ptr, '\0', bsize);
    *sign_ptr = '\0';
    key[struct_counter - 1]->length = (int) (sign_ptr - key[struct_counter - 1]->value);

}
//! This functions say if the symbol is the punctuation symbol
//! @param symbol The input symbol
//! @return 1 if it is true, 0 if not
static int ispunctmark(char symbol) {
    return (symbol == '.' || symbol == ',' || symbol == ':' ||
            symbol == ';' || symbol == '-' || symbol == ')' ||
            symbol == '\?' || symbol == '!' || symbol == ']' ||
            symbol == '\\' || symbol == '(' || symbol == '[' ||
            symbol == ' ' || symbol == '\'' || symbol == '\"' ||
            symbol == '{' || symbol == '}');
}

//! Comparator for structure string(strcmp backwards)
//! @param s1 The first element
//! @param s2 The second element
static int strcmp_backwards(const void* s1,const void* s2) {
    assert(s1 != NULL);
    assert(s2 != NULL);

    string* s1_converted = (string*) s1;
    string* s2_converted = (string*) s2;

    char* str1 = s1_converted->value + s1_converted->length - 1;
    char* str2 = s2_converted->value + s2_converted->length - 1;

    char* str1_start = s1_converted->value;
    char* str2_start = s2_converted->value;

    while (ispunctmark(*str1))
        str1--;
    while (ispunctmark(*str2))
        str2--;
    while (*str1 != *str1_start || *str2 != *str2_start) {
        if (*str1 > *str2)
            return 1;
        else if (*str1 < *str2)
            return -1;
        str1--;
        str2--;
    }    return 0;
}

//! Comparator for structure string(forward)
//! @param s1 The first element
//! @param s2 The second element
static int strcmp_forward(const void* s1, const void *s2) {
    assert(s1 != NULL);
    assert(s1 != NULL);

    return strcmp(((string*)s1)->value, ((string*)s2)->value);
}

//! This function swaps values from string
//! @param v The array of strings (structure)
//! @param m index 1
//! @param n index 2
static void swap(string* v[], long m, long n) {
    assert(v != NULL);

    string *temp = NULL;
    temp = v[m];
    v[m] = v[n];
    v[n] = temp;
}

//! qSort for string array
//! @param v The array of strings
//! @param The left index
//! @param The right index
//! @param comp Comparator
static void qSort(string* v[], long left, long right, int (*comp)(void*, void*)) {
    assert(v != NULL);

    long counter, last;

    if (left >= right)
        return ;

    last = left;
    swap(v, left, (right+left) / 2);
    for (counter = left + 1; counter <= right; counter++) {
        if ((*comp)(v[counter], v[left]) < 0)
            swap(v, counter, ++last);
    }

    swap(v, left, last);
    qSort(v, left, last - 1, comp);
    qSort(v, last + 1, right, comp);
}

void sortThemAndOutputThem(struct string* key[], int nlines) {
    assert(key != NULL);
    assert(nlines != 0);

    FILE* my_file = freopen(OUTPUT, "wt", stdout);
    int counter = 0;

    string** orig = (string**) calloc( nlines + 2, sizeof(string*));

    for (counter = 0; counter < nlines; counter++) {
        orig[counter] = key[counter];
    }

    qSort(key, 0, nlines - 1, (int (*)(void *, void *)) strcmp_forward);
    for (counter = 0; counter < nlines; counter++) {
        printf("%s\n", key[counter]->value);
    };

    printf("------------------------------------------------------------------\n");
    printf("\n");

    qSort(key, 0, nlines - 1, (int (*)(void *, void *)) strcmp_backwards);
    for (counter = 0; counter < nlines; counter++) {
        printf("%s\n", key[counter]->value);
    };

    printf("------------------------------------------------------------------\n");
    printf("\n");

    for (counter = 0; counter < nlines; counter++) {
        printf("%s\n", orig[counter]->value);
    };

    fclose(my_file);
    free(orig);
}

void clearMemory(string* key[], char* buf, int nlines) {
    assert(key != NULL);
    assert(buf != NULL);
    assert(nlines != 0);

    int counter = 0;

    for (counter = 0; counter < nlines + 2;  counter++) {
        free(key[counter]);
    }

    free(buf);
    free(key);
}