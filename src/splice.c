#include "main.h"
extern int get_line_contents();
extern int write_line();

int splice_files(src1,src2,dest,after)
char *src1;
char *src2;
char *dest;
int after;
{
    FILE *fp,*fp1,*fp2;
    int frames,line_number,line_length,line_length1;
    char *linecontents[50];
    
    line_number = 0;
    frames = 1;
    if ( (fp = fopen(src1,"r")) == NULL) {
        fprintf(stderr,"Error opening file %s.\n",src1);
        return;
    }
    if ( (fp1 = fopen(src2,"r")) == NULL) {
        fprintf(stderr,"Error opening file %s.\n",src2);
        return;
    }
    if ( (fp2 = fopen(dest,"w")) == NULL) {
        fprintf(stderr,"Error opening file %s.\n",dest);
        return;
    }
    if ((line_length = get_line_contents(fp,linecontents))>0) {
        if (strcmp(linecontents[1],"SETUP")) {
            printf("Illegal 1st line in source file\n");
            exit(1);
        }
        else write_line(fp2,linecontents,line_length);
    }
    
    while ( (line_length = get_line_contents(fp1,linecontents)) > 0) {
        if (!strcmp(linecontents[1],"PLACE")) break;
        else if (!strcmp(linecontents[1],"FILE"))
            write_line(fp2,linecontents,line_length);
    }
 
    while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
        write_line(fp2,linecontents,line_length);
        if (!strcmp(linecontents[1],"PLACE")) break;
    }

    if (after == 0) {
        while ( (line_length = get_line_contents(fp1,linecontents)) > 0) {
            if (!strcmp(linecontents[1],"NEW_FRAME")) break;
/*            if (strcmp(linecontents[1],"CAMERA")
                && strcmp(linecontents[1],"FOCUS"))
*/
            write_line(fp2,linecontents,line_length);
        }
    }

    else {
        while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
            write_line(fp2,linecontents,line_length);
            if (!strcmp(linecontents[1],"NEW_FRAME")) break;
        }
    }

    while (frames<after) {
         while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
            if (!strcmp(linecontents[1],"NEW_FRAME")) break;
            write_line(fp2,linecontents,line_length);
        }
        fprintf(fp2,"\n[ NEW_FRAME]\n");
        frames++;
    } 

    while ( (line_length = get_line_contents(fp1,linecontents)) > 0) {
        if (strcmp(linecontents[1],"CAMERA")
            && strcmp(linecontents[1],"END_PLACE")
            && strcmp(linecontents[1],"ACTION")
            && strcmp(linecontents[1],"FOCUS"))
            write_line(fp2,linecontents,line_length);
        if (!strcmp(linecontents[1],"NEW_FRAME")) break;
    }
    frames++;

    while ( (line_length = get_line_contents(fp1,linecontents)) > 0) {
        write_line(fp2,linecontents,line_length);
        if (!strcmp(linecontents[1],"NEW_FRAME")) frames++;
    }
    if (!strcmp(linecontents[1],"NEW_FRAME")) frames--;
    
    while ( (line_length = get_line_contents(fp,linecontents)) > 0) {
        if (strcmp(linecontents[1],"END_PLACE")
            && strcmp(linecontents[1],"ACTION")) {
            write_line(fp2,linecontents,line_length);
        if (!strcmp(linecontents[1],"NEW_FRAME"))
            frames++;
        }
    }
    if (!strcmp(linecontents[1],"NEW_FRAME")) frames--;
    
    if (line_length==NO_LINE_END_ERROR)
        printf("Missing ']' in line %d of %s\n",line_number,src2);
    else if (line_length == NO_LINE_START_ERROR)
        printf("Missing '[' in line %d of %s\n",line_number,src2);
    else {
        if (!strcmp(linecontents[1],"NEW_FRAME"))
            --frames;
        else fprintf(fp2,"\n[ NEW_FRAME ]\n");
        printf("frames = %d\n",frames);
    }
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
}
