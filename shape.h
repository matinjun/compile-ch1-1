#ifndef SHAPE_H
#define SHAPE_H
#include <stdlib.h>
#include <stdio.h>
/**
 * @brief define a shape class
 * 
 */
struct Shape;
struct Shape * Shape_create(int x, int y);
void Shape_move(struct Shape * self, int dx, int dy);
void Shape_show(struct Shape * self);
int Shape_getX(struct Shape * self); 
int Shape_getY(struct Shape * self);
double Shape_area(struct Shape * self);
double Shape_perimeter(struct Shape * self);
void Shape_free(struct Shape * self);

struct Rectangle;
struct Rectangle * Rectangle_create(
    int x, int y, 
    int width, 
    int height
);
int Rectangle_getWidth(struct Rectangle * self);
int Rectangle_getHeight(struct Rectangle * self);

#endif