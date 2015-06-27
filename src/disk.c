#define SETUP 0
#define PLACE 1 
#define ACTION 2
#include"main.h"

extern Settings settings;
extern clear_views();
extern show_views();
extern set_bars();
extern Widget framemsg,diskmenu[],lightmenu[],cutmenu[],splmenu[];
extern int cut,move,zoom;
extern struct Light *light_head;
extern struct Mover *frame_pntr;
extern int lightcount[3];
extern struct Mover *framelist;
extern void draw_objects();
extern Display *theDisplay;
int get_line_contents();
extern BBox *objects;
extern int num_objects;

void insert_light(ptr)
struct Light *ptr;
{
    settings.light_ptr->next = ptr;
    ptr->next = NULL;
    settings.light_ptr = settings.light_ptr->next;
}

/**************************************************************
* fileExists
*
*  Check if the given file exists.
***************************************************************/
Boolean fileExists( nameStr )
    char *nameStr;  /* name of file */
{
    char wholeNameStr[20];

   strcpy( wholeNameStr, nameStr );    

   if (fopen(wholeNameStr, "r") == NULL) 
      return FALSE;
   else 
      return TRUE;
}


/* -------------------------------------------------------------------
   saveFile

   Save the data structure settings in the named file.  Save one
   setting per line.  Use NULL lines (blank lines) to indicate
   separation between parts of the data structure.
   ------------------------------------------------------------------- */
char saveFile(nameStr)
     char *nameStr; 
{
    FILE        *fp,*fp1;       
    int          i,j,length=0;   
    int line_number,line_length;
    char *linecontents[2000],msg[80];
    int frames;

    if (!strcmp(settings.infile,nameStr)) {
        sprintf(msg,"You must specify a different filename from %s!",nameStr);
        writeMessage(msg);
        return;
    }

    if ( (fp1 = fopen(nameStr,"w")) == NULL) {
        fprintf(stderr,"Error opening file %s.\n",nameStr);
        return;
    }

    frames = 1;
    if ( (fp = fopen(settings.infile,"r")) == NULL) {
        fprintf(stderr,"Error opening file %s.\n",settings.infile);
        return;
    }

    while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
        ++line_number;
        write_line(fp1,linecontents,line_length,&frames,SAVE);
        if (!strcmp(linecontents[1],"NEW_FRAME"))
            ++frames;
    }
    if (line_length==NO_LINE_END_ERROR)
        printf("Missing ']'\n");
    else if (line_length == NO_LINE_START_ERROR)
        printf("Missing '['\n");
    else {
        if (!strcmp(linecontents[1],"NEW_FRAME"))
            --frames;
        else fprintf(fp1,"[ NEW_FRAME ]\n");
    }
    fclose(fp);
    fclose(fp1);
    sprintf(msg,"Animation saved as %s\n",nameStr);
    writeMessage(msg);
}

