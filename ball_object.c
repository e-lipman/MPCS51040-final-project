#include"ball_object.h"

//a bunch of set and get functions
double get_curx(ball_data_t * bd){
    return bd->curx;
}
double get_cury(ball_data_t * bd){
    return bd->cury;
}
double get_dx(ball_data_t * bd){
    return bd->dx;
}
double get_dy(ball_data_t * bd){
    return bd->dy;
}
void set_curx(ball_data_t * bd, double val){
    bd->curx = val;
    return;
}
void set_cury(ball_data_t * bd, double val){
    bd->cury = val;
    return;
}
void set_dx(ball_data_t * bd, double val){
    bd->dx = val;
    return;
}
void set_dy(ball_data_t * bd, double val){
    bd->dy = val;
    return;
}

void move_ball(ball_data_t * bd){
    bd->curx += bd->dx;
    bd->cury += bd->dy;
    return;
}
void ball_draw(game_object_t * obj){
    int icurx = round(get_curx(obj->obj_data));
    int icury = round(get_cury(obj->obj_data));
    screen_set(obj->game_data->screen,icurx,icury,'o');
    return;
}

bool ball_collide_ball(game_object_t * obj){
    return true;
}

void ball_bounce(game_object_t * obj, double mult_dx, double mult_dy){
    set_dx(obj->obj_data, get_dx(obj->obj_data)*mult_dx);
    set_dy(obj->obj_data, get_dy(obj->obj_data)*mult_dy);
    return;
}

static double rand_bounce(){
    return ((100-BALL_RAND_PERCENT)
        +rand()%(2*BALL_RAND_PERCENT))/((double) 100);
}

static void update_ball_traj(game_object_t * obj){
    int maxx = screen_getmaxx(obj->game_data->screen);
    int maxy = screen_getmaxy(obj->game_data->screen);
    int icurx = round(get_curx(obj->obj_data));
    int icury = round(get_cury(obj->obj_data));

    //check whether bounce off wall or ciling
    double bounce = rand_bounce();;
    
    if (icurx<0 || icurx>maxx){
        set_curx(obj->obj_data, icurx<0 ? 0 : maxx);
        ball_bounce(obj, -1*bounce, 1);
    }
    if (icury<0 || icury>maxy){
        set_cury(obj->obj_data, icury<0 ? 0 : maxy);
        ball_bounce(obj, 1, -1*bounce);
    }

    //check if hit floor
    if (icury>maxy){
        obj->game_data->lives--;    
        screen_message(obj->game_data->screen,
           "Oops! You lost a life");
        screen_refresh(obj->game_data->screen);
    }
    return;
}

void ball_ttick(game_object_t * obj){
    update_ball_traj(obj);
    move_ball(obj->obj_data);
    return;
}

void ball_destroy(game_object_t * obj){
    free(obj->obj_data);
    free(obj);
    return;
}

static game_object_vtable_t ball_vtable = {
    .draw = ball_draw,
    .collide_ball = ball_collide_ball,
    .ttick = ball_ttick,
    .destroy = ball_destroy
};

game_object_t * ball_create(game_data_t * game_data){
    game_object_t * h = (game_object_t *) malloc(sizeof(game_object_t));
    
    //create and initialize data
    h->obj_data = malloc(sizeof(ball_data_t));
    set_curx(h->obj_data, 0);
    set_cury(h->obj_data, screen_getmaxy(game_data->screen));
    set_dx(h->obj_data, BALL_STEP);
    set_dy(h->obj_data, -1*BALL_STEP);
    
    h->game_data = game_data;
    h->table = &ball_vtable ;

    return h;
}


