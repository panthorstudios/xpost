#include "main.h"
extern void mergeCallback();

extern Widget cameraview, commands, diskMenuButton, mergeMenuButton,
       cameraMenuButton,splcameraMenuButton,zoom_rbutton;
extern Widget lightMenuButton,spliceMenuButton,colorMenuButton,topLevel;
extern Widget diskmenu[4],cutmenu[6],colormenu[2],lightmenu[5],splmenu[5];
extern Widget mergemenu[3],mainmsg,box1;
extern XtAppContext appCon;
extern Pixmap diskMenuButtonPattern;
extern void loadReturn(),saveReturn(),fileNameReturn(),runTimeReturn(),
       changeFacReturn(),quitPopupPrompt(),saveChoice(),filePopupPrompt(),
       newChoice(),nullReturn();
extern XtActionsRec actionTable;
extern show_views();
extern cutCallback();
extern dollyCallback();
extern zoomCallback();
/* extern colorCallback(); */
extern lightCallback();
extern lightadjustCallback();
extern void splineCallback();
extern void draw_objects();
extern Settings settings;
extern Widget mainmsgbox;

void mergeCallback();

   /* For dialog boxes -- handles return key */
   XtActionsRec actionTableFileMenu[] =
   {
       {"loadReturn",           loadReturn},
       {"saveReturn",           saveReturn},
       {"nullReturn",           nullReturn},
       {"draw_objects",         draw_objects}
   };


void createDiskMenu() {
    Arg          argList[MAX_ARGS];    /* resource list   */
    Widget       firstLevelMenu,
                 entry;
    int          i,n;                  /* counters */

    char * menu_item_names[] = {
        "Load Animation","Save Animation", "Quit"};

    XtAppAddActions(appCon, actionTableFileMenu,
                    XtNumber(actionTableFileMenu));

    /* Create menu button */
    n=0;
    XtSetArg( argList[n], XtNfromHoriz, box1);    n++;
    XtSetArg( argList[n], XtNfromVert, mainmsgbox);    n++;
    XtSetArg( argList[n], XtNheight, BUTTON_H_SIZE); n++;
    XtSetArg( argList[n], XtNwidth, BUTTON_V_SIZE);  n++;
    XtSetArg( argList[n], XtNmenuName, "diskMenu");  n++;
    diskMenuButton = XtCreateManagedWidget("diskMenuButton",
         menuButtonWidgetClass, commands, argList, n);

    firstLevelMenu = XtCreatePopupShell("diskMenu", simpleMenuWidgetClass,
         diskMenuButton, NULL, ZERO);

    for (i = 0; i < (int) XtNumber(menu_item_names) ; i++) {
        char * item = menu_item_names[i];
        n=0;
        if (i == 1) {
            XtSetArg( argList[n], XtNsensitive, FALSE);  n++;
        }
        else {
            XtSetArg( argList[n], XtNsensitive, TRUE);  n++;
        }
        diskmenu[i] = XtCreateManagedWidget(item, smeBSBObjectClass, 
            firstLevelMenu,argList,n);
    }
    XtAddCallback(diskmenu[0], XtNcallback, filePopupPrompt,
       (XtPointer) LOAD);
    XtAddCallback(diskmenu[1], XtNcallback, filePopupPrompt,
       (XtPointer) SAVEAS);
    XtAddCallback(diskmenu[2], XtNcallback, quitPopupPrompt, NULL);
}

void createCameraMenu()
{
    Arg          argList[MAX_ARGS];    /* resource list   */
    Widget       cameraPulldownMenu,
                 entry;
    int          i,n;                  /* counters */


    char *menu_item_names[] =
    {
        "Cut","Start Move","End Move",
        "Start Zoom","End Zoom"};


    /* Create camera menu button */
    n=0;
    XtSetArg( argList[n], XtNfromVert, mainmsgbox);    n++;
    XtSetArg( argList[n], XtNfromHoriz, diskMenuButton);    n++;
    XtSetArg( argList[n], XtNheight, BUTTON_H_SIZE);        n++;
    XtSetArg( argList[n], XtNwidth, 74);                    n++;
    XtSetArg( argList[n], XtNmenuName, "cameraMenu");       n++;

    cameraMenuButton = XtCreateManagedWidget("CameraMenuButton",
         menuButtonWidgetClass, commands, argList, n);

    cameraPulldownMenu = XtCreatePopupShell("cameraMenu", simpleMenuWidgetClass,
         cameraMenuButton, NULL, ZERO);

    for (i = 0; i < (int) XtNumber(menu_item_names) ; i++)
    {
        char * item = menu_item_names[i];
        n = 0;
        XtSetArg( argList[n], XtNsensitive, FALSE); n++;
        cutmenu[i] = XtCreateManagedWidget(item, smeBSBObjectClass,
            cameraPulldownMenu, argList,n);
    }
    XtAddCallback(cutmenu[0],XtNcallback,cutCallback,NULL);
    XtAddCallback(cutmenu[1],XtNcallback,dollyCallback,NULL);
    XtAddCallback(cutmenu[2],XtNcallback,dollyCallback,NULL);
    XtAddCallback(cutmenu[3],XtNcallback,zoomCallback,NULL);
    XtAddCallback(cutmenu[4],XtNcallback,zoomCallback,NULL);
}

