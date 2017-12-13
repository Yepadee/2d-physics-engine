#include "RigidBodies.h"

//Constructors:
RigidBodies *newRigidBodies(int w, int h, int capacity) {
    RigidBodies *rbs = malloc(sizeof(RigidBodies));
    ArrayList *rbArray = newArrayList(capacity);
    rbs->rbArray = rbArray;
    rbs->q = newQTree(0, 0, 0, w, h);
    return rbs;
}
RigidBody *getRigidBody(RigidBodies *rbs, int i) {
    return get(rbs->rbArray, i);
}
void addRigidBody(RigidBodies *rbs, RigidBody *rb) {
    append(rbs->rbArray, rb);
}
void deleteRigidBody(RigidBodies *rbs, int i) {
    free(getRigidBody(rbs, i));
    removeArr(rbs->rbArray, i);
}
int numRigidBodies(RigidBodies *rbs) {
    return length(rbs->rbArray);
}

//Updating:
void collideAll(RigidBodies *rbs) {
    for (int i = 0; i < numRigidBodies(rbs); i ++) {
        RigidBody *rb1 = getRigidBody(rbs, i);
        ArrayList *toCollide = getCloseRBS(rbs->q, rb1);
        for (int j = 0; j < length(toCollide); j ++) {
            RigidBody *rb2 = get(toCollide, j);
            collide(rb1, rb2);
        }
    }
}
void resolveAllPen(RigidBodies *rbs) {
    for (int i = 0; i < numRigidBodies(rbs); i ++) {
        RigidBody *rb1 = getRigidBody(rbs, i);
        ArrayList *toResolve = getCloseRBS(rbs->q, rb1);
        for (int j = 0; j < length(toResolve); j ++) {
            RigidBody *rb2 = get(toResolve, j);
            resolveRBPen(rb1, rb2);
        }
    }
}
void moveRigidBodies(RigidBodies *rbs) {
    for (int i = 0; i < numRigidBodies(rbs); i ++) {
        RigidBody *rb = getRigidBody(rbs, i);
        updateRigidBody(rb);
    }
}
void updateRigidBodies(RigidBodies *rbs) {
    updateQTree(rbs->q, rbs->rbArray);
}
//Drawing:
void drawRigidBodies(RigidBodies *rbs, SDL_Renderer *renderer) {
    for (int i = 0; i < numRigidBodies(rbs); i ++) {
        drawRigidBody(getRigidBody(rbs, i), renderer);
    }
}
void drawRigidBodiesAABB(RigidBodies *rbs, SDL_Renderer *renderer) {
    for (int i = 0; i < numRigidBodies(rbs); i ++) {
        drawAABB(getRigidBody(rbs, i)->polygon, renderer);
    }
}
//TO REMOVE.
