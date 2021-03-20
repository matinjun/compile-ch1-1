#include "shape.h"

struct Vptr {
    double (*area)(struct Shape * self);
    double (*perimeter)(struct Shape * self);
};
struct Vptr * Vptr_create(double (*area)(struct Shape * self), double (*perimeter)(struct Shape * self)) {
    struct Vptr * s = (struct Vptr *)malloc(sizeof(struct Vptr));
    
    s->area = area;
    s->perimeter = perimeter;
}

struct Shape {
    struct Vptr * vptr;
    int x;
    int y;
};
struct Shape * Shape_create(int x, int y) {
    struct Shape * s = (struct Shape *)malloc(sizeof(struct Shape));
    s->x = x;
    s->y = y;
    s->vptr = NULL; 
    return s;
}
void Shape_free(struct Shape * self) {
    if(self->vptr) {
        free(self->vptr);
    }
    free(self);
}
void Shape_init(struct Shape * self, int x, int y) {
    self->vptr = NULL;
    self->x = x;
    self->y = y;
}
void Shape_move(struct Shape * self, int dx, int dy) {
    self->x += dx;
    self->y += dy;
}
void Shape_show(struct Shape * self) {
    printf("x = %d, y = %d", self->x, self->y);
}
int Shape_getX(struct Shape * self) {
    return self->x;
}
int Shape_getY(struct Shape * self) {
    return self->y;
}
double Shape_area(struct Shape * self) {
    return (self->vptr->area)(self);
}
double Shape_perimeter(struct Shape * self) {
    return (self->vptr->perimeter)(self);
}

struct Rectangle {
    struct Shape base;
    int width;
    int height;
};
double Rectangle_area(struct Shape * self) {
    return (double)(((struct Rectangle *)(self))->width) * (double)(((struct Rectangle *)self)->height);
}
double Rectangle_perimeter(struct Shape * self) {
    return (double)(((struct Rectangle *)(self))->width + ((struct Rectangle *)self)->height) * 2.0;
}
struct Rectangle* Rectangle_create(int x, int y, int width, int height) {
    struct Rectangle * s = (struct Rectangle *)malloc(sizeof(struct Rectangle));
    Shape_init((struct Shape *)s, x, y);
    s->base.vptr = Vptr_create(Rectangle_area, Rectangle_perimeter);
    s->width = width;
    s->height = height;
    return s;
}
void Rectangle_free(struct Rectangle * self) {
    
}
int Rectangle_getWidth(struct Rectangle * self) {
    return self->width;
}
int Rectangle_getHeight(struct Rectangle * self) {
    return self->height;
}