void createsplCameraMenu()
{
    Arg          argList[MAX_ARGS];    /* resource list   */
    Widget       splcameraPulldownMenu,
                 entry;
    int          i,n;                  /* counters */


    char *menu_item_names[] =
    {
        "Start Dolly", "Add Dolly Point",  "End Dolly", "Remove Dolly Point"};


    /* Create splice camera menu button */
    n=0;
    XtSetArg( argList[n], XtNfromVert, mainmsgbox);    n++;
    XtSetArg( argList[n], XtNfromHoriz, cameraMenuButton); n++;
    XtSetArg( argList[n], XtNheight, BUTTON_H_SIZE);       n++;
    XtSetArg( argList[n], XtNwidth, 74);                   n++;
    XtSetArg( argList[n], XtNmenuName, "splcameraMenu");   n++;

    splcameraMenuButton = XtCreateManagedWidget("splCameraMenuButton",
         menuButtonWidgetClass, commands, argList, n);

    splcameraPulldownMenu = XtCreatePopupShell("splcameraMenu",
        simpleMenuWidgetClass, splcameraMenuButton, NULL, ZERO);

   for (i = 0; i < (int) XtNumber(menu_item_names) ; i++)
    {
        char * item = menu_item_names[i];
        n=0;
        XtSetArg( argList[n], XtNsensitive, FALSE);  n++;
        splmenu[i] = XtCreateManagedWidget(item, smeBSBObjectClass,
                splcameraPulldownMenu, argList,n);
    }
    XtAddCallback(splmenu[0],XtNcallback,splineCallback,NULL);
    XtAddCallback(splmenu[1],XtNcallback,splineCallback,NULL);
    XtAddCallback(splmenu[2],XtNcallback,splineCallback,NULL);
    XtAddCallback(splmenu[3],XtNcallback,splineCallback,NULL);
}

/* ----------------------------------------------------------------
    createLightMenu

    Create menu tree.
    --------------------------------------------------------------- */

void createLightMenu()
{
    Arg          argList[MAX_ARGS];    /* resource list   */
    Widget       firstLevelMenu,
                 entry;
    int          i,n;                  /* counters */

    char * menu_item_names[] = {
        "Set Ambient Level","Add Diffuse Light", "Add Spotlight",
        "Add Warn Light","Adjust Light","Remove Light"};

    /* Create button to change light settings */
    n=0;
    XtSetArg( argList[n], XtNfromVert, mainmsgbox);    n++;
    XtSetArg( argList[n], XtNfromHoriz, splcameraMenuButton);
    n++;
    XtSetArg( argList[n], XtNheight, BUTTON_H_SIZE);
    n++;
    XtSetArg( argList[n], XtNwidth, BUTTON_V_SIZE);
    n++;
    XtSetArg( argList[n], XtNmenuName, "lightmenu");
    n++;
    lightMenuButton = XtCreateManagedWidget("Lighting", menuButtonWidgetClass,
                   commands, argList, n);

    firstLevelMenu = XtCreatePopupShell("lightmenu", simpleMenuWidgetClass,
         lightMenuButton, NULL, ZERO);

    for (i = 0; i < (int) XtNumber(menu_item_names) ; i++) {
        char * item = menu_item_names[i];
        n=0;
        XtSetArg( argList[n], XtNsensitive, FALSE);  n++;
        lightmenu[i] = XtCreateManagedWidget(item, smeBSBObjectClass,
                       firstLevelMenu,argList,n);
    }
    XtAddCallback(lightmenu[0], XtNcallback, lightCallback, NULL);
    XtAddCallback(lightmenu[1], XtNcallback, lightCallback, NULL);
    XtAddCallback(lightmenu[2], XtNcallback, lightCallback, NULL);
    XtAddCallback(lightmenu[3], XtNcallback, lightCallback, NULL);
    XtAddCallback(lightmenu[4], XtNcallback, lightadjustCallback, NULL);
    XtAddCallback(lightmenu[5], XtNcallback, lightadjustCallback, NULL);
}

void createMergeMenu()
{

    Arg    argList[MAX_ARGS];  /* resource list  */
    int    i,n=0;              /* resource count */
    Widget dialog,             /* dialog box     */
           popup,              /* Widget that contains the dialog box */
           entry,
           mergePulldownMenu;   /* Pull down menu for draw button */
    Position  x, y;            /* pixel location relative to
                                  entire window */

    char *menu_item_names[] =
        { "Merge files","Splice files" };

    n = 0;
    XtSetArg( argList[n], XtNfromVert, mainmsgbox);    n++;
    XtSetArg( argList[n], XtNfromHoriz, lightMenuButton); n++;
    XtSetArg( argList[n], XtNheight, BUTTON_H_SIZE);      n++;
    XtSetArg( argList[n], XtNwidth, BUTTON_V_SIZE);       n++;
    XtSetArg( argList[n], XtNmenuName, "mergeMenu");      n++;

    mergeMenuButton = XtCreateManagedWidget("Merge",
         menuButtonWidgetClass, commands, argList, n);

    mergePulldownMenu = XtCreatePopupShell("mergeMenu", simpleMenuWidgetClass,
         mergeMenuButton, NULL, ZERO);

    for (i = 0; i < (int) XtNumber(menu_item_names) ; i++)
    {
        char * item = menu_item_names[i];
        n=0;
        XtSetArg( argList[n], XtNsensitive, TRUE);  n++;
        mergemenu[i] = XtCreateManagedWidget(item, smeBSBObjectClass,
            mergePulldownMenu,argList,n);
    }
    XtAddCallback(mergemenu[0], XtNcallback, mergeCallback,NULL);
    XtAddCallback(mergemenu[1], XtNcallback, mergeCallback,NULL);
}

