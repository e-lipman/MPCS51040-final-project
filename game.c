
// This indicates that we want the POSIX-2001 (and before) functions as
// well.
#define _POSIX_C_SOURCE 200112L

#include "screen.h"
#include "game_object.h"
#include "game.h"
#include "util.h"
#include "paddle_object.h"
#include "ball_object.h"
#include "brick_object.h"

#include <assert.h>
#include <time.h> //  nanosleep, POSIX.1-2008
#include <sys/time.h> // gettimeofday
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NANOSEC_PER_SEC ((uint64_t) 1000000000ull)
#define MICROSEC_PER_SEC ((uint64_t) 1000000ull)
#define TIMER_TICK_PER_SEC 25

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

/* functions which are passed as args */

//dummy functions for si
static void dummy_bb(const void * element, rectangle_t * rect,
    void * userdata){
    return;
}
static bool dummy_contains(const rectangle_t * p,
    const void * element, void * userdata){
    return true;
}

//visit draw
static bool visit_draw(void * element, void * userdata){
    game_object_draw(element);
    return true;
}

//visit timer tick
static bool visit_ttick(void * element, void * userdata){
    game_object_ttick(element);
    return true;
}

//visit destroy
static bool visit_destroy(void * element, void * userdata){
    game_object_destroy(element);
    return true;
}



/* now begins game functions */

game_data_t * game_init(){
    //allocate game data
    game_data_t * game = (game_data_t *) malloc(sizeof(game_data_t));
    
    //initialize game data
    game->screen = screen_init();
    game->level_done=false;
    game->game_objects = si_create(dummy_contains, dummy_bb, NULL);

    game->ball = NULL;
    game->to_remove = NULL;

    game->brick_count = 0;
    game->score = 0;
    game->lives = LIVES;
    return game;
}

void game_destroy(game_data_t * data)
{
    //destroy screen
    screen_done(data->screen);

    //destroy all remaining objects in SI
    si_walk(data->game_objects, visit_destroy, NULL);

    //destroy SI
    si_destroy(data->game_objects);

    //destroy game data
    free(data); 
    return;
}

static void insert_brick(game_data_t * data, rectangle_t * r){    
    game_object_t * brick = brick_create(data, r);
    si_insert(data->game_objects, brick);
    data->brick_count++;    
    return;
}

static bool check_screen_size(game_data_t * data){
    return ( screen_getmaxx(data->screen) >= BRICK_WIDTH
        && ( screen_getmaxy(data->screen) >=
            ((5*PADDLE_HEIGHT)+(BRICK_ROWS*BRICK_HEIGHT))));
}

static void create_level(game_data_t * data)
{
    //create ball
    game_object_t * ball = ball_create(data);

    //create paddle
    game_object_t * paddle = paddle_create(data);

    //add ball and paddle to game data
    data->ball = ball;
    si_insert(data->game_objects, ball);
    si_insert(data->game_objects, paddle);

    //add bricks
    rectangle_t r;
    for (int y_pos = 0; 
            y_pos < (BRICK_HEIGHT*BRICK_ROWS); y_pos+=BRICK_HEIGHT){
        for (int x_pos = 0; 
            (x_pos + BRICK_WIDTH) < screen_getmaxx(data->screen);
            x_pos+=BRICK_WIDTH){
            
            //create brick rectangle
            r = rectangle_from_coords(x_pos, y_pos,
                x_pos + BRICK_WIDTH, y_pos + BRICK_HEIGHT);

            //add to brick to game
            insert_brick(data, &r);
        }
    }
    return;
}

// Returns the current system time in microseconds since the UNIX epoch
// (only use for relative comparisons)
static uint64_t get_time_in_us()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
    {
        perror("Problem getting time!");
        exit(1);
    }
    return ((uint64_t) tv.tv_sec * MICROSEC_PER_SEC) + tv.tv_usec;
}

// Suspend execution for the specified number of microseconds
static void sleep_for_us(uint64_t us)
{
    struct timespec t;
    t.tv_sec = us / MICROSEC_PER_SEC;
    t.tv_nsec = (NANOSEC_PER_SEC/MICROSEC_PER_SEC)*us;
    nanosleep(&t, NULL);
}

// Cause all objects to draw themselves on the screen.
static void draw_objects(game_data_t * data)
{
    si_walk(data->game_objects, visit_draw, NULL);
    return;
}

// Let all game objects know that there was a timer tick
static void do_timer_tick(game_data_t * data)
{
    si_walk(data->game_objects, visit_ttick, NULL);
    return;
}

// Remove all the objects from data->to_remove from the spatial index.
// Don't forget to empty data->to_remove when done.
static void possibly_remove_objects(game_data_t * data)
{
    if (data->to_remove==NULL){
        return;
    }
    
    si_remove(data->game_objects, data->to_remove);
    game_object_destroy(data->to_remove);
    data->to_remove=NULL;
    return;
}

static bool check_level_done(game_data_t * data){
    return (data->brick_count==0 || data->lives==0);
}

/* and here is the game engine! */

int main(int argc, char ** args)
{
    game_data_t * data = game_init();
    
    
    if (!check_screen_size(data)){
        screen_message(data->screen, "screen too small: expand screen or change game parameters");
        goto screen_too_small;
    }

    //welcome message - ADD LIVES
    screen_message(data->screen, "Welcome!");

    create_level(data);

    // Number of microseconds since unix epoch.
    uint64_t next_timer_tick = get_time_in_us();
    
    //border (only for drawing)
    rectangle_t r = rectangle_from_coords(0,0,
        screen_getmaxx(data->screen),
        screen_getmaxy(data->screen));

    bool stop = false;
    while (!data->level_done && !stop)
    {
        //check if LEVEL DONE
        data->level_done = check_level_done(data);

        // Check if we need to issue a timer tick to the objects.
        if (get_time_in_us() > next_timer_tick)
        {
            //check if game quit
            const int key = getch();
            if (key == 'q'){
                stop= true;
            }
            if (key == KEY_LEFT || key == KEY_RIGHT){
                ungetch(key);
            }

            next_timer_tick += (MICROSEC_PER_SEC/TIMER_TICK_PER_SEC);

            //update screen display
            screen_clear(data->screen);
            screen_update_score(data->screen, data->score, data->lives);
            screen_draw_rectangle(data->screen, &r);
            draw_objects(data);

            screen_refresh(data->screen);  // actually makes output visible
            
            /*
            // sleep for 40ms
            struct timespec tv;
            tv.tv_sec = 0;
            tv.tv_nsec = 40*1000*1000;
            nanosleep(&tv, NULL);
            */

            do_timer_tick(data);

            possibly_remove_objects(data);
        }

        // sleep 1ms (or until the next timer tick)
        // (to avoid spending 100% CPU polling the keyboard)
        // NOTE that if we somehow fell behind (next_timer_tick <
        // get_time_in_us) we won't sleep but process the pending timer ticks
        // until we're back caught up.
        if (next_timer_tick > get_time_in_us())
            sleep_for_us(MIN(1000, next_timer_tick - get_time_in_us()));
    }

    //if level won
    screen_update_score(data->screen, data->score, data->lives);
    screen_refresh(data->screen);  // actually makes output visible
    
    if (data->brick_count==0){
        screen_message(data->screen, "Congratulations! You win!");
    }
    else if (data->lives==0){
        screen_message(data->screen, "Sorry, you lose :("); 
    }

    screen_too_small:
    game_destroy(data);

    return EXIT_SUCCESS;
}
