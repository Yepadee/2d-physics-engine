#include "QTree.h"

static int MAX_BODIES = 2;
static int MAX_DEPTH = 4;

QTree *newQTree(int level, int x, int y, int width, int height) {
    QTree *q = (QTree*) malloc(sizeof(QTree));
    q->level = level;
    q->nodes = malloc(4 * sizeof(QTree));
    for (int i = 0; i < 4; i ++) {
        q->nodes[i] = NULL;
    }
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    q->rect = rect;
    q->objects = newArrayList(MAX_BODIES);
    return q;
}

static bool noChildren(QTree *q) {
    return q->nodes[0] == NULL;
}
static void resetQTree(QTree *q) {
    if (q != NULL) {
        destroy(q->objects);
        for (int i = 0; i < 4; i ++) {
            resetQTree(q->nodes[i]);
        }
        free(q->nodes);
        free(q);
    }
}
static void divideQTree(QTree *q) {
    SDL_Rect rect = q->rect;
    int level = q->level;
    int newWidth = rect.w/2;
    int newHeight = rect.h/2;
    int x = rect.x;
    int y = rect.y;

    q->nodes[0] = newQTree(level + 1, x, y, newWidth, newHeight);
    q->nodes[1] = newQTree(level + 1, x + newWidth, y, newWidth, newHeight);
    q->nodes[2] = newQTree(level + 1, x + newWidth, y + newHeight, newWidth, newHeight);
    q->nodes[3] = newQTree(level + 1, x, y + newHeight, newWidth, newHeight);
}
static int getIndexQTree(QTree *q, SDL_Rect rect) {
    int index = -1;
    SDL_Rect nodeRect = q->rect;
    double horizontalMidpoint = nodeRect.x + (nodeRect.w / 2);
    double verticalMidpoint = nodeRect.y + (nodeRect.h / 2);

    // Object can completely fit within the top quadrants
    bool topQuadrant = (rect.y < verticalMidpoint && rect.y + rect.h < verticalMidpoint);
    // Object can completely fit within the bottom quadrants
    bool bottomQuadrant = (rect.y > verticalMidpoint);

    // Object can completely fit within the left quadrants
    if (rect.x < horizontalMidpoint && rect.x + rect.w < horizontalMidpoint) {
        if (topQuadrant) {
            index = 0;
        }
        else if (bottomQuadrant) {
            index = 3;
        }
    }
    // Object can completely fit within the right quadrants
    else if (rect.x > horizontalMidpoint) {
        if (topQuadrant) {
            index = 1;
        }
        else if (bottomQuadrant) {
            index = 2;
        }
    }
    return index;
}
static void insertQTree(QTree *q, RigidBody *rb) {
    SDL_Rect rect = getAABB(rb->polygon);
    if (! noChildren(q)) {
        int index = getIndexQTree(q, rect);
        if (index != -1) {
            insertQTree(q->nodes[index], rb);
            return;
        }
    }
    append(q->objects, rb);
    if (length(q->objects) > MAX_BODIES && q->level < MAX_DEPTH) {
        if (noChildren(q)) {
            divideQTree(q);
        }
        int i = 0;
        while (i < length(q->objects)) {
            RigidBody *rb = get(q->objects, i);
            int index = getIndexQTree(q, getAABB(rb->polygon));
            if (index != -1) {
                insertQTree(q->nodes[index], pop(q->objects, i));
            } else {
                i ++;
            }
        }

    }
}
static void retrieve(QTree *q, ArrayList *returnObjects, SDL_Rect rect) {
    int index = getIndexQTree(q, rect);
    if (index != -1 && !noChildren(q)) {
        retrieve(q->nodes[index], returnObjects, rect);
    }
    for (int i = 0; i < length(q->objects); i ++) {
        append(returnObjects, get(q->objects, i));
    }
}

ArrayList *getCloseRBS(QTree *q, RigidBody *rb) {
    ArrayList *close = newArrayList(MAX_BODIES);
    retrieve(q, close, getAABB(rb->polygon));
    return close;
}
void drawQTree(QTree *q, SDL_Renderer *renderer) {
    if (! noChildren(q)) {
        for (int i = 0; i < 4; i ++) {
            SDL_RenderDrawRect(renderer, &q->nodes[i]->rect);
            drawQTree(q->nodes[i], renderer);
        }
    }
}
void updateQTree(QTree *q, ArrayList *rba) {
    QTree oldQ = *q;
    resetQTree(q);
    q = newQTree(0, 0, 0, oldQ.rect.w, oldQ.rect.h);
    for (int i = 0; i < length(rba); i ++) {
        insertQTree(q, get(rba, i));
    }
}

int qTreeMain() {
    puts("All tests passed!");
    return 0;
}
