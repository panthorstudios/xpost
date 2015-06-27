#include "main.h"

#include "icons/camera_icon.h"
#include "icons/splcamera_icon.h"
#include "icons/disk_icon.h"
#include "icons/color_icon.h"
#include "icons/light_icon.h"
#include "icons/merge_icon.h"
#include "icons/splice_icon.h"
#include "icons/xpost_icon.h"
#include "icons/slate.h"
#include "icons/camera.h"
#include "icons/blank.h"
#include "icons/left_arrow.h"
#include "icons/left10_arrow.h"
#include "icons/right_arrow.h"
#include "icons/right10_arrow.h"
#include "icons/up_arrow.h"
#include "icons/up10_arrow.h"
#include "icons/down_arrow.h"
#include "icons/down10_arrow.h"
#include "icons/spotlight.h"
#include "icons/spotfocus.h"
#include "icons/warnfocus.h"
#include "icons/light.h"

/* ================================================================
   External Global Variables & Procedures
   ================================================================ */
extern void createDiskMenu();
extern void createCameraMenu();
extern void createLightMenu();
extern void createColorMenu();
extern void createSpliceMenu();
extern void createMergeMenu();
extern void quitX();
extern void displayPixmap();
extern void writeMessage();
extern void show_views();
extern void clear_views();
extern void moveCallback();
extern void buttonCallback();
extern void createWidgets();
extern void nullReturn();

extern Widget topLevel,windowW,commands,views;
extern Widget cameraview, frontview,topview,sideview;
extern Widget diskMenuButton,cameraMenuButton,splcameraMenuButton,
              lightMenuButton,colorMenuButton,spliceMenuButton,
              mergeMenuButton;
extern Widget left10arrow1,right10arrow1,up10arrow1,down10arrow1,leftarrow1,
              rightarrow1,uparrow1,downarrow1,leftarrow2,left10arrow2,
              rightarrow2,right10arrow2,uparrow2,up10arrow2,downarrow2,
              down10arrow2,leftarrow3,left10arrow3,rightarrow3,right10arrow3,
              uparrow3,up10arrow3,downarrow3,down10arrow3;
extern Widget mainmsg,frontmsg,topmsg,sidemsg,statmsg,framemsg;
extern Widget slatebutton,camerabutton,plusbutton,minusbutton;
extern Widget frontbar1,frontbar2,topbar1,topbar2,sidebar1,sidebar2,zoombar;
extern Widget frame_lbutton,frame_rbutton,frame_l10button,frame_r10button;
extern Widget cutmenu[6],colormenu[2],lightmenu[5],zoom_lbutton,zoom_rbutton;
extern Widget popbutton1,popbutton2,popuplabel,zoommsg;
extern Widget popupbox,popleftbutton,poprightbutton,donebutton;

/* ================================================================
   Global Variables 
   ================================================================ */
    XtAppContext appCon;               
    Display *theDisplay; /* X's display id */
    GC       cameraGC,topGC,sideGC,frontGC;      /* X's graphics context id */
    Window   topWindow,sideWindow,frontWindow,cameraWindow;
    Window   outerWindow=NULL;  /* the outermost window */
    Cursor   theBusyCursor;  /* a stop watch */
    Cursor   theMoveCursor;  /* a skull and crossbones */
    Cursor   theDrawCursor;  /* a pencil */
    unsigned long  LightBluePen,GrayPen,WhitePen,BlackPen,YellowPen;
    int      *theScreen,cut,smove,move,zoom,spline,sspline;
    float    barpos,scale;
    Settings settings;
    int      lightcount[3],num_objects;
    struct Light *light_head;
    struct Light lightelement;
    struct Mover *move_head;
    struct Mover moveeelement;
    struct Mover *framelist;
    BBox     *objects;

    Pixmap   leftarrowPattern,left10arrowPattern,rightarrowPattern,
             right10arrowPattern,uparrowPattern,downarrowPattern;
    Pixmap   diskMenuButtonPattern,cameraMenuButtonPattern,
             splcameraMenuButtonPattern,lightMenuButtonPattern,
             colorMenuButtonPattern,spliceMenuButtonPattern,
             mergeMenuButtonPattern;
    Pixmap   pgmIconPattern;
    Pixmap   blankPattern,cameraPattern, slatePattern;
    Pixmap   spotPattern,spotfocusPattern,lightPattern,warnfocusPattern;
    
