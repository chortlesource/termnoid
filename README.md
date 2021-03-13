# TERMNOID - A terminal based tetris clone in C

<p align="center">
  <img src="gfx/sublime_text.png?raw=true" alt="termnoid a tetris clone!"/>
</p>

## Description

Termnoid is a tetris clone for the terminal written in C using the ncurses library. The ambition is to keep it tiny and currently it occupies 97KB when compiled.

Termnoid is not yet feature complete and lacks incremental difficulty and leveling which I am yet to implement.

## Dependencies

* A posix system (depends upon `sys/time.h`)
* libncurses >= 6.1

## Installation

`git clone git@github.com:chortlesource/termnoid.git`
`cd termnoid`
`mkdir buld && cd build`
`cmake ..`
`make`

## Controls

Key bindings can be accessed in game using `CTRL + F`

* Q   | Rotate counter clockwise
* E   | Rotate clockwise
* A   | Move left
* S   | Move down
* D   | Move right
* TAB | Toggle stats

To quit the application use `CTRL + X`

## Attribution

This project was inspired by OLC and whilst the bulk of the code is my own the shape rotation algorithm is unashamedly pinched from his source. You can find OLC [here](https://github.com/OneLoneCoder).

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
