#include "main.h"
extern Pixmap slatePattern,cameraPattern,blankPattern;
extern Pixmap spotPattern,lightPattern,spotfocusPattern,warnfocusPattern;
extern Window frontWindow,topWindow,sideWindow;
extern Widget leftarrow1,left10arrow1,uparrow1,downarrow1,
              up10arrow1,down10arrow1,rightarrow1,right10arrow1;
extern Widget leftarrow2,left10arrow2,uparrow2,downarrow2,
              up10arrow2,down10arrow2,rightarrow2,right10arrow2;
extern Widget leftarrow3,left10arrow3,uparrow3,downarrow3,
              up10arrow3,down10arrow3,rightarrow3,right10arrow3;
extern Widget camerabutton,slatebutton,frontbar1,frontbar2,statmsg;
extern Widget sidebar1,sidebar2,topbar1,topbar2,plusbutton,minusbutton;
extern Widget frame_lbutton,frame_rbutton,framemsg,zoombar;
extern Widget frame_l10button,frame_r10button,zoommsg;
extern Widget cutmenu[],splmenu[],zoom_lbutton,zoom_rbutton;
extern GC frontGC, sideGC, topGC;
extern Display *theDisplay;
extern Settings settings;
extern int zoom,move,smove,sspline,spline;
extern struct Light lightelement;
extern struct Mover *framelist;
extern draw_objects();

