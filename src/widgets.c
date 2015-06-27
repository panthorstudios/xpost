#include "main.h"
extern Display  *theDisplay;
extern Settings settings;
Widget topLevel,windowW,commands,views;
Widget cameraview, frontview,topview,sideview;
Widget diskMenuButton,cameraMenuButton,splcameraMenuButton,lightMenuButton,
       colorMenuButton,spliceMenuButton,mergeMenuButton;
Widget left10arrow1,right10arrow1,up10arrow1,down10arrow1,leftarrow1,
       rightarrow1,uparrow1,downarrow1,leftarrow2,left10arrow2,
       rightarrow2,right10arrow2,uparrow2,up10arrow2,downarrow2,
       down10arrow2,leftarrow3,left10arrow3,rightarrow3,right10arrow3,
       uparrow3,up10arrow3,downarrow3,down10arrow3;
Widget mainmsg[3],frontmsg,topmsg,sidemsg,statmsg,framemsg,frametitlemsg;
Widget maintitlemsg,slatebutton,camerabutton,plusbutton,minusbutton;
Widget frontbar1,frontbar2,topbar1,topbar2,sidebar1,sidebar2,zoombar;
Widget frame_lbutton,frame_rbutton,frame_l10button,frame_r10button;
Widget diskmenu[4],cutmenu[8],colormenu[2],lightmenu[5];
Widget splmenu[5],mergemenu[3],zoom_lbutton,zoom_rbutton,controls;
Widget popbutton1,popbutton2,popuplabel,zoommsg,mainmsgbox;
Widget popupbox,popleftbutton,poprightbutton,popleftbutton1,poprightbutton1,
       donebutton,popuplabel1,popupbar,popupbar1,box1,playbutton;
unsigned long lrcursor,udcursor,handcursor;

