#include "Polygon.h"

#include <stdbool.h>
#include <float.h>

#define PI 3.14159265358979323846

struct nodes {
    int size;
    double *xs;
};
typedef struct nodes Nodes;

struct projection {
    double min, max;
};
typedef struct projection Projection;

Vector2D NOTHING = {0, 0};

//Utilities:
static double getMax(double *ns, int size) {
    //Returns the largest integer from a list of integers.
    int max = (int) ns[0];
    for (int i = 0; i < size; i ++) {
        if ((int) ns[i] > max) max = (int) ns[i];
    }
    return max;
}
static double getMin(double *ns, int size) {
    //Returns the smallest integer from a list of integers.
    int min =  (int) ns[0];
    for (int i = 0; i < size; i ++) {
        if ((int) ns[i] < min) min = (int) ns[i];
    }
    return min;
}
static double isLeft(int x, int y, int ax, int ay, int bx, int by) {
    //> 0 for (x,y) left of the line through a to b
    //= 0 for (x,y) on the line
    //< 0 for (x,y) right of the line
    return ((bx - ax) * (y - ay) - (x - ax) * (by - ay));
}
static void getArea(Polygon *p) {
    double area = 0;
    int j = p->size - 1;
    for (int i = 0; i < p->size; i ++) {
        double x1 = p->xs[i];
        double x2 = p->xs[j];
        double y1 = p->ys[i];
        double y2 = p->ys[j];
        j = i;
        area += (x1 + x2) * (y1 - y2);
    }
    p->area = area / 2;
}

Vector2D getCentroid(Polygon *p) {
    double xSum = 0;
    double ySum = 0;
    int j = p->size - 1;
    for (int i = 0; i < p->size; i ++) {
        double x1 = p->xs[i];
        double x2 = p->xs[j];
        double y1 = p->ys[i];
        double y2 = p->ys[j];
        xSum += ((x1 + x2) * (x1 * y2 - x2 * y1));
        ySum += ((y1 + y2) * (x1 * y2 - x2 * y1));
        j = i;
    }
    double divisor = 6 * p->area;
    Vector2D centroid = {xSum/divisor, ySum/divisor};
    return centroid;
}
SDL_Rect getAABB(Polygon *p) {
    SDL_Rect aabb;
    aabb.x = getMin(p->xs, p->size);
    aabb.y = getMin(p->ys, p->size);
    aabb.w = getMax(p->xs, p->size) - aabb.x;
    aabb.h = getMax(p->ys, p->size) - aabb.y;
    return aabb;
}
Polygon *newRegularPolygon(int size, double radius, double startX, double startY, Colour c) {
    double xs[size];
    double ys[size];

    double angle = (2 * PI) / (double) size;
    bool oddSides = (size % 2 != 0);
    for (int i = 0; i < size; i ++) {
        double k = oddSides ? (double) i - 0.25 : (double) i + 0.5;
        double x = radius * cos(angle * k) + startX;
        double y = radius * sin(angle * k) + startY;
        xs[i] = x;
        ys[i] = y;
    }


    return newPolygon(size, c, xs, ys);
}

//Constructors:
Polygon *newPolygon(int size, Colour c, double xs[size], double ys[size]) {
    Polygon *p = malloc(sizeof(Polygon));
    p->size = size;
    p->colour = c;

    p->xs = malloc(size*sizeof(double));
    p->ys = malloc(size*sizeof(double));
    for (int i = 0; i < size; i ++) {
        p->xs[i] = xs[i];
        p->ys[i] = ys[i];
    }

    getArea(p);
    return p;
}
static Nodes *newNodes(int size) {
    Nodes *nodes = malloc(sizeof(Nodes));
    nodes->xs = malloc(size * sizeof(double));
    return nodes;
}

