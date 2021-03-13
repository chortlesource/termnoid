////////////////////////////////////////////////////////////////////////////
//
// termnoid - system.h
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


#ifndef _SYSTEM_H
#define _SYSTEM_H


/////////////////////////////////////////////////////////////
// SYSTEM MACRO'S
//

#define CTRL_KEY(x) ((x) & 0x1f)


/////////////////////////////////////////////////////////////
// SYSTEM TYPES
//

enum sysval {
  T_ERR = 0, T_OK = 1
};

enum state {
  T_EXIT = 0, T_RUN = 1, T_GOVER
};

struct system {
  int state;
  int height;
  int width;
  int rotate;

  long   last_time;
  double delta;
  struct game *game;
};


/////////////////////////////////////////////////////////////
// SYSTEM FUNCTION DECLARATIONS
//

// System struct functions
struct system* term_new_system(const int argc, const char *argv[]);
void           term_free_system(struct system *sys);

// System initialization functions
int            term_init_curses();
void           term_exit_curses();

// System main loop
void           term_run(struct system *sys);
void           term_render(struct system *sys);
void           term_handle_logic(struct system *sys);
void           term_print_stats(struct system *sys);

// System key functions
void           term_handle_key(struct system *sys, int opt);
void           term_handle_key_run(struct system *sys, int opt);
void           term_handle_key_over(struct system *sys, int opt);


#endif // _SYSTEM_H
