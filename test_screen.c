// This indicates that we want the POSIX-2001 (and before) functions as
// well.
#define _POSIX_C_SOURCE 200112L

#include "screen.h"
#include "game_object.h"
#include "ball_object.h"
#include "paddle_object.h"

#include <time.h> //  nanosleep, POSIX.1-2008
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

int main(int argc, char ** args)
{
    srand(time(NULL));

    // Initialize game data
    screen_t * screen = screen_init();
    game_data_t * game_data 
        = (game_data_t *) malloc(sizeof(game_data_t));
    game_data->screen = screen;

    //create ball
    game_object_t * ball = ball_create(game_data);

    //create paddle
    game_object_t * paddle = paddle_create(game_data);

    //border (only for drawing)
    rectangle_t r = rectangle_from_coords(0,0,screen_getmaxx(screen),
        screen_getmaxy(screen));

    bool stop = false;
    while (!stop)
    {
        // draw
        screen_clear(screen);
        screen_draw_rectangle(screen, &r);
        
        game_object_draw(ball);
        game_object_draw(paddle);
        screen_refresh(screen);  // actually makes output visible
        
        // Check if a key was pressed
        const int key = getch();
        if (key == 'q')
            stop= true;

        // sleep for 40ms
        struct timespec tv;
        tv.tv_sec = 0;
        tv.tv_nsec = 40*1000*1000;
        nanosleep(&tv, NULL);

        // Move objects
        game_object_ttick(ball);
        game_object_ttick(paddle);
    }

    screen_done(screen);

    game_object_destroy(ball);
    return EXIT_SUCCESS;
}
