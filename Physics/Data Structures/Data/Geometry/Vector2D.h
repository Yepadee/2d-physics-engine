#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

struct Vector2D {
    double i, j;
};
typedef struct Vector2D Vector2D;

struct Vector2DArray {
    int size;
    Vector2D *vectors;
};
typedef struct Vector2DArray Vector2DArray;


Vector2D NOTHING;


double magnitudeV2D(Vector2D v);
double dotV2D(Vector2D v1, Vector2D v2);
Vector2D subtractV2D(Vector2D v1, Vector2D v2);
Vector2D addV2D(Vector2D v1, Vector2D v2);
Vector2D multiplyV2D(Vector2D v, double n);
bool equalsV2D(Vector2D v1, Vector2D v2);
Vector2D unitV2D(Vector2D v);
Vector2D rightNormal(Vector2D v);
Vector2D leftNormal(Vector2D v);

Vector2DArray *newVector2DArray(int capacity);