void show_views() {
    int x,y,z;
    int mxs,mys,mzs;
    Arg argList[MAX_ARGS];
    char displayString[MAX_CHARS_IN_WINDOW];  /* message for user */

    XDrawImageString(theDisplay, frontWindow, frontGC, 5, 12,
        "front", 5);
    XDrawImageString(theDisplay, topWindow, topGC, 5, 12,
        "top", 3);
    XDrawImageString(theDisplay, sideWindow, sideGC, 5, 12,
        "right side", 10);

    if (settings.mode == CAMERA) {
        x = MIDDLE + (settings.slate.y/settings.scale);
        y = MIDDLE - (settings.slate.z/settings.scale);
        XCopyPlane(theDisplay,slatePattern,frontWindow,
            frontGC,0,0,16,16,x,y,1);

        x = MIDDLE + (settings.slate.y/settings.scale);
        y = (settings.slate.x/settings.scale) + MIDDLE;
        XCopyPlane(theDisplay,slatePattern,topWindow,
            topGC,0,0,16,16,x,y,1);

        x = MIDDLE - (settings.slate.x/settings.scale);
        y = MIDDLE - (settings.slate.z/settings.scale);
        XCopyPlane(theDisplay,slatePattern,sideWindow,
            sideGC,0,0,16,16,x,y,1);

        x = MIDDLE + (settings.camera.y/settings.scale);
        y = MIDDLE - (settings.camera.z/settings.scale);
        XCopyPlane(theDisplay,cameraPattern,frontWindow,
            frontGC,0,0,16,16,x,y,1);

        x = MIDDLE + (settings.camera.y/settings.scale) ;
        y = (settings.camera.x/settings.scale) + MIDDLE;
        XCopyPlane(theDisplay,cameraPattern,topWindow,
            topGC,0,0,16,16,x,y,1);

        x = MIDDLE - (settings.camera.x/settings.scale);
        y = MIDDLE - (settings.camera.z/settings.scale);
        XCopyPlane(theDisplay,cameraPattern,sideWindow,
            sideGC,0,0,16,16,x,y,1);

        if (settings.moving == CAMERA) {
            mxs=settings.camera.x;
            mys=settings.camera.y;
            mzs=settings.camera.z;
        }
        else if (settings.moving == SLATE) {
            mxs=settings.slate.x;
            mys=settings.slate.y;
            mzs=settings.slate.z;
        }
    }
    else {
        if (settings.light_type != DIFFUSE) {
            x = MIDDLE + (lightelement.lfy/settings.scale);
            y = MIDDLE - (lightelement.lfz/settings.scale);
            if (settings.light_type == WARN)
                XCopyPlane(theDisplay,warnfocusPattern,frontWindow,
                    frontGC,0,0,16,16,x,y,1);
            else
                XCopyPlane(theDisplay,spotfocusPattern,frontWindow,
                    frontGC,0,0,16,16,x,y,1);

            x = MIDDLE+(lightelement.lfy/settings.scale);
            y = (lightelement.lfx/settings.scale) + MIDDLE;
            if (settings.light_type == WARN)
                XCopyPlane(theDisplay,warnfocusPattern,topWindow,
                    topGC,0,0,16,16,x,y,1);
            else
                XCopyPlane(theDisplay,spotfocusPattern,topWindow,
                    topGC,0,0,16,16,x,y,1);

            x = MIDDLE - (lightelement.lfx/settings.scale);
            y = MIDDLE - (lightelement.lfz/settings.scale);
            if (settings.light_type == WARN)
                XCopyPlane(theDisplay,warnfocusPattern,sideWindow,
                    sideGC,0,0,16,16,x,y,1);
            else
                XCopyPlane(theDisplay,spotfocusPattern,sideWindow,
                    sideGC,0,0,16,16,x,y,1);
        }

        x = MIDDLE + (lightelement.ly/settings.scale);
        y = MIDDLE - (lightelement.lz/settings.scale);
        if (settings.light_type != DIFFUSE)
            XCopyPlane(theDisplay,spotPattern,frontWindow,
                frontGC,0,0,16,16,x,y,1);
        else
            XCopyPlane(theDisplay,lightPattern,frontWindow,
                frontGC,0,0,16,16,x,y,1);

        x = MIDDLE + (lightelement.ly/settings.scale) ;
        y = (lightelement.lx/settings.scale) + MIDDLE;
        if (settings.light_type != DIFFUSE)
            XCopyPlane(theDisplay,spotPattern,topWindow,
                topGC,0,0,16,16,x,y,1);
        else
            XCopyPlane(theDisplay,lightPattern,topWindow,
                topGC,0,0,16,16,x,y,1);

        x = MIDDLE - (lightelement.lx/settings.scale);
        y = MIDDLE - (lightelement.lz/settings.scale);
        if (settings.light_type != DIFFUSE)
            XCopyPlane(theDisplay,spotPattern,sideWindow,
                sideGC,0,0,16,16,x,y,1);
        else
            XCopyPlane(theDisplay,lightPattern,sideWindow,
                sideGC,0,0,16,16,x,y,1);

        if (settings.moving == LIGHT) {
            mxs=lightelement.lx;
            mys=lightelement.ly;
            mzs=lightelement.lz;
        }
        else if (settings.moving == LIGHTFOCUS) {
            mxs=lightelement.lfx;
            mys=lightelement.lfy;
            mzs=lightelement.lfz;
        }
    }
    sprintf(displayString,"X: %d  Y: %d  Z: %d",mxs,mys,mzs);
    XtSetArg(argList[0], XtNlabel, displayString);
    XtSetValues( statmsg, argList, 1);

}
void clear_views() {
int x,y,z;
    if (settings.mode == CAMERA) {
        x = MIDDLE + (settings.camera.y/settings.scale);
        y = MIDDLE - (settings.camera.z/settings.scale);
        XCopyPlane(theDisplay,blankPattern,frontWindow,
            frontGC,0,0,16,16,x,y,1);
        x = MIDDLE + (settings.camera.y/settings.scale);
        y = (settings.camera.x/settings.scale) + MIDDLE;
        XCopyPlane(theDisplay,blankPattern,topWindow,
            topGC,0,0,16,16,x,y,1);
        x = MIDDLE - (settings.camera.x/settings.scale);
        y = MIDDLE - (settings.camera.z/settings.scale);
        XCopyPlane(theDisplay,blankPattern,sideWindow,
            sideGC,0,0,16,16,x,y,1);

        x = MIDDLE + (settings.slate.y/settings.scale);
        y = MIDDLE - (settings.slate.z/settings.scale);
        XCopyPlane(theDisplay,blankPattern,frontWindow,
            frontGC,0,0,16,16,x,y,1);
        x = MIDDLE+(settings.slate.y/settings.scale);
        y = (settings.slate.x/settings.scale) + MIDDLE;
        XCopyPlane(theDisplay,blankPattern,topWindow,
            topGC,0,0,16,16,x,y,1);
        x = MIDDLE - (settings.slate.x/settings.scale);
        y = MIDDLE - (settings.slate.z/settings.scale);
        XCopyPlane(theDisplay,blankPattern,sideWindow,
            sideGC,0,0,16,16,x,y,1);
    }
    else {
        x = MIDDLE + (lightelement.ly/settings.scale);
        y = MIDDLE - (lightelement.lz/settings.scale);
        XCopyPlane(theDisplay,blankPattern,frontWindow,
            frontGC,0,0,16,16,x,y,1);
        x = MIDDLE + (lightelement.ly/settings.scale);
        y = (lightelement.lx/settings.scale) + MIDDLE;
        XCopyPlane(theDisplay,blankPattern,topWindow,
            topGC,0,0,16,16,x,y,1);
        x = MIDDLE - (lightelement.lx/settings.scale);
        y = MIDDLE - (lightelement.lz/settings.scale);
        XCopyPlane(theDisplay,blankPattern,sideWindow,
            sideGC,0,0,16,16,x,y,1);

        if (settings.light_type !=DIFFUSE) {
            x = MIDDLE + (lightelement.lfy/settings.scale);
            y = MIDDLE - (lightelement.lfz/settings.scale);
            XCopyPlane(theDisplay,blankPattern,frontWindow,
                frontGC,0,0,16,16,x,y,1);
            x = MIDDLE+(lightelement.lfy/settings.scale);
            y = (lightelement.lfx/settings.scale) + MIDDLE;
            XCopyPlane(theDisplay,blankPattern,topWindow,
                topGC,0,0,16,16,x,y,1);
            x = MIDDLE - (lightelement.lfx/settings.scale);
            y = MIDDLE - (lightelement.lfz/settings.scale);
            XCopyPlane(theDisplay,blankPattern,sideWindow,
                sideGC,0,0,16,16,x,y,1);
        }
    }
}