extern unsigned long  LightBluePen,GrayPen,WhitePen,BlackPen,YellowPen;
extern float barpos;
extern void moveCallback();
extern void buttonCallback();
extern void change_bars();
extern void scaleCallback();
extern void frameCallback();
extern void zoomCallback();
extern void move_zoombar();
extern void zoombutCallback();
extern void draw_objects();
extern void playCallback();
void createWidgets()
{

    Arg          argList[MAX_ARGS];    /* resource list   */
    int          n;                    /* argument count  */
    char         displayString[MAX_CHARS_IN_WINDOW];  /* message for user */
    XtTranslations bartable,toggletable,cmdtable;
    String       trans =
        " <Btn1Down>: StartScroll(Continuous)MoveThumb()NotifyThumb() \n\
          <Btn1Motion>: MoveThumb()NotifyThumb() \n\
          <Btn1Up>:     NotifyScroll(Proportional)EndScroll()";

    bartable =  XtParseTranslationTable(trans);

    trans =
        "<EnterWindow>: highlight(Always) \n\
         <LeaveWindow>:unhighlight() \n\
         <Btn1Down>,<Btn1Up>: set()notify() ";
    toggletable =  XtParseTranslationTable(trans);

    handcursor = XCreateFontCursor(theDisplay,XC_hand2);
    lrcursor = XCreateFontCursor(theDisplay,XC_sb_h_double_arrow);
    udcursor = XCreateFontCursor(theDisplay,XC_sb_v_double_arrow);

    n=0;
    XtSetArg(argList[n], XtNbackground, LightBluePen); n++;
    windowW = XtCreateManagedWidget( "Big Window", formWidgetClass,
        topLevel,argList,n);

    n=0;
    XtSetArg(argList[n], XtNbackground, GrayPen); n++;
    views = XtCreateManagedWidget( "views_window", formWidgetClass,
      windowW,argList,n);

    n = 0;
    sprintf( displayString, 
      "X: %d  Y: %d  Z: %d",settings.camera.x,settings.camera.y,
          settings.camera.z);
    XtSetArg( argList[n], XtNlabel, displayString);  n++;
    XtSetArg( argList[n], XtNwidth, 180);            n++;
    statmsg = XtCreateManagedWidget("statusmessage", labelWidgetClass,
                   views, argList, n);

    n=0;
    XtSetArg( argList[n], XtNfromVert, statmsg);    n++;
    XtSetArg( argList[n], XtNheight, 180);          n++;
    XtSetArg( argList[n], XtNwidth, 180);           n++;
    XtSetArg( argList[n], XtNbackground, GrayPen); n++;
    frontview = XtCreateManagedWidget("front_view", formWidgetClass, views,
           argList, n);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, frontview);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    leftarrow1 = XtCreateManagedWidget("la1", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(leftarrow1,XtNcallback,moveCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, frontview);    n++;
    XtSetArg( argList[n], XtNfromHoriz, leftarrow1);    n++;
    XtSetArg( argList[n], XtNorientation, XtorientHorizontal); n++;
    XtSetArg( argList[n], XtNscrollHCursor, handcursor);    n++;
    XtSetArg( argList[n], XtNscrollUCursor, lrcursor);    n++;
    XtSetArg( argList[n], XtNlength, 115);    n++;
    XtSetArg( argList[n], XtNthickness, 26);    n++;
    XtSetArg( argList[n], XtNtranslations, bartable);    n++;
    frontbar1 = XtCreateManagedWidget("front_bar1", scrollbarWidgetClass,
        views, argList,n);
    XtAddCallback(frontbar1,XtNjumpProc,change_bars,&barpos);
    XawScrollbarSetThumb(frontbar1,0.5,-1.0);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, frontview);    n++;
    XtSetArg( argList[n], XtNfromHoriz, frontbar1);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    rightarrow1 = XtCreateManagedWidget("ra1", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(rightarrow1,XtNcallback,moveCallback,NULL);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, statmsg );    n++;
    XtSetArg( argList[n], XtNfromHoriz, frontview );    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;

    XtSetArg( argList[n], XtNheight, 26);            n++;
    uparrow1 = XtCreateManagedWidget("ua1", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(uparrow1,XtNcallback,moveCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, uparrow1);    n++;
    XtSetArg( argList[n], XtNfromHoriz, frontview);    n++;
    XtSetArg( argList[n], XtNorientation, XtorientVertical); n++;
    XtSetArg( argList[n], XtNlength, 115);    n++;
    XtSetArg( argList[n], XtNthickness, 26);    n++;
    XtSetArg( argList[n], XtNscrollVCursor, handcursor);    n++;
    XtSetArg( argList[n], XtNscrollRCursor, udcursor);    n++;
    XtSetArg( argList[n], XtNtranslations, bartable);    n++;
    frontbar2 = XtCreateManagedWidget("front_bar2", scrollbarWidgetClass,
        views, argList,n);
    XtAddCallback(frontbar2,XtNjumpProc,change_bars,&barpos);
    XawScrollbarSetThumb(frontbar2,0.5,-1.0);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, frontbar2);    n++;
    XtSetArg( argList[n], XtNfromHoriz, frontview);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    downarrow1 = XtCreateManagedWidget("da1", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(downarrow1,XtNcallback,moveCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, frontbar1);    n++;
    XtSetArg( argList[n], XtNheight, 180);         n++;
    XtSetArg( argList[n], XtNwidth, 180);          n++;
    XtSetArg( argList[n], XtNbackground, GrayPen); n++;
    topview = XtCreateManagedWidget("top_view", formWidgetClass, views,
       argList, n);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, topview);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    leftarrow2 = XtCreateManagedWidget("la2", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(leftarrow2,XtNcallback,moveCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, topview);    n++;
    XtSetArg( argList[n], XtNfromHoriz, leftarrow2);    n++;
    XtSetArg( argList[n], XtNorientation, XtorientHorizontal); n++;
    XtSetArg( argList[n], XtNscrollHCursor, handcursor);    n++;
    XtSetArg( argList[n], XtNscrollUCursor, lrcursor);    n++;
    XtSetArg( argList[n], XtNlength, 115);    n++;
    XtSetArg( argList[n], XtNthickness, 26);    n++;
    XtSetArg( argList[n], XtNtranslations, bartable);    n++;
    topbar1 = XtCreateManagedWidget("top_bar1", scrollbarWidgetClass,
        views, argList,n);
    XtAddCallback(topbar1,XtNjumpProc,change_bars,&barpos);
    XawScrollbarSetThumb(topbar1,0.5,-1.0);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, topview);    n++;
    XtSetArg( argList[n], XtNfromHoriz, topbar1);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    rightarrow2 = XtCreateManagedWidget("ra2", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(rightarrow2,XtNcallback,moveCallback,NULL);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, frontbar1 );    n++;
    XtSetArg( argList[n], XtNfromHoriz, topview );    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    uparrow2 = XtCreateManagedWidget("ua2", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(uparrow2,XtNcallback,moveCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, uparrow2);    n++;
    XtSetArg( argList[n], XtNfromHoriz, topview);    n++;
    XtSetArg( argList[n], XtNorientation, XtorientVertical); n++;
    XtSetArg( argList[n], XtNlength, 115);    n++;
    XtSetArg( argList[n], XtNthickness, 26);    n++;
    XtSetArg( argList[n], XtNscrollVCursor, handcursor);    n++;
    XtSetArg( argList[n], XtNscrollRCursor, udcursor);    n++;
    XtSetArg( argList[n], XtNtranslations, bartable);    n++;
    topbar2 = XtCreateManagedWidget("top_bar2", scrollbarWidgetClass,
        views, argList,n);
    XtAddCallback(topbar2,XtNjumpProc,change_bars,&barpos);
    XawScrollbarSetThumb(topbar2,0.5,-1.0);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, topbar2);    n++;
    XtSetArg( argList[n], XtNfromHoriz, topview);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    downarrow2 = XtCreateManagedWidget("da2", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(downarrow2,XtNcallback,moveCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, topbar1);    n++;
    XtSetArg( argList[n], XtNheight, 180);          n++;
    XtSetArg( argList[n], XtNwidth, 180);           n++;
    XtSetArg( argList[n], XtNbackground, GrayPen); n++;
    sideview = XtCreateManagedWidget("side_view", formWidgetClass, views,
         argList, n);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, sideview);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    leftarrow3 = XtCreateManagedWidget("la3", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(leftarrow3,XtNcallback,moveCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, sideview);    n++;
    XtSetArg( argList[n], XtNfromHoriz, leftarrow3);    n++;
    XtSetArg( argList[n], XtNorientation, XtorientHorizontal); n++;
    XtSetArg( argList[n], XtNscrollHCursor, handcursor);    n++;
    XtSetArg( argList[n], XtNscrollUCursor, lrcursor);    n++;
    XtSetArg( argList[n], XtNlength, 115);    n++;
    XtSetArg( argList[n], XtNthickness, 26);    n++;
    XtSetArg( argList[n], XtNtranslations, bartable);    n++;
    sidebar1 = XtCreateManagedWidget("side_bar1", scrollbarWidgetClass,
        views, argList,n);
    XtAddCallback(sidebar1,XtNjumpProc,change_bars,&barpos);
    XawScrollbarSetThumb(sidebar1,0.5,-1.0);
    
    n = 0;
    XtSetArg( argList[n], XtNfromVert, sideview);    n++;
    XtSetArg( argList[n], XtNfromHoriz, sidebar1);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    rightarrow3 = XtCreateManagedWidget("ra3", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(rightarrow3,XtNcallback,moveCallback,NULL);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, topbar1 );    n++;
    XtSetArg( argList[n], XtNfromHoriz, sideview );    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    uparrow3 = XtCreateManagedWidget("ua3", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(uparrow3,XtNcallback,moveCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, uparrow3);    n++;
    XtSetArg( argList[n], XtNfromHoriz, sideview);    n++;
    XtSetArg( argList[n], XtNorientation, XtorientVertical); n++;
    XtSetArg( argList[n], XtNlength, 115);    n++;
    XtSetArg( argList[n], XtNthickness, 26);    n++;
    XtSetArg( argList[n], XtNscrollVCursor, handcursor);    n++;
    XtSetArg( argList[n], XtNscrollRCursor, udcursor);    n++;
    XtSetArg( argList[n], XtNtranslations, bartable);    n++;
    sidebar2 = XtCreateManagedWidget("side_bar2", scrollbarWidgetClass,
        views, argList,n);
    XtAddCallback(sidebar2,XtNjumpProc,change_bars,&barpos);
    XawScrollbarSetThumb(sidebar2,0.5,-1.0);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, sidebar2);    n++;
    XtSetArg( argList[n], XtNfromHoriz, sideview);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    downarrow3 = XtCreateManagedWidget("da3", commandWidgetClass,
                   views, argList, n);
    XtAddCallback(downarrow3,XtNcallback,moveCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNborderWidth, 0); n++;
    XtSetArg( argList[n], XtNfromHoriz,views);    n++;
    commands = XtCreateManagedWidget( "commands", formWidgetClass, windowW,
                                   argList,n);

    n=0;
    box1 = XtCreateManagedWidget( "box1", formWidgetClass, commands,
                                   argList,n);

    n = 0;
    XtSetArg( argList[n], XtNwidth, 175);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    XtSetArg( argList[n], XtNborderWidth, 0);            n++;
    XtSetArg( argList[n], XtNlabel, "Frame");  n++;
    frametitlemsg = XtCreateManagedWidget("frametitlemessage", labelWidgetClass,
                   box1, argList, n);

    n=0;
    XtSetArg( argList[n], XtNfromVert, frametitlemsg);    n++;
    XtSetArg( argList[n], XtNfromHoriz, views);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    frame_lbutton = XtCreateManagedWidget("fr_lb", commandWidgetClass, box1,
              argList, n);
    XtAddCallback(frame_lbutton,XtNcallback,frameCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, frametitlemsg);    n++;
    XtSetArg( argList[n], XtNfromHoriz, frame_lbutton);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    frame_l10button = XtCreateManagedWidget("fr_l10b", commandWidgetClass,
        box1, argList, n);
    XtAddCallback(frame_l10button,XtNcallback,frameCallback,NULL);

    n = 0;
    XtSetArg( argList[n], XtNfromVert, frametitlemsg);    n++;
    sprintf( displayString, "%d",
      settings.frames);
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    XtSetArg( argList[n], XtNlabel, displayString);  n++;
    XtSetArg( argList[n], XtNfromHoriz, frame_l10button);    n++;
    framemsg = XtCreateManagedWidget("framemessage", labelWidgetClass,
                   box1, argList, n);

    n=0;
    XtSetArg( argList[n], XtNfromVert, frametitlemsg);    n++;
    XtSetArg( argList[n], XtNfromHoriz, framemsg);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    frame_r10button = XtCreateManagedWidget("fr_r10b", commandWidgetClass,
        box1, argList, n);
    XtAddCallback(frame_r10button,XtNcallback,frameCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, frametitlemsg);    n++;
    XtSetArg( argList[n], XtNfromHoriz, frame_r10button);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    frame_rbutton = XtCreateManagedWidget("fr_rb", commandWidgetClass, box1,            argList, n);
    XtAddCallback(frame_rbutton,XtNcallback,frameCallback,NULL);

/*    n=0;
    XtSetArg( argList[n], XtNfromVert, frametitlemsg);    n++;
    XtSetArg( argList[n], XtNfromHoriz, frame_rbutton);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    playbutton = XtCreateManagedWidget("playb", commandWidgetClass,
        box1, argList, n);
    XtAddCallback(playbutton,XtNcallback,playCallback,NULL);
*/
    n = 0;
    XtSetArg( argList[n], XtNwidth, 175);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    XtSetArg( argList[n], XtNlabel, "Zoom: 30");  n++;
    XtSetArg( argList[n], XtNfromVert, framemsg);    n++;
    XtSetArg( argList[n], XtNborderWidth, 0);            n++;
    zoommsg = XtCreateManagedWidget("zoommessage", labelWidgetClass,
                   box1, argList, n);

    n=0;
    XtSetArg( argList[n], XtNfromVert, zoommsg);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    zoom_lbutton = XtCreateManagedWidget("zoom_lb", commandWidgetClass,
            box1, argList, n);
    XtAddCallback(zoom_lbutton,XtNcallback,zoombutCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, zoommsg);    n++;
    XtSetArg( argList[n], XtNfromHoriz, zoom_lbutton);    n++;
    XtSetArg( argList[n], XtNorientation, XtorientHorizontal); n++;
    XtSetArg( argList[n], XtNlength, 108);    n++;
    XtSetArg( argList[n], XtNthickness, 26);    n++;
    XtSetArg( argList[n], XtNscrollVCursor, handcursor);    n++;
    XtSetArg( argList[n], XtNscrollRCursor, udcursor);    n++;
    XtSetArg( argList[n], XtNtranslations, bartable);    n++;
    zoombar = XtCreateManagedWidget("zoombar", scrollbarWidgetClass,
        box1, argList,n);
    XtAddCallback(zoombar,XtNjumpProc,move_zoombar,&barpos);
    XawScrollbarSetThumb(zoombar,(30.0/189),-1.0);

    n=0;
    XtSetArg( argList[n], XtNfromVert, zoommsg);    n++;
    XtSetArg( argList[n], XtNfromHoriz, zoombar);    n++;
    XtSetArg( argList[n], XtNwidth, 26);            n++;
    XtSetArg( argList[n], XtNheight, 26);            n++;
    zoom_rbutton = XtCreateManagedWidget("zoom_rb", commandWidgetClass,
            box1, argList, n);
    XtAddCallback(zoom_rbutton,XtNcallback,zoombutCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert, zoombar);    n++;
    XtSetArg( argList[n], XtNborderWidth, 0);            n++;
    controls = XtCreateManagedWidget("controls", formWidgetClass,
            box1, argList, n);

    n = 0;
    sprintf( displayString, "%s",
      "Camera");
    XtSetArg( argList[n], XtNlabel, displayString);  n++;
    XtSetArg( argList[n], XtNstate, True);    n++;
    XtSetArg( argList[n], XtNwidth, 55);            n++;
    XtSetArg( argList[n], XtNtranslations, toggletable);    n++;
    camerabutton = XtCreateManagedWidget("slbutt", toggleWidgetClass,
                   controls, argList, n);
    XtAddCallback(camerabutton,XtNcallback,buttonCallback,NULL);

    n = 0;
    sprintf( displayString, "%s",
      "Slate");
    XtSetArg( argList[n], XtNlabel, displayString);  n++;
    XtSetArg( argList[n], XtNfromHoriz, camerabutton);    n++;
    XtSetArg( argList[n], XtNradioGroup, camerabutton);    n++;
    XtSetArg( argList[n], XtNwidth, 55);            n++;
    XtSetArg( argList[n], XtNtranslations, toggletable);    n++;
    slatebutton = XtCreateManagedWidget("slbutt", toggleWidgetClass,
                   controls, argList, n);
    XtAddCallback(slatebutton,XtNcallback,buttonCallback,NULL);

    n=0;
    sprintf( displayString, "%s",
      "+");
    XtSetArg( argList[n], XtNlabel, displayString);  n++;
    XtSetArg( argList[n], XtNfromHoriz, slatebutton);    n++;
    plusbutton = XtCreateManagedWidget("fr_rb", commandWidgetClass, controls,
      argList, n);
    XtAddCallback(plusbutton,XtNcallback,scaleCallback,NULL);

    n=0;
    sprintf( displayString, "%s",
      "-");
    XtSetArg( argList[n], XtNlabel, displayString);  n++;
    XtSetArg( argList[n], XtNfromHoriz, plusbutton);    n++;
    minusbutton = XtCreateManagedWidget("fr_rb", commandWidgetClass, controls,
      argList, n);
    XtAddCallback(minusbutton,XtNcallback,scaleCallback,NULL);

    n = 0;
    sprintf( displayString, "%s",
      "XPOST version 1.0 by Patrick Joiner");
    XtSetArg( argList[n], XtNlabel, displayString);  n++;
    XtSetArg( argList[n], XtNfromHoriz, box1);    n++;
    XtSetArg( argList[n], XtNwidth, 395);            n++;
    maintitlemsg = XtCreateManagedWidget("maintitlemessage", labelWidgetClass,
                   commands, argList, n);

    n = 0;
    XtSetArg( argList[n], XtNlabel, "");  n++;
    XtSetArg( argList[n], XtNfromVert, maintitlemsg);    n++;
    XtSetArg( argList[n], XtNfromHoriz, box1);    n++;
    XtSetArg( argList[n], XtNjustify, XtJustifyLeft);    n++;
    XtSetArg( argList[n], XtNheight, 60);            n++;
    XtSetArg( argList[n], XtNwidth, 385);            n++;
    mainmsgbox = XtCreateManagedWidget("message", formWidgetClass,
                   commands, argList, n);

    n = 0;
    XtSetArg( argList[n], XtNlabel, ">");  n++;
    XtSetArg( argList[n], XtNjustify, XtJustifyLeft);    n++;
    XtSetArg( argList[n], XtNheight, 13);            n++;
    XtSetArg( argList[n], XtNwidth, 385);            n++;
    XtSetArg( argList[n], XtNborderWidth, 0);            n++;
    mainmsg[0] = XtCreateManagedWidget("message", labelWidgetClass,
                   mainmsgbox, argList, n);

    n = 0;
    XtSetArg( argList[n], XtNlabel, ">");  n++;
    XtSetArg( argList[n], XtNfromVert, mainmsg[0]);    n++;
    XtSetArg( argList[n], XtNjustify, XtJustifyLeft);    n++;
    XtSetArg( argList[n], XtNheight, 13);            n++;
    XtSetArg( argList[n], XtNwidth, 385);            n++;
    XtSetArg( argList[n], XtNborderWidth, 0);            n++;
    mainmsg[1] = XtCreateManagedWidget("message2", labelWidgetClass,
                   mainmsgbox, argList, n);

    n = 0;
    XtSetArg( argList[n], XtNlabel, ">");  n++;
    XtSetArg( argList[n], XtNfromVert, mainmsg[1]);    n++;
    XtSetArg( argList[n], XtNjustify, XtJustifyLeft);    n++;
    XtSetArg( argList[n], XtNheight, 13);            n++;
    XtSetArg( argList[n], XtNwidth, 385);            n++;
    XtSetArg( argList[n], XtNborderWidth, 0);            n++;
    mainmsg[2] = XtCreateManagedWidget("message3", labelWidgetClass,
                   mainmsgbox, argList, n);

    n=0;
    XtSetArg( argList[n], XtNfromVert, commands);    n++;
    XtSetArg( argList[n], XtNfromHoriz, views);    n++;
    XtSetArg( argList[n], XtNheight, 507);          n++;
    XtSetArg( argList[n], XtNwidth, 590 );           n++;
    XtSetArg( argList[n], XtNbackground, BlackPen); n++;
    cameraview = XtCreateManagedWidget("camera_view", formWidgetClass, windowW,               argList, n);

    createDiskMenu();
    createCameraMenu();
    createsplCameraMenu();
    createLightMenu();
    createMergeMenu();
}

