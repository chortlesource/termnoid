////////////////////////////////////////////////////////////////////////////
//
// termnoid - system.c
//
// Copyright (c) 2021 Christopher M. Short
//
// This file is part of termnoid.
//
// termnoid is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// termnoid is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with termnoid. If not, see <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////

#include "termnoid.h"


/////////////////////////////////////////////////////////////
// SYSTEM STATIC FUNCTIONS
//

static double term_calc_delta(struct system *sys) {
  double rvalue = 0.0;

  if(sys) {
    /// Obtain the current time in microseconds
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // Calculate delta time
    sys->delta = tv.tv_usec - sys->last_time;
    rvalue = sys->delta;
  }

  return rvalue;
}

/////////////////////////////////////////////////////////////
// SYSTEM FUNCTION IMPLEMENTATION
//

struct system*  term_new_system(const int argc, const char *argv[]) {
  // Allocate memory to the system struct
  struct system *sys = malloc(sizeof(struct system));

  if(sys) {
    struct game *game = malloc(sizeof(struct game));

    if(game) {
      // Initialize the game buffer
      term_init_buffer(game);

      sys->game = game;
      sys->game->shape  = 0;
      sys->game->rotate = R_0;
      sys->game->pos_x  = (buffer_w / 2) - 1;
      sys->game->pos_y  = (buffer_h / 2) - 1;

      sys->height    = 0;
      sys->width     = 0;
      sys->rotate    = 0;
      sys->delta     = 0.0;
      sys->last_time = 0;
      sys->state     = T_RUN; // To enable the main loop
    } else {
      free(sys);
      sys = NULL;
    }
  }

  return sys;
}


void term_free_system(struct system *sys) {
  if(sys) {
    if(sys->game)
      free(sys->game);
    // Free the system struct
    free(sys);
  }
}


int term_init_curses() {
   // Initialize ncurses
    initscr();
    wtimeout(stdscr, 1);
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    // Check terminal for colours
    if(has_colors() == false) {
      printf("[TERMNOID]: Error This terminal does not support colours.\n");
      endwin();
      return T_ERR;
    }

    start_color();
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_RED);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    init_pair(6, COLOR_BLACK, COLOR_YELLOW);
    init_pair(7, COLOR_BLACK, COLOR_CYAN);
    init_pair(8, COLOR_BLACK, COLOR_MAGENTA);
    refresh();
    return T_OK;
}


void term_exit_curses() {
  endwin();
}


void term_run(struct system *sys) {
  // Initialize ncurses
  if(!term_init_curses())
    return;

  // Find out the size of the terminal
  getmaxyx(stdscr, sys->height, sys->width);

  double rate    = 0.01;
  double elapsed = 0.00;


  if(sys) {
    while(sys->state != T_EXIT) {

      // Handle the passage of time
      term_calc_delta(sys);

      if(sys->delta > 0.60) sys->delta = 0.60;
      elapsed += sys->delta;

      // Fix that timestep to run at a const rate
      while(elapsed >= rate) {
        // Handle logic
        term_handle_logic(sys->game);

        // Render the screen
        term_render(sys);

        // Handle user input
        int opt  = wgetch(stdscr);
        term_handle_key(sys, opt);

        elapsed -= rate;
      }
    }
  }

  return;
}


void term_render(struct system *sys) {
  if(sys) {
    // Ensure the window is the correct size
    getmaxyx(stdscr, sys->height, sys->width);
    wresize(stdscr, sys->height, sys->width);
    werase(stdscr);

    // Calculate the dimensions of the game buffer
    int min_x = (sys->width / 2) - (buffer_w / 2);
    int min_y = (sys->height / 2) - (buffer_h / 2);


    // Print the game buffer
    for(int x = 0; x < buffer_w; x++) {
      for(int y = 0; y < buffer_h; y++)
        switch(sys->game->scr_buff[y * buffer_w + x]) {
        case '0':
          wattron(stdscr, COLOR_PAIR(2));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(2));
          break;
        case '1':
          wattron(stdscr, COLOR_PAIR(3));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(3));
          break;
        case '2':
          wattron(stdscr, COLOR_PAIR(4));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(4));
          break;
        case '3':
          wattron(stdscr, COLOR_PAIR(5));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(5));
          break;
        case '4':
          wattron(stdscr, COLOR_PAIR(6));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(6));
          break;
        case '5':
          wattron(stdscr, COLOR_PAIR(3));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(3));
          break;
        case '6':
          wattron(stdscr, COLOR_PAIR(4));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(4));
          break;
        case '7':
          wattron(stdscr, COLOR_PAIR(5));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(5));
          break;
        case '#':
          wattron(stdscr, COLOR_PAIR(8));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(8));
        default:
          break;
        };
    }
  }

  wrefresh(stdscr);
}


void term_handle_key(struct system *sys, int opt) {
  if(opt) {
    switch(opt) {
    case KEY_RESIZE:
      getyx(stdscr, sys->height, sys->width);
      endwin();
      refresh();
      break;
    case CTRL_KEY('x'):
      sys->state = T_EXIT;
      break;
    case 'f':
      sys->game->shape += 1;
      if(sys->game->shape >= 7)
        sys->game->shape = 0;
      break;
    case 'w':
      if(term_can_move_shape(sys->game, sys->game->pos_x, sys->game->pos_y - 1))
        sys->game->pos_y -= 1;
      break;
    case 'a':
      if(term_can_move_shape(sys->game, sys->game->pos_x - 1, sys->game->pos_y))
        sys->game->pos_x -= 1;
      break;
    case 's':
      if(term_can_move_shape(sys->game, sys->game->pos_x, sys->game->pos_y + 1))
        sys->game->pos_y += 1;
      break;
    case 'd':
      if(term_can_move_shape(sys->game, sys->game->pos_x + 1, sys->game->pos_y))
        sys->game->pos_x += 1;
      break;
    case 'e':
      if(sys->game->rotate < 3) {
        sys->game->rotate += 1;
        if(!term_can_move_shape(sys->game, sys->game->pos_x, sys->game->pos_y))
          sys->game->rotate -= 1;
      } else {
        sys->game->rotate = 0;
      }
      break;
      case 'q':
        if(sys->game->rotate > 0) {
          sys->game->rotate -= 1;
          if(term_can_move_shape(sys->game, sys->game->pos_x, sys->game->pos_y))
            sys->game->rotate += 1;
        } else {
          sys->game->rotate = 3;
        }
        break;
    default:
      break;
    };
  }
}
