#include "tweaks.hh"

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW64__)

#include <windows.h>

#elif defined(__APPLE__)

#include <objc/NSObjCRuntime.h>
#include <objc/objc-runtime.h>

#elif defined(__linux__)

#include <gtk/gtk.h>

#endif

void toggleWindowFrame(void *window, bool frame) {
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW64__)
    HWND hwnd = (HWND) window;
    LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
    if (!frame) {
        lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    } else {
        lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    }

    SetWindowLong(hwnd, GWL_STYLE, lStyle);

    LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (!frame) {
        lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    } else {
        lExStyle |= (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    }
    SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);

    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

#elif defined(__linux__)

    gtk_window_set_decorated((GtkWindow*) window, frame ? TRUE : FALSE);
#endif
}