int write_line(fpnt,linecontents,ll,frames,function)
FILE *fpnt;
char *linecontents[];
int  ll;
int *frames;
int function;
{
    int i,j;
    float delta,delta1,dx,dy,dz,xval,yval,zval;
    struct Light *lptr;

    if (!strcmp(linecontents[1],"MATRIX")) {
        fprintf(fpnt,"[ %s %s\n",linecontents[1],linecontents[2]);
        for (i=0; i<4; i++) {
            fprintf(fpnt,"        ");
            for (j=0; j<3; j++)
                fprintf(fpnt,"%s ",linecontents[3+(i*3)+j]);
            if (i<3) fprintf(fpnt,"\n");
            else fprintf(fpnt,"]\n");
        }
    }
    if (function == SAVE) {
        if (!strcmp(linecontents[1],"LIGHT"));
        else if (!strcmp(linecontents[1],"MATRIX"));
        else if (!strcmp(linecontents[1],"CAMERA") ||
                !strcmp(linecontents[1],"FOCUS") ||
                !strcmp(linecontents[1],"AOV"));
        else {
            for (i=0;i<ll;i++) {
                fprintf(fpnt,"%s ",linecontents[i]);
            }
            fprintf(fpnt,"\n");
            if (!strcmp(linecontents[1],"SETUP")) {
                lptr = settings.light_head->next;
                while (lptr != NULL) {
                    switch (lptr->lighttype) {
                        case DIFFUSE : {
                            fprintf(fpnt,"[ LIGHT DIFFUSE %f %f %f %f ]\n",
                                lptr->lightint,lptr->lx,lptr->ly,lptr->lz);
                            break;
                        }
                        case SPOT : {
                            fprintf(fpnt,
                            "[ LIGHT SPOT %f %f %f %f %f %f %f %f ]\n",
                            lptr->lightint,lptr->lightcoeff,lptr->lx,lptr->ly,
                            lptr->lz,lptr->lfx,lptr->lfy,lptr->lfz);
                            break;
                        }
                        case WARN : {
                            fprintf(fpnt,
                            "[ LIGHT WARN %f %f %f %f %f %f %f %f ]\n",
                            lptr->lightint,lptr->lightcoeff,lptr->lx,lptr->ly,
                            lptr->lz,lptr->lfx,lptr->lfy,lptr->lfz);
                            break; 
                        }
                    }
                    lptr = lptr->next;
                }
            }
            if (!strcmp(linecontents[1],"PLACE")) {
                fprintf(fpnt,"[ CAMERA %f %f %f ]\n",
                    framelist[1].camera.x,
                    framelist[1].camera.y,
                    framelist[1].camera.z);
                fprintf(fpnt,"[ FOCUS %f %f %f ]\n",
                    framelist[1].slate.x,
                    framelist[1].slate.y,
                    framelist[1].slate.z);
                fprintf(fpnt,"[ AOV %d ]",framelist[1].zoomval);
            }
            if (!strcmp(linecontents[1],"NEW_FRAME")) {
                if (framelist[*frames].movetype != NONE && (*frames) >= 1) {
                    fprintf(fpnt,"[ CAMERA %f %f %f ]\n",
                        framelist[*frames].camera.x,
                        framelist[*frames].camera.y,
                        framelist[*frames].camera.z);
                    fprintf(fpnt,"[ FOCUS %f %f %f ]\n",
                        framelist[*frames].slate.x,
                        framelist[*frames].slate.y,
                        framelist[*frames].slate.z);
                }
                if (framelist[*frames].zoomtype != NONE)
                    fprintf(fpnt,"[ AOV %d ]",framelist[*frames].zoomval);
            }
        }
        fprintf(fpnt,"\n");
    }
}