void set_bars()
{
    float mxs,mys,mzs,bv1,bv2,bv3,bv4;

    if (settings.mode == CAMERA) {
        if (settings.moving == CAMERA) {
            mxs=settings.camera.x/settings.scale;
            mys=settings.camera.y/settings.scale;
            mzs=settings.camera.z/settings.scale;
        }
        else {
            mxs=settings.slate.x/settings.scale;
            mys=settings.slate.y/settings.scale;
            mzs=settings.slate.z/settings.scale;
        }
    }
    else {
        if (settings.moving == LIGHT) {
            mxs=lightelement.lx/settings.scale;
            mys=lightelement.ly/settings.scale;
            mzs=lightelement.lz/settings.scale;
        }
        else {
            mxs=lightelement.lx/settings.scale;
            mys=lightelement.ly/settings.scale;
            mzs=lightelement.lz/settings.scale;
        }
    }
    bv1 = -0.475+(mys/170);
    bv2 = 0.475-(mxs/170);
    bv3 = 0.475-(mzs/170);
    bv4 = 0.475+(mxs/170);
    XawScrollbarSetThumb(frontbar1,bv1,-1.0);
    XawScrollbarSetThumb(topbar1,bv1,-1.0);
    XawScrollbarSetThumb(frontbar2,bv3,-1.0);
    XawScrollbarSetThumb(sidebar2,bv3,-1.0);
    XawScrollbarSetThumb(topbar2,.95-bv2,-1.0);
    XawScrollbarSetThumb(sidebar1,bv2,-1.0);
}

void moveCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    int movex,movey,movez;
    Arg argList[MAX_ARGS];
    char displayString[MAX_CHARS_IN_WINDOW];  /* message for user */
    float bar12h;

    clear_views();
    if (settings.mode == CAMERA) {
        if (settings.moving == CAMERA) {
            movex = settings.camera.x/settings.scale;
            movey = settings.camera.y/settings.scale;
            movez = settings.camera.z/settings.scale;
        }
        else {
            movex = settings.slate.x/settings.scale;
            movey = settings.slate.y/settings.scale;
            movez = settings.slate.z/settings.scale;
        }
    }
    else {
        if (settings.moving == LIGHT) {
            movex = lightelement.lx/settings.scale;
            movey = lightelement.ly/settings.scale;
            movez = lightelement.lz/settings.scale;
        }
        else {
            movex = lightelement.lfx/settings.scale;
            movey = lightelement.lfy/settings.scale;
            movez = lightelement.lfz/settings.scale;
        }
    }

    if (widget == leftarrow1 || widget == leftarrow2) {
        if (--movey<-80) movey=-80;
    }
    if (widget == rightarrow1 || widget == rightarrow2) {
        if (++movey>80) movey=80;
    }
    if (widget == uparrow1 || widget == uparrow3) {
        if (++movez>80) movez=80;
    }
    if (widget == uparrow2 || widget == rightarrow3) {
        if (--movex<-80) movex=-80;
    }
    if (widget == downarrow2 || widget == leftarrow3) {
        if (++movex>80) movex=80;
    }
    if (widget == downarrow1 || widget == downarrow3) {
        if (--movez<-80) movez=-80;
    }
    if (settings.mode == CAMERA) {
        if (settings.moving == CAMERA) {
            settings.camera.x = movex*settings.scale;
            settings.camera.y = movey*settings.scale;
            settings.camera.z = movez*settings.scale;
        }
        else {
            settings.slate.x = movex*settings.scale;
            settings.slate.y = movey*settings.scale;
            settings.slate.z = movez*settings.scale;
        }
    }
    else {
        if (settings.moving == LIGHT) {
            lightelement.lx = movex*settings.scale;
            lightelement.ly = movey*settings.scale;
            lightelement.lz = movez*settings.scale;
        }
        else {
            lightelement.lfx = movex*settings.scale;
            lightelement.lfy = movey*settings.scale;
            lightelement.lfz = movez*settings.scale;
        }
    } 

    show_views();
    set_bars();
    draw_objects();
}

void buttonCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    if (widget == camerabutton) {
        if (settings.mode == CAMERA)
            settings.moving = CAMERA;
        else settings.moving = LIGHT;
    }
    if (widget == slatebutton) { 
        if (settings.mode == CAMERA)
            settings.moving = SLATE;
        else settings.moving = LIGHTFOCUS;
    }
    set_bars();
}

