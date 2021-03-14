////////////////////////////////////////////////////////////////////////////
//
// termnoid - game.h
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

#ifndef _GAME_H
#define _GAME_H


/////////////////////////////////////////////////////////////
// GAME MACRO'S
//

#define buffer_w 14
#define buffer_h 18


/////////////////////////////////////////////////////////////
// GAME TYPES
//

enum rotation {
  R_0 = 0, R_90 = 1, R_180 = 2, R_270 = 3
};

struct game {
  char scr_buff[buffer_w * buffer_h];
  char lvl_buff[buffer_w * buffer_h];

  int  shape;
  int  pos_x;
  int  pos_y;
  int  rotate;

  int  speed;
  int  level;
  int  score;
  int  game_over;

  float elapsed;
};


/////////////////////////////////////////////////////////////
// GAME FUNCTION DECLARATIONS
//

// Game logic functions
int  term_generate_random(int l, int h);
int  term_get_rotation(int x, int y, int rotation);
int  term_is_valid_move(struct game *game, int x, int y);
void term_is_stuck(struct game *game);
void term_remove_line(struct game *game, int line);
void term_check_lines(struct game *game);
void term_move_down(struct game *game, float delta);
void term_score(struct game *game, int lines);
void term_respawn(struct game *game);
void term_reset(struct game *game);

// Game buffer functions
void term_gen_buffer(struct game *game);
void term_gen_screen(struct game *game);


#endif // _GAME_H
