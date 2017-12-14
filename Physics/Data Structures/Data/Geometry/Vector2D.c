#include "Vector2D.h"

double magnitudeV2D(Vector2D v) {
    return sqrt(pow(v.i, 2) + pow(v.j, 2));
}

double dotV2D(Vector2D v1, Vector2D v2) {
    return (v1.i * v2.i) + (v1.j * v2.j);
}

double crossV2D(Vector2D v1, Vector2D v2) {
    return (v1.i * v2.j) - (v2.i * v1.j);
}

Vector2D unitV2D(Vector2D v) {
    double mag = magnitudeV2D(v);
    Vector2D unit = {v.i/mag, v.j/mag};
    return unit;
}

Vector2D subtractV2D(Vector2D v1, Vector2D v2) {
    Vector2D distance = {v1.i - v2.i, v1.j - v2.j}; //Remeber to alter main program
    return distance;
}

Vector2D addV2D(Vector2D v1, Vector2D v2) {
    Vector2D distance = {v1.i + v2.i, v1.j + v2.j};
    return distance;
}

Vector2D multiplyV2D(Vector2D v, double n) {
    Vector2D v2 = {v.i * n, v.j * n};
    return v2;
}

bool equalsV2D(Vector2D v1, Vector2D v2) {
    return v1.i == v2.i && v1.j == v2.j;
}

void moveV2D(Vector2D *v1, Vector2D v2) {
    v1->i += v2.i;
    v1->j += v2.j;
}

Vector2D rightNormal(Vector2D v){
    Vector2D rotated = {v.j, -v.i};
    return rotated;
}

Vector2D leftNormal(Vector2D v){
    Vector2D rotated = {-v.j, v.i};
    return rotated;
}

Vector2DArray *newVector2DArray(int capacity) {
    Vector2DArray *v2da = malloc(sizeof(Vector2DArray));
    v2da->size = capacity;
    v2da->vectors = malloc(capacity * sizeof(Vector2D));
    return v2da;
}