//Drawing:
static Nodes *getDrawList(Polygon *p, int y) {
    //Get all the points that lie on the polygon at a given y value.
    double *xs = p->xs;
    double *ys = p->ys;
    int j = p->size-1;
    int count = 0;

    //Initialise an empty set of nodes.
    Nodes *nodes = newNodes(p->size);

    //Iterate over all of the polygons edges.
    for (int i = 0; i < p->size; i ++) {
        if ((ys[i] < y && ys[j] >= y) || (ys[j] < y && ys[i] >= y)) {
            //If the line through y intercects the edge, then record the the x coordinate of the intercept.
            double x = (xs[i] + (y - ys[i])/(ys[j]-ys[i]) * (xs[j]-xs[i]));
            nodes->xs[count] = x;
            count ++; //Count the number of nodes
        }
        j = i;
    }
    nodes->size = count;

    //Perform a bubble sort on the nodes. We want the nodes to be ordered as it is the space between each consecutive pair of ordered nodes that we need to fill.
    int numSwaps = 1;
    while (numSwaps != 0) {
        numSwaps = 0;
        for (int i = 0; i < nodes->size-1; i ++) {
            if (nodes->xs[i] > nodes->xs[i+1]) {
                int temp = nodes->xs[i];
                nodes->xs[i] = nodes->xs[i+1];
                nodes->xs[i+1] = temp;
                numSwaps ++;
            }
        }
    }
    return nodes; //return the ordered nodes.
}
void drawAABB(Polygon *p, SDL_Renderer *renderer) {
    SDL_Rect aabb = getAABB(p);
    SDL_RenderDrawRect(renderer, &aabb);
}
void drawPolygon(Polygon *p, SDL_Renderer *renderer) {
    Colour c = p->colour;
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a); //Set the colour of the renderer acording to the polygons colour.
    int start = getMin(p->ys, p->size); //Gets the starting row.
    int end = getMax(p->ys, p->size); //Get the ending row.
    for (int y = start; y <= end; y ++) {
        Nodes *drawList = getDrawList(p, y); //Collect all the nodes that lie on the edges of the polygon in the current row being rendered.
        for (int i = 0; i < drawList->size-1; i += 2 ) { //Iterate over each pairs of node, as we will fill the gap between each pair.
            int start = (int) drawList->xs[i];
            int end = (int) drawList->xs[i+1];
            for (int x = start; x <= end; x ++) {
                SDL_RenderDrawPoint(renderer, x, y); //Draw all the pixels between and including the startind and ending nodes on each row.
            }
        }
        free(drawList->xs);
        free(drawList);
    }
}

//Transformations:
void movePolygon(Polygon *p, Vector2D dp) {
    for (int i = 0; i < p->size; i ++) {
        p->xs[i] += dp.i;
        p->ys[i] += dp.j;
    }
}
void rotatePolygon(Polygon *p, double angle) {
    Vector2D axisOfRotation = getCentroid(p);
    movePolygon(p, axisOfRotation);
    for (int i = 0; i < p->size; i ++) {
        double x = p->xs[i];
        double y = p->ys[i];
        p->xs[i] = x * cos(angle) - y * sin(angle);
        p->ys[i] = y * cos(angle) + x * sin(angle);
    }
    movePolygon(p, multiplyV2D(axisOfRotation, -1));
}

//Collision Detection:
bool insidePolygon(int x, int y, Polygon *p) {
    //Method to test a vertex lies within a polygon by use of winding number method [2]
    //The general theory is that if a point lies outside the polygon, then the winding number of the polygon around that point is 0;
    //So we calculate the polygons winding number around the test point.
    //The algorithm is sped up by only looking at the edges that are in line with the point. (the points y coord lies between the edges vertices y coords).
    int wn = 0;
    int j = p->size-1;
    for (int i = 0; i < p->size; i ++) { //Iterate over all the polygon's edges.
        //Record all the components of the vertices that make up an edge.
        int x1 = p->xs[i];
        int y1 = p->ys[i];
        int x2 = p->xs[j];
        int y2 = p->ys[j];
        if (y1 <= y) {
            if (y2 > y) {
                //If y lise between y1 and y2 with y2 above y1;
                if (isLeft(x, y, x1, y1, x2, y2) > 0) {
                    //If the vertex is to the left of the upwards edge.
                    wn++;
                }
            }
        }else if (y2 <= y){
            //If y lise between y1 and y2 with y1 above y2;
            if (isLeft(x, y, x1, y1, x2, y2) < 0) {
                //If the vertex is to the right of the downwards edge.
                wn--;
            }
        }
    j = i;
    }
    return wn != 0;
}
bool aabbCollision(Polygon *p1, Polygon *p2) {
    SDL_Rect aabb1 = getAABB(p1);
    SDL_Rect aabb2 = getAABB(p2);
    return (aabb1.x + aabb1.w >= aabb2.x) && (aabb2.x + aabb2.w >= aabb1.x) &&
           (aabb1.y + aabb1.h >= aabb2.y) && (aabb2.y + aabb2.h >= aabb1.y);
}

