#pragma once

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

#include<stdlib.h>
#include<string.h>
#include"util.h"
#include"spatial_index.h"
#include"screen.h"
#include"game_consts.h"

typedef struct game_object_t game_object_t;
typedef struct game_data_t game_data_t;

typedef struct game_object_vtable_t
{
    void (*draw)(game_object_t * obj);        
    bool (*collide_ball)(game_object_t * obj);
    void (*ttick)(game_object_t * obj);
    void (*destroy)(game_object_t * obj);
} game_object_vtable_t;

struct game_object_t {
    game_object_vtable_t * table;
    void * obj_data;
    game_data_t * game_data;
};

struct game_data_t
{
    screen_t * screen;
    bool level_done;
    si_handle game_objects;   
    game_object_t * ball;     
    game_object_t * to_remove;
    unsigned int brick_count;
    unsigned int score;
    unsigned int lives;
};

static inline void game_object_draw(game_object_t * obj){
    obj->table->draw(obj);
    return;
}
static inline bool game_object_collide_ball(game_object_t * obj){
    return obj->table->collide_ball(obj);
}
static inline void game_object_ttick(game_object_t * obj){
    obj->table->ttick(obj);
    return;
}
static inline void game_object_destroy(game_object_t * obj){
    obj->table->destroy(obj);
    return;
}
