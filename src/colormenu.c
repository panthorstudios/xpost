#include "main.h"

extern Settings settings;
extern Display *theDisplay;
extern Widget lightmenu[5];
extern int cut,zoom,move;
extern void DestroyPopupPrompt();
extern Widget cameraview,popupbox;
extern void destroybox();
void colorlistCallback();

void colorCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg         argList[MAX_ARGS];
    Widget      colorparent,colorlabel,colorviewport,colorbutton1,
                colorbutton2,colorlist;
    Position    x, y;
    Dimension   width, height;
    Cardinal    n;
    char        displayString[MAX_CHARS_IN_WINDOW];
    float       barpos;
    char        *colornames[] = { "red","green","blue","yellow","orange",
                              "magenta","turquoise","black",(char *)NULL };

    XtTranslateCoords(cameraview, (Position) 10, (Position) 10, &x, &y);

    n = 0;
    XtSetArg(argList[n], XtNx, x);                         n++;
    XtSetArg(argList[n], XtNy, y);                         n++;

    popupbox = XtCreatePopupShell("colorPrompt", transientShellWidgetClass,
                                     cameraview, argList, n);

    colorparent = XtCreateManagedWidget("colorparent", formWidgetClass,
                        popupbox,NULL,ZERO);

    n = 0;
    XtSetArg( argList[n], XtNlabel, "Select color:");        n++;
    XtSetArg( argList[n], XtNborderWidth, 0);        n++;
    colorlabel = XtCreateManagedWidget("colorlabel", labelWidgetClass,
        colorparent, argList,n);

    n = 0;
    sprintf(displayString,"Done");
    XtSetArg(argList[n], XtNfromHoriz, colorlabel );              n++;
    XtSetArg(argList[n], XtNlabel, displayString); n++;
    colorbutton1 = XtCreateManagedWidget("colorbutton", commandWidgetClass,
        colorparent, argList,n);
    XtAddCallback(colorbutton1,XtNcallback,destroybox,NULL);

    n = 0;
    sprintf(displayString,"Cancel");
    XtSetArg(argList[n], XtNfromHoriz, colorbutton1 );              n++;
    XtSetArg(argList[n], XtNlabel, displayString); n++;
    colorbutton2 = XtCreateManagedWidget("colorbutton", commandWidgetClass,
        colorparent, argList,n);
    XtAddCallback(colorbutton2,XtNcallback,destroybox,NULL);

/*    n = 0;
    XtSetArg(argList[n], XtNallowVert, TRUE); n++;
    XtSetArg(argList[n], XtNuseRight, TRUE); n++;
    XtSetArg(argList[n], XtNfromVert, colorbutton );              n++;
    colorviewport = XtCreateManagedWidget("colorviewport", viewportWidgetClass,
        colorparent, argList,n);
*/

    n = 0;
    XtSetArg( argList[n], XtNlist, colornames);        n++;
    XtSetArg(argList[n], XtNfromVert, colorlabel );              n++;
    XtSetArg(argList[n], XtNdefaultColumns,3);        n++;
    colorlist = XtCreateManagedWidget("colorlist", listWidgetClass,
        colorparent, argList,n);
    XtAddCallback(colorlist,XtNcallback,colorlistCallback,NULL);

    XawListHighlight(colorlist,0); 

    XtPopup(popupbox, XtGrabExclusive);

}
void colorlistCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    XawListReturnStruct *item = (XawListReturnStruct*)call_data;
    printf( "selected item %d; \"%s\"\n", item->list_index, item->string );
}
