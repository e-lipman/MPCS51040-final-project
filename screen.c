#include "screen.h"

#include <string.h>
#include <curses.h>
#include <stdbool.h>
#include <locale.h>
#include <assert.h>
#include <stdlib.h>

struct screen_t
{
   WINDOW * main_win;
   WINDOW * score_win;
   int maxx;
   int maxy;
};

static WINDOW * win = NULL;
static unsigned int count = 0;

void screen_update_score (screen_t * scr,
      unsigned int score, unsigned int lives)
{
   int xsize = getmaxx (scr->score_win);
   char buf[255];
   snprintf (buf, sizeof(buf), "Score: %u, Lives: %u",
                    score, lives);

   werase (scr->score_win);
   mvwhline (scr->score_win, 0, 0, ' ', xsize);
   mvwaddstr (scr->score_win, 0, (xsize-strlen(buf))/2, buf);
   wrefresh (scr->score_win);
}

screen_t * screen_init ()
{
   screen_t * s = malloc (sizeof(screen_t));

   if (!win)
   {
      setlocale (LC_ALL, "");
      win = initscr ();
      cbreak ();
      noecho ();
      curs_set(0);
      nodelay (win, true);
      nonl ();
      keypad (stdscr, TRUE);
      ++count;
   }

   int screen_maxy = getmaxy (win);
   int screen_maxx = getmaxx (win);

   s->main_win = subwin(win, screen_maxy-1, screen_maxx, 0, 0);
   s->score_win = subwin(win, 1, 0, screen_maxy-1, 0);

   s->maxx = getmaxx(s->main_win);
   s->maxy = getmaxy(s->main_win);

   // Make sure screen is empty
   screen_clear (s);

   return s;
}


void screen_clear (screen_t * s)
{
   wbkgdset (s->main_win, ' ');
   werase (s->main_win);
   werase (s->score_win);

   // Redraw score
   screen_update_score (s, 0, 0);
}

// Draw a horizontal line
void screen_draw_hline(screen_t * scr, int x, int y, unsigned int len)
{
    mvwhline(scr->main_win, y, x, ACS_HLINE, len);
}

// Draw a vertical line
void screen_draw_vline(screen_t * scr, int x, int y, unsigned int len)
{
    mvwvline(scr->main_win, y, x, ACS_VLINE, len);
}

void screen_draw_rectangle(screen_t * scr, rectangle_t * r){
    int xmin = rectangle_min_x(r);
    int ymin = rectangle_min_y(r);
    int xmax = rectangle_max_x(r);
    int ymax = rectangle_max_y(r);
    screen_draw_vline(scr, xmin, ymin, (ymax-ymin)+1);
    screen_draw_vline(scr, xmax, ymin, (ymax-ymin)+1);
    screen_draw_hline(scr, xmin, ymin, (xmax-xmin)+1);
    screen_draw_hline(scr, xmin, ymax, (xmax-xmin)+1);
    return;
}

void screen_done (screen_t * s)
{
   assert (count);

   delwin (s->main_win);
   delwin (s->score_win);

   free (s);

   --count;

   if (!count)
   {
      delwin (win);
      endwin();
   }
}

void screen_refresh (screen_t * s)
{
   wrefresh(s->main_win);
   wrefresh(s->score_win);
}

void screen_message (screen_t * s, const char * str)
{
   static char * msg = "Press SPACE to continue...";
   mvwaddstr(s->main_win, s->maxy/2, (s->maxx-strlen(str))/2, str);
   mvwaddstr(s->main_win, 1+s->maxy/2, (s->maxx-strlen(msg))/2, msg);
   screen_refresh (s);
   char c;
   while ((c=getch()) != ' ')
   {
   }
}

static inline chtype char2chtype (char c)
{
   return (c == SCREEN_CHAR_SNAKE ? ACS_BLOCK : c);
}

static inline char chtype2char (chtype c)
{
   c &= A_CHARTEXT;
   return (c == ACS_BLOCK ? SCREEN_CHAR_SNAKE : c);
}

void screen_set (screen_t * s,
      unsigned int x, unsigned int y, screen_char_t c)
{
   mvwaddch (s->main_win, y, x, char2chtype(c));
}

screen_char_t screen_get (const screen_t * s, unsigned int x, unsigned int y)
{
   return chtype2char(mvwinch(s->main_win, y, x));
}

unsigned int screen_getmaxx (const screen_t * s)
{
   return s->maxx-1;
}

unsigned int screen_getmaxy (const screen_t * s)
{
   return s->maxy-1;
}

unsigned int screen_midpointx (const screen_t * scr){
   return (scr->maxx)/2;
}

WINDOW * screen_get_mainwin (screen_t * scr)
{
   return scr->main_win;
}

