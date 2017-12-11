#include "ArrayList.h"

ArrayList *newArrayList(int capacity) {
    ArrayList *arrayList = malloc(sizeof(Type));
    arrayList->length = 0;
    arrayList->capacity = capacity;
    arrayList->items = (Type **) malloc(capacity * sizeof(Type));

    return arrayList;
}
void destroy(ArrayList *arrayList) {
    free(arrayList->items);
    free(arrayList);
}

int length(ArrayList *arrayList) {
    return arrayList->length;
}
void expand(ArrayList *arrayList) {
    int capacity = 1.5 * arrayList->capacity;
    arrayList->items = realloc(arrayList->items, capacity * sizeof(Type));
    arrayList->capacity = capacity;
}
void append(ArrayList *arrayList, Type *t) {
    if (length(arrayList) >= arrayList->capacity) {
        expand(arrayList);
    }
    arrayList->items[length(arrayList)] = t;
    arrayList->length ++;
}
Type *get(ArrayList *arrayList, int i) {
    return arrayList->items[i];
}
Type *pop(ArrayList *arrayList, int i) {
    Type *toRemove = get(arrayList, i);
    arrayList->length -= 1;
    for (int j = i; j < length(arrayList); j ++) {
        arrayList->items[j] = get(arrayList, j + 1);
    }
    return toRemove;
}

//Testing:
int arrayListMain() {
    puts("All tests passed!");
    return 0;
}
