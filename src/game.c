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

static const char *shapes[7] = {
  "0010001000100010",
  "0020022000200000",
  "0000033003300000",
  "0040044004000000",
  "0500055000500000",
  "0600060006600000",
  "0070007007700000"
};



/////////////////////////////////////////////////////////////
// GAME FUNCTION IMPLEMENTATION
//

// Game logic functions
void term_handle_logic(struct game *game) {
  // Render the tetromino
  term_render_screen(game);
}

int  term_get_rotation(int x, int y, int rotation) {
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
  if(game) {
    for(int tx = 0; tx < 4; tx++) {
      for(int ty = 0; ty < 4; ty++) {

        // Calculate the shape and level position
        int sp = term_get_rotation(tx, ty, game->rotate);
        int lp = (y + ty) * buffer_w + (x + tx);

        if((x + tx) >= 0 && (x + tx) < buffer_w) {
          if((y + ty) >= 0 && (y + ty) < buffer_h) {
            if(shapes[game->shape][sp] != '0' && game->lvl_buff[lp] != '0')
              return 0;
          }
        }
      }
    }
  }

  return 1;
}


void term_init_buffer(struct game *game) {
  // Zero out the game field
  for(int i = 0; i < buffer_h * buffer_w; i++)
    game->lvl_buff[i] = '0';

  // Initialize the border
  for(int x = 0; x < buffer_w; x++) {
    for(int y = 0; y < buffer_h; y++) {
      if(x == 0 || x == buffer_w - 1)
        game->lvl_buff[y * buffer_w + x] = '#';

      if(y == 0 || y == buffer_h - 1)
       game->lvl_buff[y * buffer_w + x] = '#';
    }
  }
}


void term_render_screen(struct game *game) {
  if(game) {
    // Copy accross the lvl_buff data
    int buffsize = buffer_w * buffer_h;
    for(int i = 0; i < buffsize; i++)
      game->scr_buff[i] = game->lvl_buff[i];

    // Next copy accross the shape
    for(int x = 0; x < 4; x++) {
      for(int y = 0; y < 4; y++) {
        if(shapes[game->shape][term_get_rotation(x, y, game->rotate)] != '0')
          game->scr_buff[(game->pos_y + y) * buffer_w + (game->pos_x + x)] = shapes[game->shape][term_get_rotation(x, y, game->rotate)];
      }
    }
  }
}