void change_bars(w,client_data,percent_pt)
Widget w;
XtPointer client_data;
XtPointer percent_pt;
{
float barval;
    int movex,movey,movez;

    barval = *(float*)percent_pt;
    if (barval>0.95) barval=0.95;
    if (barval<0.0) barval=0.0;
    clear_views();
    if (settings.mode == CAMERA) {
        if (settings.moving == CAMERA) {
            movex = settings.camera.x/settings.scale;
            movey = settings.camera.y/settings.scale;
            movez = settings.camera.z/settings.scale;
        }
        else {
            movex =  settings.slate.x/settings.scale;
            movey =  settings.slate.y/settings.scale;
            movez =  settings.slate.z/settings.scale;
        }
    }
    else {
        if (settings.moving == LIGHT) {
            movex = lightelement.lx/settings.scale;
            movey = lightelement.ly/settings.scale;
            movez = lightelement.lz/settings.scale;
        }
        else {
            movex = lightelement.lfx/settings.scale;
            movey = lightelement.lfy/settings.scale;
            movez = lightelement.lfz/settings.scale;
        }
    }

    if (w==frontbar1 || w==topbar1) {
        XawScrollbarSetThumb(frontbar1,barval,-1.0);
        XawScrollbarSetThumb(topbar1,barval,-1.0);
        movey = (int)((-0.475 + barval) * 170);
    }
    if (w==frontbar2 || w==sidebar2) {
        XawScrollbarSetThumb(frontbar2,barval,-1.0);
        XawScrollbarSetThumb(sidebar2,barval,-1.0);
        movez = (int)((0.475 - barval) * 170);
    }
    if (w==sidebar1) {
        XawScrollbarSetThumb(topbar2,.95-barval,-1.0);
        XawScrollbarSetThumb(sidebar1,barval,-1.0);
        movex = (int)((0.475 - barval) * 170);
    }
    if (w==topbar2) {
        XawScrollbarSetThumb(sidebar1,.95-barval,-1.0);
        XawScrollbarSetThumb(topbar2,barval,-1.0);
        movex = (int)((-0.475 + barval) * 170);
    }
    if (settings.mode == CAMERA) {
        if (settings.moving == CAMERA) {
            settings.camera.x = movex*settings.scale;
            settings.camera.y = movey*settings.scale;
            settings.camera.z = movez*settings.scale;
        }
        else {
            settings.slate.x = movex*settings.scale;
            settings.slate.y = movey*settings.scale;
            settings.slate.z = movez*settings.scale;
        }
    }
    else {
        if (settings.moving == LIGHT) {
            lightelement.lx = movex*settings.scale;
            lightelement.ly = movey*settings.scale;
            lightelement.lz = movez*settings.scale;
        }
        else {
            lightelement.lfx = movex*settings.scale;
            lightelement.lfy = movey*settings.scale;
            lightelement.lfz = movez*settings.scale;
        }
    }
    show_views();
    draw_objects();
}
void scaleCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    float test;

    clear_views();
    if (widget == plusbutton) {
        test = settings.scale*2;
        if (test <= 512)
            settings.scale = test;
    }
    if (widget == minusbutton) {
        test = settings.scale/2;
        if (test > 0.25 && settings.camera.x < (test*80)
                      && settings.camera.x > (test*-80)
                      && settings.camera.y < (test*80)
                      && settings.camera.y > (test*-80))
            settings.scale = test;
    }
    show_views();
    set_bars();
}

void frameCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[MAX_ARGS];
    char displayString[MAX_CHARS_IN_WINDOW];  /* message for user */
    int i;

    if (settings.frames > 0) {
        if (widget == frame_lbutton) {
            if (--(settings.current_frame) == 0)
                settings.current_frame = 1;
        }
        if (widget == frame_rbutton) {
            if (++(settings.current_frame) > settings.frames)
                settings.current_frame = settings.frames;
        }
        if (widget == frame_l10button) {
            if ((settings.current_frame - 10) < 1) 
                settings.current_frame = 1;
            else 
                settings.current_frame -= 10;
        }
        if (widget == frame_r10button) {
            if ((settings.current_frame+10) > settings.frames)
                settings.current_frame = settings.frames;
            else settings.current_frame += 10;
        }

    for (i=0;i<5;i++) {
            XtSetArg(argList[0], XtNsensitive,FALSE);
            XtSetValues( cutmenu[i], argList, 1);
        if (i <4) {
            XtSetArg(argList[0], XtNsensitive,FALSE);
            XtSetValues( splmenu[i], argList, 1);
        }
    }

    if (settings.moving == CAMERA) {
        if (framelist[settings.current_frame].movetype == NONE) {
            if (move == FALSE) {
                XtSetArg(argList[0], XtNsensitive,TRUE);
                XtSetValues( cutmenu[0], argList, 1);
                XtSetArg(argList[0], XtNsensitive, TRUE);
                XtSetValues( cutmenu[1], argList, 1);
                XtSetArg(argList[0], XtNsensitive, TRUE);
                XtSetValues( cutmenu[3], argList, 1);
            }
            else {
                XtSetArg(argList[0], XtNsensitive,TRUE);
                XtSetValues( cutmenu[2], argList, 1);
            }
            if (spline == FALSE) {
                if (settings.current_frame < (settings.frames-1)) {
                    XtSetArg(argList[0], XtNsensitive,TRUE);
                    XtSetValues( splmenu[0], argList, 1);
                }
            }
            else {
                if (settings.current_frame > (settings.start+1)) {
                    XtSetArg(argList[0], XtNsensitive,TRUE);
                    XtSetValues( splmenu[2], argList, 1);
                    XtSetArg(argList[0], XtNsensitive, TRUE);
                    XtSetValues( splmenu[1], argList, 1);
                }
            }
        }
        else if (framelist[settings.current_frame].movetype == DOLLYEND) {
            if (move == FALSE) {
                XtSetArg(argList[0], XtNsensitive, TRUE);
                XtSetValues( cutmenu[1], argList, 1);
            }
        }
        else if (framelist[settings.current_frame].movetype == SPLINEPOINT) {
            if (spline == TRUE) {
                XtSetArg(argList[0], XtNsensitive,TRUE);
                XtSetValues( splmenu[3], argList, 1);
            }
        }
        if (framelist[settings.current_frame].zoomtype == NONE) {
            if (zoom == TRUE) {
                XtSetArg(argList[0], XtNsensitive, TRUE);
                XtSetValues( cutmenu[4], argList, 1);
            }
            else {
                XtSetArg(argList[0], XtNsensitive, TRUE);
                XtSetValues( cutmenu[3], argList, 1);
            }
        }
    }
    else {
        if (framelist[settings.current_frame].smovetype == NONE) {
            if (smove == FALSE) {
                XtSetArg(argList[0], XtNsensitive,TRUE);
                XtSetValues( cutmenu[0], argList, 1);
                XtSetArg(argList[0], XtNsensitive, TRUE);
                XtSetValues( cutmenu[1], argList, 1);
                XtSetArg(argList[0], XtNsensitive, TRUE);
                XtSetValues( cutmenu[3], argList, 1);
            }
            else {
                XtSetArg(argList[0], XtNsensitive,TRUE);
                XtSetValues( cutmenu[2], argList, 1);
            }
            if (sspline == FALSE) {
                if (settings.current_frame < (settings.frames-1)) {
                    XtSetArg(argList[0], XtNsensitive,TRUE);
                    XtSetValues( splmenu[0], argList, 1);
                }
            }
            else {
                if (settings.current_frame > (settings.start+1)) {
                    XtSetArg(argList[0], XtNsensitive,TRUE);
                    XtSetValues( splmenu[2], argList, 1);
                    XtSetArg(argList[0], XtNsensitive, TRUE);
                    XtSetValues( splmenu[1], argList, 1);
                }
            }
        }
        else if (framelist[settings.current_frame].smovetype == DOLLYEND) {
            if (smove == FALSE) {
                XtSetArg(argList[0], XtNsensitive, TRUE);
                XtSetValues( cutmenu[1], argList, 1);
            }
        }
        else if (framelist[settings.current_frame].smovetype == SPLINEPOINT) {
            if (sspline == TRUE) {
                XtSetArg(argList[0], XtNsensitive,TRUE);
                XtSetValues( splmenu[3], argList, 1);
            }
        }
    }
    if (framelist[settings.current_frame].zoomtype == NONE) {
        if (zoom == TRUE) {
            XtSetArg(argList[0], XtNsensitive, TRUE);
            XtSetValues( cutmenu[4], argList, 1);
        }
        else {
            XtSetArg(argList[0], XtNsensitive, TRUE);
            XtSetValues( cutmenu[3], argList, 1);
        }

    }
    clear_views();
    settings.camera.x = framelist[settings.current_frame].camera.x;
    settings.camera.y = framelist[settings.current_frame].camera.y;
    settings.camera.z = framelist[settings.current_frame].camera.z;
    settings.slate.x = framelist[settings.current_frame].slate.x;
    settings.slate.y = framelist[settings.current_frame].slate.y;
    settings.slate.z = framelist[settings.current_frame].slate.z;
    settings.zoom    = framelist[settings.current_frame].zoomval;
    show_views();
    set_bars();
    XawScrollbarSetThumb(zoombar,
        (float)(framelist[settings.current_frame].zoomval)/189,-1.0);
    sprintf(displayString,"Zoom: %d",(int)framelist[settings.current_frame].zoomval);
    XtSetArg(argList[0], XtNlabel, displayString);
    XtSetValues( zoommsg, argList, 1);
    sprintf(displayString,"%d",settings.current_frame);
    XtSetArg(argList[0], XtNlabel, displayString);
    XtSetValues( framemsg, argList, 1);
    draw_objects();
    }
}

