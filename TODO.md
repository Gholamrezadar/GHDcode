# TODO

## Cursor

- [x] Cursor shouldn't move on empty space
- [x] Cursor should have a memory of the last horizontal position

## Editor

- [x] Insert text at cursor
- [x] Delete text at cursor (backspace)
  - [x] Needs work
- [x] Enter key should insert a new line
- [x] Only Render the visible lines
- [x] Set cursor position with mouse
  - [x] Is off by a few pixels (fixed by subtracting editors window padding.)
- [ ] Ctrl+backspace to delete a word before the cadet
- [ ] Scrolling
  - [x] Scroll wheel
  - [x] Follow cursor
  - [ ] Page up/down
  - [ ] Ctrl + Home/end
- [ ] Duplicate line with Ctrl+Shift+Arrow
- [ ] Move line up/down with Alt+Arrow
- [ ] Load text file from command line arguments
- [ ] New file (Ctrl+N)
- [ ] Save file (Ctrl+S)
- [ ] Selection
  - [ ] Select text (Shift+Movement) or (Ctrl+A)
  - [ ] Copy selected text (Ctrl+C)
  - [ ] Paste text (Ctrl+V)
  - [ ] Cut selected text (Ctrl+X)
  - [ ] Delete selected text
- [ ] Undo/Redo

## File

- [x] Save file
- [x] Load file
- [x] Take command line argument for file address

## Known Bugs

- [ ] viewportLineCount doesnt work properly
  - To recreate, shift+Q to reduce the vertical spacing then ctrl+Q x2 to reduce font. you will see linenumbers disappear after a certain number of lines.
  - fixed but still off by 1 sometimes!
- [x] vertical line spacing changes when zooming for the first time
