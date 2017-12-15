//Learning resources:
//http://alienryderflex.com/polygon/ [0] - Point-In-RigidBody Algorithm.
//http://alienryderflex.com/polygon_fill/ [1] - RigidBody Fill Algorithm.
//http://geomalgorithms.com/a03-_inclusion.html [2] - Point-In-RigidBody Algorithm (Winding Number Method).
//http://www.dyn4j.org/2010/01/sat/ [3] - Seperating Axis Theorem.
#define SDL_MAIN_HANDLED

#include "Physics\RigidBodies.h"
#include "Input\MouseInput.h"
#include "Input\KeyboardInput.h"

#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>

const char TITLE[] = "Collision";
const int W = 1280, H = 720;
const double TPS = 60.0;
const double T = 1.0 / 60.0;

struct State {
    bool running, setupMode, testMode, movingRB, gravity;
};
typedef struct State State;

//Initialisation:
State *newState() {
    State *s = malloc(sizeof(State));
    s->running = true;
    s->setupMode = false;
    s->testMode = false;
    s->movingRB = false;
    s->gravity = true;
    return s;
}
RigidBodies *getRigidBodies() {
    //Create an empty pointer to a polygon list.
    int numRigidBodies = 0;
    int numWalls = 4;
    RigidBodies *rbs = newRigidBodies(W, W, numRigidBodies + numWalls);

    int thickness = 40;
    double wallXS1[] = {0, 0, thickness, thickness};
    double wallYS1[] = {0, 720, 720, 0};

    double wallXS2[] = {W - thickness, W - thickness, W, W};
    double wallYS2[] = {0, H, H, 0};

    double wallXS3[] = {0, 0, W, W};
    double wallYS3[] = {0, thickness, thickness, 0};

    double wallXS4[] = {0, 0, W, W};
    double wallYS4[] = {H - thickness, H, H, H - thickness};

    Colour cw = {0, 0, 0, 255};

    Polygon *w1 = newPolygon(4, cw, wallXS1, wallYS1);
    Polygon *w2 = newPolygon(4, cw, wallXS2, wallYS2);
    Polygon *w3 = newPolygon(4, cw, wallXS3, wallYS3);
    Polygon *w4 = newPolygon(4, cw, wallXS4, wallYS4);

    addRigidBody(rbs, newRigidBody(1, 1, true, w1));
    addRigidBody(rbs, newRigidBody(1, 1, true, w2));
    addRigidBody(rbs, newRigidBody(1, 1, true, w3));
    addRigidBody(rbs, newRigidBody(1, 1, true, w4));
    return rbs;
}

//Helper:
int randInt(int min, int max) {
    return (rand() % max) + min;
}
RigidBody *randomRigidBody() {
    double mass = randInt(1, 10);
    double e = (double) randInt(1, 100) / 100;
    bool immovable = false;
    int size = randInt(4, 9);
    double radius = (mass + 5) * 4;

    double startX = Mi.x;
    double startY = Mi.y;

    int r = randInt(0, 255);
    int g = randInt(0, 255);
    int b = randInt(0, 255);
    int a = 255;

    Colour c = {r, g, b, a};

    Polygon *p = newRegularPolygon(size, radius, startX, startY, c);
    return newRigidBody(mass, e, immovable, p);
}
bool touchingMouse(RigidBody *rb) {
    return insidePolygon(Mi.x, Mi.y, rb->polygon);
}

