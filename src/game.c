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

void           term_reset(struct game *game) {
  if(game) {
    // Initialize the game buffer
    term_build_buffer(game);

    game->shape     = term_generate_random(0, 6);
    game->pos_x     = (buffer_w / 2) - 2;
    game->pos_y     = 1;
    game->rotate    = R_0;
    game->speed     = 1000;
    game->level     = 0;
    game->score     = 0;
    game->game_over = 0;
    game->elapsed   = 0;
  }
}


int  term_generate_random(int l, int h) {
  srand(time(0));
  return (rand() % (h - l + 1)) + l;
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


void term_shape_is_stuck(struct game *game) {
  if(game) {
    for(int x = 0; x < 4; x++) {
      for(int y = 0; y < 4; y++) {
        if(shapes[game->shape][term_get_rotation(x, y, game->rotate)] != '0')
          game->lvl_buff[(game->pos_y + y) * buffer_w + (game->pos_x + x)] = 'X';
      }
    }
  }
}


int   term_line_found(struct game *game) {
  if(game) {
    for(int y = 1; y < buffer_h; y++) {
      int lcount = 0;
      for(int x = 1; x < buffer_w; x++) {
        if(game->lvl_buff[y*buffer_w+x] == 'X')
          lcount += 1;
      }
      if(lcount == buffer_w - 2) {
        return y;
      }
    }
  }
  return 0;
}


void  term_remove_line(struct game *game, int line) {
  if(game) {
    for(int x = buffer_w; x > 0; x--) {
      for(int y = line; y > 1; y--) {
        game->lvl_buff[y*buffer_w+x] = game->lvl_buff[(y-1)*buffer_w+x];
      }
    }
  }
}


void  term_move_shape_down(struct game *game, float delta) {
  if(game) {
    game->elapsed += delta;

    if(game->elapsed > game->speed) {
      if(term_can_move_shape(game, game->pos_x, game->pos_y + 1)) {
        game->pos_y += 1;
      } else {
        term_shape_is_stuck(game);
        term_respawn(game);
      }

      game->elapsed = 0;
    }
  }
}


void  term_respawn(struct game *game) {
  if(game) {
    game->shape     = term_generate_random(0, 6);
    game->pos_x     = (buffer_w / 2) - 2;
    game->pos_y     = 1;
    game->rotate    = R_0;
    game->game_over = 0;
    game->elapsed   = 0;

    if(!term_can_move_shape(game, game->pos_x, game->pos_y + 1))
      game->game_over = 1;
  }
}


void term_build_buffer(struct game *game) {
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


void term_build_screen(struct game *game) {
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


void term_check_lines(struct game *game) {
  if(game) {
    int line   = 0;
    int lcount = 0;
    while((line = term_line_found(game))) {
      term_remove_line(game, line);
      lcount += 1;
    }
    term_score(game, lcount);
  }
}


void term_score(struct game *game, int lines) {
  if(game) {
    switch(lines) {
    case 0:
      break;
    case 1:
      game->score += 40 * (game->level + 1);
      break;
    case 2:
      game->score += 100 * (game->level + 1);
      break;
    case 3:
      game->score += 300 * (game->level + 1);
      break;
    case 4:
    default:
      game->score += 1200 * (game->level + 1);
      break;
    };
  }
}