/******************************************************************
* createXWindow
*******************************************************************/
void createXWindow()
{
   XSetWindowAttributes theWindowAttributes;
   unsigned long        theWindowMask;

   Arg       argList[MAX_ARGS];    /* resource list   */            
   XGCValues theGCValues;          /* return value from XCreateGC */

   outerWindow   = XtWindow( topLevel );
   cameraWindow  = XtWindow( cameraview );
   frontWindow   = XtWindow( frontview );
   topWindow     = XtWindow( topview );
   sideWindow    = XtWindow( sideview );

    /* Create graphics contexts */
    if ((cameraGC = XCreateGC( theDisplay, cameraWindow, (unsigned long) 0,
        &theGCValues)) == 0)
    quitX("initX couldn't get a graphic context");

    if ((frontGC = XCreateGC( theDisplay, frontWindow, (unsigned long) 0,
        &theGCValues)) == 0)
    quitX("initX couldn't get a graphic context");

    if ((topGC = XCreateGC( theDisplay, topWindow, (unsigned long) 0,
        &theGCValues)) == 0)
    quitX("initX couldn't get a graphic context");

    if ((sideGC = XCreateGC( theDisplay, sideWindow, (unsigned long) 0,
        &theGCValues)) == 0)
    quitX("initX couldn't get a graphic context");

    XSetForeground(theDisplay, cameraGC, WhitePen);
    XSetBackground(theDisplay, cameraGC, BlackPen);
    XSetForeground(theDisplay, frontGC, BlackPen);
    XSetBackground(theDisplay, frontGC, GrayPen);
    XSetForeground(theDisplay, topGC, BlackPen);
    XSetBackground(theDisplay, topGC, GrayPen);
    XSetForeground(theDisplay, sideGC, BlackPen);
    XSetBackground(theDisplay, sideGC, GrayPen);
}

void set_colors() {
int theDepth;
Colormap theColormap;
XColor theHardwareColor, theRGBColor;

    theDisplay    = XtDisplay( topLevel );
    theScreen     = ( int *)XtScreen( topLevel );
    theDepth  = DefaultDepth(theDisplay,*theScreen);
    BlackPen  = BlackPixel( theDisplay, *theScreen);
    WhitePen  = WhitePixel( theDisplay, *theScreen);
    GrayPen   = WhitePen;
    YellowPen = WhitePen;
    LightBluePen = WhitePen;

    if (theDepth > 1) {
        theColormap = DefaultColormap(theDisplay,*theScreen);

        if ((XLookupColor(theDisplay,theColormap,"Yellow",
            &theRGBColor,&theHardwareColor)) &&
            (XAllocColor(theDisplay,theColormap,&theHardwareColor))) {
            YellowPen = theHardwareColor.pixel;
        }
        if ((XLookupColor(theDisplay,theColormap,"LightBlue",
            &theRGBColor,&theHardwareColor)) &&
            (XAllocColor(theDisplay,theColormap,&theHardwareColor))) {
            LightBluePen = theHardwareColor.pixel;
        }
        if ((XLookupColor(theDisplay,theColormap,"LightGray",
            &theRGBColor,&theHardwareColor)) &&
            (XAllocColor(theDisplay,theColormap,&theHardwareColor))) {
            GrayPen = theHardwareColor.pixel;
        }
    }
}

