#include"paddle_object.h"

void paddle_draw(game_object_t * obj){
    screen_draw_rectangle(obj->game_data->screen, obj->obj_data);
    return;
}

bool paddle_collide_ball(game_object_t * obj){
    point_t ball_point = point_create(
                    round(get_curx(obj->game_data->ball->obj_data)),
                    round(get_cury(obj->game_data->ball->obj_data)));
    return rectangle_contains(obj->obj_data, ball_point);;
}

static void move_paddle(game_object_t * obj, bool left){
    int shift;
    rectangle_t * r;
    rectangle_t new_rect;
    r = (rectangle_t *) (obj->obj_data);
    if (left==true){
        shift = -1*MIN(PADDLE_STEP, rectangle_min_x(r));
        //shift = -1*((rectangle_min_x(r)>PADDLE_STEP)? 
        //    PADDLE_STEP : rectangle_min_x(r));
    }
    else {
        shift = MIN(PADDLE_STEP, 
            screen_getmaxx(obj->game_data->screen) 
            - rectangle_max_x(r));
    }

    //shift paddle
    point_t new_min = point_shift(rectangle_min(r),shift,0);
    point_t new_max = point_shift(rectangle_max(r),shift,0);
    new_rect = rectangle_create(new_min,new_max);
    memcpy(r, &new_rect, sizeof(rectangle_t));
}

void paddle_ttick(game_object_t * obj){
    //move paddle
    const int key = getch();
    switch (key)
    {
        case 'q':
            ungetch(key);
            break;
        case KEY_LEFT:
        case KEY_RIGHT:
            move_paddle(obj, key == KEY_LEFT);
            break;
    }

    //possibly deflect ball
    if (game_object_collide_ball(obj)){
        ball_bounce(obj->game_data->ball,1,-1);
    }
    return;
}

void paddle_destroy(game_object_t * obj){
    free(obj->obj_data);
    free(obj);
    return;
}

static game_object_vtable_t paddle_vtable = {
    .draw = paddle_draw,
    .collide_ball = paddle_collide_ball,
    .ttick = paddle_ttick,
    .destroy = paddle_destroy
};

game_object_t * paddle_create(game_data_t * game_data){
    game_object_t * h = (game_object_t *) malloc(sizeof(game_object_t));
    
    //initialize object data (rectangle)
    int mid = screen_midpointx(game_data->screen);
    int ymax = screen_getmaxy(game_data->screen);
    rectangle_t r = rectangle_from_coords(
        /*x1*/ mid - PADDLE_HFWIDTH,
        /*y1*/ ymax - PADDLE_HEIGHT,
        /*x2*/ mid + PADDLE_HFWIDTH,
        /*y2*/ ymax);
    h->obj_data = malloc (sizeof(rectangle_t));
    memcpy(h->obj_data, (void *) &r, sizeof(rectangle_t));

    h->game_data = game_data;
    h->table = &paddle_vtable;

    return h;
}
