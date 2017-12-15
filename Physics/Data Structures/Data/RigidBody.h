#include "Geometry\Polygon.h"


struct RigidBody {
    Polygon *polygon;

    double mass;
    double invMass;
    double mOfInertia;
    double invMOfInertia;
    double xv;
    double yv;
    double w;
    double e;

    bool canMove;
    double xDist;
    double yDist;
};
typedef struct RigidBody RigidBody;

bool isImmovable(RigidBody *rb);
RigidBody *newRigidBody(double mass, double e, bool immovable, Polygon *p);

//Drawing:
void drawRigidBody(RigidBody *rb, SDL_Renderer *renderer);

//Physics:
void collide(RigidBody *rb1, RigidBody *rb2);
void resolveRBPen(RigidBody *rb1, RigidBody *rb2);

//Updating:
void moveRigidBody(RigidBody *rb, Vector2D dp);
void applyGravity(RigidBody *rb);
void updateRigidBody(RigidBody *rb, double dt);
