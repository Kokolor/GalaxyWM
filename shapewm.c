#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))

char *menu[] = {"rofi","-show","run",NULL};
char *term[] = {"alacritty",NULL};
char *wall[] = {"nitrogen","--restore",NULL};

/*Variable declarations*/
Window root;
Display *dpy;
XWindowAttributes attr;
XButtonEvent bevent;
XEvent event; 
XKeyEvent kevent;
KeySym ks;
bool running=true;

void spawn(char* arg[], Display *dpy);
void keypressed(KeySym Sym, Display *dpy, Window sw);

int main(){
		int xdiff, ydiff;

		dpy = XOpenDisplay(0x0);
		if (!dpy){
				return 1;
		}
		root = DefaultRootWindow(dpy);

		spawn(wall, dpy);

		XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("u")), Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);
		XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("t")), Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);
		XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("c")), Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);
		XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("r")), Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);
		XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("q")), Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);

		XGrabButton(dpy, 1, Mod4Mask, root, True, ButtonPressMask, GrabModeAsync,GrabModeAsync, None, None);
		XGrabButton(dpy, 3, Mod4Mask, root, True, ButtonPressMask, GrabModeAsync,GrabModeAsync, None, None);

		while (1){
				XNextEvent(dpy,&event);

				switch(event.type){
						case CreateNotify:
								break;
						case KeyPress:
								kevent = event.xkey;
								ks = XkbKeycodeToKeysym(dpy,kevent.keycode,0,0);	
								keypressed(ks,dpy,event.xkey.subwindow);
								break;
						case ButtonPress:
								XGrabPointer(dpy, event.xbutton.subwindow, True,
									PointerMotionMask|ButtonReleaseMask, GrabModeAsync,
									GrabModeAsync, None, None, CurrentTime);
								XGetWindowAttributes(dpy, event.xbutton.subwindow, &attr);
								bevent = event.xbutton;
								break;
						case MotionNotify: 
								while(XCheckTypedEvent(dpy, MotionNotify, &event));
								xdiff = event.xbutton.x_root - bevent.x_root;
								ydiff = event.xbutton.y_root - bevent.y_root;
								XMoveResizeWindow(dpy, event.xmotion.window,
								   	attr.x + (bevent.button==1 ? xdiff : 0),
									attr.y + (bevent.button==1 ? ydiff : 0),
									MAX(1, attr.width + (bevent.button==3 ? xdiff : 0)),
									MAX(1, attr.height + (bevent.button==3 ? ydiff : 0)));
								break;
						case ButtonRelease:
								XUngrabPointer(dpy, CurrentTime);
								break;
						case KeyRelease:
								break;
						default: printf("Error!");
				}
		}
}

void spawn(char* arg[],Display *dpy){
		if(fork()==0){
				if(dpy)
					close(ConnectionNumber(dpy));
				setsid();
				int status_code = execvp(arg[0],arg);
				if (status_code==-1){
						printf("Error Occured!");
				}
		}
}

void keypressed(KeySym Sym, Display *dpy, Window sw){
		switch(Sym){
				case XK_r:
					spawn(menu, dpy);
					break;
				case XK_t:
					spawn(term, dpy);
					break;
				case XK_q:
					running = false;
					break;
				case XK_u:
					if(event.xkey.subwindow)
							XRaiseWindow(dpy,sw);
					break;
				case XK_c:
					if(event.xkey.subwindow){
						event.xclient.type = ClientMessage;
						event.xclient.window = sw;
						event.xclient.message_type = XInternAtom(dpy, "WM_PROTOCOLS", true);
						event.xclient.format = 32;
						event.xclient.data.l[0] = XInternAtom(dpy, "WM_DELETE_WINDOW",false);
						event.xclient.data.l[1] = CurrentTime;
						XSendEvent(dpy, sw, False, NoEventMask, &event);
					}
					break;
				default: break;
		} 
}