#include "main.h"

extern Settings settings;
extern Widget cutmenu[8];
extern int cut,zoom,move,sspline,spline;
extern struct Mover *framelist;
extern void do_spline();
extern int spline;
extern Widget splmenu[];

extern void fill_frames();

void splineCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[MAX_ARGS];
    struct Mover *begin_frame,*work_frame,*ptr;
    float camdx,camdy,camdz,sldx,sldy,sldz,delta,delta1;
    int i,t,startframe,endframe,points;

    t = settings.current_frame;
    XtSetArg(argList[0], XtNsensitive,FALSE);
    XtSetValues( splmenu[0], argList, 1);

    if (widget == splmenu[0]) {
        if (settings.moving == CAMERA) {
            spline = TRUE;
            settings.start = t;
            if (framelist[t].movetype == NONE) {
                framelist[t].movetype = SPLINESTART;
            }
            else {
                (framelist[t].movetype = SPLINESTARTEND);
            }
            framelist[t].camera.x   = settings.camera.x;
            framelist[t].camera.y   = settings.camera.y;
            framelist[t].camera.z   = settings.camera.z;
        }
        else {
            sspline = TRUE;
            settings.start = t;
            if (framelist[t].smovetype == NONE) {
                framelist[t].smovetype = SPLINESTART;
            }
            else {
                (framelist[t].smovetype = SPLINESTARTEND);
            }
            framelist[t].slate.x = settings.slate.x;
            framelist[t].slate.y = settings.slate.y;
            framelist[t].slate.z = settings.slate.z;
        }
    }
    else if (widget == splmenu[1]) {
        XtSetArg(argList[0], XtNsensitive,FALSE);
        XtSetValues( splmenu[1], argList, 1);
        XtSetArg(argList[0], XtNsensitive,FALSE);
        XtSetValues( splmenu[2], argList, 1);
        if (settings.moving == CAMERA) {
            framelist[t].movetype = SPLINEPOINT;
            framelist[t].camera.x   = settings.camera.x;
            framelist[t].camera.y   = settings.camera.y;
            framelist[t].camera.z   = settings.camera.z;
        }
        else {
            framelist[t].smovetype = SPLINEPOINT;
            framelist[t].slate.x = settings.slate.x;
            framelist[t].slate.y = settings.slate.y;
            framelist[t].slate.z = settings.slate.z;
        }
        XtSetArg(argList[0], XtNsensitive,TRUE);
        XtSetValues( splmenu[3], argList, 1);
    }
    else if (widget == splmenu[2]) {
        XtSetArg(argList[0], XtNsensitive,FALSE);
        XtSetValues( splmenu[1], argList, 1);
        XtSetArg(argList[0], XtNsensitive,FALSE);
        XtSetValues( splmenu[2], argList, 1);
        if (settings.moving == CAMERA) {
            spline = FALSE;
            framelist[t].movetype = SPLINEEND;
            framelist[t].camera.x   = settings.camera.x;
            framelist[t].camera.y   = settings.camera.y;
            framelist[t].camera.z   = settings.camera.z;

            points = 0;
            startframe = settings.start;
            endframe = settings.current_frame;
            for (i=startframe; i<=endframe; i++) 
                if (framelist[i].movetype == SPLINESTART   || 
                   framelist[i].movetype == SPLINESTARTEND ||
                   framelist[i].movetype == SPLINEPOINT    ||
                   framelist[i].movetype == SPLINEEND)
                    points++;
            do_spline(startframe,endframe,points);
        }
        else {
            sspline = FALSE;
            framelist[t].smovetype = SPLINEEND;
            framelist[t].slate.x = settings.slate.x;
            framelist[t].slate.y = settings.slate.y;
            framelist[t].slate.z = settings.slate.z;

            points = 0;
            startframe = settings.start;
            endframe = settings.current_frame;
            for (i=startframe; i<=endframe; i++)
                if (framelist[i].smovetype == SPLINESTART   ||
                   framelist[i].smovetype == SPLINESTARTEND ||
                   framelist[i].smovetype == SPLINEPOINT    ||
                   framelist[i].smovetype == SPLINEEND)
                    points++;
            do_spline(startframe,endframe,points);
        }
    }
    else if (widget == splmenu[3]) {
        if (settings.moving == CAMERA) {
            framelist[t].movetype = NONE;
            framelist[t].camera.x  = framelist[t-1].camera.x;
            framelist[t].camera.y  = framelist[t-1].camera.y;
            framelist[t].camera.z  = framelist[t-1].camera.z;
        }
        else {
            framelist[t].smovetype = NONE;
            framelist[t].slate.x  = framelist[t-1].slate.x;
            framelist[t].slate.y  = framelist[t-1].slate.y;
            framelist[t].slate.z  = framelist[t-1].slate.z;
        }
        XtSetArg(argList[0], XtNsensitive,FALSE);
        XtSetValues( splmenu[3], argList, 1);
    }
    fill_frames(t);
}
