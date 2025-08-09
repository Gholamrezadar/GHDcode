#pragma once
#include <raylib.h>

// struct for editor config
struct EditorConfig {
    // Sizes
    float windowPadding;
    float verticalLineSpacing;
    float baseVerticalLineSpacing;
    int lineNumberWidth;
    int spaceBetweenNumbersAndText;
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

    // debug
    bool debugGrid = false;
    bool debugInfo = true;
};