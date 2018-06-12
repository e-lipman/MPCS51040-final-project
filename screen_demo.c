
// This indicates that we want the POSIX-2001 (and before) functions as
// well.
#define _POSIX_C_SOURCE 200112L

#include "screen.h"

#include <time.h> //  nanosleep, POSIX.1-2008
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

int main(int argc, char ** args)
{
    // Initialize game data
    screen_t * screen = screen_init();

    // Looks like I might have been one off in screen_getmax[xy]; feel free to fix.
    // (in other words, screen_getmaxx -> screen_get_width)
    const int maxx = screen_getmaxx(screen)-1;
    const int maxy = screen_getmaxy(screen)-1;

    double curx = ((double) maxx)/2;
    double cury = ((double) maxy)/2; 

    double dx = 1;
    double dy = 1;

    bool stop = false;
    while (!stop)
    {
        int icurx = round(curx);
        int icury = round(cury);

        // draw
        screen_clear(screen);
        screen_set(screen, icurx, icury, 'o');
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

        // Move ball
        curx += dx;
        cury += dy;

        if (icurx < 0 || icurx > maxx)
        {
            curx = (curx < 0 ? 0 : maxx);
            dx *= -1 * (.75 + ((rand() % 50)/(double)100));
        }
        if (icury < 0 || icury > maxy)
        {
            cury = ( cury < 0 ? 0 : maxy);
            dy *= -1* (.75 + ((rand() % 50)/(double)100));;
        }
    }

    screen_done(screen);

    return EXIT_SUCCESS;
}
