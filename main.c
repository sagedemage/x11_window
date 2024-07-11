#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main(int argc, char** args) {
	int width = 800;
	int height = 600;

	Display *display = XOpenDisplay(0);

	if(!display) {
		printf("No display available!\n");
		exit(1);
	}

	int root = DefaultRootWindow(display);
	int defaultScreen = DefaultScreen(display);
	
	int blackColor = BlackPixel(display, defaultScreen);
	int whiteColor = WhitePixel(display, defaultScreen);

	int screenBitDepth = 24;
	XVisualInfo visinfo = {};

	if (!XMatchVisualInfo(display, defaultScreen, screenBitDepth, TrueColor, &visinfo)) {
		printf("No matching visual info\n");
		exit(1);
	}

	XSetWindowAttributes windowAttr;
	windowAttr.background_pixel = 0;
	windowAttr.colormap = XCreateColormap(display, root, visinfo.visual, AllocNone);
	windowAttr.event_mask = StructureNotifyMask;
	unsigned long attributeMask = CWBackPixel | CWColormap | CWEventMask;

	Window window = XCreateWindow(display, root,
			0, 0,
			width, height, 0,
			visinfo.depth, InputOutput,
			visinfo.visual, attributeMask, &windowAttr);

	if (!window) {
		printf("Window wasn't created properly!\n");
		exit(1);
	}

	XSelectInput(display, window, StructureNotifyMask);
	XStoreName(display, window, "X11 Window!");
	XMapWindow(display, window);
	
	GC gc = XCreateGC(display, window, 0, 0);
	XSetForeground(display, gc, whiteColor);

	int windowOpen = 1;
	while(windowOpen) {
		XEvent ev = {};
		while(XPending(display) > 0) {
			XNextEvent(display, &ev);
			switch(ev.type) {
				case MapNotify: {
				} break;
				case DestroyNotify: {
					XDestroyWindowEvent* e = (XDestroyWindowEvent*) &ev;
					if(e->window == window) {
						windowOpen = 0;
					}
				} break;
			}
		}

		XDrawLine(display, window, gc, 10, 10, 790, 590);
		XDrawLine(display, window, gc, 10, 590, 790, 10);
		XFlush(display);
	}

	return 0;
}
