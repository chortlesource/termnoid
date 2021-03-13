////////////////////////////////////////////////////////////////////////////
//
// termnoid - termnoid.h
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
// MAIN FUNCTION
//

int main(const int argc, const char *argv[]) {
  // Handle args
  if(term_handle_args(argc, argv)) {
    // Initialize the system struct
    struct system *sys = term_new_system(argc, argv);

    if(sys) {
      // Run the main application loop
      term_run(sys);

      // Free system struct
      term_free_system(sys);
      term_exit_curses();
    }
  }

  return 0;
}
