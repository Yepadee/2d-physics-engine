#include "Vector2D.h"
#include <SDL2/SDL.h>

struct Colour {
    Uint8 r, g, b, a; //A structure grouping all the rgba components of a colour.
};
typedef struct Colour Colour;

struct Polygon {
    Colour colour;
    int size; //Number of vertices.
    int area;
    double *xs; //List of x coords.
    double *ys; //List of y coods.
};
typedef struct Polygon Polygon;

Polygon *newPolygon(int size, Colour c, double xs[size], double ys[size]);

//Utilities:
Polygon *newRegularPolygon(int size, double radius, double startX, double startY, Colour c);
SDL_Rect getAABB(Polygon *p);

//Transformations:
void movePolygon(Polygon *p, Vector2D dp);
void rotatePolygon(Polygon *p, double angle);

//Updating:
bool insidePolygon(int x, int y, Polygon *p);
bool colliding(Polygon *p1, Polygon *p2);
Vector2D getMTV(Polygon *p1, Polygon *p2);
void resolvePenEq(Polygon *p1, Polygon *p2);
void resolvePenDom(Polygon *p1, Polygon *p2);

//Drawing:
void drawPolygon(Polygon *p, SDL_Renderer *renderer);
void drawAABB(Polygon *p, SDL_Renderer *renderer);
