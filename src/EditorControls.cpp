#include "Editor.h"

void Editor::HandleZoom() {
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) {
        config.editorZoom -= config.zoomSpeed;
        config.editorZoom = std::max(0.5f, config.editorZoom);
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
}

void Editor::HandleDebugGrid() {
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_G)) {
        config.drawGrid = !config.drawGrid;
    }
}

void Editor::HandleDebugInfo() {
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H)) {
        config.drawDebugInfo = !config.drawDebugInfo;
    }
}

void Editor::HandleVerticalSpacing() {
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
}

void Editor::HandleLineNumbers() {
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) {
        config.drawLineNumbers = !config.drawLineNumbers;
    }
}

void Editor::HandleCursorMovement() {
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT) ||
        (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_H) || IsKeyPressedRepeat(KEY_H))) {
        cursorPosition.x -= 1;
        cursorPosition.x = std::max(
            config.lineNumberWidth + config.spaceBetweenNumbersAndText,
            (int)cursorPosition.x);
        lastCursorPositionX = cursorPosition.x;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT) ||
        (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L) || IsKeyPressedRepeat(KEY_L))) {
        cursorPosition.x += 1;
        cursorPosition.x =
            std::min((int)editorData->getLine(cursorPosition.y).size() +
                         config.lineNumberWidth +
                         config.spaceBetweenNumbersAndText,
                     (int)cursorPosition.x);
        lastCursorPositionX = cursorPosition.x;
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP) ||
        (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_K) || IsKeyPressedRepeat(KEY_K))) {
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
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN) ||
        (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_J) || IsKeyPressedRepeat(KEY_J))) {
        cursorPosition.y += 1;
        if (cursorPosition.y > viewportLineCount - 1) {
            viewportStart += 1;
            viewportStart = std::min(viewportStart, editorData->getNumLines() - viewportLineCount - 1);
            cursorPosition.y = viewportLineCount - 1;
        }
        // cursorPosition.y =
        //     std::min(cursorPosition.y, viewportLineCount);
        // std::min(editorData->getNumLines() - 1, cursorPosition.y);
        // also set the x position to the std::max of position and
        // lastCursorPositionX
        cursorPosition.x = std::max(cursorPosition.x, lastCursorPositionX);
    }
}

void Editor::HandleMouseClick() {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        // std::cout << GetMouseX() << std::endl;
        int y = GetMouseY();
        y -= config.windowPadding;
        y /= config.gridHeight + config.verticalLineSpacing;
        cursorPosition.y = y > editorData->getNumLines() - 1 ? editorData->getNumLines() - 1 : y;

        int x = GetMouseX();  // / (int)config.gridWidth) * (int)config.gridWidth  + config.windowPadding;
        x -= config.windowPadding;
        x /= config.gridWidth;
        cursorPosition.x = x;
        // std::cout << GetMouseY() << std::endl;
    }
}

void Editor::HandleTyping() {
    int key;
    while ((key = GetCharPressed()) != 0) {  // Check if a key is pressed
        // ignore backspace
        if (key == KEY_BACKSPACE) {
            break;
        }

        if (key >= 32 && key <= 126) {  // Filter for printable ASCII characters
            // Convert from grid space to line space
            int charNumber = cursorPosition.x -
                             config.lineNumberWidth -
                             config.spaceBetweenNumbersAndText;
            editorData->insertChar(cursorPosition.y, charNumber,
                                   (char)key);
            // move cursor to the right
            // TODO: do we need to change the last cursor
            // position?
            cursorPosition.x++;
        }
    }
}

void Editor::HandleBackspace() {
    if (IsKeyPressedRepeat(KEY_BACKSPACE) || IsKeyPressed(KEY_BACKSPACE)) {
        // Convert from grid space to line space
        int charNumber = cursorPosition.x -
                         config.lineNumberWidth -
                         config.spaceBetweenNumbersAndText;

        // ignore backspace if at the first char
        if (charNumber > 0) {
            // delete the char at prev position
            editorData->deleteChar(cursorPosition.y, charNumber - 1);
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
                cursorPosition.x = config.lineNumberWidth + config.spaceBetweenNumbersAndText + editorData->lines[cursorPosition.y].length();
                // merge this line with the previous one
                editorData->lines[lineNumber - 1] = editorData->lines[lineNumber - 1] + editorData->lines[lineNumber];
                // delete the line
                editorData->lines.erase(editorData->lines.begin() + lineNumber);
            }
        }
    }
}

void Editor::HandleDelete() {
}

void Editor::HandleNewLine() {
    if (IsKeyPressed(KEY_ENTER)) {
        // Convert from grid space to line space
        int charNumber = cursorPosition.x -
                         config.lineNumberWidth -
                         config.spaceBetweenNumbersAndText;
        editorData->insertLineBreak(cursorPosition.y,
                                    charNumber);
        // move cursor to the left
        // TODO: do we need to change the last cursor
        // position?
        cursorPosition.x = config.lineNumberWidth +
                           config.spaceBetweenNumbersAndText;
        cursorPosition.y++;
    }
}

void Editor::HandleTab() {
    if (IsKeyPressed(KEY_TAB)) {
        // Convert from grid space to line space
        int charNumber = cursorPosition.x -
                         config.lineNumberWidth -
                         config.spaceBetweenNumbersAndText;
        editorData->insertChar(cursorPosition.y, charNumber, '\t');
        // move cursor to the right
        cursorPosition.x += 4;
    }
}

void Editor::HandleHomeEnd() {
    if (IsKeyPressed(KEY_HOME)) {
        cursorPosition.x = 0 + config.lineNumberWidth + config.spaceBetweenNumbersAndText;
    }

    if (IsKeyPressed(KEY_END)) {
        cursorPosition.x = editorData->lines[cursorPosition.y].length() + config.lineNumberWidth + config.spaceBetweenNumbersAndText;
    }
}

void Editor::HandleLineDelete() {
    // TODO: doesn't work has edge case bugs (delete first line to see)
    if (IsKeyPressed(KEY_D) &&
        IsKeyDown(KEY_LEFT_CONTROL)) {
        // Convert from grid space to line space
        int charNumber = cursorPosition.x -
                         config.lineNumberWidth -
                         config.spaceBetweenNumbersAndText;
        editorData->deleteLine(cursorPosition.y);
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

void Editor::HandleScrollWheel() {
    if (GetMouseWheelMoveV().y != 0.0f) {
        viewportStart -= GetMouseWheelMoveV().y;
        viewportStart = std::max(viewportStart, 0);
        viewportStart = std::min(viewportStart, editorData->getNumLines() - viewportLineCount - 1);
    }
}

void Editor::CalculateViewportSizes() {
    // Vertical size
    viewportLineCount = ceil((float)GetScreenHeight() / (config.gridHeight+config.verticalLineSpacing));
    viewportLineCount = std::max(viewportLineCount, 1);
    viewportLineCount = std::min(viewportLineCount, editorData->getNumLines());
}