char loadFile( nameStr )
     char *nameStr;  /* name of file */
{
    FILE *fp; 
    struct Light *lptr;
    struct Mover *pntr;
    Arg argList[MAX_ARGS];
    char displayString[MAX_CHARS_IN_WINDOW];  /* message for user */
    int i,j,k,lasts,lastc,lastz,section,frameno;
    int scaled_right,line_number,line_length,onum;
    char *linecontents[2000],msg[80];

    sprintf(msg,"Parsing data files...\n");
    writeMessage(msg);
    sleep(1);
    if ( (fp = fopen(nameStr,"r")) == NULL) {
        return(ERR_OPEN_FILE_FOR_READ);
    }
    settings.frames = 0;
    num_objects = 0;
    if (get_line_contents(fp,linecontents) > 0) {
        while(get_line_contents(fp,linecontents) > 0) {
            if (!strcmp(linecontents[1],"FILE") || 
                !strcmp(linecontents[1],"INSTANCE"))
            num_objects++; 
            if (!strcmp(linecontents[1],"NEW_FRAME"))
                settings.frames++;
        }
    }
    else {
        sprintf(msg,"%s is an empty file!\n",nameStr);
        writeMessage(msg);
        return(ERROR);
    }
    if (strcmp(linecontents[1],"NEW_FRAME"))
        settings.frames++;
    fclose(fp);
    framelist = (struct Mover *)calloc(1+settings.frames,sizeof(struct Mover));
    objects = (BBox *)calloc(num_objects,sizeof(BBox));
    if ( (fp = fopen(nameStr,"r")) == NULL) {
        return(ERR_OPEN_FILE_FOR_READ);
    }
    line_number = 0;
    lasts=lastc=lastz=1;
    settings.scale = 1;
    settings.infile = (char *)malloc(strlen(nameStr)+1);
    strcpy(settings.infile,nameStr);
    clear_views();
    onum = 0;
    frameno = 1;
    for (i=1; i <= settings.frames;i++) {
        framelist[i].movetype = NONE;
        framelist[i].zoomtype = NONE;
        framelist[i].smovetype = NONE;
        framelist[i].camera.x = 60;
        framelist[i].camera.y = 0;
        framelist[i].camera.z = 0;
        framelist[i].slate.x = 0;
        framelist[i].slate.y = 0;
        framelist[i].slate.z = 0;
        framelist[i].zoomval = 30;
    }

    while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
        if (!strcmp(linecontents[1],"SETUP")) section = SETUP; 
        if (!strcmp(linecontents[1],"PLACE")) section = PLACE; 
        if (!strcmp(linecontents[1],"ACTION")) section = ACTION; 
        if (!strcmp(linecontents[1],"FILE") ||
            !strcmp(linecontents[1],"INSTANCE")) {
            load_object(linecontents[2],onum);
            strcpy(objects[onum].name,linecontents[3]);
            onum++;
        }
        if (!strcmp(linecontents[1],"CAMERA")) {
            if (section == ACTION) {
                framelist[frameno].camera.x = atof(linecontents[2]);
                framelist[frameno].camera.y = atof(linecontents[3]);
                framelist[frameno].camera.z = atof(linecontents[4]);
                lastc = frameno;
            }
            else {
                framelist[1].camera.x = atof(linecontents[2]);
                framelist[1].camera.y = atof(linecontents[3]);
                framelist[1].camera.z = atof(linecontents[4]);
            }
        }
        if (!strcmp(linecontents[1],"FOCUS")) {
            if (section == ACTION) {
                lasts = frameno;
                framelist[frameno].slate.x = atof(linecontents[2]);
                framelist[frameno].slate.y = atof(linecontents[3]);
                framelist[frameno].slate.z = atof(linecontents[4]);
            }
            else {
                framelist[1].slate.x = atof(linecontents[2]);
                framelist[1].slate.y = atof(linecontents[3]);
                framelist[1].slate.z = atof(linecontents[4]);
            }
        }
        if (!strcmp(linecontents[1],"AOV")) {
            if (section == ACTION) {
                lastz = frameno;
                framelist[frameno].zoomval = (int)(atof(linecontents[2]));
            }
            else {
                framelist[1].zoomval = (int)(atof(linecontents[2]));
            }
        }
        if (!strcmp(linecontents[1],"LIGHT")) {
            if (!strcmp(linecontents[2],"AMBIENT")) 
                settings.ambient = atof(linecontents[2]);
            if (!strcmp(linecontents[2],"DIFFUSE")) {
                lptr = (struct Light *)malloc(sizeof(struct Light));
                lightcount[DIFFUSE]++;
                lptr->lighttype = DIFFUSE;
                lptr->lightid   = lightcount[DIFFUSE];
                lptr->lightint = atof(linecontents[3]);
                lptr->lx = atof(linecontents[4]);
                lptr->ly = atof(linecontents[5]);
                lptr->lz = atof(linecontents[6]);
                insert_light(lptr);
            }
            if (!strcmp(linecontents[2],"SPOT")) {
                lptr = (struct Light *)malloc(sizeof(struct Light));
                lightcount[SPOT]++;
                lptr->lighttype = SPOT;
                lptr->lightid   = lightcount[SPOT];
                lptr->lightint = atof(linecontents[3]);
                lptr->lightcoeff = atof(linecontents[4]);
                lptr->lx = atof(linecontents[5]);
                lptr->ly = atof(linecontents[6]);
                lptr->lz = atof(linecontents[7]);
                insert_light(lptr);
            }
            if (!strcmp(linecontents[2],"WARN")) {
                lptr = (struct Light *)malloc(sizeof(struct Light));
                lightcount[WARN]++;
                lptr->lighttype = WARN;
                lptr->lightid   = lightcount[WARN];
                lptr->lightint = atof(linecontents[3]);
                lptr->lightcoeff = atof(linecontents[4]);
                lptr->lx = atof(linecontents[5]);
                lptr->ly = atof(linecontents[6]);
                lptr->lz = atof(linecontents[7]);
                insert_light(lptr);
            }
        }
        if (!strcmp(linecontents[1],"NEW_FRAME"))
            frameno++;
    }
    fclose(fp);
    settings.camera.x = framelist[1].camera.x;
    settings.camera.y = framelist[1].camera.y;
    settings.camera.z = framelist[1].camera.z;
    settings.slate.x = framelist[1].slate.x;
    settings.slate.y = framelist[1].slate.y;
    settings.slate.z = framelist[1].slate.z;
    settings.zoom = framelist[1].zoomval;

    scaled_right = FALSE;
    while (!scaled_right) {
        if (settings.camera.x > (settings.scale*80)  ||
            settings.camera.y > (settings.scale*80)  ||
            settings.camera.z > (settings.scale*80)  ||
            settings.slate.x > (settings.scale*80)   ||
            settings.slate.y > (settings.scale*80)   ||
            settings.slate.z > (settings.scale*80)   ||
            settings.camera.x < -(settings.scale*80) ||
            settings.camera.y < -(settings.scale*80) ||
            settings.camera.z < -(settings.scale*80) ||
            settings.slate.x < -(settings.scale*80)  ||
            settings.slate.y < -(settings.scale*80)  ||
            settings.slate.z < -(settings.scale*80)) {
                if (settings.scale <=512) settings.scale *=2;
                else scaled_right = TRUE;
        }
        else scaled_right = TRUE;
    }
    if (lastc != settings.frames)
        for (i=lastc+1;i<=settings.frames;i++) {
            framelist[i].camera.x = framelist[lastc].camera.x;
            framelist[i].camera.y = framelist[lastc].camera.y;
            framelist[i].camera.z = framelist[lastc].camera.z;
        }
    if (lasts != settings.frames)
        for (i=lasts+1;i<=settings.frames;i++) {
            framelist[i].slate.x = framelist[lasts].slate.x;
            framelist[i].slate.y = framelist[lasts].slate.y;
            framelist[i].slate.z = framelist[lasts].slate.z;
        }
    if (lastz != settings.frames)
        for (i=lastz+1;i<=settings.frames;i++) 
            framelist[i].zoomval = framelist[lastz].zoomval;

    show_views();
    set_bars();
    settings.current_frame = 1;
    sprintf(displayString,"%d",settings.current_frame);
    XtSetArg(argList[0], XtNlabel, displayString);
    XtSetValues( framemsg, argList, 1);

    XtSetArg(argList[0], XtNsensitive, TRUE);
    XtSetValues( diskmenu[1], argList, 1);
    XtSetArg(argList[0], XtNsensitive, TRUE);
    XtSetValues( cutmenu[0], argList, 1);
    XtSetArg(argList[0], XtNsensitive, TRUE);
    XtSetValues( cutmenu[1], argList, 1);
    XtSetArg(argList[0], XtNsensitive, TRUE);
    XtSetValues( cutmenu[3], argList, 1);
    XtSetArg(argList[0], XtNsensitive, TRUE);
    XtSetValues( splmenu[0], argList, 1);
    for (i=0;i<6;i++) {
        XtSetArg(argList[0], XtNsensitive, TRUE);
        XtSetValues(lightmenu[i], argList, 1);
    }
    draw_objects();
}

