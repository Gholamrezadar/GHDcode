#include <math.h>
#include <raylib.h>

#include <iostream>
#include <string>
#include <vector>

#include "EditorConfig.h"
#include "EditorData.h"
#include "utils.h"

// Initial values for EditorConfig
// TODO: just convert EditorConfig to a class with some utility methods
void initializeEditorConfig(EditorConfig& editor) {
    editor = {
        .windowPadding = 10.0f,
        .baseVerticalLineSpacing = 5.0f,
        .lineNumberWidth = 3,
        .spaceBetweenNumbersAndText = 2,
        .editorZoom = 2.0f,
        .zoomSpeed = 0.5f,
        .fontSize = 16.0f,
        .gridLineWidth = 2.0f,
        .cursorBlinkSpeed = 10.0f,
        .editorBackgroundColor = Color{13, 17, 23},
        .editorTextColor = Color{255, 255, 255, 255},
        .editorLineNumbersColor = Color{255, 255, 255, 100},
        .editorVerticalLineColor = Color{255, 255, 255, 16},
        .editorCursorColor = Color{47, 129, 247, 170},
        .editorGridColor = Color{255, 0, 255, 100},
        .debugGrid = false,
    };
    editor.verticalLineSpacing = editor.baseVerticalLineSpacing * editor.editorZoom;
    editor.fontSize = int(16 * editor.editorZoom);
}

