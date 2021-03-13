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
// SYSTEM STATIC VARS / FUNCTIONS
//

static const char *APP_NAME = "termnoid";
static const char *APP_AUTH = "C. M. Short";
static const char *APP_VERS = "0.0.1-beta";

static const char *bindings[9] = {
  "-- KEY MAP --",
    "Q   | Rotate counter clockwise",
    "E   | Rotate clockwise",
    "A   | Move shape left",
    "D   | Move shape right",
    "S   | Move shape down",
    "TAB | Toggle stats",
    "^F  | Toggle this prompt",
    "^X  | Exit program"
    };

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
      term_reset(game);
      sys->game = game;

      sys->height    = 0;
      sys->width     = 0;
      sys->stats     = 1;
      sys->keys      = 0;
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


int term_handle_args(const int argc, const char *argv[]) {
  if(argc == 2) {
    char opt1[] = "-v";
    char opt2[] = "-V";

    if(strncmp(opt1, argv[1], 3) == 0) {
      printf("%s [%s] by %s.\n", APP_NAME, APP_VERS, APP_AUTH);
      return 0;
    }

    if(strncmp(opt2, argv[1], 3) == 0) {
      printf("%s [%s] by %s.\n", APP_NAME, APP_VERS, APP_AUTH);
      return 0;
    }
  }

  return 1;
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
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_RED);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    init_pair(6, COLOR_BLACK, COLOR_YELLOW);
    init_pair(7, COLOR_BLACK, COLOR_CYAN);
    init_pair(8, COLOR_WHITE, COLOR_MAGENTA);
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

      if(sys->delta > 0.80) sys->delta = 0.80;
      elapsed += sys->delta;

      // Fix that timestep to run at a const rate
      while(elapsed >= rate) {

        // Handle logic
        if(sys->state == T_RUN)
          term_handle_logic(sys);

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
          break;
        case 'X':
          wattron(stdscr, COLOR_PAIR(1));
          mvwaddch(stdscr, min_y + y, min_x + x, ' ');
          wattroff(stdscr, COLOR_PAIR(1));
          break;
        default:
          break;
        };
    }
  }

  // Print the stats
  if(sys->stats)
    term_print_stats(sys);

  // Print the overlay if required
  if(sys->state == T_PAUSE || sys->state == T_GOVER)
    term_print_overlay(sys);

  // Print the key bindings if required
  if(sys->keys)
    term_print_keys(sys);

  // Print the program borders
  term_print_borders(sys);

  wrefresh(stdscr);
}


void           term_handle_logic(struct system *sys) {
  // Move the shape down
  term_move_shape_down(sys->game, sys->delta);

  // Check to see if any lines are completed
  term_check_lines(sys->game);

  // Build the screen buffer before render call
  term_build_screen(sys->game);

  // Check to see if game over
  if(sys->game->game_over)
    sys->state = T_GOVER;
}


void term_print_borders(struct system *sys) {
  wattron(stdscr, COLOR_PAIR(8));

  // Print the frame
  for(int x = 0; x < sys->width; x++) {
    for(int y = 0; y < sys->height; y++) {
      // Render the frame sides
      if(x == 0 || x == sys->width - 1)
        mvwaddch(stdscr, y, x, ' ');

      // Render the frame top and bottom
      if(y == 0 || y == sys->height - 1)
        mvwaddch(stdscr, y, x, ' ');
    }
  }

  // Print the application title
  mvwprintw(stdscr, 0, (sys->width / 2) - (strnlen(APP_NAME, 12) / 2), APP_NAME);

  // Print the footer details
  char *qstr[4] = { "[^X]", " Exit", "[^F]", " Key Bindings" };
  int  offset   = strlen(qstr[0]) + strlen(qstr[3]);

  mvwprintw(stdscr, sys->height - 1, 1 +strlen(qstr[0]), qstr[1]);
  mvwprintw(stdscr, sys->height - 1, offset + 1 + strlen(qstr[2]), qstr[3]);
  wattroff(stdscr, COLOR_PAIR(8));

  wattron(stdscr, COLOR_PAIR(1));
  mvwprintw(stdscr, sys->height - 1, 1, qstr[0]);
  mvwprintw(stdscr, sys->height - 1, offset + 1, qstr[2]);
  mvwprintw(stdscr, sys->height - 1, sys->width - strlen(APP_VERS) - 1, APP_VERS);
  wattroff(stdscr, COLOR_PAIR(1));
}


void term_print_stats(struct system *sys) {
  char score[] = "SCORE: ";
  char level[] = "LEVEL: ";

  int slen = snprintf(NULL, 0, "%d", sys->game->score);
  int llen = snprintf(NULL, 0, "%d", sys->game->level);

  char sval[slen];
  char lval[llen];

  snprintf(sval, slen + 1, "%d", sys->game->score);
  snprintf(lval, llen + 1, "%d", sys->game->level);

  int startx = (sys->width / 5) * 3;
  int starty = (sys->height / 5) * 2;

  wattron(stdscr, COLOR_PAIR(1));
  mvwprintw(stdscr, starty, startx, "%s", score);
  mvwprintw(stdscr, starty + 1, startx, "%s", level);
  mvwprintw(stdscr, starty, startx + strlen(score), sval);
  mvwprintw(stdscr, starty + 1, startx + strlen(level), lval);
  wattroff(stdscr, COLOR_PAIR(1));
}


