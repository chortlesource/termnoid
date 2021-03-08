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

#define buffer_w 12
#define buffer_h 18


/////////////////////////////////////////////////////////////
// GAME TYPES
//

struct game {
  char buffer[buffer_w * buffer_h];
  int  shape;

  int pos_x;
  int pos_y;
};


static const char *tetromino[5] = {
  "0000011001100000",
  "0000002000200220",
  "0000030003000330",
  "0044044000000000",
  "0000005005500050"
};


#endif // _GAME_H
