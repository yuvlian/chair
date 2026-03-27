#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>

typedef struct {
    int thickness;
    int length;
    int gap;
    COLORREF color;

    int dot_enabled;
    int dot_size;
    COLORREF dot_color;

    int outline_enabled;
    int outline_thickness;
    COLORREF outline_color;
} Config;

void load_config(Config* config);

#endif
