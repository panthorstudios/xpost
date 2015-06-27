#include "main.h"

/* ================================================================
   EXTERNAL GLOBAL PROCEDURES
   ================================================================ */
extern void initX();
extern void init();
extern Settings settings;
void quitX( char *);
extern Widget mainmsg[3];
/* =========================
   EXTERNAL GLOBAL VARIABLES
   ================================================================ */
   XtAppContext appCon;               
   extern Display      *theDisplay; /* X's display id          */
   extern GC           cameraGC;       /* X's graphics context id */
   extern Window       cameraWindow;   /* Window id of pad widget */
   extern Window       outerWindow;  

/* ---------------------------------------------------------------
   main
   --------------------------------------------------------------- */

main (argc, argv)
int argc; char *argv[];
{
   XEvent  theEvent;     /* The next X event */
   char msg[MAX_CHARS_IN_WINDOW];
   char *name[] = {"xpost"};
   char filename[20];

   /* initialize display */
   initX(1, &name);  /* set up X windows */
   init();           /* initialize variables */
   /* get command line arguments */
   if (argc > 1) 
	quitX( "no parameters are required for xpost");

/*   sleep(4);
*/
    while(TRUE) {
        XPeekEvent( theDisplay, &theEvent );
/*        if ( theEvent.type == MapNotify || theEvent.type == Expose)
            show_views();
*/
        while (XtAppPending(appCon))
            XtAppProcessEvent(appCon, XtIMAll);
    }
}
void quitX( errorStr )
   char errorStr[];
{

  if ( errorStr != NULL)
     fprintf(stderr, "FATAL ERROR: %s\n", errorStr);

  XCloseDisplay( theDisplay );
  exit(0);
}

void writeMessage( msgStr )
   char msgStr[];
{
        Arg     argList[MAX_ARGS]; /* resource list */
        int     n;                 /* # of args     */
        char    msgstr[80];

        strcpy(msgstr,">");
        strcat(msgstr,msgStr);
        if (strlen(settings.message2)!=1)
            strcpy(settings.message1,settings.message2);
        if (strlen(settings.message3)!=1)
            strcpy(settings.message2,settings.message3);
        strcpy(settings.message3,msgstr);

        n = 0;
        XtSetArg( argList[n], XtNlabel, settings.message1); n++;
        XtSetValues( mainmsg[0], argList, n);

        n = 0;
        XtSetArg( argList[n], XtNlabel, settings.message2); n++;
        XtSetValues( mainmsg[1], argList, n);

        n = 0;
        XtSetArg( argList[n], XtNlabel, settings.message3); n++;
        XtSetValues( mainmsg[2], argList, n);
}
