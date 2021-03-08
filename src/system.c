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
// SYSTEM FUNCTION IMPLEMENTATION
//

struct system*  term_new_system(const int argc, const char *argv[]) {
  // Allocate memory to the system struct
  struct system *sys = malloc(sizeof(struct system));

  if(sys) {
    struct game *game = malloc(sizeof(struct game));

    if(game) {
      // Zero out the game field
      for(int i = 0; i < buffer_h * buffer_w; i++)
        game->buffer[i] = '0';

      sys->game = game;
      sys->game->shape = 0;
      sys->game->pos_x = (buffer_w / 2);
      sys->game->pos_y = (buffer_h / 2);
      sys->state = T_RUN; // To enable the main loop
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
    cbreak();
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

  if(sys) {
    while(sys->state != T_EXIT) {

      // Render the screen
      term_render(sys);

      // Handle user input
      int opt  = wgetch(stdscr);
      sys->key = opt;

      switch(opt) {
      case KEY_RESIZE:
        getyx(stdscr, sys->height, sys->width);
        endwin();
        refresh();
        break;
      case CTRL_KEY('x'):
        sys->state = T_EXIT;
        break;
      case 's':
        sys->game->shape += 1;
        if(sys->game->shape >= 5)
          sys->game->shape = 0;
      default:
        endwin();
        refresh();
        break;
      };
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

    // Test the addition of tetromino's to the game
    for(int x = 0; x < 4; x++) {
      for(int y = 0; y < 4; y++) {
        if(tetromino[sys->game->shape][y*4+x] != 0)
          sys->game->buffer[(sys->game->pos_y + y) * buffer_w + (sys->game->pos_x + x)] = tetromino[sys->game->shape][y*4+x];
      }
    }

    // Print the game buffer
    for(int x = 0; x < buffer_w; x++) {
      for(int y = 0; y < buffer_h; y++)
        switch(sys->game->buffer[y * buffer_w + x]) {
        default:
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
          wattron(stdscr, COLOR_PAIR(7));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(7));
          break;
        };
    }
  }
}