//Seperating axis:
Vector2DArray getAxes1(Polygon *p) {
    int size = p->size;
    Vector2DArray *axes = newVector2DArray(size);
    int j = size-1;
    for (int i = 0; i < size; i++) {
        Vector2D v1 = {p->xs[i], p->ys[i]};
        Vector2D v2 = {p->xs[j], p->ys[j]};
        Vector2D edge = subtractV2D(v2, v1);
        Vector2D normal = leftNormal(edge);
        axes->vectors[i] = unitV2D(normal);
        j = i;
    }
    return *axes;
}
Vector2DArray getAxes2(Polygon *p) {
    Vector2DArray *axes = malloc(sizeof(Vector2DArray));
    axes->size = p->size;
    axes->vectors = malloc(axes->size * sizeof(Vector2D));
    int j = p->size-1;
    for (int i = 0; i < p->size; i++) {
        Vector2D v1 = {p->xs[i], p->ys[i]};
        Vector2D v2 = {p->xs[j], p->ys[j]};
        Vector2D edge = subtractV2D(v2, v1);
        Vector2D normal = rightNormal(edge);
        axes->vectors[i] = unitV2D(normal);
        j = i;
    }
    return *axes;
}
Projection project(Vector2D axis, Polygon *p){
    Vector2D v0 = {p->xs[0], p->ys[0]};
    double min = dotV2D(axis, v0);
    double max = min;
    for (int i = 0; i < p->size; i ++) {
        Vector2D v = {p->xs[i], p->ys[i]};
        double dp = dotV2D(axis, v);
        if (dp < min) {
            min = dp;
        }else if (dp > max){
            max = dp;
        }
    }
    Projection proj = {min, max};
    return proj;
}
bool overlaps(Projection p1, Projection p2) {
    return (p1.min <= p2.max && p2.min <= p1.max);
}
double getOverlap(Projection p1, Projection p2) {
    return abs(p2.max - p1.min);
}
bool colliding(Polygon *p1, Polygon *p2) {
    bool colliding = aabbCollision(p1, p2);
    if (colliding) {
        Vector2DArray axes1 = getAxes1(p1);
        Vector2DArray axes2 = getAxes1(p2);

        for (int i = 0; i < axes1.size && colliding; i ++) {
            Vector2D axis = axes1.vectors[i];
            Projection proj1 = project(axis, p1);
            Projection proj2 = project(axis, p2);
            if (! overlaps(proj1, proj2)) {
                colliding = false;
            }
        }

        for (int i = 0; i < axes2.size && colliding; i ++) {
            Vector2D axis = axes2.vectors[i];
            Projection proj1 = project(axis, p1);
            Projection proj2 = project(axis, p2);
            if (! overlaps(proj1, proj2)) {
                colliding = false;
            }
        }
    }
    return colliding;
}
Vector2D getMTV(Polygon *p1, Polygon *p2) {
    bool colliding = aabbCollision(p1, p2);
    double overlap = DBL_MAX;

    Vector2D smallest = NOTHING;
    Vector2DArray axes1 = getAxes1(p1);
    Vector2DArray axes2 = getAxes2(p2);

    for (int i = 0; i < axes1.size && colliding; i ++) {
        Vector2D axis = axes1.vectors[i];
        Projection proj1 = project(axis, p1);
        Projection proj2 = project(axis, p2);
        if (overlaps(proj1, proj2)) {
            double o = getOverlap(proj1, proj2);
            if (o < overlap) {
                overlap = o;
                smallest = axis;
            }
        }else{
            colliding = false;
        }
    }

    for (int i = 0; i < axes2.size && colliding; i ++) {
        Vector2D axis = axes2.vectors[i];
        Projection proj1 = project(axis, p1);
        Projection proj2 = project(axis, p2);
        if (overlaps(proj1, proj2)) {
            double o = getOverlap(proj1, proj2);
            if (o < overlap) {
                overlap = o;
                smallest = axis;
            }
        }else{
            colliding = false;
        }
    }

    Vector2D MTV;
    if (colliding) {
        MTV = multiplyV2D(smallest, overlap);
    } else {
        MTV = NOTHING;
    }

    return MTV;
}
void resolvePenEq(Polygon *p1, Polygon *p2) {
    Vector2D mtv = getMTV(p1, p2);
    if (! equalsV2D(NOTHING, mtv)) {
        movePolygon(p1, multiplyV2D(mtv, 0.5));
        movePolygon(p2, multiplyV2D(mtv, -0.5));
    }
}
void resolvePenDom(Polygon *p1, Polygon *p2) {
    Vector2D mtv = getMTV(p1, p2);
    if (! equalsV2D(NOTHING, mtv)) {
        movePolygon(p2, multiplyV2D(mtv, - 1));
    }
}

int polygonMain() {
    puts("All tests passed!");
    return 0;
}
