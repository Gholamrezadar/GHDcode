# GHDcode

This is a text editor written in C++ using raylib.
I wanted to understand how text editors work under the hood.

<!-- ![demo image](images/demo_cursor.png) -->
![demo gif](images/demov1.gif)

## Features

- Display basic text
- Line numbers
- Zoom in and out
- Change vertical line spacing
- Cursor movment

## How to use

- Use `Ctrl + Q` to zoom out and `Ctrl + E` to zoom in
- Use `Shift + Q` to decrease the vertical spacing and `Shift + E` to increase the vertical spacing
- Use `Ctrl + G` to toggle the debug grid
- Use Arrow keys to move the cursor (or Ctrl + ijkl)
- Type any character to insert a character (lower+upper case, numbers)
- Type `Backspace` to delete a character
- Type `Enter` to insert a new line
- Use `Crtl + D` to delete a line
- Type `Tab` to insert 4 spaces

## TODO

View TODO.md for a list of features to be implemented.

## Demos

- Text Input works + TAB handling
    ![demo image](images/tabs_text_input.png)
- Add cursor/cadet
    ![demo image](images/demo_cursor.png)
- Right Now!
    ![demo image](images/demo_charbased.png)
- Added Shortcuts for changing line spacing
    ![demo image](images/demo_charbased_grid.png)
- Added Line numbers and Text + padding
    ![demo image](images/demo_linespacing_and_text.png)
- Add vertical line spacing 
    ![demo image](images/demo_linespacing.png)
- Main character grid 
    ![demo image](images/demo_grid_2.png)

## Build

Build using CMake.

```sh
cmake -B build -S .
cmake --build build --config Debug
```
