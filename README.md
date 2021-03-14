# termnoid
A tetris clone for the terminal written in `C`

<p align="center">
  <img src="gfx/termnoid.png?raw=true" alt="termnoid a tetris clone!"/>
</p>

## Description

Termnoid is a tetris clone for the terminal written in C using the ncurses library. The ambition is to keep it tiny and currently it occupies around 95KB when compiled.

As with most tetris clones Termnoid features incremental difficulty based upon score. Leveling up results in accelerating the tetrimino's descent.

## Dependencies

* A posix system (depends upon `sys/time.h`)
* libncurses >= 6.1
* cmake >= 3.1

## Installation

* `git clone git@github.com:chortlesource/termnoid.git`
* `cd termnoid`
* `mkdir buld && cd build`
* `cmake ..`
* `make`

## Controls

Key bindings can be accessed in game using `CTRL + F`

* Q   | Rotate counter clockwise
* E   | Rotate clockwise
* A   | Move left
* S   | Move down
* D   | Move right
* TAB | Toggle stats
* ^F  | Show keybinding
* ^N  | Start a new game
* ^X  | Exit the program

## Attribution

This project was inspired by OLC and the rotation algorithm used is unashamedly pinched from his source. You can find OLC [here](https://github.com/OneLoneCoder).

## Copyright

Copyright (c) 2021 Christopher M. Short

termnoid is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

termnoid is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.
