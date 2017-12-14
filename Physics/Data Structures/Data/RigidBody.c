#include "RigidBody.h"

//Helper:
bool isImmovable(RigidBody *rb) {
    return rb->invMass == 0.0000;
}
static double min(double x, double y) {
    return x < y ? x : y;
}
static double max(double x, double y) {
    return x > y ? x : y;
}
static void getMOfInertia(RigidBody *rb) {
    rb->mOfInertia = 1;
}

//Constructors:
RigidBody *newRigidBody(double mass, double e, bool immovable, Polygon *p) {
    RigidBody *rb = malloc(sizeof(RigidBody));

    rb->polygon = p;

    rb->mass = mass;

    rb->e = e;
    rb->xv = 0;
    rb->yv = 0;
    rb->w = 0;

    rb->canMove = false;
    rb->xDist = 0;
    rb->yDist = 0;

    if (immovable) {
        rb->invMass = 0;
        rb->invMOfInertia = 0;
    } else {
        rb->invMass = (1/mass);
        rb->invMOfInertia = (1/rb->mOfInertia);
    }

    return rb;
}

//Updating:
void collide(RigidBody *rb1, RigidBody *rb2) {
    Vector2D MTV = getMTV(rb1->polygon, rb2->polygon);
    Vector2D basis = unitV2D(MTV);
    double overlap = magnitudeV2D(MTV);
    if (! equalsV2D(MTV, NOTHING)) {
        Vector2D v1 = {rb1->xv, rb1->yv};
        Vector2D v2 = {rb2->xv, rb2->yv};
        Vector2D velDif = subtractV2D(v2, v1);
        double velAlongNormal = dotV2D(velDif, basis);
        if(velAlongNormal > 0) {
            double e = min(rb1->e, rb2->e);
            float imp = (-(1 + e) * velAlongNormal) / (rb1->invMass + rb2->invMass);

            Vector2D impulse = multiplyV2D(basis, imp);

            rb1->xv -= rb1->invMass * impulse.i;
            rb1->yv -= rb1->invMass * impulse.j;

            rb2->xv += rb2->invMass * impulse.i;
            rb2->yv += rb2->invMass * impulse.j;
        }
        double percent = 0.2; // usually 20% to 80%
        double slop = 0.01; // usually 0.01 to 0.1
        double magnitude = (max(overlap - slop, 0) / (rb1->invMass + rb2->invMass)) * percent;

        Vector2D correction = multiplyV2D(basis, magnitude);
        if (! isImmovable(rb1) && ! isImmovable(rb2)) {
            movePolygon(rb1->polygon, multiplyV2D(correction, rb1->invMass));
            movePolygon(rb2->polygon, multiplyV2D(correction, -rb2->invMass));
        } else if (isImmovable(rb1) && ! isImmovable(rb2)) {
            movePolygon(rb2->polygon, multiplyV2D(correction, rb2->invMass + rb1->invMass));
        } else if (!isImmovable(rb1) && isImmovable(rb2)) {
            movePolygon(rb1->polygon, multiplyV2D(correction, rb2->invMass + rb1->invMass));
        }
    }
}
void resolveRBPen(RigidBody *rb1, RigidBody *rb2) {
    if (! isImmovable(rb1) && ! isImmovable(rb2)){
        resolvePenEq(rb1->polygon, rb2->polygon);
    } else if (isImmovable(rb1) && ! isImmovable(rb2)) {
        resolvePenDom(rb1->polygon, rb2->polygon);
    } else if (! isImmovable(rb1) && isImmovable(rb2)) {
        resolvePenDom(rb2->polygon, rb1->polygon);
    }
}

//Drawing:
void drawRigidBody(RigidBody *rb, SDL_Renderer *renderer) {
    drawPolygon(rb->polygon, renderer);
}

//Updating:
void moveRigidBody(RigidBody *rb, Vector2D dp) {
    movePolygon(rb->polygon, dp);
}
void rotateRigidBody(RigidBody *rb, double angle) {
    rotatePolygon(rb->polygon, angle);
}
void updateRigidBody(RigidBody *rb, double dt) {
    Vector2D dp = {rb->xv * dt, rb->yv * dt};
    if (! isImmovable(rb)) {
        rb->yv += 9.8;
    }
    moveRigidBody(rb, dp);
    rotateRigidBody(rb, rb->w);
}

//Testing:
int rigidBodyMain() {
    puts("All tests passed!");
    return 0;
}
