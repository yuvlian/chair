#include <windows.h>
#include "config.h"

Config g_Cfg;

LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

void draw_rect(HDC hdc, int x, int y, int w, int h, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    RECT r = { x, y, x + w, y + h };
    FillRect(hdc, &r, brush);
    DeleteObject(brush);
}

void draw_dot(HDC hdc, int x, int y, int size, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    HGDIOBJ old_brush = SelectObject(hdc, brush);
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HGDIOBJ old_pen = SelectObject(hdc, pen);
    Ellipse(hdc, x - size, y - size, x + size, y + size);
    SelectObject(hdc, old_pen);
    DeleteObject(pen);
    SelectObject(hdc, old_brush);
    DeleteObject(brush);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE _prev, LPSTR _args, int show_cmd) {
    load_config(&g_Cfg);

    const char* class_name = "chair_overlay";

    WNDCLASS window_class = {0};
    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = instance;
    window_class.lpszClassName = class_name;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&window_class);

    int max_extent = 0;
    int ot = g_Cfg.outline_enabled ? g_Cfg.outline_thickness : 0;

    if (g_Cfg.length > 0 && g_Cfg.thickness > 0) {
        int line_extent = g_Cfg.gap + g_Cfg.length + ot;
        if (line_extent > max_extent) max_extent = line_extent;
        
        int thick_extent = (g_Cfg.thickness + ot * 2 + 1) / 2;
        if (thick_extent > max_extent) max_extent = thick_extent;
    }

    if (g_Cfg.dot_enabled) {
        int dot_extent = g_Cfg.dot_size + ot;
        if (dot_extent > max_extent) max_extent = dot_extent;
    }

    int crosshair_size = (max_extent * 2) + 2;

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    int x_pos = (screen_width - crosshair_size) / 2;
    int y_pos = (screen_height - crosshair_size) / 2;

    HWND window = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_APPWINDOW,
        class_name, "chair", WS_POPUP,
        x_pos, y_pos, crosshair_size, crosshair_size,
        NULL, NULL, instance, NULL
    );

    SetLayeredWindowAttributes(window, RGB(1, 1, 1), 0, LWA_COLORKEY);
    ShowWindow(window, show_cmd);
    UpdateWindow(window);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(window, &ps);

        HBRUSH bg_brush = CreateSolidBrush(RGB(1, 1, 1));
        FillRect(hdc, &ps.rcPaint, bg_brush);
        DeleteObject(bg_brush);

        RECT rect;
        GetClientRect(window, &rect);
        int center_x = (rect.right - rect.left) / 2;
        int center_y = (rect.bottom - rect.top) / 2;

        int gap = g_Cfg.gap;
        int len = g_Cfg.length;
        int thick = g_Cfg.thickness;
        COLORREF color = g_Cfg.color;

        if (g_Cfg.outline_enabled && len > 0 && thick > 0) {
            int ot = g_Cfg.outline_thickness;
            int total_thick = thick + ot * 2;
            COLORREF oc = g_Cfg.outline_color;
            // order: top, bottom, left, right
            draw_rect(hdc, center_x - total_thick / 2, center_y - gap - len - ot, total_thick, len + ot * 2, oc);
            draw_rect(hdc, center_x - total_thick / 2, center_y + gap - ot, total_thick, len + ot * 2, oc);
            draw_rect(hdc, center_x - gap - len - ot, center_y - total_thick / 2, len + ot * 2, total_thick, oc);
            draw_rect(hdc, center_x + gap - ot, center_y - total_thick / 2, len + ot * 2, total_thick, oc);
        }

        if (len > 0 && thick > 0) {
            // order: top, bottom, left, right
            draw_rect(hdc, center_x - thick / 2, center_y - gap - len, thick, len, color);
            draw_rect(hdc, center_x - thick / 2, center_y + gap, thick, len, color);
            draw_rect(hdc, center_x - gap - len, center_y - thick / 2, len, thick, color);
            draw_rect(hdc, center_x + gap, center_y - thick / 2, len, thick, color);
        }

        if (g_Cfg.dot_enabled) {
            if (g_Cfg.outline_enabled) {
                draw_dot(hdc, center_x, center_y, g_Cfg.dot_size + g_Cfg.outline_thickness, g_Cfg.outline_color);
            }
            draw_dot(hdc, center_x, center_y, g_Cfg.dot_size, g_Cfg.dot_color);
        }

        EndPaint(window, &ps);
    }
    return 0;

    case WM_ERASEBKGND:
        return 1;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(window, message, w_param, l_param);
}