void term_print_overlay(struct system *sys) {
  if(sys->state == T_PAUSE) {
    char mesg[] = "--- GAME PAUSED ---";
    int x = (sys->width / 2) - (strlen(mesg) / 2);
    int y = (sys->height / 2);

    wattron(stdscr, COLOR_PAIR(1));
    mvwprintw(stdscr, y, x, "%s", mesg);
    wattroff(stdscr, COLOR_PAIR(1));
  }

  if(sys->state == T_GOVER) {
    char mesg1[] = "--- GAME OVER ---";
    char mesg2[] = "[TAB TO RESTART]";
    int x1 = (sys->width / 2) - (strlen(mesg1) / 2);
    int x2 = (sys->width / 2) - (strlen(mesg2) / 2);
    int y  = (sys->height / 2);

    wattron(stdscr, COLOR_PAIR(1));
    mvwprintw(stdscr, y, x1, "%s", mesg1);
    mvwprintw(stdscr, y + 2, x2, "%s", mesg2);
    wattroff(stdscr, COLOR_PAIR(1));
  }
}


void term_print_keys(struct system *sys) {
  // Declare some helper vars
  int width  = strlen(bindings[1]) + 4; // widest string
  int height = 14;
  int startx = (sys->width / 2) - (width / 2);
  int starty = (sys->height / 2) - (height / 2);

  // First pass paint black
  wattron(stdscr, COLOR_PAIR(1));
  for(int x = startx; x < startx + width; x++) {
    for(int y = starty; y <starty + height; y++) {
      // Render the frame

      mvwaddch(stdscr, y, x, ' ');
    }
  }
  wattroff(stdscr, COLOR_PAIR(1));

  // Second pass paint the frame
  wattron(stdscr, COLOR_PAIR(8));
  for(int x = startx; x < startx + width; x++) {
    for(int y = starty; y <starty + height; y++) {
      if(x == startx || x == startx + width - 1)
        mvwaddch(stdscr, y, x, ' ');

      if(y == starty || y == starty + height - 1)
        mvwaddch(stdscr, y, x, ' ');
    }
  }
  wattroff(stdscr, COLOR_PAIR(8));

  wattron(stdscr, COLOR_PAIR(1));
  for(int i = 1; i < 9; i++)
    mvwprintw(stdscr, starty+ 3 + i, startx + 2, "%s", bindings[i]);

  int tpos = (startx + width / 2) - (strlen(bindings[0]) / 2);
  mvwprintw(stdscr, starty+ 2, tpos, "%s", bindings[0]);
  wattroff(stdscr, COLOR_PAIR(1));
}


void term_handle_key(struct system *sys, int opt) {
  if(opt) {
    switch(opt) {
    case KEY_RESIZE:
      getyx(stdscr, sys->height, sys->width);
      endwin();
      refresh();
      break;
    default:
      switch(sys->state) {
      case T_RUN:
        term_handle_key_run(sys, opt);
        break;
      case T_GOVER:
        term_handle_key_over(sys, opt);
        break;
      case T_PAUSE:
        term_handle_key_pause(sys, opt);
        break;
      default:
        break;
      };
    };
  }
}


void term_handle_key_run(struct system *sys, int opt) {
  if(opt) {
    switch(opt) {
    case CTRL_KEY('x'):
      sys->state = T_EXIT;
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
          if(!term_can_move_shape(sys->game, sys->game->pos_x, sys->game->pos_y))
            sys->game->rotate += 1;
        } else {
          sys->game->rotate = 3;
        }
        break;
    case 'p':
      if(sys->state == T_RUN)
        sys->state = T_PAUSE;
      break;
    case '\t':
      sys->stats = !sys->stats;
      break;
    case CTRL_KEY('f'):
      sys->keys = !sys->keys;
      break;
    default:
      break;
    };
  }
}


void term_handle_key_pause(struct system *sys, int opt) {
  if(opt) {
    switch(opt) {
    case 'p':
      sys->state = T_RUN;
      break;
    case '\t':
      sys->stats = !sys->stats;
      break;
    case CTRL_KEY('f'):
      sys->keys = !sys->keys;
      break;
    default:
      break;
    };
  }
}


void term_handle_key_over(struct system *sys, int opt) {
  if(opt) {
    switch(opt) {
    case CTRL_KEY('x'):
      sys->state = T_EXIT;
      break;
    case '\t':
      term_reset(sys->game);
      sys->state = T_RUN;
    case CTRL_KEY('f'):
      sys->keys = !sys->keys;
      break;
    default:
      break;
    };
  }
}
