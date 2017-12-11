#include "Geometry\Polygon.h"


struct RigidBody {
    Polygon *polygon;

    double mass;
    double invMass;
    double xv;
    double yv;
    double e;

    bool canMove;
    int xDist;
    int yDist;
};
typedef struct RigidBody RigidBody;


RigidBody *newRigidBody(double mass, double e, bool immovable, Polygon *p);

//Drawing:
void drawRigidBody(RigidBody *rb, SDL_Renderer *renderer);

//Physics:
void collide(RigidBody *rb1, RigidBody *rb2);
void resolveRBPen(RigidBody *rb1, RigidBody *rb2);

//Updating:
void moveRigidBody(RigidBody *rb, Vector2D dp);
void updateRigidBody(RigidBody *rb);
