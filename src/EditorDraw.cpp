#include "Editor.h"

void Editor::DrawDebugGrid() {
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
}

void Editor::DrawLineNumbers() {
    if (config.drawLineNumbers) {
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
}

void Editor::DrawText() {
    if (config.drawText) {
        // Text
        for (int i = 0; i < editorData->getNumLines(); i++) {
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
            std::string line_text = editorData->getLine(i + viewportStart);
            DrawTextEx(font, line_text.c_str(), Vector2{x, y},
                       config.fontSize, 0, config.editorTextColor);
        }
    }
}

void Editor::DrawCursor() {
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
}

void Editor::DrawDebugInfo() {
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
}