////////////////////////////////////////////////////////////////////////////
//
// termnoid - game.c
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
// GAME STATIC GLOBALS
//

static const char *tetromino[5] = {
  "0000011001100000",
  "0000002000200220",
  "0000030003000330",
  "0044044000000000",
  "0000005005500050"
};


/////////////////////////////////////////////////////////////
// GAME FUNCTION IMPLEMENTATION
//

// Game logic functions
void term_handle_logic(struct game *game) {
  // Render the tetromino
  term_render_shape(game);
}

int  term_rotate_shape(int x, int y, int rotation) {
  int rvalue = 0;

  switch(rotation) {
  case R_0:
    rvalue = y * 4 + x;
    break;
  case R_90:
    rvalue = y + 12 - (x * 4);
    break;
  case R_180:
    rvalue = 15 - (y * 4) - x;
    break;
  case R_270:
    rvalue = 3 - y + (x * 4);
    break;
  default:
    break;
  };

  return rvalue;
}

int  term_can_move_shape(struct game *game, int x, int y) {
  return 1;
}


void term_init_buffer(struct game *game) {
  // Zero out the game field
  for(int i = 0; i < buffer_h * buffer_w; i++)
    game->buffer[i] = '0';

  // Initialize the border
  for(int x = 0; x < buffer_w; x++) {
    for(int y = 0; y < buffer_h; y++) {
      if(x == 0 || x == buffer_w - 1)
        game->buffer[y * buffer_w + x] = '#';

      if(y == 0 || y == buffer_h - 1)
       game->buffer[y * buffer_w + x] = '#';
    }
  }
}


void term_render_shape(struct game *game) {
  // Test the addition of tetromino's to the game
  for(int x = 0; x < 4; x++) {
    for(int y = 0; y < 4; y++) {
      if(tetromino[game->shape][term_rotate_shape(x, y, game->rotate)] != 0)
        game->buffer[(game->pos_y + y) * buffer_w + (game->pos_x + x)] = tetromino[game->shape][term_rotate_shape(x, y, game->rotate)];
    }
  }
}
