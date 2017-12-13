#include "Data\RigidBody.h"

typedef RigidBody Type;

struct ArrayList {
    int length; //Number of polygons.
    int capacity;
    Type **items;
};
typedef struct ArrayList ArrayList;

ArrayList *newArrayList(int capacity);
void destroy(ArrayList *arrayList);
int length(ArrayList *arrayList);
void expand(ArrayList *arrayList);
void append(ArrayList *arrayList, Type *t);
void removeArr(ArrayList *arrayList, int i);
Type *get(ArrayList *arrayList, int i);
RigidBody *pop(ArrayList *arrayList, int i);
