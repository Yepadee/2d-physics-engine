#include "Data Structures\QTree.h"

struct RigidBodies {
    ArrayList *rbArray;
    QTree *q;
};
typedef struct RigidBodies RigidBodies;

//Constructors:
RigidBodies *newRigidBodies(int w, int h, int capacity);
RigidBody *getRigidBody(RigidBodies *rbs, int i);
void addRigidBody(RigidBodies *rbs, RigidBody *rb);
int numRigidBodies(RigidBodies *rbs);

//Updating:
void collideAll(RigidBodies *rbs);
void resolveAllPen(RigidBodies *rbs);
void moveRigidBodies(RigidBodies *rbs);
void updateRigidBodies(RigidBodies *rbs);
//Drawing:
void drawRigidBodies(RigidBodies *rbs, SDL_Renderer *renderer);
void drawRigidBodiesAABB(RigidBodies *rbs, SDL_Renderer *renderer);
