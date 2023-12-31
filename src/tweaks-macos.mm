#include "tweaks-macos.hh"
#import <Cocoa/Cocoa.h>


void toggleWindowFrameMacOS(void *window, bool frame) {
    NSWindow *window1 = (NSWindow *) window;

    NSWindowStyleMask current = [window1 styleMask];

    if (frame) {
        current &= ~NSWindowStyleMaskBorderless;
    } else {
        current |= NSWindowStyleMaskBorderless;
    }

    [window1 setStyleMask:current];
}