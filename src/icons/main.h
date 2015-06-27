/* ================================================================
   LIBRARIES USED
   ================================================================ */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Do Not Change Order of the next 4 Include Files */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Command.h>
#include <X11/Paned.h>
#include <X11/Shell.h>
#include <X11/cursorfont.h>

#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/Sme.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/List.h>


/* ================================================================
   TYPE DEFINITIONS
   ================================================================ */

typedef float Real;

/* ================================================================
   ENUMERATED TYPES
   ================================================================ */
#define TRUE 1
#define FALSE 0

#define ON 2
#define OFF 3
#define SAVEAS 4
#define LOAD 5

/* ================================================================
   CONSTANTS
   ================================================================ */
#define CAMERA   0
#define SLATE    1
#define LIGHT    2
#define MIDDLE   82
#define MAX_ARGS 50
#define MAX_CHARS_IN_WINDOW 80
#define MAX_ERROR_MSG 80
#define MAX_WEIGHTS 10
#define INT_STR 8
#define PAD_H_SIZE 800
#define PAD_V_SIZE 1000
#define BUTTON_H_SIZE 74 
#define BUTTON_V_SIZE 74 
#define DIALOG_HEIGHT 150
#define DIALOG_WIDTH 200
#define DIALOG_POS_X 10     /* x,y position of dialog box in pad window */
#define DIALOG_POS_Y 10
#define DIALOG_POS_X_2 30   /* x,y position of 2nd dialog box in pad window */
#define DIALOG_POS_Y_2 30
#define OFFSET 0
#define MAX_DIALOG_WIDTH 40
#define ICON_SIZE 64        /* Icons are square */
#define WIDTH_ICON 64 
#define HEIGTH_ICON 64 
#define PAD_ROWS PAD_H_SIZE/ICON_SIZE+1
#define PAD_COLUMNS PAD_V_SIZE/ICON_SIZE+1

#define EV_MASK (ButtonPressMask | \
                 ExposureMask )

#define END_LIST -1
#define END_SECTION -2

/* ================================================================
   ERROR MESSAGES
   ================================================================ */
#define NO_ERR 10
#define ERR_OPEN_FILE_FOR_WRITE 11
#define ERR_OPEN_FILE_FOR_READ 12
#define OK                    0
#define ERROR                -1
#define EOF_FOUND            -2
#define NO_LINE_END_ERROR    -3
#define NO_LINE_START_ERROR  -4
#define MALLOC_ERROR         -5
#define LINE_ERROR           -6

/* ================================================================
   GLOBAL DATA STRUCTURES
   ================================================================ */
typedef struct {
    int x,y,z;
} vector;

typedef struct {
    int frame;
    int x,y,z;
} Cutframe;

typedef struct {
    int frame;
    int val;
} Zoomdat;

typedef struct {
    char   *infile;
    char   *outfile;
    char   *fileName;
    vector camera;
    vector slate;
    float  ambient;
    float  scale;
    Zoomdat zstart;
    Zoomdat zend;
    int    moving;
    int    frames;
    int    current_frame;
    int    zoom;
    Cutframe cutframe;
    Cutframe movestart;
    Cutframe moveend;
} Settings;

enum { XS_DEFAULT, XS_KILL, XS_DRAW, XS_BUSY } CurrentCursor; 
