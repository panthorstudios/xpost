#include "main.h"

extern Settings settings;
extern Widget cutmenu[8];
extern int cut,zoom,move,smove;
extern struct Info *movelisthead;
extern struct Mover *framelist;

void fill_frames();
void fill_zoom_frames();

void cutCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    int i,done;
    struct Mover *ptr;

    i = settings.current_frame;
    if (settings.moving == CAMERA) {
        framelist[i].movetype = CUT;
        framelist[i].camera.x = settings.camera.x;
        framelist[i].camera.y = settings.camera.y;
        framelist[i].camera.z = settings.camera.z;
    }
    else {
        framelist[i].smovetype = CUT;
        framelist[i].slate.x = settings.slate.x;
        framelist[i].slate.y = settings.slate.y;
        framelist[i].slate.z = settings.slate.z;
    }
    fill_frames(i);
}

void fill_frames(i)
int i;
{
    int done;
    done = FALSE;
    i +=1;
    if (settings.moving == CAMERA) {
        while (i<=settings.frames && framelist[i].movetype == NONE) {
            framelist[i].camera.x = settings.camera.x;
            framelist[i].camera.y = settings.camera.y;
            framelist[i].camera.z = settings.camera.z;
            i+=1;
        }
    }
    else {
        while (i<=settings.frames && framelist[i].smovetype == NONE) {
            framelist[i].slate.x = settings.slate.x;
            framelist[i].slate.y = settings.slate.y;
            framelist[i].slate.z = settings.slate.z;
            i += 1;
        }
    }
}
void fill_zoom_frames(i)
int i;
{
    i++;
    while (i<=settings.frames && framelist[i].zoomtype == NONE) {
        framelist[i].zoomval = settings.zoom; 
        i++;
    }
}

void dollyCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[MAX_ARGS];
    struct Mover *begin_frame,*work_frame,*ptr;
    float camdx,camdy,camdz,sldx,sldy,sldz,delta,delta1;
    int i,t,startframe,endframe;

    t = settings.current_frame;

    if (widget == cutmenu[1]) {
        XtSetArg(argList[0], XtNsensitive,FALSE);
        XtSetValues( cutmenu[1], argList, 1);
        if (settings.moving == CAMERA) {
            move = TRUE;
            if (framelist[t].movetype == NONE) {
                framelist[t].movetype = DOLLYSTART;
            }
            else {
                (framelist[t].movetype = DOLLYSTARTEND);
            }
            framelist[t].camera.x   = settings.camera.x;
            framelist[t].camera.y   = settings.camera.y;
            framelist[t].camera.z   = settings.camera.z;
        }
        else {
            smove = TRUE;
            if (framelist[t].smovetype == NONE) {
                framelist[t].smovetype = DOLLYSTART;
            }
            else {
                (framelist[t].smovetype = DOLLYSTARTEND);
            }
            framelist[t].slate.x = settings.slate.x;
            framelist[t].slate.y = settings.slate.y;
            framelist[t].slate.z = settings.slate.z;
        }
    }
    else if (widget == cutmenu[2]) {
        XtSetArg(argList[0], XtNsensitive,FALSE);
        XtSetValues( cutmenu[2], argList, 1);
        XtSetArg(argList[0], XtNsensitive,TRUE);
        XtSetValues( cutmenu[1], argList, 1);
        if (settings.moving == CAMERA) {
            move = FALSE;
            framelist[t].movetype = DOLLYEND;
            framelist[t].camera.x   = settings.camera.x;
            framelist[t].camera.y   = settings.camera.y;
            framelist[t].camera.z   = settings.camera.z;
        }
        else {
            smove = FALSE;
            framelist[t].smovetype = DOLLYEND;
            framelist[t].slate.x = settings.slate.x;
            framelist[t].slate.y = settings.slate.y;
            framelist[t].slate.z = settings.slate.z;
        }

        startframe = t;
        if (settings.moving == CAMERA) {
            while (framelist[startframe].movetype != DOLLYSTART && 
               framelist[startframe].movetype != DOLLYSTARTEND)
               startframe--;
            endframe = t;
            delta = startframe - endframe;
            camdx = (float)(framelist[startframe].camera.x - 
                     settings.camera.x)/delta;
            camdy = (float)(framelist[startframe].camera.y -
                     settings.camera.y)/delta;
            camdz = (float)(framelist[startframe].camera.z -
                     settings.camera.z)/delta;
            for (i=startframe+1;i<endframe;i++) {
                delta1 = (float)i - (float)startframe;
                framelist[i].movetype = DOLLY;
                framelist[i].camera.x = framelist[startframe].camera.x
                                        +(delta1*camdx);
                framelist[i].camera.y = framelist[startframe].camera.y
                                        +(delta1*camdy);
                framelist[i].camera.z = framelist[startframe].camera.z
                                        +(delta1*camdz);
            }
        }
        else {
            while (framelist[startframe].smovetype != DOLLYSTART &&
               framelist[startframe].smovetype != DOLLYSTARTEND)
               startframe--;
            endframe = t;
            delta = startframe - endframe;
            sldx = (float)(framelist[startframe].slate.x -
                    settings.slate.x)/delta;
            sldy = (float)(framelist[startframe].slate.y -
                    settings.slate.y)/delta;
            sldz = (float)(framelist[startframe].slate.z -
                    settings.slate.z)/delta;
            for (i=startframe+1;i<endframe;i++) {
                delta1 = (float)i - (float)startframe;
                framelist[i].smovetype = DOLLY;

                framelist[i].slate.x = framelist[startframe].slate.x
                                       +(delta1*sldx);
                framelist[i].slate.y = framelist[startframe].slate.y
                                       +(delta1*sldy);
                framelist[i].slate.z = framelist[startframe].slate.z
                                       +(delta1*sldz);
            }        
        }
    }
    fill_frames(t);
}

void zoomCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[MAX_ARGS];
    struct Mover *work_frame,*begin_frame;
    int i,t,startframe,endframe;
    float dz,delta,delta1;

    t = settings.current_frame;

    if (widget == cutmenu[3]) {
        zoom = TRUE;
        framelist[t].zoomtype = ZOOMSTART;
        framelist[t].zoomval = settings.zoom;
        XtSetArg(argList[0], XtNsensitive, FALSE);
        XtSetValues( cutmenu[3], argList, 1);
        XtSetArg(argList[0], XtNsensitive, TRUE);
        XtSetValues( cutmenu[4], argList, 1);
    }
    if (widget == cutmenu[4]) {
        startframe = t;
        endframe = t;
        framelist[t].zoomtype = ZOOMEND;
        framelist[t].zoomval = settings.zoom;
        while (framelist[startframe].zoomtype != ZOOMSTART)
            startframe--;

        delta = endframe-startframe;
        dz = (float)((framelist[endframe].zoomval) -
                    (framelist[startframe].zoomval))/delta;
        for (i=startframe+1;i<endframe;i++) {
            delta1 = i - (float)startframe;
            framelist[i].zoomval = (framelist[startframe].zoomval)+(delta1*dz);
            framelist[i].zoomtype = ZOOM;
        }        

        XtSetArg(argList[0], XtNsensitive, FALSE);
        XtSetValues( cutmenu[4], argList, 1);
        XtSetArg(argList[0], XtNsensitive, TRUE);
        XtSetValues( cutmenu[3], argList, 1);
        fill_zoom_frames(settings.current_frame);
    }
}
