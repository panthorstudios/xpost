/* ================================================================
   LIBRARIES USED
   ================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// #include <floatingpoint.h>
// #include <sys/ieeefp.h>

/* Do Not Change Order of the next 4 Include Files */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
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
#include <X11/Xaw/AsciiText.h>

#define  sqr(M)    ((M)*(M))
#define pi 3.141592654
/* ================================================================
   ENUMERATED TYPES
   ================================================================ */
#define TRUE 1
#define FALSE 0

#define ON 2
#define OFF 3
#define SAVEAS 4
#define LOAD 5
#define SAVE 6

/* ================================================================
   CONSTANTS
   ================================================================ */
#define CAMERA         0
#define SLATE          1
#define LIGHT          2
#define LIGHTFOCUS     3

#define DIFFUSE        0 
#define SPOT           1 
#define WARN           2 
#define AMBIENT        3

#define NONE          -1
#define CUT            0
#define DOLLYSTART     1
#define DOLLY          2
#define DOLLYPOINT     3
#define DOLLYEND       4 
#define DOLLYSTARTEND  5
#define ZOOMSTART      6 
#define ZOOM           7 
#define ZOOMPOINT      8 
#define ZOOMEND        9 
#define SPLINESTART    10  
#define SPLINE         11 
#define SPLINEPOINT    12
#define SPLINEEND      13
#define SPLINESTARTEND 14
#define DELETE         15
#define ADJUST         16
#define MERGE          17
#define SPLICE         18

#define MIDDLE                82
#define MAX_ARGS              50
#define MAX_CHARS_IN_WINDOW   80
#define MAX_ERROR_MSG         80
#define MAX_DIALOG_WIDTH      60
#define WINDOW_H            590
#define WINDOW_V            507
#define DIALOG_POS_X          10
#define DIALOG_POS_Y          10
#define BUTTON_H_SIZE         74 
#define BUTTON_V_SIZE         74 
#define EV_MASK (ButtonPressMask | \
                 ExposureMask )

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
typedef float vertex[3];
typedef float matrix[4][4];
typedef float vector[4];
typedef struct {
    float x,y,z;
} camvector;

typedef struct {
    int frame;
    int x,y,z;
} Cutframe;

typedef struct {
    int frame;
    int val;
} Zoomdat;


typedef struct {
    vertex verts[8];
    char name[20];
} BBox;
typedef struct {
    vector verts[8];
    char name[20];
} BBox1;

struct Light {
    int lighttype;
    int lightid;
    float lightint;
    float lightcoeff;
    float lx,ly,lz;
    float lfx,lfy,lfz;
    struct Light *next;
};

struct Mover {
    int movetype;
    int smovetype;
    int zoomtype;
    int frame;
    camvector camera;
    camvector slate;
    int zoomval;
}; 

typedef struct {
    char   *infile;
    char   *outfile;
    char   *fileName;
    char   src1[80];
    char   src2[80];
    char   dest[80];
    int    mergeframe;
    camvector camera;
    camvector slate;
    float  ambient;
    float  ambient_temp;
    int    light_type;
    float  scale;
    Zoomdat zstart;
    Zoomdat zend;
    int    moving;
    int    mode;
    int    frames;
    int    current_frame;
    int    zoom;
    int    start;
    char   message1[80];
    char   message2[80];
    char   message3[80];
    struct Mover *move_ptr;
    struct Mover *move_head;
    struct Light *light_ptr;
    struct Light *light_head;
    Cutframe cutframe;
    Cutframe movestart;
    Cutframe moveend;
} Settings;
