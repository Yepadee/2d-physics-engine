#include "ArrayList.h"

struct QTree {
    int level;
    ArrayList *objects;
    SDL_Rect rect;
    struct QTree **nodes;
};
typedef struct QTree QTree;

QTree *newQTree(int level, int x, int y, int width, int height);
ArrayList *getCloseRBS(QTree *q, RigidBody *rb);
void drawQTree(QTree *q, SDL_Renderer *renderer);
void updateQTree(QTree *q, ArrayList *rbs);
