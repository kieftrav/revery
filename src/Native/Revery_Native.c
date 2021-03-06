#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <string.h>

#define UNUSED(x) (void)(x)

#ifdef WIN32
#include "ReveryWin32.h"
#include <combaseapi.h>
#include <windows.h>
#elif __APPLE__
#include "ReveryCocoa.h"
#import "ReveryAppDelegate.h"
#else
#include "ReveryGtk.h"
#endif

CAMLprim value revery_initializeApp() {
#ifdef __APPLE__
    SDLAppDelegate *sdlDelegate = [NSApp delegate];
    ReveryAppDelegate *delegate = [ReveryAppDelegate newWithSDLDelegate:sdlDelegate];
    [NSApp setDelegate:delegate];
#elif WIN32
    HRESULT hr = CoInitialize(NULL);
    if (hr != S_OK) {
        fprintf(stderr, "WARNING: COM initialization failed.");
    }
#endif
    return Val_unit;
}

CAMLprim value revery_uninitializeApp() {
#ifdef WIN32
    CoUninitialize();
#endif
    return Val_unit;
}


CAMLprim value revery_initializeWindow(value vWin) {
    CAMLparam1(vWin);
    void *win = (void *)vWin;
#ifdef WIN32
    /* This flag often gets unset when the window decoration is removed.
       This Chromium comment is the source of this fix:
       https://chromium.googlesource.com/chromium/src.git/+/46.0.2478.0/chrome/browser/ui/views/apps/chrome_native_app_window_views_win.cc#71
    */
    HWND window = (HWND)win;
    int current_style = GetWindowLong(window, GWL_STYLE);
    SetWindowLong(window, GWL_STYLE, current_style | WS_CAPTION);
#else
    UNUSED(win);
#endif
    CAMLreturn(Val_unit);
}