/********************************************************************
* readBitmaps()
*
* This function reads the included bitmap files into Pixmaps for
* buttons, etc.
*********************************************************************/
void readBitmaps()
{
    unsigned int width, height;     
    Arg argList[MAX_ARGS];
    int i,j;                        /* loop counters */

    width = 64;
    height = 64;

    diskMenuButtonPattern = XCreateBitmapFromData( theDisplay, outerWindow, 
        disk_icon_bits, width, height);
    cameraMenuButtonPattern = XCreateBitmapFromData( theDisplay,
outerWindow,
        camera_icon_bits, width, height);
    splcameraMenuButtonPattern = XCreateBitmapFromData( theDisplay,
outerWindow,
        splcamera_icon_bits, width, height);
    lightMenuButtonPattern = XCreateBitmapFromData( theDisplay, outerWindow, 
        light_icon_bits, width, height);
    colorMenuButtonPattern = XCreateBitmapFromData( theDisplay, outerWindow,

        color_icon_bits, width, height);
    spliceMenuButtonPattern = XCreateBitmapFromData( theDisplay, outerWindow,
        splice_icon_bits, width, height);
    mergeMenuButtonPattern = XCreateBitmapFromData( theDisplay,
outerWindow,
        merge_icon_bits, width, height);
    pgmIconPattern = XCreateBitmapFromData( theDisplay, outerWindow,
        xpost_icon_bits, 64,64);

    width = 16; height = 16;
    blankPattern = XCreateBitmapFromData( theDisplay, outerWindow, 
          blank_bits, width, height);
    slatePattern = XCreateBitmapFromData( theDisplay, outerWindow,
                slate_bits, width, height);
    cameraPattern = XCreateBitmapFromData( theDisplay, outerWindow,
                camera_bits, width, height);
    leftarrowPattern = XCreateBitmapFromData( theDisplay, outerWindow,
                left_arrow_bits, width, height);
    left10arrowPattern = XCreateBitmapFromData( theDisplay, outerWindow,
                left10_arrow_bits, width, height);
    rightarrowPattern = XCreateBitmapFromData( theDisplay, outerWindow,
                right_arrow_bits, width, height);
    right10arrowPattern = XCreateBitmapFromData( theDisplay, outerWindow,
                right10_arrow_bits, width, height);
    uparrowPattern = XCreateBitmapFromData( theDisplay, outerWindow,
                up_arrow_bits, width, height);
    downarrowPattern = XCreateBitmapFromData( theDisplay, outerWindow,
                down_arrow_bits, width, height);
    lightPattern = XCreateBitmapFromData( theDisplay, outerWindow,
        light_bits, width,height);
    spotPattern = XCreateBitmapFromData( theDisplay, outerWindow,
        spotlight_bits, width,height);
    spotfocusPattern = XCreateBitmapFromData( theDisplay, outerWindow,
        spotfocus_bits, width,height);
    warnfocusPattern = XCreateBitmapFromData( theDisplay, outerWindow,
        warnfocus_bits, width,height);

    XtSetArg(argList[0], XtNbitmap, diskMenuButtonPattern);
    XtSetValues( diskMenuButton, argList, 1);
    XtSetArg(argList[0], XtNbitmap, cameraMenuButtonPattern);
    XtSetValues( cameraMenuButton, argList, 1);
    XtSetArg(argList[0], XtNbitmap, splcameraMenuButtonPattern);
    XtSetValues( splcameraMenuButton, argList, 1);
    XtSetArg(argList[0], XtNbitmap, lightMenuButtonPattern);
    XtSetValues( lightMenuButton, argList, 1);
/*    XtSetArg(argList[0], XtNbitmap, colorMenuButtonPattern);
    XtSetValues( colorMenuButton, argList, 1);
    XtSetArg(argList[0], XtNbitmap, spliceMenuButtonPattern);
    XtSetValues( spliceMenuButton, argList, 1);
*/
    XtSetArg(argList[0], XtNbitmap, mergeMenuButtonPattern);
    XtSetValues( mergeMenuButton, argList, 1);

    XtSetArg(argList[0], XtNbitmap, leftarrowPattern);
    XtSetValues( leftarrow1 , argList, 1);
    XtSetArg(argList[0], XtNbitmap, rightarrowPattern);
    XtSetValues( rightarrow1, argList, 1);
    XtSetArg(argList[0], XtNbitmap, uparrowPattern);
    XtSetValues( uparrow1, argList, 1);
    XtSetArg(argList[0], XtNbitmap, downarrowPattern);
    XtSetValues( downarrow1, argList, 1);

    XtSetArg(argList[0], XtNbitmap, leftarrowPattern);
    XtSetValues( leftarrow2 , argList, 1);
    XtSetArg(argList[0], XtNbitmap, rightarrowPattern);
    XtSetValues( rightarrow2, argList, 1);
    XtSetArg(argList[0], XtNbitmap, uparrowPattern);
    XtSetValues( uparrow2, argList, 1);
    XtSetArg(argList[0], XtNbitmap, downarrowPattern);
    XtSetValues( downarrow2, argList, 1);

    XtSetArg(argList[0], XtNbitmap, leftarrowPattern);
    XtSetValues( leftarrow3 , argList, 1);
    XtSetArg(argList[0], XtNbitmap, rightarrowPattern);
    XtSetValues( rightarrow3, argList, 1);
    XtSetArg(argList[0], XtNbitmap, uparrowPattern);
    XtSetValues( uparrow3, argList, 1);
    XtSetArg(argList[0], XtNbitmap, downarrowPattern);
    XtSetValues( downarrow3, argList, 1);

    XtSetArg(argList[0], XtNbitmap, leftarrowPattern);
    XtSetValues( frame_lbutton, argList, 1);
    XtSetArg(argList[0], XtNbitmap, rightarrowPattern);
    XtSetValues( frame_rbutton, argList, 1);

    XtSetArg(argList[0], XtNbitmap, left10arrowPattern);
    XtSetValues( frame_l10button, argList, 1);
    XtSetArg(argList[0], XtNbitmap, right10arrowPattern);
    XtSetValues( frame_r10button, argList, 1);

    XtSetArg(argList[0], XtNbitmap, leftarrowPattern);
    XtSetValues( zoom_lbutton, argList, 1);
    XtSetArg(argList[0], XtNbitmap, rightarrowPattern);
    XtSetValues( zoom_rbutton, argList, 1);

}

