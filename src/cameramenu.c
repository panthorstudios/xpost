#ifndef lint 
        static char sccs_id[] = "@(#)optMenu.c	1.21 12/1/91 22:02:40";
#endif /* lint */

#include "main.h"

/* ================================================================
   EXTERNAL PROCEDURES 
   ================================================================ */
extern void quitX( char *);
extern void writeMessage( char *);
extern void DestroyPopupPrompt( Widget, XtPointer, XtPointer);



/* ================================================================
   EXTERNAL GLOBAL VARIABLES
   ================================================================ */
extern Display     *theDisplay;
extern Window       cameraWindow;  /* Window id of pad widget */
extern GC           cameraGC;      /* X's graphics context id */
extern XtAppContext appCon;
extern Widget topLevel, cameraview, commands, fileMenuButton;
extern Widget optMenuButton;    /* Options Menu button */
extern XtActionsRec actionTable;
extern Pixmap optionMenuButtonPattern;
extern Settings settings;

/* ================================================================
   PROTOTYPES
   ================================================================ */
void fileNameReturn (Widget, XEvent*, String *, Cardinal* );
void runTimeReturn  (Widget, XEvent*, String*, Cardinal*);
void changeFacReturn (Widget, XEvent*, String*, Cardinal*);
void changeFileName( Widget,  XtPointer, XtPointer );
void changeRunTime ( Widget,  XtPointer, XtPointer );
void changeTraceFacilityList( Widget,  XtPointer, XtPointer );


/* ================================================================
   GLOBAL VARIABLES
   ================================================================ */


/* ================================================================
   CALLBACK PROCEDURES 
   ================================================================ */
/* ----------------------------------------------------------------
    fileNameReturn

    Callback procedure.  Called by pressing return in dialog box.
    Calls changeFileName.
    --------------------------------------------------------------- */

void fileNameReturn(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
    Widget dialog = XtParent(widget);
    changeFileName(widget, (XtPointer) dialog, (XtPointer) NULL);
}

/* ----------------------------------------------------------------
    changeFileName

    Callback procedure.  Called by clicking "OK" on dialog box or
    by pressing return in dialog box for File Name option.  Gets
    file name from the user and changes the name in settings.

    client_data is the widget id of the dialog box
    --------------------------------------------------------------- */
void changeFileName(w, client_data, call_data)
    Widget      w;                      /* "SAVE AS" widget */
    XtPointer   client_data, call_data;
{

    Widget dialog,     /* dialog box                    */
	   popup;      /* Widget that contains the dialog box */
    String fileName;   /* input from user */

    /* Get file name */
    dialog = (Widget) client_data;
    fileName =  XawDialogGetValueString(dialog);

    strcpy( settings.fileName, fileName);
    writeMessage("File name changed");

    /* Get rid of popup & dialog box, restore screen */
    popup = XtParent( (Widget) client_data);
    XtDestroyWidget(popup);
}

/* ----------------------------------------------------------------
    fileNameChoice

    Called by File Name entry on Option menu.

    Callback procedure.  Pops up a dialog box to allow the user to
    enter a new file name.

    client_data is unused
    --------------------------------------------------------------- */
void fileNameChoice(w, client_data, call_data)
    Widget	w;		        /* "SAVE AS" widget */
    XtPointer   client_data, call_data;	
{

    Position   x,y;    /* position of dialog box        */
    Arg         argList[MAX_ARGS];   /* resource list   */
    int        n;      /* arg list                      */
    Widget dialog,     /* dialog box                    */
           popup;      /* Widget that contains the dialog box */

    /* Position dialog box in pad window */
    XtTranslateCoords(cameraview, (Position) DIALOG_POS_X,
			(Position) DIALOG_POS_Y, &x, &y);

    /* Create a popup widget to hold dialog box */
    n = 0;
    XtSetArg(argList[n], XtNx, x);                         n++;
    XtSetArg(argList[n], XtNy, y);                         n++;
    popup = XtCreatePopupShell("fileNamePrompt", transientShellWidgetClass,
			       cameraview, argList, n);

    /* create dialog box */
    n=0;
    XtSetArg( argList[n],XtNlabel, "Enter new file name");
    n++;
    XtSetArg( argList[n],XtNvalue, settings.fileName);
    n++;
    dialog = XtCreateManagedWidget("dialog", dialogWidgetClass, popup,
				   argList, n);

    /* Add 2 buttons */
    XawDialogAddButton(dialog, "OK", changeFileName, (XtPointer) dialog);
    XawDialogAddButton(dialog, "Cancel", DestroyPopupPrompt,(XtPointer)dialog);

    XtPopup(popup, XtGrabNone);
}


