#include "util.h"

void point_print(point_t p, FILE * where){
    fprintf(where,"(%d,%d)", p.x, p.y);
    return;
}

point_t point_shift(point_t p, int dx, int dy){
    p.x+=dx;
    p.y+=dy;
    return p;
}

void rectangle_print(rectangle_t r,FILE * where){
    point_print(r.p1, where);   
    fprintf(where,"-");
    point_print(r.p2,where);   
}

//validate recatngle
static void rval(const rectangle_t * r){
    assert(r->p1.x <= r->p2.x && r->p1.y<=r->p2.y);
}

//returns min of two distinct ints
static int int_min(int a, int b){
    return (a<=b) ? a: b;
}

static int int_max(int a, int b){
    return (a>=b) ? a: b;
}

rectangle_t rectangle_create(point_t p1, point_t p2){
    rectangle_t r;
    r.p1 = point_create(int_min(p1.x,p2.x),int_min(p1.y,p2.y));
    r.p2 = point_create(int_max(p1.x,p2.x),int_max(p1.y,p2.y));
    rval(&r);
    return r;
}

rectangle_t rectangle_from_coords(coordinate_t x1, coordinate_t y1,
                coordinate_t x2, coordinate_t y2){
    point_t p1 = point_create(x1,y1);
    point_t p2 = point_create(x2,y2);
    return rectangle_create(p1,p2);
}

rectangle_t rectangle_combine(const rectangle_t * r1, const rectangle_t * r2){
    coordinate_t xmin = int_min(r1->p1.x, r2->p1.x);
    coordinate_t xmax = int_max(r1->p2.x, r2->p2.x);
    coordinate_t ymin = int_min(r1->p1.y, r2->p1.y);
    coordinate_t ymax = int_max(r1->p2.y, r2->p2.y);
    return rectangle_create(point_create(xmin,ymin),point_create(xmax,ymax));
}

rectangle_t rectangle_combine_array(const rectangle_t * r, unsigned int count){
    rectangle_t res = r[0];
    for (int i=1; i<count; i++){
        res = rectangle_combine(&res, r+i);
    }
    return res;
}

unsigned int rectangle_area(const rectangle_t * r){
    return (r->p2.x - r->p1.x)*(r->p2.y - r->p1.y);
}

bool rectangle_intersect(const rectangle_t * r1, const rectangle_t * r2,
                            rectangle_t * ret){
    if ((r1->p2.x < r2->p1.x) || (r1->p2.y < r2->p1.y)
        ||(r2->p2.x < r1->p1.x) || (r2->p2.y < r1->p1.y)){
        return false;
    }
    else {
        point_t p1,p2;
        p1 = point_create(int_max(r1->p1.x,r2->p1.x),int_max(r1->p1.y,r2->p1.y));
        p2 = point_create(int_min(r1->p2.x,r2->p2.x),int_min(r1->p2.y,r2->p2.y));
        *ret = rectangle_create(p1,p2);
        return true;
    }
}

unsigned int rectangle_intersect_area(const rectangle_t * r1, 
                            const rectangle_t * r2){
    rectangle_t ret;
    if (rectangle_intersect(r1,r2,&ret)==false){
        return 0;
    }
    else {
        return rectangle_area(&ret);
    }
}

bool rectangle_contains(const rectangle_t * r, point_t p){
    return (p.x >= r->p1.x && p.x <= r->p2.x && p.y >= r->p1.y && p.y<=r->p2.y);
}

void rectangle_corners(const rectangle_t * r, point_t * p1, point_t * p2,
        point_t * p3, point_t * p4){
    int all_x[2] = {r->p1.x, r->p2.x}; 
    int all_y[2] = {r->p1.y, r->p2.y}; 
    p1->x = all_x[0];
    p2->x = all_x[0];
    p3->x = all_x[1];
    p4->x = all_x[1];
    p1->y = all_y[0];
    p2->y = all_y[1];
    p3->y = all_y[0];
    p4->y = all_y[1];
}

coordinate_t rectangle_min_x(const rectangle_t * r){
    return r->p1.x;
}
coordinate_t rectangle_max_x(const rectangle_t * r){
    return r->p2.x;
}
coordinate_t rectangle_min_y(const rectangle_t * r){
    return r->p1.y;
}
coordinate_t rectangle_max_y(const rectangle_t * r){
    return r->p2.y;
}

point_t rectangle_min(const rectangle_t * r){
    return r->p1;
}
point_t rectangle_max(const rectangle_t * r){
    return r->p2;
}
