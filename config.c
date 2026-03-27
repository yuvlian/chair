#include "config.h"
#include <stdio.h>

void load_config(Config* config) {
    const char* file = "./chair.ini";

    if (GetFileAttributesA(file) == INVALID_FILE_ATTRIBUTES) {
        WritePrivateProfileStringA("Crosshair", "thickness", "2", file);
        WritePrivateProfileStringA("Crosshair", "length", "6", file);
        WritePrivateProfileStringA("Crosshair", "gap", "2", file);
        WritePrivateProfileStringA("Crosshair", "red", "255", file);
        WritePrivateProfileStringA("Crosshair", "green", "255", file);
        WritePrivateProfileStringA("Crosshair", "blue", "0", file);

        WritePrivateProfileStringA("Dot", "enabled", "0", file);
        WritePrivateProfileStringA("Dot", "size", "2", file);
        WritePrivateProfileStringA("Dot", "red", "255", file);
        WritePrivateProfileStringA("Dot", "green", "255", file);
        WritePrivateProfileStringA("Dot", "blue", "255", file);

        WritePrivateProfileStringA("Outline", "enabled", "0", file);
        WritePrivateProfileStringA("Outline", "thickness", "1", file);
        WritePrivateProfileStringA("Outline", "red", "0", file);
        WritePrivateProfileStringA("Outline", "green", "0", file);
        WritePrivateProfileStringA("Outline", "blue", "0", file);
    }

    config->thickness = GetPrivateProfileIntA("Crosshair", "thickness", 2, file);
    config->length = GetPrivateProfileIntA("Crosshair", "length", 6, file);
    config->gap = GetPrivateProfileIntA("Crosshair", "gap", 2, file);
    int r = GetPrivateProfileIntA("Crosshair", "red", 255, file);
    int g = GetPrivateProfileIntA("Crosshair", "green", 255, file);
    int b = GetPrivateProfileIntA("Crosshair", "blue", 0, file);
    config->color = RGB(r, g, b);

    config->dot_enabled = GetPrivateProfileIntA("Dot", "enabled", 0, file);
    config->dot_size = GetPrivateProfileIntA("Dot", "size", 2, file);
    r = GetPrivateProfileIntA("Dot", "red", 255, file);
    g = GetPrivateProfileIntA("Dot", "green", 255, file);
    b = GetPrivateProfileIntA("Dot", "blue", 255, file);
    config->dot_color = RGB(r, g, b);

    config->outline_enabled = GetPrivateProfileIntA("Outline", "enabled", 0, file);
    config->outline_thickness = GetPrivateProfileIntA("Outline", "thickness", 1, file);
    r = GetPrivateProfileIntA("Outline", "red", 0, file);
    g = GetPrivateProfileIntA("Outline", "green", 0, file);
    b = GetPrivateProfileIntA("Outline", "blue", 0, file);
    config->outline_color = RGB(r, g, b);
}
