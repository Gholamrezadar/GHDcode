#pragma once

#include <math.h>
#include <raylib.h>

#include <iostream>
#include <string>
#include <vector>

#include "EditorConfig.h"
#include "EditorData.h"

// TODO:
class Editor {
   public:
    Editor() {
        Init("lorem.txt");
    }
    Editor(const std::string& file_address) {
        Init(file_address);
    }

    ~Editor() {
    }

    void Cleanup() {
        UnloadFont(font);
        CloseWindow();
    }

    void Init(const std::string& file_address) {
        // TODO: load from file
        rawText = "Hi hello bye!";
        editorData = EditorData(rawText);

        cursorPosition.x = config.lineNumberWidth + config.spaceBetweenNumbersAndText;
        cursorPosition.y = 0;

        lastCursorPositionX = cursorPosition.x;
        
        // Window Creation
        SetTraceLogLevel(LOG_NONE);
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        InitWindow(config.windowWidth, config.windowHeight, config.applicationName.c_str());
        
        // Set FPS limit
        SetTargetFPS(60);

        // Load Fonts (This has to happen after window initialization)
        font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                                  config.fontSize, nullptr, 0);

        // Set grid dimensions based on font
        config.gridWidth = MeasureTextEx(font, "A", config.fontSize, 0).x;
        config.gridHeight = MeasureTextEx(font, "A", config.fontSize, 0).y;
    }

    // TODO: move to a seperate file, right now it requires a lot of folding to be readable
    void Update() {
        std::cout<<config.fontSize << " " << config.editorZoom;
        std::cout<<std::endl;
        // Zoom (Ctrl+Q and Ctrl+E)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) {
            config.editorZoom -= config.zoomSpeed;
            config.editorZoom = std::max(0.5f, config.editorZoom);
            std::cout << "zoom: " << config.editorZoom << std::endl;
            if (config.editorZoom > 0.5f) {
                // int fontSize = int(16 * config.editorZoom);
                config.fontSize = int(16 * config.editorZoom);
                UnloadFont(font);
                font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                                  config.fontSize, nullptr, 0);
                config.gridWidth =
                    MeasureTextEx(font, "A", config.fontSize, 0).x;
                config.gridHeight =
                    MeasureTextEx(font, "A", config.fontSize, 0).y;
                config.verticalLineSpacing =
                    config.baseVerticalLineSpacing * config.editorZoom;
            }
        }
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_E)) {
            config.editorZoom += config.zoomSpeed;
            config.editorZoom = std::min(5.0f, config.editorZoom);
            std::cout << "zoom: " << config.editorZoom << std::endl;
            if (config.editorZoom < 5.0f) {
                // int fontSize = int(16 * config.editorZoom);
                config.fontSize = int(16 * config.editorZoom);
                UnloadFont(font);
                font = LoadFontEx("resources/fonts/FiraCode-Regular.ttf",
                                  config.fontSize, nullptr, 0);
                config.gridWidth =
                    MeasureTextEx(font, "A", config.fontSize, 0).x;
                config.gridHeight =
                    MeasureTextEx(font, "A", config.fontSize, 0).y;
                config.verticalLineSpacing =
                    config.baseVerticalLineSpacing * config.editorZoom;
            }
        }

        // Toggle debug grid (Ctrl+G)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_G)) {
            config.drawGrid = !config.drawGrid;
        }

        // Toggle debug info (Ctrl+H)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H)) {
            config.drawDebugInfo = !config.drawDebugInfo;
        }

        // Change vertical spacing (Shift+E and Shift+Q)
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_E)) {
            config.baseVerticalLineSpacing += 5;
            config.baseVerticalLineSpacing =
                std::min(config.baseVerticalLineSpacing, 100.0f);
            config.verticalLineSpacing =
                config.baseVerticalLineSpacing * config.editorZoom;
        }
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Q)) {
            config.baseVerticalLineSpacing -= 5;
            config.baseVerticalLineSpacing =
                std::max(config.baseVerticalLineSpacing, 0.0f);
            config.verticalLineSpacing =
                config.baseVerticalLineSpacing * config.editorZoom;
        }

        // Cursor movement
        if (IsKeyPressed(KEY_LEFT) ||
            (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H))) {
            cursorPosition.x -= 1;
            cursorPosition.x = std::max(
                config.lineNumberWidth + config.spaceBetweenNumbersAndText,
                (int)cursorPosition.x);
            lastCursorPositionX = cursorPosition.x;
        }
        if (IsKeyPressed(KEY_RIGHT) ||
            (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L))) {
            cursorPosition.x += 1;
            cursorPosition.x =
                std::min((int)editorData.getLine(cursorPosition.y).size() +
                             config.lineNumberWidth +
                             config.spaceBetweenNumbersAndText,
                         (int)cursorPosition.x);
            lastCursorPositionX = cursorPosition.x;
        }
        if (IsKeyPressed(KEY_UP) ||
            (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_K))) {
            cursorPosition.y -= 1;
            // cursorPosition.y = std::max(0, cursorPosition.y);
            if (cursorPosition.y < 0) {
                viewportStart -= 1;
                viewportStart = std::max(viewportStart, 0);
                cursorPosition.y = 0;
            }
            // also set the x position to the std::max of position and
            // lastCursorPositionX
            cursorPosition.x = std::max(cursorPosition.x, lastCursorPositionX);
        }
        if (IsKeyPressed(KEY_DOWN) ||
            (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_J))) {
            cursorPosition.y += 1;
            if (cursorPosition.y > viewportLineCount - 1) {
                viewportStart += 1;
                viewportStart = std::min(viewportStart, editorData.getNumLines() - viewportLineCount - 1);
                cursorPosition.y = viewportLineCount - 1;
            }
            // cursorPosition.y =
            //     std::min(cursorPosition.y, viewportLineCount);
            // std::min(editorData.getNumLines() - 1, cursorPosition.y);
            // also set the x position to the std::max of position and
            // lastCursorPositionX
            cursorPosition.x = std::max(cursorPosition.x, lastCursorPositionX);
        }

        // Handle mouse click
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            // std::cout << GetMouseX() << std::endl;
            int y = GetMouseY();
            y -= config.windowPadding;
            y /= config.gridHeight + config.verticalLineSpacing;
            cursorPosition.y = y > editorData.getNumLines() - 1 ? editorData.getNumLines() - 1 : y;

            int x = GetMouseX();  // / (int)config.gridWidth) * (int)config.gridWidth  + config.windowPadding;
            x -= config.windowPadding;
            x /= config.gridWidth;
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
                                         config.lineNumberWidth -
                                         config.spaceBetweenNumbersAndText;
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
                                     config.lineNumberWidth -
                                     config.spaceBetweenNumbersAndText;

                    // ignore backspace if at the first char
                    if (charNumber > 0) {
                        // delete the char at prev position
                        editorData.deleteChar(cursorPosition.y, charNumber - 1);
                        // move cursor to the left
                        cursorPosition.x--;
                        // clamp cursor to the line width
                        cursorPosition.x = std::max(
                            config.lineNumberWidth +
                                config.spaceBetweenNumbersAndText,
                            (int)cursorPosition.x);
                    }
                    // if at first cursor position -> merge this line with the previous one
                    // aka delete the line break
                    if (charNumber == 0) {
                        int lineNumber = cursorPosition.y;
                        if (lineNumber > 0) {
                            // move the cursor up
                            cursorPosition.y = std::max(0, (int)cursorPosition.y - 1);
                            // move the cursor to the end of the line
                            cursorPosition.x = config.lineNumberWidth + config.spaceBetweenNumbersAndText + editorData.lines[cursorPosition.y].length();
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
                                     config.lineNumberWidth -
                                     config.spaceBetweenNumbersAndText;
                    editorData.insertLineBreak(cursorPosition.y,
                                               charNumber);
                    // move cursor to the left
                    // TODO: do we need to change the last cursor
                    // position?
                    cursorPosition.x = config.lineNumberWidth +
                                       config.spaceBetweenNumbersAndText;
                    cursorPosition.y++;
                }
            }

            // Home and End keys
            {
                if (IsKeyPressed(KEY_HOME)) {
                    cursorPosition.x = 0 + config.lineNumberWidth + config.spaceBetweenNumbersAndText;
                }

                if (IsKeyPressed(KEY_END)) {
                    cursorPosition.x = editorData.lines[cursorPosition.y].length() + config.lineNumberWidth + config.spaceBetweenNumbersAndText;
                }
            }

            // Delete line by pressing Ctrl+D
            // TODO: doesn't work has edge case bugs (delete first line to see)
            {
                if (IsKeyPressed(KEY_D) &&
                    IsKeyDown(KEY_LEFT_CONTROL)) {
                    // Convert from grid space to line space
                    int charNumber = cursorPosition.x -
                                     config.lineNumberWidth -
                                     config.spaceBetweenNumbersAndText;
                    editorData.deleteLine(cursorPosition.y);
                    // move cursor to the left
                    cursorPosition.x = config.lineNumberWidth +
                                       config.spaceBetweenNumbersAndText;
                    cursorPosition.y--;
                    // clamp cursor to the line width
                    cursorPosition.x = std::max(
                        config.lineNumberWidth +
                            config.spaceBetweenNumbersAndText,
                        (int)cursorPosition.x);
                }
            }

            // Tab key
            {
                if (IsKeyPressed(KEY_TAB)) {
                    // Convert from grid space to line space
                    int charNumber = cursorPosition.x -
                                     config.lineNumberWidth -
                                     config.spaceBetweenNumbersAndText;
                    editorData.insertChar(cursorPosition.y, charNumber, '\t');
                    // move cursor to the right
                    cursorPosition.x += 4;
                }
            }
        }

        // if the cursor is out of bounds, clamp it to the current line
        // width
        if (cursorPosition.x > editorData.getLine(cursorPosition.y).size() +
                                   config.lineNumberWidth +
                                   config.spaceBetweenNumbersAndText) {
            // Save the last cursor position.x
            lastCursorPositionX =
                std::max(lastCursorPositionX, cursorPosition.x);

            // Clamp cursor x position to the current line width
            cursorPosition.x = editorData.getLine(cursorPosition.y).size() +
                               config.lineNumberWidth +
                               config.spaceBetweenNumbersAndText;
        }

        // reload the last cursor position.x if possible
        if (lastCursorPositionX >= cursorPosition.x &&
            lastCursorPositionX <=
                editorData.getLine(cursorPosition.y).size() +
                    config.lineNumberWidth +
                    config.spaceBetweenNumbersAndText) {
        }
    }

    void Draw() {
        BeginDrawing();
        ClearBackground(config.editorBackgroundColor);

        // Draw the working grid (this is a grid that every character can be placed on)
        if (config.drawGrid) {
            int gridRows =
                static_cast<int>(GetScreenHeight() / config.gridHeight);
            int gridCols = static_cast<int>(GetScreenWidth() / config.gridWidth);

            // Draw vertical lines
            for (int i = 0; i < gridCols + 1; i++) {
                DrawRectangle(i * config.gridWidth + config.windowPadding,
                              config.windowPadding, config.gridLineWidth,
                              GetScreenHeight() - config.windowPadding * 2,
                              config.editorGridColor);
            }

            // Draw Horizontal lines
            for (int i = 0; i < gridRows + 1; i++) {
                DrawRectangle(config.windowPadding,
                              i * config.verticalLineSpacing +
                                  i * config.gridHeight +
                                  config.windowPadding,
                              GetScreenWidth() - config.windowPadding * 2,
                              config.gridLineWidth, config.editorGridColor);
                DrawRectangle(config.windowPadding,
                              i * config.verticalLineSpacing +
                                  (i + 1) * config.gridHeight +
                                  config.windowPadding,
                              GetScreenWidth() - config.windowPadding * 2,
                              config.gridLineWidth, config.editorGridColor);
            }
        }

        // Draw the line numbers
        if (config.drawLineNumbers) {
            std::cout<<"LINESSSSS\n";
            // Line Numbers
            SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
            // TODO: dont start from 0, have a viewport start, end
            for (int i = 0; i < viewportLineCount; i++) {
                float x = config.windowPadding;
                float y = i * config.verticalLineSpacing +
                          i * config.gridHeight + config.windowPadding;

                std::string line_number_text =
                    leftPad(std::to_string(i + viewportStart + 1), config.lineNumberWidth);
                DrawTextEx(font, line_number_text.c_str(), Vector2{x, y},
                           config.fontSize, 0, config.editorLineNumbersColor);
                if (y >= GetScreenHeight()) break;
            }
        }

        // Draw the text character by character
        if (config.drawText) {
            // Text
            for (int i = 0; i < editorData.getNumLines(); i++) {
                float x = config.windowPadding +
                          config.gridWidth * (config.lineNumberWidth + 2);
                float y = i * config.verticalLineSpacing +
                          i * config.gridHeight + config.windowPadding;
                // count viewport lines (TODO: Can be calculated algebraically)
                // viewportLineCount++;

                if (y >= GetScreenHeight()) {
                    viewportLineCount = i;
                    break;
                }
                std::string line_text = editorData.getLine(i + viewportStart);
                DrawTextEx(font, line_text.c_str(), Vector2{x, y},
                           config.fontSize, 0, config.editorTextColor);
            }
        }

        // Draw the cursor
        if (config.drawCursor) {
            float cursorOpacity =
                sin(GetTime() * config.cursorBlinkSpeed) * 0.5f + 0.5f;
            cursorOpacity = std::min(cursorOpacity, 0.8f);
            Color cursorColor = Color{
                config.editorCursorColor.r,
                config.editorCursorColor.g,
                config.editorCursorColor.b,
                static_cast<unsigned char>(cursorOpacity * 255),
            };
            DrawRectangle(
                cursorPosition.x * config.gridWidth + config.windowPadding,
                (cursorPosition.y) * config.gridHeight +
                    cursorPosition.y * config.verticalLineSpacing +
                    config.windowPadding,
                config.gridWidth, config.gridHeight, cursorColor);
        }

        // Draw FPS
        if (config.drawFPS) {
            DrawFPS(GetScreenWidth() - 110, 10);
        }

        // Draw debug info (viewportStart, viewportLineCount, cursorPosition, etc)
        if (config.drawDebugInfo) {
            const float debug_text_left = GetScreenWidth() - 240;
            const float debug_text_top = GetScreenHeight() - 110;

            // viewportStart
            std::string debug_string_viewport_start = "viewportStart: " + std::to_string(viewportStart);
            DrawTextEx(font, debug_string_viewport_start.c_str(), Vector2{debug_text_left, debug_text_top + 40.0f}, 22.0f, 0.0f, GREEN);

            // viewportLineCount
            std::string debug_string_viewport_line_count = "viewportLineCount: " + std::to_string(viewportLineCount);
            DrawTextEx(font, debug_string_viewport_line_count.c_str(), Vector2{debug_text_left, debug_text_top + 60.0f}, 22.0f, 0.0f, GREEN);

            // Cursor Position
            std::string debug_string_cursor_position = "cursorPosition: " + std::to_string(int(cursorPosition.x)) + ", " + std::to_string(int(cursorPosition.y));
            DrawTextEx(font, debug_string_cursor_position.c_str(), Vector2{debug_text_left, debug_text_top + 80.0f}, 22.0f, 0.0f, GREEN);
        }

        EndDrawing();
    }

    int Run() {
        while (!WindowShouldClose()) {
            Update();
            Draw();
        }
        // TODO: return -1 if something went wrong
        return 0;
    }

   private:
    // Config
    EditorConfig config;

    // Text data
    EditorData editorData;
    std::string rawText = "";

    Font font;

    int viewportStart = 0;
    int viewportLineCount = 17;
    float lastCursorPositionX = 0.0f;  // why float?
    Vector2 cursorPosition = {0.0f, 0.0f};
};