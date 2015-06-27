
/**********************************************************************
   origin: 
      A simple Simple Menu Widget example.
      November 30, 1989 - Chris D. Peterson
      $XConsortium: xmenu1.c,v 1.8 89/12/15 19:33:15 kit Exp $
 
**
**********************************************************************/

#include "main.h"

/* ================================================================
   EXTERNAL PROCEDURES 
   ================================================================ */
extern void quitX(char *);
extern Boolean fileExists( char *);
extern char saveFile( char *);
extern void writeMessage(char *);


/* ================================================================
   EXTERNAL GLOBAL VARIABLES
   ================================================================ */
extern Widget cameraview, commands, fileMenuButton, optMenuButton;
extern Widget topLevel;
extern XtAppContext appCon;               
extern Pixmap fileMenuButtonPattern;
extern Settings settings;
extern Display *theDisplay;

/* ================================================================
   PROTOTYPES
   ================================================================ */
void savePopupPrompt(Widget, XtPointer, XtPointer);
void saveChoice(Widget, XtPointer, XtPointer);
void loadReturn(Widget, XEvent*, String*, Cardinal*);
void saveReturn(Widget, XEvent*, String*, Cardinal*);
void overwriteTheFile(Widget, XtPointer, XtPointer);
void saveTheFile(Widget, XtPointer, XtPointer);
void DestroyPopupPrompt(Widget, XtPointer, XtPointer);
void nullReturn();

/* ================================================================
   CALLBACK PROCEDURES 
   ================================================================ */
/* ----------------------------------------------------------------
   quitChoice

   Called when "Quit" is selected on FILE menu.

	Arguments: w - the menu item that was selected.
                 junk, garbage - *** unused ***.
	Returns: 
   ---------------------------------------------------------------- */
void quitChoice(w, junk, garbage)
   Widget w;
   XtPointer junk, garbage;
{
       quitX(NULL);
}

/* ----------------------------------------------------------------
    quitPopupPrompt

    Called by pressing QUIT on FILE menu.

    client_data: unused
    --------------------------------------------------------------- */
void quitPopupPrompt(parent,client_data, call_data)
    Widget	parent;  /* dialog box that called this procedure */
    XtPointer client_data, call_data;
{
    Arg		args[5];       /* argument list                       */
    Widget	popup, dialog; /* new widgets                         */
    Position	x, y;          /* pixel position of upper left corner */
    Dimension	width, height; /* size of widget                      */
    Cardinal	n;             /* counter                             */
    char        userPrompt[MAX_DIALOG_WIDTH];  /* Question for user   */
    String fileName;           /* passed as input from user */

    quitX(NULL);
}
/*    XtTranslateCoords(cameraview, (Position) 30, (Position) 30, &x, &y);

    n = 0;
    XtSetArg(args[n], XtNx, x);				n++;
    XtSetArg(args[n], XtNy, y);				n++;
    popup = XtCreatePopupShell("noPrompt", transientShellWidgetClass,
			       cameraview, args, n);

    n=0;
    XtSetArg( args[n],XtNlabel, "Are you sure you want to quit?");
    n++;
    dialog = XtCreateManagedWidget("dialog", dialogWidgetClass, popup,
				   args, n);

    XawDialogAddButton(dialog, "Yes", quitChoice, NULL);
    XawDialogAddButton(dialog, "No", DestroyPopupPrompt,(XtPointer)dialog);

    XtPopup(popup, XtGrabExclusive);
}
*/


/* ----------------------------------------------------------------
    saveTheFile

    Callback procedure.  Called by pressing SAVE on file menu or
    by clicking "DONE" or by pressing return in dialog box for 
    SAVE AS. Checks if file exists and 
    prompts to overwrite it.  Then, calls procedure to save the 
    file under the name given in settings.fileName.
    --------------------------------------------------------------- */
void saveTheFile(w, client_data, call_data)
    Widget	w;		        /* "SAVE AS" widget */
    XtPointer   client_data, call_data;	
{

    Widget dialog,     /* dialog box                    */
           popup;      /* Widget that contains the dialog box */
    String fileName;   /* input from user */
    char msg[MAX_CHARS_IN_WINDOW];    /* error message   */

    dialog = (Widget) client_data;
    fileName = XawDialogGetValueString(dialog);

    if (saveFile(fileName) == ERR_OPEN_FILE_FOR_WRITE )
    {
        sprintf( msg, "Unable to open file %s.  Animation not saved",fileName);
	writeMessage(msg) ;
    }

    popup = XtParent( (Widget) client_data);
    XtPopdown(popup);
}

/* ----------------------------------------------------------------
    saveChoice

    Callback procedure.  Called by pressing SAVE on file menu.

    Checks if file exists and 
    prompts to overwrite it.  Then, calls procedure to save the 
    file under the name given in settings.fileName.
    --------------------------------------------------------------- */
