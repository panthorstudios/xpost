#include "main.h"

load_object(fileName,obj)
    char *fileName;
    Object *obj;
{
    FILE *fp;
    int i,done,line_number,line_length,line_length1;
    char *linecontents[50];
    float x,y,z,xmax,xmin,ymax,ymin,zmax,zmin;
    
    if ( (fp = fopen(fileName,"r")) == NULL) {
        sprintf(msg,"WARNING: Object file %s not found.\n",src1);
        writeMessage(msg);
        return;
    }
    done = FALSE;
    while (!done) {
        if (get_line_contents(fp,linecontents)>0) {
            if (!strcmp(linecontents[1],"VERTEX")) {
                xmax = atof(linecontents[3]);
                ymax = atof(linecontents[4]);
                zmax = atof(linecontents[5]);
                xmin = xmax;
                ymin = ymax;
                zmin = zmax;
                done = TRUE;
            }
       }
   }
    
   while (get_line_contents(fp,linecontents)>0) {
        if (!strcmp(linecontents[1],"VERTEX")) {
            x = atof(linecontents[3]);
            y = atof(linecontents[4]);
            z = atof(linecontents[5]);
            if (x>xmax) xmax = x;
            else if (x<xmin) xmin = x;
            if (y>ymax) ymax = y;
            else if (y<ymin) ymin = y;
            if (z>zmax) zmax = z;
            else if (z<zmin) zmin = z;
        }
    }
    fclose(fp);
    printf("xmin = %f xmax = %f\n",xmin,xmax);
    printf("ymin = %f ymax = %f\n",ymin,ymax);
    printf("zmin = %f zmax = %f\n",zmin,zmax);
    obj->verts[0][0] = xmin; obj->verts[0][1] = ymin; obj->verts[0][2] = zmin;
    obj->verts[1][0] = xmin; obj->verts[1][1] = ymax; obj->verts[1][2] = zmin;
    obj->verts[2][0] = xmin; obj->verts[2][1] = ymax; obj->verts[2][2] = zmax;
    obj->verts[3][0] = xmin; obj->verts[3][1] = ymin; obj->verts[3][2] = zmax;
    obj->verts[4][0] = xmax; obj->verts[4][1] = ymax; obj->verts[4][2] = zmin;
    obj->verts[5][0] = xmax; obj->verts[5][1] = ymin; obj->verts[5][2] = zmin;
    obj->verts[6][0] = xmax; obj->verts[6][1] = ymin; obj->verts[6][2] = zmax;
    obj->verts[7][0] = xmax; obj->verts[7][1] = ymax; obj->verts[7][2] = zmax;
}