void move_zoombar(w,client_data,percent_pt)
Widget w;
XtPointer client_data;
XtPointer percent_pt;
{
    Arg argList[MAX_ARGS];
    char displayString[MAX_CHARS_IN_WINDOW];  /* message for user */
    float barval;

    barval = *(float*)percent_pt;
    if (barval>0.95) barval=0.95;
    if (barval<0.0) barval=0.0;
    XawScrollbarSetThumb(zoombar,barval,-1.0);
    settings.zoom = rint(189*barval);

    sprintf(displayString,"Zoom: %d",settings.zoom);
    XtSetArg(argList[0], XtNlabel, displayString);
    XtSetValues( zoommsg, argList, 1);
    draw_objects();
}

void zoombutCallback(widget,client_data,percent_pt)
Widget widget;
XtPointer client_data;
XtPointer percent_pt;
{
    Arg argList[MAX_ARGS];
    char displayString[MAX_CHARS_IN_WINDOW];  /* message for user */

    if (widget == zoom_lbutton) {
        if (--settings.zoom < 0) settings.zoom = 0;
    }
    if (widget == zoom_rbutton) {
        if (++settings.zoom > 180) settings.zoom = 180;
    }
    XawScrollbarSetThumb(zoombar,(float)settings.zoom/189,-1.0);

    sprintf(displayString,"Zoom: %d",settings.zoom);
    XtSetArg(argList[0], XtNlabel, displayString);
    XtSetValues( zoommsg, argList, 1);
    draw_objects();
}
void playCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    int i,j,k,sframe,delay;
    if (settings.frames == 0) return;
    delay = 64000;

    sframe = settings.current_frame;
    for (i=1;i<=settings.frames;i++) {
        clear_views();
        settings.current_frame = i;
        settings.camera.x = framelist[settings.current_frame].camera.x;
        settings.camera.y = framelist[settings.current_frame].camera.y;
        settings.camera.z = framelist[settings.current_frame].camera.z;
        settings.slate.x = framelist[settings.current_frame].slate.x;
        settings.slate.y = framelist[settings.current_frame].slate.y;
        settings.slate.z = framelist[settings.current_frame].slate.z;
        settings.zoom    = framelist[settings.current_frame].zoomval;
        show_views();
        set_bars();
        draw_objects();
        for (j=0;j<delay;j++);
    }
    clear_views();
    settings.current_frame = sframe;
    settings.camera.x = framelist[settings.current_frame].camera.x;
    settings.camera.y = framelist[settings.current_frame].camera.y;
    settings.camera.z = framelist[settings.current_frame].camera.z;
    settings.slate.x = framelist[settings.current_frame].slate.x;
    settings.slate.y = framelist[settings.current_frame].slate.y;
    settings.slate.z = framelist[settings.current_frame].slate.z;
    settings.zoom    = framelist[settings.current_frame].zoomval;
    show_views();
    set_bars();
    draw_objects();
}
