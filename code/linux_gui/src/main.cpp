#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <sys/utsname.h>
 
int main(int argc, char** argv)
{
   Display* dpy = XOpenDisplay(NULL);
   if (dpy == NULL) 
      {
         fprintf(stderr, "Cannot open display\n");
         exit(1);
      }
 
   int s = DefaultScreen(dpy);
   Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, s), 10, 10, 800, 600, 1, BlackPixel(dpy, s), WhitePixel(dpy, s));
   XSelectInput(dpy, win, ExposureMask | KeyPressMask);
   XMapWindow(dpy, win);
  
   XStoreName(dpy, win, "lol title header");
   
   Atom WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", False); 
   XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);  
 
   XEvent e;
   for(;;)
      {
         XNextEvent(dpy, &e);
         if (e.type == Expose) 
            {
               int y_offset = 20;

               const char* s1 = "this is a string lel";
  
               //const char* s2 = "line 2"; 
               XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, s1, strlen(s1));
               y_offset += 20;
               //XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, s2, strlen(s2));
               //y_offset += 20;
 
               XWindowAttributes  wa;
               XGetWindowAttributes(dpy, win, &wa);
               int width = wa.width;
               int height = wa.height;
               char buf[128]={0};
               sprintf(buf, "window size is %dx%d", width, height);
               XDrawString(dpy, win, DefaultGC(dpy, s), 10, y_offset, buf, strlen(buf));
               y_offset += 20;
            }
 
         if (e.type == KeyPress)
            {
               char buf[128] = {0};
               KeySym keysym;
               int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);
               if (keysym == XK_Escape)
                  break;
            }
         if ((e.type == ClientMessage) && 
             (static_cast<unsigned int>(e.xclient.data.l[0]) == WM_DELETE_WINDOW))
            {
               break;
            }
      }
 
   XDestroyWindow(dpy, win);
   XCloseDisplay(dpy);
   return 0;
}
