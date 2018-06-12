#include"brick_object.h"

void brick_draw(game_object_t * obj){
    screen_draw_rectangle(obj->game_data->screen, obj->obj_data);
    return;
}

bool brick_collide_ball(game_object_t * obj){
    point_t ball_point = point_create(
                    round(get_curx(obj->game_data->ball->obj_data)),
                    round(get_cury(obj->game_data->ball->obj_data)));
    return rectangle_contains(obj->obj_data, ball_point);;
}

void brick_ttick(game_object_t * obj){
    //possibly deflect ball and remove brick
    if (game_object_collide_ball(obj)){
        //request to remove brick
        //
        if (obj->game_data->to_remove==NULL){
            obj->game_data->to_remove = obj;
            obj->game_data->brick_count--;
            obj->game_data -> score++;
        }
        
        /* GAVE UP ON FANCY BOUNCE
        void * bd = obj->game_data->ball->obj_data;
        point_t x_plus = point_create(
                get_curx(bd) + get_dx(bd),0, get_cury(bd));
        point_t y_plus = point_create(
                get_curx(bd), get_cury(bd) + get_dy(bd));
        
        //determine bounce direction
        double dx_mult = 
            rectangle_contains(obj->obj_data, x_plus) ? -1 : 1;
        double dy_mult = 
            rectangle_contains(obj->obj_data, y_plus) ? -1 : 1;
        */
        
        //bounce ball
        ball_bounce(obj->game_data->ball, 1 ,-1);
    }
    return;
}

void brick_destroy(game_object_t * obj){
    free(obj->obj_data);
    free(obj);
    return;
}

static game_object_vtable_t brick_vtable = {
    .draw = brick_draw,
    .collide_ball = brick_collide_ball,
    .ttick = brick_ttick,
    .destroy = brick_destroy
};

game_object_t * brick_create(game_data_t * game_data, rectangle_t * r){
    game_object_t * h = (game_object_t *) malloc(sizeof(game_object_t));
    h->obj_data = malloc (sizeof(rectangle_t));
    memcpy(h->obj_data, (void *) r, sizeof(rectangle_t));

    h->game_data = game_data;
    h->table = &brick_vtable;

    return h;
}
