//Learning resources:
//http://alienryderflex.com/polygon/ [0] - Point-In-RigidBody Algorithm.
//http://alienryderflex.com/polygon_fill/ [1] - RigidBody Fill Algorithm.
//http://geomalgorithms.com/a03-_inclusion.html [2] - Point-In-RigidBody Algorithm (Winding Number Method).
//http://www.dyn4j.org/2010/01/sat/ [3] - Seperating Axis Theorem.
#define SDL_MAIN_HANDLED

#include <assert.h>

#include "Physics\RigidBodies.h"
#include "Input\MouseInput.h"
#include "Input\KeyboardInput.h"

const char TITLE[] = "Collision";
const int W = 1280, H = 720;
const double TPS = 60;

//Initialisation:
RigidBodies *getRigidBodies() {
    //Create an empty pointer to a polygon list.
    int numRigidBodies = 5;
    RigidBodies *rbs = newRigidBodies(W, H, numRigidBodies);

    int n;

    //Set up a set of polygons:
    n = 5;
    Colour c0 = {255, 165, 0, 255};
    double xs0[] = {550, 521, 532, 568, 579};
    double ys0[] = {520, 541, 574, 574, 541};

    for (int i = 0; i < 20; i ++) {
        double mass = 1;
        double e = 1;
        bool immovable = false;
        Polygon *p0 = newPolygon(n, c0, xs0, ys0);
        addRigidBody(rbs, newRigidBody(mass, e, immovable, p0));
    }
    RigidBody *rb = getRigidBody(rbs, 10);
    rb->xv = 10;
    rb->yv = 1;

    return rbs;
}

//Drawing:
void drawBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}
void draw(SDL_Renderer *renderer, RigidBodies *rbs) {
    drawBackground(renderer);
    drawRigidBodies(rbs, renderer);
}

//Updating:
bool touchingMouse(RigidBody *rb) {
    return insidePolygon(Mi.x, Mi.y, rb->polygon);
}
void moveByUser(RigidBodies *rbs) {
    //Vector2D mousePos = {Mi.x, Mi.y};
    for (int i = 0; i < numRigidBodies(rbs); i ++) {
        RigidBody *rb = getRigidBody(rbs, i);
        Polygon *p = rb->polygon;
        if (touchingMouse(rb) && mousePressed(SDL_BUTTON_LEFT)) {
            rb->xDist = Mi.x - p->xs[0];
            rb->yDist = Mi.y - p->ys[0];
            rb->canMove = true;
        }

        if (rb->canMove && mouseMoving()) {
            int initialX = p->xs[0];
            int initialY = p->ys[0];

            int finalX = Mi.x - rb->xDist;
            int finalY = Mi.y - rb->yDist;
            int dx = finalX - initialX;
            int dy = finalY - initialY;
            Vector2D dp = {dx, dy};
            moveRigidBody(rb, dp);
        }
        if (mouseReleased(SDL_BUTTON_LEFT)) {
            rb->canMove = false;
        }
    }
}

void update(RigidBodies *rbs, bool setupMode) {
    updateRigidBodies(rbs);
    if (setupMode) {
        moveByUser(rbs);
        resolveAllPen(rbs);
    } else {
        moveRigidBodies(rbs);
        collideAll(rbs);
    }
}

//Main Loop:
void loop(SDL_Renderer *renderer, bool test) {
    double T = 1000/TPS; //Calculate time for one tick in ms.
    SDL_Event ev;

    bool running = true;
    bool setupMode = true;

    RigidBodies *rbs = getRigidBodies();
    resolveAllPen(rbs);

    while (running) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                running = false;
            }
        }

        if (keyPressed(SDL_SCANCODE_T)) test = ! test;
        if (keyPressed(SDL_SCANCODE_RETURN)) setupMode = ! setupMode;
        if (keyPressed(SDL_SCANCODE_ESCAPE)) running = ! running;

        updateKeyboard();
        updateMouse(ev);
        update(rbs, setupMode);
        draw(renderer, rbs);
        if (test) {
            SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
            drawRigidBodiesAABB(rbs, renderer);
            drawQTree(rbs->q, renderer);
        }

        SDL_RenderPresent(renderer); //Draw the polygons.
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

    /*
    Vector2D v1 = {3, 4};
    Vector2D v2 = {6, 8};
    assert(magnitudeV2D(v1) == 5);
    assert(dotV2D(v1, v2) == 50);
    Vector2D d = subtractV2D(v2, v1);
    assert(d.i == 3 && d.j == 4);
    */


    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(W, H, 0, &window, &renderer);
    SDL_SetWindowTitle(window, TITLE);
    loop(renderer, true);
    SDL_Quit();

    puts("All tests passed!");

}

//Main:
int main(int argc, char *argv[]) {
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

    loop(renderer, false);

    return 0;
}
