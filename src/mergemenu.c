#include "main.h"

extern Settings settings;
extern Display *theDisplay;
extern Widget mergemenu[],popbutton1,popbutton2,sidebar1,views,donebutton;
extern Pixmap leftarrowPattern,rightarrowPattern;
extern int lightcount[5];
extern Widget cameraview,popupbox,popuplabel,camerabutton,slatebutton;
extern Widget windowW,popleftbutton,poprightbutton;
extern unsigned long LightBluePen,handcursor,lrcursor;
extern void set_bars();
extern void merge_files();
extern void splice_files();
void   destroymbox();
void   domerge();

int    mfunction;
Widget srcbox,destbox,frmbox,src1box;

void mergeCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[7];
    int i,n,function;
    Widget mergeparent,msg21,msg1,msg2,msg3,msg4,donebox,canbox;
    char        displayString[MAX_CHARS_IN_WINDOW];
    Position    x, y;

    if (widget == mergemenu[0]) mfunction = MERGE;
    else mfunction = SPLICE;
 
    XtTranslateCoords(cameraview, (Position) 50, (Position) 50, &x, &y);

    n = 0;
    XtSetArg(argList[n], XtNx, x);                         n++;
    XtSetArg(argList[n], XtNy, y);                         n++;

    popupbox = XtCreatePopupShell("lightPrompt", transientShellWidgetClass,
                                     cameraview, argList, n);

    mergeparent = XtCreateManagedWidget("mergeparent", formWidgetClass,
                        popupbox,NULL,ZERO);

    n=0;
    if (mfunction == MERGE) {
        XtSetArg( argList[n], XtNlabel, "MERGE FILES");    n++;
    }
    else {
        XtSetArg( argList[n], XtNlabel, "SPLICE FILES");    n++;
    }
    XtSetArg( argList[n], XtNforeground, LightBluePen);    n++;
    XtSetArg( argList[n], XtNborderWidth, 0);    n++;
    msg1 = XtCreateManagedWidget("msg1", labelWidgetClass,
        mergeparent, argList,n);

    n=0;
    XtSetArg( argList[n], XtNborderWidth, 0);    n++;
    XtSetArg( argList[n], XtNfromVert,msg1);    n++;
    XtSetArg( argList[n], XtNlabel, "Source File 1:");    n++;
    msg2 = XtCreateManagedWidget("msg2", labelWidgetClass,
        mergeparent, argList,n);

    n=0;
    XtSetArg( argList[n], XtNfromVert,msg2);    n++;
    XtSetArg( argList[n], XtNwidth, 100);    n++;
    XtSetArg( argList[n], XtNeditType, XawtextEdit);    n++;
    srcbox = XtCreateManagedWidget("srcbox", asciiTextWidgetClass,
        mergeparent, argList,n);

    n=0;
    XtSetArg( argList[n], XtNborderWidth, 0);    n++;
    XtSetArg( argList[n], XtNfromVert,srcbox);    n++;
    XtSetArg( argList[n], XtNlabel, "Source File 2:");    n++;
    msg21 = XtCreateManagedWidget("msg21", labelWidgetClass,
        mergeparent, argList,n);

    n=0;
    XtSetArg( argList[n], XtNfromVert,msg21);    n++;
    XtSetArg( argList[n], XtNwidth, 100);    n++;
    XtSetArg( argList[n], XtNeditType, XawtextEdit);    n++;
    src1box = XtCreateManagedWidget("src1box", asciiTextWidgetClass,
        mergeparent, argList,n);

    n=0;
    XtSetArg( argList[n], XtNborderWidth, 0);    n++;
    XtSetArg( argList[n], XtNfromVert,src1box);    n++;
    XtSetArg( argList[n], XtNlabel, "Destination File:");    n++;
    msg3 = XtCreateManagedWidget("msg3", labelWidgetClass,
        mergeparent, argList,n);

    n=0;
    XtSetArg( argList[n], XtNfromVert,msg3);    n++;
    XtSetArg( argList[n], XtNwidth, 100);    n++;
    XtSetArg( argList[n], XtNeditType, XawtextEdit);    n++;
    destbox = XtCreateManagedWidget("destbox", asciiTextWidgetClass,
        mergeparent, argList,n);

    n=0;
    if (mfunction == MERGE) {
        XtSetArg( argList[n], XtNlabel, "Merge Start Frame:");    n++;
    }
    else {
        XtSetArg( argList[n], XtNlabel, "Splice After Frame:");    n++;
    }
    XtSetArg( argList[n], XtNborderWidth, 0);    n++;
    XtSetArg( argList[n], XtNfromVert,destbox);    n++;
    msg4 = XtCreateManagedWidget("msg4", labelWidgetClass,
        mergeparent, argList,n);

    n=0;
    XtSetArg( argList[n], XtNfromVert,msg4);    n++;
    XtSetArg( argList[n], XtNwidth, 100);    n++;
    XtSetArg( argList[n], XtNeditType, XawtextEdit);    n++;
    frmbox = XtCreateManagedWidget("frmbox", asciiTextWidgetClass,
        mergeparent, argList,n);

    n=0;
    XtSetArg( argList[n], XtNfromVert,frmbox);    n++;
    XtSetArg( argList[n], XtNlabel, "Done");    n++;
    popbutton1 = XtCreateManagedWidget("donebox", commandWidgetClass,
        mergeparent, argList,n);
    XtAddCallback(popbutton1,XtNcallback,domerge,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert,frmbox);    n++;
    XtSetArg( argList[n], XtNfromHoriz,popbutton1);    n++;
    XtSetArg( argList[n], XtNlabel, "Cancel");    n++;
    popbutton2 = XtCreateManagedWidget("canbox", commandWidgetClass,
        mergeparent, argList,n);
    XtAddCallback(popbutton2,XtNcallback,destroymbox,NULL);

    XtPopup(popupbox,XtGrabExclusive);
}

void destroymbox(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[1];

    XtPopdown(popupbox);
    XFlush(theDisplay);
}
void domerge(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[1];
    String str;

    XtSetArg(argList[0], XtNstring, &str);
    XtGetValues(srcbox, argList, 1);
    strcpy(settings.src1,str);
    XtSetArg(argList[0], XtNstring, &str);
    XtGetValues(src1box, argList, 1);
    strcpy(settings.src2,str);

    XtSetArg(argList[0], XtNstring, &str);
    XtGetValues(destbox, argList, 1);
    strcpy(settings.dest,str);

    XtSetArg(argList[0], XtNstring, &str);
    XtGetValues(frmbox, argList, 1);
    if ((settings.mergeframe = atoi(str)) <1) {
        writeMessage("Improper frame number");
        return;
    }
    if (strcmp(settings.src1,settings.src2) &&
         strcmp(settings.src1,settings.dest) &&
         strcmp(settings.src2,settings.dest)) {

    if (mfunction == MERGE)
        merge_files(settings.src1,settings.src2,settings.dest,
                    settings.mergeframe);
    else
        splice_files(settings.src1,settings.src2,settings.dest,
                    settings.mergeframe);
    }
    XtPopdown(popupbox);
    XFlush(theDisplay);

}
