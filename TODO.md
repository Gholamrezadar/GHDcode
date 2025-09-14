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
- [ ] Take command line argument for file address

## Known Bugs

- [ ] viewportLineCount doesnt work properly
  - To recreate, shift+Q to reduce the vertical spacing then ctrl+Q x2 to reduce font. you will see linenumbers disappear after a certain number of lines.
  - fixed but still off by 1 sometimes!
- [x] vertical line spacing changes when zooming for the first time
- [ ] ctrl L and crrl H do two different things!
- [ ] Zoom in does one more zoom level than necessary (goes to 5 instead of max of 4)

## Notes

Things to do next:

1. Save file
2. Undo/Redo
3. Selection and copy paste
4. Image Effects
5. Imgui debug
