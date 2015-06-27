#include "main.h"

extern int get_line_contents();
extern int write_line();

int merge_files(src1,src2,dest,startat)
char *src1;
char *src2;
char *dest;
int startat;
{
    FILE *fp,*fp1,*fp2;
    int frames,line_number,line_length,line_length1;
    char *linecontents[50];
    char *msg[80];
 
    line_number = 0;
    frames = 1;
    if ( (fp = fopen(src1,"r")) == NULL) {
        sprintf(msg,"Error opening file %s.\n",src1);
        writeMessage(msg);
        return;
    }
    if ( (fp1 = fopen(src2,"r")) == NULL) {
        sprintf(msg,"Error opening file %s.\n",src2);
        writeMessage(msg);
        return;
    }
    if ( (fp2 = fopen(dest,"w")) == NULL) {
        sprintf(msg,"Error opening file %s.\n",dest);
        writeMessage(msg);
        return;
    }
    while ((line_length = get_line_contents(fp,linecontents))>0) {
        if (!strcmp(linecontents[1],"SETUP")) {
            write_line(fp2,linecontents,line_length);
            break;
        }
    }
    
    while ( (line_length = get_line_contents(fp1,linecontents)) > 0) {
        if (!strcmp(linecontents[1],"PLACE")) break;
        else if (!strcmp(linecontents[1],"FILE") ||
                 !strcmp(linecontents[1],"INSTANCE"))
            write_line(fp2,linecontents,line_length);
    }
 
    while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
        write_line(fp2,linecontents,line_length);
        if (!strcmp(linecontents[1],"PLACE")) break;
    }

    if (startat == 1) {
        while ( (line_length = get_line_contents(fp1,linecontents)) > 0) {
           if (!strcmp(linecontents[1],"NEW_FRAME")) break;
           if (strcmp(linecontents[1],"END_PLACE")
                && strcmp(linecontents[1],"ACTION")
                && strcmp(linecontents[1],"CAMERA")
                && strcmp(linecontents[1],"FOCUS"))
                write_line(fp2,linecontents,line_length);
        }
    }

    while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
        write_line(fp2,linecontents,line_length);
        if (!strcmp(linecontents[1],"NEW_FRAME")) break;
    }

    while (startat < frames) {
        while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
            if (!strcmp(linecontents[1],"NEW_FRAME")) break;
            write_line(fp2,linecontents,line_length);
        }
        frames++;
        fprintf(fp2,"[ NEW_FRAME ]\n");
    }

    if (startat != 1) {
        while ( (line_length = get_line_contents(fp1,linecontents)) > 0) {
           if (!strcmp(linecontents[1],"NEW_FRAME")) break;
           if (strcmp(linecontents[1],"END_PLACE")
                && strcmp(linecontents[1],"ACTION")
                && strcmp(linecontents[1],"CAMERA")
                && strcmp(linecontents[1],"FOCUS"))
                write_line(fp2,linecontents,line_length);
        }
        while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
            if (!strcmp(linecontents[1],"NEW_FRAME")) break;
            write_line(fp2,linecontents,line_length);
        }
        frames++;
        fprintf(fp2,"[ NEW_FRAME ]\n");
    }
    
    while (line_length>0 || line_length1>0) {
        while ( (line_length = get_line_contents(fp1,linecontents)) > 0) {
            if (!strcmp(linecontents[1],"NEW_FRAME")) break;
            if (strcmp(linecontents[1],"CAMERA")
                && strcmp(linecontents[1],"ACTION")
                && strcmp(linecontents[1],"FOCUS"))
                write_line(fp2,linecontents,line_length);
        }
        while ( (line_length1 = get_line_contents(fp,linecontents)) > 0) {
            if (!strcmp(linecontents[1],"NEW_FRAME")) break;
            write_line(fp2,linecontents,line_length1);
        }
        fprintf(fp2,"\n[ NEW_FRAME]\n");
        frames++;
    } 
    
    
    if (line_length==NO_LINE_END_ERROR)
        printf("Missing ']' in line %d of %s\n",line_number,src2);
    else if (line_length == NO_LINE_START_ERROR)
        printf("Missing '[' in line %d of %s\n",line_number,src2);
    else {
        if (!strcmp(linecontents[1],"NEW_FRAME"))
            --frames;
        printf("frames = %d\n",frames);
    }
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
}