//Updating:
void add(RigidBodies *rbs, State *s) {
    if (mousePressed(SDL_BUTTON_LEFT) && ! s->movingRB) {
        RigidBody *rb = randomRigidBody();
        //rb->xv = randInt(64, 256);
        //rb->yv = randInt(64, 256);
        //rb->w = (double) randInt(0, 50) / 100;
        addRigidBody(rbs, rb);
    }
}
void delete(RigidBodies *rbs, State *s) {
    if (! s->movingRB) {
        int toDelete = -1;
        for (int i = 0; i < numRigidBodies(rbs) && toDelete == -1; i ++) {
            RigidBody *rb = getRigidBody(rbs, i);
            if (touchingMouse(rb) && mousePressed(SDL_BUTTON_RIGHT)) {
                toDelete = i;
            }
        }
        if (toDelete >= 0) {
            deleteRigidBody(rbs, toDelete);
        }
    }
}
void moveByUser(RigidBodies *rbs, State *s) {
    for (int i = 0; i < numRigidBodies(rbs); i ++) {
        RigidBody *rb = getRigidBody(rbs, i);
        Polygon *p = rb->polygon;
        if (touchingMouse(rb) && mousePressed(SDL_BUTTON_LEFT)) {
            rb->xDist = (double) Mi.x - p->xs[0];
            rb->yDist = (double) Mi.y - p->ys[0];
            rb->canMove = true;
            s->movingRB = true;
        }
        double initialX = p->xs[0];
        double initialY = p->ys[0];

        double finalX = (double) Mi.x - rb->xDist;
        double finalY = (double) Mi.y - rb->yDist;
        double dx = finalX - initialX;
        double dy = finalY - initialY;
        if (rb->canMove) {
            if (mouseMoving()){
                Vector2D dp = {dx, dy};
                moveRigidBody(rb, dp);
            }
            if (! isImmovable(rb)) {
                rb->xv = dx * 10;
                rb->yv = dy * 10;
            }

        }
        if (mouseReleased(SDL_BUTTON_LEFT)) {
            rb->canMove = false;
            s->movingRB = false;
        }
    }
}
void update(RigidBodies *rbs, State *s) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev) != 0) {
        if (ev.type == SDL_QUIT) {
            s->running = false;
        }
    }

    updateKeyboard();
    updateMouse(ev);

    if (keyPressed(SDL_SCANCODE_T)) s->testMode = ! s->testMode;
    if (keyPressed(SDL_SCANCODE_G)) s->gravity = ! s->gravity;
    if (keyPressed(SDL_SCANCODE_RETURN)) s->setupMode = ! s->setupMode;
    if (keyPressed(SDL_SCANCODE_ESCAPE)) s->running = ! s->running;



    moveByUser(rbs, s);
    add(rbs, s);
    delete(rbs, s);

    if (s->setupMode) {
        resolveAllPen(rbs);
    } else {
        updateRigidBodies(rbs, T, s->gravity);
        collideAll(rbs);
    }
    //resolveAllPen(rbs);



}

//Drawing:
void drawBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}
void draw(RigidBodies *rbs, State *s, SDL_Renderer *renderer) {
    drawBackground(renderer);
    drawRigidBodies(rbs, renderer);

    if (s->testMode) {
        SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
        drawRigidBodiesAABB(rbs, renderer);
        drawQTree(rbs->q, renderer);
    }

    SDL_RenderPresent(renderer); //Draw the rigid bodies.
}

//Main Loop:
void loop(SDL_Renderer *renderer) {
    State *s = newState();
    double T = 1000/TPS; //Calculate time for one tick in ms.
    RigidBodies *rbs = getRigidBodies();

    double uSPreTick = T * 1000;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long int lastTime, now;
    lastTime = tv.tv_usec;

    double unprocessed = 0;
    bool canRender = false;
    while(s->running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                s->running = false;
            }
        }
        gettimeofday(&tv, NULL);
        now = tv.tv_usec;
        if (lastTime > now) {
            lastTime -= 1000000;
        }
        unprocessed += (double) (now - lastTime) / uSPreTick; //Calculate how many updates should have been processed to maintain desired TPS.
        lastTime = now;

        if (canRender) {
            draw(rbs, s, renderer);
        }

        canRender = false;
        while(unprocessed >= 1) {
            update(rbs, s); //Processes all unprocessed updates.
            unprocessed -= 1;
            canRender = true;
        }

        SDL_Delay(T); //Pause.
    }
    /*
    for (int i = 0; i < rbs->size; i ++) {
        free(rbs->list[i]->polygon->xs);
        free(rbs->list[i]->polygon->ys);
        free(rbs->list[i]->polygon);
        free(rbs->list[i]);
    }
    free(rbs);
    */
}

//Tests:
void tests() {

    int size = 4;
    double xs[] = {600, 600, 100, 10};
    double ys[] = {200, 300, 200, 100};
    Colour c = {120, 20, 100, 255};

    Polygon *p = newPolygon(size, c, xs, ys);

    assert(p->size == size);
    assert(p->xs[0] == xs[0]);
    assert(p->ys[0] == ys[0]);

    puts("All tests passed!");

}

//Main:
int main(int argc, char *argv[]) {
    srand(time(NULL));
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s", SDL_GetError());
        return 0;
    }

    if (argc > 1){
        tests();
        return 0;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(W, H, 0, &window, &renderer);
    SDL_SetWindowTitle(window, TITLE);

    loop(renderer);

    return 0;
}
