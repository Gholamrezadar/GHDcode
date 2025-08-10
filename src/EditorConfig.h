#pragma once

#include <raylib.h>

// struct for editor config
class EditorConfig {

public:
    // Strings
    std::string applicationName;

    // Sizes
    int windowWidth;
    int windowHeight;
    float windowPadding;
    int lineNumberWidth;
    int spaceBetweenNumbersAndText;
    float verticalLineSpacing;
    float baseVerticalLineSpacing;
    float editorZoom;
    float zoomSpeed;
    float fontSize;
    float gridWidth;
    float gridHeight;
    float gridLineWidth;
    float cursorBlinkSpeed;

    // Colors
    Color editorBackgroundColor;
    Color editorTextColor;
    Color editorLineNumbersColor;
    Color editorVerticalLineColor;
    Color editorCursorColor;
    Color editorGridColor;

    // Debug
    bool drawFPS;
    bool drawGrid;
    bool drawDebugInfo;

    // Draw Booleans
    bool drawLineNumbers;
    bool drawCursor;
    bool drawText;

public:
    EditorConfig() {
        // Strings
        applicationName = "GHDCode";

        // Sizes
        windowWidth = 1280;
        windowHeight = 720;
        lineNumberWidth = 3;
        spaceBetweenNumbersAndText = 2;
        windowPadding = 10.0f;
        baseVerticalLineSpacing = 5.0f;
        editorZoom = 2.0f;
        zoomSpeed = 0.5f;
        fontSize = 16.0f;
        gridLineWidth = 2.0f;
        cursorBlinkSpeed = 10.0f;

        // Colors
        editorBackgroundColor = Color{13, 17, 23};
        editorTextColor = Color{255, 255, 255, 255};
        editorLineNumbersColor = Color{255, 255, 255, 100};
        editorVerticalLineColor = Color{255, 255, 255, 16};
        editorCursorColor = Color{47, 129, 247, 170};
        editorGridColor = Color{255, 0, 255, 100};

        // Debug
        drawGrid = false;
        drawDebugInfo = true;
        drawFPS = true;

        // Draw Booleans
        drawLineNumbers = true;
        drawCursor = true;
        drawText = true;
    }
};