int get_line_contents(fpnt,linecontents)
FILE *fpnt;
char *linecontents[];
{
    int items,done,leftparen,rightparen;
    char item[80];

    items = 0;
    leftparen = 0;
    rightparen = 0;
    done = FALSE;
    while (!done) {
        if ( fscanf(fpnt,"%s",item) == ERROR)
            return(EOF_FOUND);
        else {
            if(( linecontents[items] =
                (char *)malloc(strlen(item)+1)) == NULL)
                return(MALLOC_ERROR);
            strcpy( linecontents[items++], item );
            if ((strcspn(item,"[")==0)) leftparen+=1;
            if ((strcspn(item,"]")==(strlen(item)-1))) rightparen+=1;
            if (rightparen>leftparen) {
                printf("leftparen = %d\n",leftparen);
                printf("rightparen = %d\n",rightparen);
                 return(NO_LINE_END_ERROR);
            }
            if (leftparen && leftparen==rightparen) done = TRUE;
        }
    }
    if (items < 3) return(LINE_ERROR);
    return(items);
}
load_object(fileName,obj)
    char *fileName;
    int obj;
{
    FILE *fp;
    int i,j,l,done,line_number,line_length,line_length1;
    char *linecontents[2000],msg[80];
    float x,y,z,xmax,xmin,ymax,ymin,zmax,zmin;
    matrix m;
    BBox1 temp,temp1;

    if ( (fp = fopen(fileName,"r")) == NULL) {
        sprintf(msg,"WARNING: Object file %s not found.\n",fileName);
        writeMessage(msg);
        return;
    }
    done = FALSE;
    while (!done) {
        if ((get_line_contents(fp,linecontents))>0) { 
            if (!strcmp(linecontents[1],"VERTEX")) {
                if (!strcmp(linecontents[4],"NORMAL")) {
                    xmax = atof(linecontents[9]);
                    ymax = atof(linecontents[10]);
                    zmax = atof(linecontents[11]);
                }
                else {
                    xmax = atof(linecontents[3]);
                    ymax = atof(linecontents[4]);
                    zmax = atof(linecontents[5]);
                }
                xmin = xmax;
                ymin = ymax;
                zmin = zmax;
                done = TRUE;
            }
       }
   }

   while ((l=get_line_contents(fp,linecontents))>0) {  
        if (!strcmp(linecontents[1],"VERTEX")) {
            if (!strcmp(linecontents[4],"NORMAL")) {
                x = atof(linecontents[9]);
                y = atof(linecontents[10]);
                z = atof(linecontents[11]);
            }
            else {
                x = atof(linecontents[3]);
                y = atof(linecontents[4]);
                z = atof(linecontents[5]);
            }
            if (x>xmax) xmax = x;
            else if (x<xmin) xmin = x;
            if (y>ymax) ymax = y;
            else if (y<ymin) ymin = y;
            if (z>zmax) zmax = z;
            else if (z<zmin) zmin = z;
        }
        if (!strcmp(linecontents[1],"INSTANCE")) {
            for (i=0;i<4;i++) {
                m[0][i] = atof(linecontents[4+i]);
                m[1][i] = atof(linecontents[8+i]);
                m[2][i] = atof(linecontents[12+i]);
                m[3][i] = atof(linecontents[16+i]);
            }
        }
    }
    fclose(fp);

    temp.verts[0][0] = xmin; temp.verts[0][1] = ymin; temp.verts[0][2] = zmin;
    temp.verts[1][0] = xmin; temp.verts[1][1] = ymax; temp.verts[1][2] = zmin;
    temp.verts[2][0] = xmin; temp.verts[2][1] = ymax; temp.verts[2][2] = zmax;
    temp.verts[3][0] = xmin; temp.verts[3][1] = ymin; temp.verts[3][2] = zmax;
    temp.verts[4][0] = xmax; temp.verts[4][1] = ymax; temp.verts[4][2] = zmin;
    temp.verts[5][0] = xmax; temp.verts[5][1] = ymin; temp.verts[5][2] = zmin;
    temp.verts[6][0] = xmax; temp.verts[6][1] = ymin; temp.verts[6][2] = zmax;
    temp.verts[7][0] = xmax; temp.verts[7][1] = ymax; temp.verts[7][2] = zmax;
    temp.verts[0][3] = 1;
    temp.verts[1][3] = 1;
    temp.verts[2][3] = 1;
    temp.verts[3][3] = 1;
    temp.verts[4][3] = 1;
    temp.verts[5][3] = 1;
    temp.verts[6][3] = 1;
    temp.verts[7][3] = 1;

    for (i=0;i<8;i++) {
        vec_mult(temp.verts[i],m,temp1.verts[i]);
        for (j=0;j<3;j++)
            objects[obj].verts[i][j] = temp1.verts[i][j];
    }
}



