#include "list.h"

int main() {
    list_t* my_list;
    my_list = (struct list*) calloc(1, sizeof(*my_list));

    listConstruct(my_list);
    LIST_INSERT_FIRST(my_list, 5);
    listPrint(*my_list);

    free(my_list);
    return 0;
}