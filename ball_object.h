#pragma once

#include<math.h>
#include"game_object.h"


typedef struct ball_data_t ball_data_t;

struct ball_data_t {
    double curx;
    double cury;
    double dx;
    double dy;
};


double get_dx(ball_data_t * bd);
double get_dy(ball_data_t * bd);
double get_curx(ball_data_t * bd);
double get_cury(ball_data_t * bd);
void set_dx(ball_data_t * bd, double val);
void set_dy(ball_data_t * bd, double val);
void set_curx(ball_data_t * bd, double val);
void set_cury(ball_data_t * bd, double val);

game_object_t * ball_create(game_data_t * data);

//change dx and dy of ball by multipliers
void ball_bounce(game_object_t * obj, double mult_dx, double mult_dy);