void saveChoice(w, client_data, call_data)
    Widget	w;		        /* "SAVE AS" widget */
    XtPointer   client_data, call_data;	
{

    String fileName;   /* input from user */
    char msg[MAX_CHARS_IN_WINDOW];    /* error message   */


    /* Get file name */
    fileName = (char *) client_data;

    /* Does the file exist ? */
    if ( saveFile( fileName ) == ERR_OPEN_FILE_FOR_WRITE )
    {
        sprintf( msg, "Unable to open file %s.  File not saved", fileName );
	writeMessage( msg );
    }
    else
        writeMessage("Settings saved");
}

/* ----------------------------------------------------------------
    loadTheFile

    Calls procedure in file.c to load the new file.
    --------------------------------------------------------------- */
void loadTheFile(w, client_data, call_data)
    Widget	w;		        /* "SAVE AS" widget */
    XtPointer   client_data, call_data;	
{

    Widget dialog,     /* dialog box                    */
           popup;      /* Widget that contains the dialog box */
    String fileName;   /* input from user */
    char msg[MAX_CHARS_IN_WINDOW];    /* error message   */
    int i,j;

    /* Get file name */
    popup = XtParent( (Widget) client_data);
    dialog = (Widget) client_data;
    fileName = XawDialogGetValueString(dialog);
    if ( loadFile( fileName ) == ERR_OPEN_FILE_FOR_READ )
    {
        sprintf( msg, "Unable to access file %s", fileName);
	writeMessage( msg );
    }
    else {
        sprintf(msg,"File %s loaded",fileName);
        writeMessage(msg);
    }
    XtPopdown(popup);
    XFlush(theDisplay);
}

/* ----------------------------------------------------------------
    SaveReturn

    Callback procedure.  Called by pressing return in dialog box. 
    Calls saveTheFile.
    --------------------------------------------------------------- */

void saveReturn(widget, event, params, num_params)
    Widget widget;		
    XEvent *event;		
    String *params;	
    Cardinal *num_params;
{
    Widget dialog = XtParent(widget);
    saveTheFile(widget, (XtPointer) dialog, (XtPointer) NULL);
}

/* ----------------------------------------------------------------
    LoadReturn

    Callback procedure.  Called by pressing return in dialog box. 
    Calls loadTheFile.
    --------------------------------------------------------------- */

void loadReturn(widget, event, params, num_params)
    Widget widget;		
    XEvent *event;		
    String *params;	
    Cardinal *num_params;
{
    Widget dialog = XtParent(widget);
    loadTheFile(widget, (XtPointer) dialog, (XtPointer) NULL);
}

void nullReturn(widget, event, params, num_params)
    Widget widget;
    XEvent *event;
    String *params;
    Cardinal *num_params;
{
}

/* ----------------------------------------------------------------
    filePopupPrompt

    Called by "LOAD" & "SAVE AS".  Gets filename.

    client_data contains LOAD vs SAVE AS identifier as
       an int (defined in modeller.h)
    --------------------------------------------------------------- */
void filePopupPrompt(button, client_data, call_data)
    Widget	button;		
    XtPointer client_data, call_data;
{
    Arg		args[5];
    Widget	popup, dialog;
    Position	x, y;
    Dimension	width, height;
    Cardinal	n;

    /* Position dialog box in pad window */
    XtTranslateCoords(cameraview, (Position) 10, (Position) 10, &x, &y);

    /* Create a popup widget to hold dialog box */
    n = 0;
    XtSetArg(args[n], XtNx, x);				n++;
    XtSetArg(args[n], XtNy, y);				n++;

    /* Must use different widget name here so I can have 2 
       different entries in the translations tables if the users 
       hits return */
    if ( (int) client_data == SAVEAS )
         popup = XtCreatePopupShell("savePrompt", transientShellWidgetClass,
                                     cameraview, args, n);
    else
         popup = XtCreatePopupShell("loadPrompt", transientShellWidgetClass, 
                                     cameraview, args, n);

    /* user input is from the dialog box */
    n=0;
    XtSetArg( args[n],XtNlabel, "Filename:"); n++;
    XtSetArg( args[n],XtNvalue, "");          n++;
    dialog = XtCreateManagedWidget("dialog", dialogWidgetClass, popup,
				   args, n);

    /* Add 2 buttons */
    if ( (int) client_data == SAVEAS )
       XawDialogAddButton(dialog, "Done", saveTheFile, (XtPointer) dialog);
    else
       XawDialogAddButton(dialog, "Load", loadTheFile, (XtPointer) dialog);

    XawDialogAddButton(dialog, "Cancel", DestroyPopupPrompt,(XtPointer)dialog);

    XtPopup(popup, XtGrabExclusive);
}

/* ----------------------------------------------------------------
    DestroyPopupPrompt

    Used by all popup's with "Cancel" button.
    --------------------------------------------------------------- */
/*	Function Name: DestroyPopupPrompt
 *	Description: Destroys the popup dialog widget.
 *	Arguments: w - *** UNUSED ***.
 *                 client_data - the dialog widget.  This widget is a direct
 *                               child of the popup shell to destroy.
 *                 call_data - *** UNUSED **.
 *	Returns: none.
 */

void DestroyPopupPrompt(widget, client_data, call_data)
    Widget	widget;		
    XtPointer client_data, call_data;	
{
    
    Widget popup = XtParent( (Widget) client_data);
    XtPopdown(popup);
    XFlush(theDisplay);
}