int main(void) {
    // Create window
    int screenWidth = 1280;
    int screenHeight = 720;

    // TODO: they should go into EditorConfig
    int viewportStart = 0;
    int viewportLineCount = 0;

    // SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "GHDcode");
    // SetTargetFPS(60);

    
    // Data
    std::string textData;

    // read from file
    // textData = readFile("resources/sample.txt");
    // textData = readFile("resources/lorem.txt");
    // textData = readFile("resources/lorem_short.txt");

    // read from command line
    // textData = readFileFromCommandLine(argc, argv);

    // empty text
    // TODO: Segmentation fault with empty text
    textData = "";

    // Scroll test text
    textData = R"(1
2
3 333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
    )";

    // Editor Data
    EditorData editorData(textData);

    // Editor Config
    EditorConfig editor;
    initializeEditorConfig(editor);

    // Cursor (TODO: should go into EditorConfig)
    Vector2 cursorPosition = Vector2{0.0f, 0.0f};
    // TODO: zero cursor
    cursorPosition.x = editor.lineNumberWidth + editor.spaceBetweenNumbersAndText;
    cursorPosition.y = 0;

    // TODO: should go into EditorConfig
    float lastCursorPositionX = cursorPosition.x;

    // Load Fonts
    Font font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                           editor.fontSize, nullptr, 0);

    // Set grid dimensions based on font
    editor.gridWidth = MeasureTextEx(font, "A", editor.fontSize, 0).x;
    editor.gridHeight = MeasureTextEx(font, "A", editor.fontSize, 0).y;


    // Main loop (ESC to exit)
    
    while (!WindowShouldClose()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        ///////////////////////// Input Handling /////////////////////////
        {
            // Zoom (Ctrl+Q and Ctrl+E)
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) {
                editor.editorZoom -= editor.zoomSpeed;
                editor.editorZoom = MAX(0.5f, editor.editorZoom);
                std::cout << "zoom: " << editor.editorZoom << std::endl;
                if (editor.editorZoom > 0.5f) {
                    // int fontSize = int(16 * editor.editorZoom);
                    editor.fontSize = int(16 * editor.editorZoom);
                    UnloadFont(font);
                    font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                                      editor.fontSize, nullptr, 0);
                    editor.gridWidth =
                        MeasureTextEx(font, "A", editor.fontSize, 0).x;
                    editor.gridHeight =
                        MeasureTextEx(font, "A", editor.fontSize, 0).y;
                    editor.verticalLineSpacing =
                        editor.baseVerticalLineSpacing * editor.editorZoom;
                }
            }
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_E)) {
                editor.editorZoom += editor.zoomSpeed;
                editor.editorZoom = MIN(5.0f, editor.editorZoom);
                std::cout << "zoom: " << editor.editorZoom << std::endl;
                if (editor.editorZoom < 5.0f) {
                    // int fontSize = int(16 * editor.editorZoom);
                    editor.fontSize = int(16 * editor.editorZoom);
                    UnloadFont(font);
                    font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                                      editor.fontSize, nullptr, 0);
                    editor.gridWidth =
                        MeasureTextEx(font, "A", editor.fontSize, 0).x;
                    editor.gridHeight =
                        MeasureTextEx(font, "A", editor.fontSize, 0).y;
                    editor.verticalLineSpacing =
                        editor.baseVerticalLineSpacing * editor.editorZoom;
                }
            }

            // Toggle debug grid (Ctrl+G)
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_G)) {
                editor.debugGrid = !editor.debugGrid;
            }

            // Change vertical spacing (Shift+E and Shift+Q)
            if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_E)) {
                editor.baseVerticalLineSpacing += 5;
                editor.baseVerticalLineSpacing =
                    MIN(editor.baseVerticalLineSpacing, 100);
                editor.verticalLineSpacing =
                    editor.baseVerticalLineSpacing * editor.editorZoom;
            }
            if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Q)) {
                editor.baseVerticalLineSpacing -= 5;
                editor.baseVerticalLineSpacing =
                    MAX(editor.baseVerticalLineSpacing, 0);
                editor.verticalLineSpacing =
                    editor.baseVerticalLineSpacing * editor.editorZoom;
            }

            // Cursor movement
            if (IsKeyPressed(KEY_LEFT) ||
                (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H))) {
                cursorPosition.x -= 1;
                cursorPosition.x = MAX(
                    editor.lineNumberWidth + editor.spaceBetweenNumbersAndText,
                    cursorPosition.x);
                lastCursorPositionX = cursorPosition.x;
            }
            if (IsKeyPressed(KEY_RIGHT) ||
                (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L))) {
                cursorPosition.x += 1;
                cursorPosition.x =
                    MIN(editorData.getLine(cursorPosition.y).size() +
                            editor.lineNumberWidth +
                            editor.spaceBetweenNumbersAndText,
                        cursorPosition.x);
                lastCursorPositionX = cursorPosition.x;
            }
            if (IsKeyPressed(KEY_UP) ||
                (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_K))) {
                cursorPosition.y -= 1;
                // cursorPosition.y = MAX(0, cursorPosition.y);
                if (cursorPosition.y < 0) {
                    viewportStart -= 1;
                    viewportStart = MAX(viewportStart, 0);
                    cursorPosition.y = 0;
                }
                // also set the x position to the max of position and
                // lastCursorPositionX
                cursorPosition.x = MAX(cursorPosition.x, lastCursorPositionX);
            }
            if (IsKeyPressed(KEY_DOWN) ||
                (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_J))) {
                cursorPosition.y += 1;
                if (cursorPosition.y > viewportLineCount - 1) {
                    viewportStart += 1;
                    viewportStart = MIN(viewportStart, editorData.getNumLines() - viewportLineCount - 1);
                    cursorPosition.y = viewportLineCount - 1;
                }
                // cursorPosition.y =
                //     MIN(cursorPosition.y, viewportLineCount);
                // MIN(editorData.getNumLines() - 1, cursorPosition.y);
                // also set the x position to the max of position and
                // lastCursorPositionX
                cursorPosition.x = MAX(cursorPosition.x, lastCursorPositionX);
            }

            // Handle mouse click
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                // std::cout << GetMouseX() << std::endl;
                int y = GetMouseY();
                y -= editor.windowPadding;
                y /= editor.gridHeight + editor.verticalLineSpacing;
                cursorPosition.y = y > editorData.getNumLines() - 1 ? editorData.getNumLines() - 1 : y;

                int x = GetMouseX();  // / (int)editor.gridWidth) * (int)editor.gridWidth  + editor.windowPadding;
                x -= editor.windowPadding;
                x /= editor.gridWidth;
                cursorPosition.x = x;
                // std::cout << GetMouseY() << std::endl;
            }

            // Text editing (typing, deleting, etc.)
            {
                // Insert character by typing
                {
                    int key;
                    while ((key = GetCharPressed()) !=
                           0) {  // Check if a key is pressed
                        // std::cout << "key1: " << key << std::endl;
                        // ignore backspace
                        if (key == KEY_BACKSPACE) {
                            break;
                        }

                        if (key >= 32 &&
                            key <=
                                126) {  // Filter for printable ASCII characters
                            // Convert from grid space to line space
                            int charNumber = cursorPosition.x -
                                             editor.lineNumberWidth -
                                             editor.spaceBetweenNumbersAndText;
                            editorData.insertChar(cursorPosition.y, charNumber,
                                                  (char)key);
                            // move cursor to the right
                            // TODO: do we need to change the last cursor
                            // position?
                            cursorPosition.x++;
                        }
                    }
                }

                // Delete character by pressing backspace
                {
                    // int key;
                    // while ((key = GetCharPressed()) != 0)
                    // {

                    //     std::cout << "key: " << key << std::endl;
                    //     // Check if a backspace key was pressed
                    //     if (key == KEY_BACKSPACE)
                    //     {
                    if (IsKeyPressedRepeat(KEY_BACKSPACE) || IsKeyPressed(KEY_BACKSPACE)) {
                        // Convert from grid space to line space
                        int charNumber = cursorPosition.x -
                                         editor.lineNumberWidth -
                                         editor.spaceBetweenNumbersAndText;

                        // ignore backspace if at the first char
                        if (charNumber > 0) {
                            // delete the char at prev position
                            editorData.deleteChar(cursorPosition.y, charNumber - 1);
                            // move cursor to the left
                            cursorPosition.x--;
                            // clamp cursor to the line width
                            cursorPosition.x = MAX(
                                editor.lineNumberWidth +
                                    editor.spaceBetweenNumbersAndText,
                                cursorPosition.x);
                        }
                        // if at first cursor position -> merge this line with the previous one
                        // aka delete the line break
                        if (charNumber == 0) {
                            int lineNumber = cursorPosition.y;
                            if (lineNumber > 0) {
                                // move the cursor up
                                cursorPosition.y = MAX(0, cursorPosition.y - 1);
                                // move the cursor to the end of the line
                                cursorPosition.x = editor.lineNumberWidth + editor.spaceBetweenNumbersAndText + editorData.lines[cursorPosition.y].length();
                                // merge this line with the previous one
                                editorData.lines[lineNumber - 1] = editorData.lines[lineNumber - 1] + editorData.lines[lineNumber];
                                // delete the line
                                editorData.lines.erase(editorData.lines.begin() + lineNumber);
                            }
                        }
                    }
                    // }
                }

                // new line by pressing enter
                {
                    if (IsKeyPressed(KEY_ENTER)) {
                        // Convert from grid space to line space
                        int charNumber = cursorPosition.x -
                                         editor.lineNumberWidth -
                                         editor.spaceBetweenNumbersAndText;
                        editorData.insertLineBreak(cursorPosition.y,
                                                   charNumber);
                        // move cursor to the left
                        // TODO: do we need to change the last cursor
                        // position?
                        cursorPosition.x = editor.lineNumberWidth +
                                           editor.spaceBetweenNumbersAndText;
                        cursorPosition.y++;
                    }
                }

                // Home and End keys
                {
                    if (IsKeyPressed(KEY_HOME)) {
                        cursorPosition.x = 0 + editor.lineNumberWidth + editor.spaceBetweenNumbersAndText;
                    }

                    if (IsKeyPressed(KEY_END)) {
                        cursorPosition.x = editorData.lines[cursorPosition.y].length() + editor.lineNumberWidth + editor.spaceBetweenNumbersAndText;
                    }
                }

                // Delete line by pressing Ctrl+D
                // TODO: doesn't work has edge case bugs (delete first line to see)
                {
                    if (IsKeyPressed(KEY_D) &&
                        IsKeyDown(KEY_LEFT_CONTROL)) {
                        // Convert from grid space to line space
                        int charNumber = cursorPosition.x -
                                         editor.lineNumberWidth -
                                         editor.spaceBetweenNumbersAndText;
                        editorData.deleteLine(cursorPosition.y);
                        // move cursor to the left
                        cursorPosition.x = editor.lineNumberWidth +
                                           editor.spaceBetweenNumbersAndText;
                        cursorPosition.y--;
                        // clamp cursor to the line width
                        cursorPosition.x = MAX(
                            editor.lineNumberWidth +
                                editor.spaceBetweenNumbersAndText,
                            cursorPosition.x);
                    }
                }

                // Tab key
                {
                    if (IsKeyPressed(KEY_TAB)) {
                        // Convert from grid space to line space
                        int charNumber = cursorPosition.x -
                                         editor.lineNumberWidth -
                                         editor.spaceBetweenNumbersAndText;
                        editorData.insertChar(cursorPosition.y, charNumber, '\t');
                        // move cursor to the right
                        cursorPosition.x += 4;
                    }
                }
            }

            // if the cursor is out of bounds, clamp it to the current line
            // width
            if (cursorPosition.x > editorData.getLine(cursorPosition.y).size() +
                                       editor.lineNumberWidth +
                                       editor.spaceBetweenNumbersAndText) {
                // Save the last cursor position.x
                lastCursorPositionX =
                    MAX(lastCursorPositionX, cursorPosition.x);

                // Clamp cursor x position to the current line width
                cursorPosition.x = editorData.getLine(cursorPosition.y).size() +
                                   editor.lineNumberWidth +
                                   editor.spaceBetweenNumbersAndText;
            }

            // reload the last cursor position.x if possible
            if (lastCursorPositionX >= cursorPosition.x &&
                lastCursorPositionX <=
                    editorData.getLine(cursorPosition.y).size() +
                        editor.lineNumberWidth +
                        editor.spaceBetweenNumbersAndText) {
            }
        }


        ///////////////////////// Drawing /////////////////////////
        {
            BeginDrawing();
            ClearBackground(editor.editorBackgroundColor);
            DrawFPS(screenWidth - 110, 10);

            // Draw the working grid (this is a grid that every character can be placed on)
            if (true) {
                // Draw the grid
                if (editor.debugGrid) {
                    int gridRows =
                        static_cast<int>(screenHeight / editor.gridHeight);
                    int gridCols = static_cast<int>(screenWidth / editor.gridWidth);

                    // Draw vertical lines
                    for (int i = 0; i < gridCols + 1; i++) {
                        DrawRectangle(i * editor.gridWidth + editor.windowPadding,
                                    editor.windowPadding, editor.gridLineWidth,
                                    screenHeight - editor.windowPadding * 2,
                                    editor.editorGridColor);
                    }

                    // Draw Horizontal lines
                    for (int i = 0; i < gridRows + 1; i++) {
                        DrawRectangle(editor.windowPadding,
                                    i * editor.verticalLineSpacing +
                                        i * editor.gridHeight +
                                        editor.windowPadding,
                                    screenWidth - editor.windowPadding * 2,
                                    editor.gridLineWidth, editor.editorGridColor);
                        DrawRectangle(editor.windowPadding,
                                    i * editor.verticalLineSpacing +
                                        (i + 1) * editor.gridHeight +
                                        editor.windowPadding,
                                    screenWidth - editor.windowPadding * 2,
                                    editor.gridLineWidth, editor.editorGridColor);
                    }
                }
            }

            // Draw the line numbers
            if (true) {
                // Line Numbers
                SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
                // TODO: dont start from 0, have a viewport start, end
                for (int i = 0; i < viewportLineCount; i++) {
                    float x = editor.windowPadding;
                    float y = i * editor.verticalLineSpacing +
                            i * editor.gridHeight + editor.windowPadding;

                    std::string line_number_text =
                        leftPad(std::to_string(i + viewportStart + 1), editor.lineNumberWidth);
                    DrawTextEx(font, line_number_text.c_str(), Vector2{x, y},
                            editor.fontSize, 0, editor.editorLineNumbersColor);
                    if (y >= GetScreenHeight()) break;
                }
            }

            // Draw the text character by character
            if (true) {
                // Text
                // TODO: dont start from 0, have a viewport start, end
                for (int i = 0; i < editorData.getNumLines(); i++) {
                    float x = editor.windowPadding +
                            editor.gridWidth * (editor.lineNumberWidth + 2);
                    float y = i * editor.verticalLineSpacing +
                            i * editor.gridHeight + editor.windowPadding;
                    // count viewport lines (TODO: Can be calculated algebraically)
                    // viewportLineCount++;

                    if (y >= GetScreenHeight()) {
                        viewportLineCount = i;
                        break;
                    }
                    std::string line_text = editorData.getLine(i + viewportStart);
                    DrawTextEx(font, line_text.c_str(), Vector2{x, y},
                            editor.fontSize, 0, editor.editorTextColor);
                }
            }

            // Draw the cursor
            if (true) {
                float cursorOpacity =
                    sin(GetTime() * editor.cursorBlinkSpeed) * 0.5f + 0.5f;
                cursorOpacity = MIN(cursorOpacity, 0.8f);
                Color cursorColor = Color{
                    editor.editorCursorColor.r,
                    editor.editorCursorColor.g,
                    editor.editorCursorColor.b,
                    static_cast<unsigned char>(cursorOpacity * 255),
                };
                DrawRectangle(
                    cursorPosition.x * editor.gridWidth + editor.windowPadding,
                    (cursorPosition.y) * editor.gridHeight +
                        cursorPosition.y * editor.verticalLineSpacing +
                        editor.windowPadding,
                    editor.gridWidth, editor.gridHeight, cursorColor);
            }

            // Draw debug info
            if (true) {
            const float debug_text_left = GetScreenWidth() - 300;

            // viewportStart
            std::string debug_string_viewport_start = "viewportStart: " + std::to_string(viewportStart); 
            DrawTextEx(font, debug_string_viewport_start.c_str() , Vector2{debug_text_left, 40.0f}, 22.0f, 0.0f, GREEN);

            // viewportLineCount
            std::string debug_string_viewport_line_count = "viewportLineCount: " + std::to_string(viewportLineCount); 
            DrawTextEx(font, debug_string_viewport_line_count.c_str() , Vector2{debug_text_left, 60.0f}, 22.0f, 0.0f, GREEN);

            // Cursor Position
            std::string debug_string_cursor_position = "cursorPosition: " + std::to_string(int(cursorPosition.x)) + ", " + std::to_string(int(cursorPosition.y)); 
            DrawTextEx(font, debug_string_cursor_position.c_str() , Vector2{debug_text_left, 80.0f}, 22.0f, 0.0f, GREEN);
        }
            
            EndDrawing();
        }
    }

    // De-Initialization
    UnloadFont(font);
    CloseWindow();

    return 0;
}