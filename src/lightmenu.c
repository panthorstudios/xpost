#include "main.h"

extern Settings settings;
extern Display *theDisplay;
extern Widget lightmenu[5],popbutton1,popbutton2,sidebar1,views,donebutton;
extern Pixmap leftarrowPattern,rightarrowPattern;
extern int cut,zoom,move;
extern int lightcount[5];
extern void DestroyPopupPrompt();
extern Widget cameraview,popupbox,popuplabel,popuplabel1,camerabutton;
extern Widget controls,slatebutton;
extern Widget windowW,popleftbutton,poprightbutton,popleftbutton1,
    poprightbutton1,popupbar,popupbar1;
extern unsigned long handcursor,lrcursor;
extern struct Light *light_head;
extern struct Light lightelement;
extern void set_bars();
extern void insert_light();

void handlelight();
void destroybox();
void move_ambbar();
void ldoneCallback();
void lightbutCallback();
void lightlistCallback();
int lightnumber;
int lightaction;

void destroybox(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[1];

    if (widget == popbutton2 && settings.light_type == AMBIENT)
        settings.ambient = settings.ambient_temp;
    clear_views();
    settings.moving = CAMERA;
    settings.mode =  CAMERA;
    XtSetArg( argList[0], XtNlabel, "Camera");
    XtSetValues( camerabutton, argList, 1);
    XtSetArg( argList[0], XtNlabel, "Slate");
    XtSetValues( slatebutton, argList, 1);
    show_views();
    XtPopdown(popupbox);
    XFlush(theDisplay);
    lightelement.lx  = 0;
    lightelement.ly  = 0;
    lightelement.lz  = 0;
    lightelement.lfx = 0;
    lightelement.lfy = 0;
    lightelement.lfz = 0;
    lightelement.lightint = 0;
    lightelement.lightcoeff = 0;
}

void move_ambbar(w,client_data,percent_pt)
Widget w;
XtPointer client_data;
XtPointer percent_pt;
{
    char        displayString[MAX_CHARS_IN_WINDOW];
    float barval;
    Arg         argList[1];

    barval = *(float*)percent_pt;
    if (barval>0.95) barval=0.95;
    if (barval<0.0) barval=0.0;
    XawScrollbarSetThumb(w,barval,-1.0);
    if (settings.light_type == AMBIENT) {
        settings.ambient = barval/0.95;
        sprintf( displayString, "Ambient Level: %f",settings.ambient);
    }
    else {
        lightelement.lightint = barval/0.95;
        sprintf( displayString, "Light Intensity: %f",lightelement.lightint);
    }
    XtSetArg(argList[0], XtNlabel, displayString);
    XtSetValues( popuplabel, argList, 1);

}

void move_intbar(w,client_data,percent_pt)
Widget w;
XtPointer client_data;
XtPointer percent_pt;
{
    char displayString[MAX_CHARS_IN_WINDOW];
    float barval;
    Arg argList[1];

    barval = *(float*)percent_pt;
    if (barval>0.95) barval=0.95;
    if (barval<0.0) barval=0.0;
    XawScrollbarSetThumb(w,barval,-1.0);
    lightelement.lightcoeff = barval/0.95;
    sprintf( displayString, "Light Coefficient: %f",lightelement.lightcoeff);
    XtSetArg(argList[0], XtNlabel, displayString);
    XtSetValues( popuplabel1, argList, 1);
}

void lightCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[7];
    int i,n;
    Widget lightparent,lightleft,lightright,lightleft1,
           lightright1;
    char        displayString[MAX_CHARS_IN_WINDOW];
    Position    x, y;
    float       barpos;
    XtTranslations bartable;
    String       trans =
        " <Btn1Down>: StartScroll(Continuous)MoveThumb()NotifyThumb() \n\
          <Btn1Motion>: MoveThumb()NotifyThumb() \n\
          <Btn1Up>:     NotifyScroll(Proportional)EndScroll() ";

    bartable =  XtParseTranslationTable(trans);

    if (widget != lightmenu[0]) {
        clear_views();
        settings.moving = LIGHT;
        settings.mode =  LIGHT;
        XtSetArg( argList[0], XtNlabel, "Light");
        XtSetValues( camerabutton, argList, 1);
        XtSetArg( argList[0], XtNlabel, "Focus");
        XtSetValues( slatebutton, argList, 1);

        if (widget == lightmenu[1]) 
            settings.light_type = DIFFUSE;
        if (widget == lightmenu[2]) 
            settings.light_type = SPOT;
        if (widget == lightmenu[3]) 
            settings.light_type = WARN;

        if (widget == lightmenu[1])
            XtSetArg( argList[0], XtNsensitive,FALSE);
        else
            XtSetArg( argList[0], XtNsensitive,TRUE);
        XtSetValues( slatebutton, argList, 1);
        show_views();
    }
    else settings.light_type = AMBIENT;

    XtTranslateCoords(cameraview, (Position) 50, (Position) 50, &x, &y);

    n = 0;
    XtSetArg(argList[n], XtNx, x);                         n++;
    XtSetArg(argList[n], XtNy, y);                         n++;

    popupbox = XtCreatePopupShell("lightPrompt", transientShellWidgetClass,
                                     cameraview, argList, n);

    lightparent = XtCreateManagedWidget("lightparent", formWidgetClass,
                        popupbox,NULL,ZERO);
    n=0;
    if(widget != lightmenu[0])
        sprintf( displayString, "Light Intensity: %f", lightelement.lightint);
    else
        sprintf( displayString, "Ambient Level: %f",settings.ambient);
    XtSetArg( argList[n], XtNlabel, displayString);  n++;
    XtSetArg( argList[n], XtNborderWidth, 0);  n++;
    popuplabel = XtCreateManagedWidget("lightlabel", labelWidgetClass,
                        lightparent,argList, n);

    n=0;
    XtSetArg( argList[n], XtNfromVert,popuplabel);    n++;
    XtSetArg( argList[n], XtNwidth, 26);    n++;
    XtSetArg( argList[n], XtNheight, 26);    n++;
    XtSetArg(argList[n], XtNbitmap, leftarrowPattern);  n++;
    popleftbutton = XtCreateManagedWidget("light_lbutton", commandWidgetClass,
        lightparent, argList,n);
    XtAddCallback(popleftbutton,XtNcallback,lightbutCallback,NULL);

    n=0;
    XtSetArg( argList[n], XtNfromVert,popuplabel);    n++;
    XtSetArg( argList[n], XtNfromHoriz,popleftbutton);    n++;
    XtSetArg( argList[n], XtNorientation, XtorientHorizontal); n++;
    XtSetArg( argList[n], XtNlength, 105);    n++;
    XtSetArg( argList[n], XtNthickness, 26);    n++;
    XtSetArg( argList[n], XtNscrollHCursor, handcursor);    n++;
    XtSetArg( argList[n], XtNscrollUCursor, lrcursor);    n++;
    XtSetArg( argList[n], XtNtranslations, bartable);    n++;
    popupbar = XtCreateManagedWidget("lightbar", scrollbarWidgetClass,
        lightparent, argList,n);
    XtAddCallback(popupbar,XtNjumpProc,move_ambbar,&barpos);
    if (settings.light_type == AMBIENT)
        XawScrollbarSetThumb(popupbar,(settings.ambient*0.95),-1.0);
    else
        XawScrollbarSetThumb(popupbar,(lightelement.lightint*0.95),-1.0);

    n=0;
    XtSetArg( argList[n], XtNfromVert,popuplabel);    n++;
    XtSetArg( argList[n], XtNfromHoriz,popupbar);    n++;
    XtSetArg( argList[n], XtNwidth, 26);    n++;
    XtSetArg( argList[n], XtNheight, 26);    n++;
    XtSetArg(argList[n], XtNbitmap, rightarrowPattern);  n++;
    poprightbutton = XtCreateManagedWidget("light_lbutton", commandWidgetClass,
        lightparent, argList,n);
    XtAddCallback(poprightbutton,XtNcallback,lightbutCallback,NULL);

    if (settings.light_type == SPOT || settings.light_type == WARN) {
        n=0;
        sprintf( displayString, "Light Coefficient : %f", lightelement.lightcoeff);
        XtSetArg( argList[n], XtNlabel, displayString);  n++;
        XtSetArg( argList[n], XtNfromVert,popleftbutton);    n++;
        XtSetArg( argList[n], XtNborderWidth, 0);  n++;
        popuplabel1 = XtCreateManagedWidget("lightlabel", labelWidgetClass,
                        lightparent,argList, n);

        n=0;
        XtSetArg( argList[n], XtNfromVert,popuplabel1);    n++;
        XtSetArg( argList[n], XtNwidth, 26);    n++;
        XtSetArg( argList[n], XtNheight, 26);    n++;
        XtSetArg(argList[n], XtNbitmap, leftarrowPattern);  n++;
        popleftbutton1 = XtCreateManagedWidget("light_lbutton1", commandWidgetClass,
        lightparent, argList,n);
        XtAddCallback(popleftbutton1,XtNcallback,lightbutCallback,NULL);

        n=0;
        XtSetArg( argList[n], XtNfromVert,popuplabel1);    n++;
        XtSetArg( argList[n], XtNfromHoriz,popleftbutton1);    n++;
        XtSetArg( argList[n], XtNorientation, XtorientHorizontal); n++;
        XtSetArg( argList[n], XtNlength, 105);    n++;
        XtSetArg( argList[n], XtNthickness, 26);    n++;
        XtSetArg( argList[n], XtNscrollHCursor, handcursor);    n++;
        XtSetArg( argList[n], XtNscrollUCursor, lrcursor);    n++;
        XtSetArg( argList[n], XtNtranslations, bartable);    n++;
        popupbar1 = XtCreateManagedWidget("lightbar1", scrollbarWidgetClass,
            lightparent, argList,n);
        XtAddCallback(popupbar1,XtNjumpProc,move_intbar,&barpos);
        XawScrollbarSetThumb(popupbar1,(lightelement.lightcoeff*0.95),-1.0);

        n=0;
        XtSetArg( argList[n], XtNfromVert,popuplabel1);    n++;
        XtSetArg( argList[n], XtNfromHoriz,popupbar1);    n++;
        XtSetArg( argList[n], XtNwidth, 26);    n++;
        XtSetArg( argList[n], XtNheight, 26);    n++;
        XtSetArg(argList[n], XtNbitmap, rightarrowPattern);  n++;
        poprightbutton1 = XtCreateManagedWidget("light_rbutton1",
            commandWidgetClass,lightparent, argList,n);
        XtAddCallback(poprightbutton1,XtNcallback,lightbutCallback,NULL);
    }

    n=0;
    XtSetArg( argList[n], XtNlabel, "Done");  n++;
    XtSetArg( argList[n], XtNwidth, 55);  n++;
    if (settings.light_type == SPOT || settings.light_type == WARN) {
        XtSetArg( argList[n], XtNfromVert, popupbar1);  n++;
    }
    else {
        XtSetArg( argList[n], XtNfromVert, popupbar);  n++;
    }
    popbutton1 = XtCreateManagedWidget("lmovedone", commandWidgetClass,
                        lightparent,argList, n);
    XtAddCallback(popbutton1,XtNcallback,ldoneCallback,NULL);

    n = 0;
    XtSetArg( argList[n], XtNfromHoriz, popbutton1);    n++;
    if (settings.light_type == SPOT || settings.light_type == WARN) {
        XtSetArg( argList[n], XtNfromVert, popupbar1);    n++;
    }
    else {
        XtSetArg( argList[n], XtNfromVert, popupbar);    n++;
    }
    XtSetArg( argList[n], XtNlabel, "Cancel");  n++;
    popbutton2 = XtCreateManagedWidget("ambbut2", commandWidgetClass,
                   lightparent, argList, n);
    XtAddCallback(popbutton2,XtNcallback,destroybox,NULL);

    XtPopup(popupbox,XtGrabExclusive);
    if (widget != lightmenu[0]) {
        XtAddGrab(views,FALSE,FALSE);
        XtAddGrab(controls,FALSE,FALSE);
    }
}

void ldoneCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[7];
    int i,n;
    struct Light *lptr;

    clear_views();
    settings.moving = CAMERA;
    settings.mode =  CAMERA;
    XtSetArg( argList[0], XtNlabel, "Camera");
    XtSetValues( camerabutton, argList, 1);
    XtSetArg( argList[0], XtNlabel, "Slate");
    XtSetValues( slatebutton, argList, 1);
    XtPopdown(popupbox);
    show_views();

    XtSetArg( argList[0], XtNsensitive,TRUE);
    XtSetValues( slatebutton, argList, 1);
    lptr = (struct Light *)malloc(sizeof(struct Light));
    lightcount[settings.light_type]++;

    lptr->lighttype = settings.light_type;
    lptr->lightid   = lightcount[settings.light_type];
    lptr->lightint = lightelement.lightint;
    lptr->lightcoeff = lightelement.lightcoeff;
    lptr->lx = lightelement.lx;
    lptr->ly = lightelement.ly;
    lptr->lz = lightelement.lz;
    lptr->lfx = lightelement.lfx;
    lptr->lfy = lightelement.lfy;
    lptr->lfz = lightelement.lfz;

    settings.light_ptr->next = lptr;
    lptr->next = NULL;
    settings.light_ptr = settings.light_ptr->next;

    lightelement.lx  = 0;
    lightelement.ly  = 0;
    lightelement.lz  = 0;
    lightelement.lfx = 0;
    lightelement.lfy = 0;
    lightelement.lfz = 0;
    lightelement.lightint = 0;
    lightelement.lightcoeff = 0;
}

void lightbutCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg         argList[MAX_ARGS];
    char        displayString[MAX_CHARS_IN_WINDOW];

    if (widget == popleftbutton)
        if (settings.light_type == AMBIENT){
            if ((settings.ambient -= (1.0/105)) < 0)
                settings.ambient = 0;
        }
        else {
            if ((lightelement.lightint -= (1.0/105)) < 0)
                lightelement.lightint = 0;
        }
    if (widget == poprightbutton)
        if (settings.light_type == AMBIENT){
            if ((settings.ambient += (1.0/105)) > 1)
                settings.ambient = 1;
        }
        else {
            if ((lightelement.lightint += (1.0/105)) > 1)
                lightelement.lightint = 1;
        }
    if (widget == popleftbutton1)
        if ((lightelement.lightcoeff -= (1.0/105)) < 0)
            lightelement.lightcoeff = 0;
    if (widget == poprightbutton1)
        if ((lightelement.lightcoeff += (1.0/105)) > 1)
            lightelement.lightcoeff = 1;

    if (settings.light_type == AMBIENT) {
        sprintf( displayString, "Ambient Level: %f",settings.ambient);
        XawScrollbarSetThumb(popupbar,settings.ambient*0.95,-1.0);
    }
    else {
        sprintf( displayString, "Light Intensity: %f",lightelement.lightint);
        XawScrollbarSetThumb(popupbar,lightelement.lightint*0.95,-1.0);
    }
    XtSetArg(argList[0], XtNlabel, displayString);
    XtSetValues( popuplabel, argList, 1);
    if (settings.light_type == SPOT || settings.light_type == WARN) {
        sprintf( displayString, "Light Coefficient: %f",lightelement.lightcoeff);
        XtSetArg(argList[0], XtNlabel, displayString);
        XtSetValues( popuplabel1, argList, 1);
        XawScrollbarSetThumb(popupbar1,lightelement.lightcoeff*0.95,-1.0);
    }
}

void lightadjustCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg         argList[MAX_ARGS];
    Widget      lightparent,lightlabel,lightbutton1,lightbutton2,lightlist;
    Position    x, y;
    Dimension   width, height;
    Cardinal    n;
    char        displayString[MAX_CHARS_IN_WINDOW];
    char        *lightnames[10];
    int         numlights,i,j,lightcntr;

    numlights = 0;
    for (i=0;i<3;i++)
        numlights += lightcount[i];

    if (numlights == 0) return;

    lightcntr = 0;
    if (widget == lightmenu[4]) lightaction = ADJUST;
    else lightaction = DELETE;

    for (i=0;i<3;i++) {
        for (j=0;j<lightcount[i];j++) {
            if (i==0) sprintf(displayString,"Diffuse light #%d",j+1);
            if (i==1) sprintf(displayString,"Spot light #%d",j+1);
            if (i==2) sprintf(displayString,"Warn light #%d",j+1);
            if ((lightnames[lightcntr] =
                (char *)malloc(strlen(displayString)+1)) != NULL) {
                strcpy(lightnames[lightcntr++],displayString);
            }
            else {
                printf("malloc error in lights\n");
                return;
            }
        }
    } 
    XtTranslateCoords(cameraview, (Position) 10, (Position) 10, &x, &y);

    n = 0;
    XtSetArg(argList[n], XtNx, x);                         n++;
    XtSetArg(argList[n], XtNy, y);                         n++;

    popupbox = XtCreatePopupShell("adjustlightPrompt",transientShellWidgetClass,
                                     cameraview, argList, n);

    lightparent = XtCreateManagedWidget("lightparent", formWidgetClass,
                        popupbox,NULL,ZERO);

    if (lightaction == DELETE)
        strcpy(displayString,"Select light to remove:");
    else
        strcpy(displayString,"Select light to adjust:");
    n = 0;
    XtSetArg( argList[n], XtNlabel, displayString); n++;
    XtSetArg( argList[n], XtNborderWidth, 0);                   n++;
    lightlabel = XtCreateManagedWidget("lightlabel", labelWidgetClass,
        lightparent, argList,n);

    n = 0;
    XtSetArg(argList[n], XtNfromHoriz, lightlabel );              n++;
    XtSetArg(argList[n], XtNlabel, "Done"); n++;
    lightbutton1 = XtCreateManagedWidget("lightbutton1", commandWidgetClass,
        lightparent, argList,n);
    XtAddCallback(lightbutton1,XtNcallback,handlelight,NULL);

    n = 0;
    XtSetArg(argList[n], XtNfromHoriz, lightbutton1 );              n++;
    XtSetArg(argList[n], XtNlabel, "Cancel"); n++;
    lightbutton2 = XtCreateManagedWidget("lightbutton2", commandWidgetClass,
        lightparent, argList,n);
    XtAddCallback(lightbutton2,XtNcallback,destroybox,NULL);

    n = 0;
    XtSetArg(argList[n], XtNfromVert, lightlabel );              n++;
    XtSetArg(argList[n], XtNdefaultColumns,1);        n++;
    XtSetArg(argList[n], XtNnumberStrings, numlights); n++;
    XtSetArg(argList[n], XtNlist, lightnames); n++;
    lightlist = XtCreateManagedWidget("lightlist", listWidgetClass,
        lightparent, argList,n);
    XtAddCallback(lightlist,XtNcallback,lightlistCallback,NULL);
    XawListHighlight(lightlist,0);

    XtPopup(popupbox, XtGrabExclusive);
}

void lightlistCallback(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    XawListReturnStruct *item = (XawListReturnStruct*)call_data;
    lightnumber = item->list_index;
}

void handlelight(widget,client_data,call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Arg argList[1];
    int i,sum,ltype,lid;
    struct Light *pntr,*pntr1,*tmp;
     
    sum = 0;
    for (i=0;i<3;i++) {
        sum += lightcount[i];
        ltype = i + DIFFUSE;
        if (lightnumber < sum) {
            lid = 1 + lightcount[i] - (sum - lightnumber);
            break;
        }
    }
    lightcount[ltype] -= 1;
    pntr = settings.light_head->next;
    pntr1 = settings.light_head;
    while (pntr->lighttype != ltype || pntr->lightid != lid) {
        printf("looping\n");
        if (pntr->lighttype == ltype)
            pntr->lightid -= 1;
        pntr1 = pntr;
        pntr = pntr->next;
    }
    lightelement.lx  = pntr->lx;
    lightelement.ly  = pntr->ly;
    lightelement.lz  = pntr->lz;
    lightelement.lfx = pntr->lfx;
    lightelement.lfy = pntr->lfy;
    lightelement.lfz = pntr->lfz;
    lightelement.lightint = pntr->lightint;
    lightelement.lightcoeff = pntr->lightcoeff;
    if (settings.light_ptr == pntr)
        settings.light_ptr = pntr1;
    pntr1->next = pntr->next;
    free(pntr);

    if (lightaction == DELETE) {
        clear_views();
        settings.moving = CAMERA;
        settings.mode =  CAMERA;
        XtSetArg( argList[0], XtNlabel, "Camera");
        XtSetValues( camerabutton, argList, 1);
        XtSetArg( argList[0], XtNlabel, "Slate");
        XtSetValues( slatebutton, argList, 1);
        show_views();
        set_bars();
        XtPopdown(popupbox);
        XFlush(theDisplay);
    }
    else {
        XtPopdown(popupbox);
        XFlush(theDisplay);
        lightCallback(lightmenu[ltype+1],(XtPointer)NULL,(XtPointer)NULL);
    }

}