/********************************************************************
* makeIcon 
*
* This function creates an icon for the program. 
*********************************************************************/
void makeIcon()
{
    XWMHints theWMHints;
    
    theWMHints.icon_pixmap = pgmIconPattern; 
    theWMHints.initial_state = NormalState; 
    theWMHints.flags = IconPixmapHint | StateHint; 
    XSetWMHints( theDisplay, outerWindow, &theWMHints );
}
/**********************************************************
* init()
*
* This function initializes the global variable settings.
***********************************************************/
void init()
{
    int i;
    struct Mover *pntr;
    struct Light *lptr;

    for (i=0; i<3;i++)
        lightcount[i] = 0;
    settings.cutframe.frame =0;
    settings.zoom = 30;
    settings.camera.x = 60; settings.camera.y = 0 ; settings.camera.z = 0;
    settings.slate.x = 0;
    settings.slate.y = 0;
    settings.slate.z = 0;
    settings.moving = CAMERA;
    settings.scale = 1.0;
    settings.mode = CAMERA;
    settings.zoom = 30;
    settings.light_type = DIFFUSE;
    lptr = malloc(sizeof(struct Light));
    lptr->next = NULL;
    settings.light_head = lptr;
    settings.light_ptr = lptr;
    strcpy(settings.message1,">");
    strcpy(settings.message2,">");
    strcpy(settings.message3,">");

    lightelement.lx = 0;
    lightelement.ly = 0;
    lightelement.lz = 0;
    lightelement.lfx = 0;
    lightelement.lfy = 0;
    lightelement.lfz = 0;
    barpos=0.5;
    cut = FALSE;
    spline = FALSE;
    sspline = FALSE;
    move = FALSE;
    smove = FALSE;
    zoom = FALSE;
    set_bars();
    show_views();
    num_objects = 0;
}
/*********************************************************************
* initX
*
* This function sets up all of the X window stuff for the program.
*********************************************************************/
void initX( argc, argv)
   int argc;
   char *argv[];
{ 
   int i,j;
    XSetWindowAttributes     xswa;

   String fallbackResources[]  =
   {
     "*icon_pixmap: ./facilityIcon",
     "*.resizable:              FALSE",
     "*.font: fixed", //  -*-helvetica-bold-r-normal--12-*", 

    "*MenuButton.background:    lightblue",
    "*Form*Label.foreground:    navyblue",
    "xpost*side_view.background:  lightblue",
    "*Text*translations: #override \n <Key>Return: nullReturn()",
    "*input: True",
    "*Dialog*Text.width: 200",
    "*Dialog*Text.background: White",
    "*prompt*value: ",
    "*savePrompt*value.translations: #override \n <Key>Return: saveReturn()",
    "*loadPrompt*value.translations: #override \n <Key>Return: loadReturn()",
    "*fileNamePrompt*value.translations: #override \n <Key>Return: fileNameReturn()",
    "*bitmapFilePath   /cis/thunder0/pjj/xpost",
     NULL,
   };

    topLevel = XtAppInitialize(&appCon, "xpost", NULL, 0, &argc, argv, 
                     fallbackResources, NULL, 0);
    set_colors();
    createWidgets();
    XtRealizeWidget(topLevel);
    createXWindow(); 
    readBitmaps();
    makeIcon();
    xswa.backing_store = Always;
    XChangeWindowAttributes(theDisplay, cameraWindow,
                            CWBackingStore, &xswa);
    XChangeWindowAttributes(theDisplay, topWindow,
                            CWBackingStore, &xswa);
    XChangeWindowAttributes(theDisplay, frontWindow,
                            CWBackingStore, &xswa);
    XChangeWindowAttributes(theDisplay, sideWindow,
                            CWBackingStore, &xswa);
}
