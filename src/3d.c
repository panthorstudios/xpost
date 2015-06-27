#include "main.h"
extern Settings settings;
extern struct Mover *framelist;
extern Display *theDisplay;
extern Window cameraWindow;
extern GC  cameraGC;
extern BBox *objects;
extern int num_objects;
extern unsigned long YellowPen;

vector  p1,p2,p3,vrp,vup,vpn,dop,cop;
matrix  Nper;
float   backang,aov,dist,umax,umin,vmax,vmin,back,front,zview,pnorm;

float dot_product(a,b)
vertex a,b;
{
    return((a[0]*b[0])+(a[1]*b[1])+(a[2]*b[2]));
}

float norm(a)
vertex a;
{
    return(sqrt(sqr(a[0])+sqr(a[1])+sqr(a[2])));
}
float vnorm(a)
vector a;
{
    return(sqrt(sqr(a[0])+sqr(a[1])+sqr(a[2])));
}

void normalize(a)
vector a;
{
    float snorm;

    snorm = norm(a);
    a[0] /=snorm;
    a[1] /=snorm;
    a[2] /=snorm;
}
void vnormalize(a)
vector a;
{
    float snorm;

    snorm = vnorm(a);
    a[0] /=snorm;
    a[1] /=snorm;
    a[2] /=snorm;
    a[3] = 1;
}
void cross_product(a,b,c)
vertex a,b,c;
{
    float amag,bmag,cmag;

    c[0] = ((a[1]*b[2])-(a[2]*b[1]));
    c[1] = -((a[0]*b[2])-(b[0]*a[2]));
    c[2] = ((a[0]*b[1])-(b[0]*a[1]));
}
void vcross_product(a,b,c)
vector a,b,c;
{
    float amag,bmag,cmag;

    c[0] = ((a[1]*b[2])-(a[2]*b[1]));
    c[1] = -((a[0]*b[2])-(b[0]*a[2]));
    c[2] = ((a[0]*b[1])-(b[0]*a[1]));
    c[3] = 1;
}

void clear_matrix(c)
float (*c)[4];
{
    int i,j;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            c[i][j] = 0;
}

void set_diag(c)
float (*c)[4];
{
    int i;

    for (i = 0; i < 4; i++)
        c[i][i] = 1;
}

void make_transl_matrix(x, y, z, c)
float x;
float y;
float z;
float (*c)[4];
{
    clear_matrix(c);
    set_diag(c);
    c[3][0] = x;
    c[3][1] = y;
    c[3][2] = z;
}

void make_scale_matrix(x, y, z, c)
float x;
float y;
float z;
float (*c)[4];
{
    clear_matrix(c);
    set_diag(c);
    c[0][0] = x;
    c[1][1] = y;
    c[2][2] = z;
}

void mat_mult(a, b, c)
float (*a)[4];
float (*b)[4];
float (*c)[4];
{
    int i,j,k;

    clear_matrix(c);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 4; k++) {
                c[i][j] = c[i][j] + (a[i][k] * b[k][j]);
            }
        }
    }
}
void vec_mult(a, b, c)
float a[4];
float (*b)[4];
float c[4];
{
    int i, j, k;

    for (i = 0; i < 4; i++) { ;
        c[i] = 0;
        for (j = 0; j < 4; j++) { ;
            c[i] = c[i] + (a[j] * b[j][i]);
        }
    }
}



void get_transf_matrix(tr) 
float (*tr)[4];
{
    matrix       tr1,m1a, m1, m2, m3, m4, m5, m6, m7, m8, m6a, m8a, m9;
    vector       rx, ry, rz, p2pr, p2dpr, p2tpr, p3pr, p3dpr, p3tpr, doppr,
doppr1, vrppr, vrppr1, coppr;
    float       l, d, d1, d2, d3, a, b, sinephi1, sinephi2, sinephi3, cosinephi1,
cosinephi2, cosinephi3, vpn_norm;
    int i,j,k;

    make_transl_matrix(-(vrp[0] + cop[0]), -(vrp[1] + cop[1]), -(vrp[2] +
cop[2]), m1a); 

    make_transl_matrix(-vrp[0], -vrp[1], -vrp[2], m1); 

    clear_matrix(m2); 
    set_diag(m2); 
    rz[0] = vpn[0]; 
    rz[1] = vpn[1]; 
    rz[2] = vpn[2]; 
    rz[3] = 1; 
    vnormalize(rz);
    vcross_product(vup, rz, rx); 
    vnormalize(rx);
    vcross_product(rx, rz, ry); 
    for (i = 0; i <= 2; i++) { ; 
        m2[i][0] = rx[i]; 
        m2[i][1] = ry[i]; 
        m2[i][2] = rz[i]; 
    } 

    clear_matrix(m5); 
    set_diag(m5); 
    m5[2][2] = -1; 

    vec_mult(vrp, m1a, vrppr); 
    vec_mult(vrppr, m2, vrppr1); 

    clear_matrix(m6a); 
    set_diag(m6a); 

    a = -(vrppr1[0] + (0.5 * (umax + umin))) / vrppr1[2]; 
    b = -(vrppr1[1] + (0.5 * (vmax + vmin))) / vrppr1[2]; 
    m6a[2][0] = a; m6a[2][1] = b; 

    vec_mult(vrppr1, m6a, vrppr); 
    make_scale_matrix((2 * vrppr[2] / ((vrppr[2] + back) * (umax - umin))),
    (2* vrppr[2] / ((vrppr[2] + back) * (vmax - vmin))),
    (1 / (vrppr[2] + back)),m8a);

    mat_mult(m1a, m2, tr); 
    mat_mult(tr, m5, tr1);
    mat_mult(tr1, m6a, tr); 
    mat_mult(tr, m8a, tr1); 
    zview = vrppr[2] / (vrppr[2] + back); 
    clear_matrix(m9); 
    set_diag(m9); 
    m9[3][3] = 0; 
    m9[2][3] = 1 / zview; 
    mat_mult(tr1, m9, tr); 
} 

draw_objects()
{  
    vector old[8],new[8],new1[8];
    int i,j,obs;

    cop[0] = settings.camera.x;
    cop[1] = settings.camera.y;
    cop[2] = settings.camera.z;
    cop[3] = 1;
    vrp[0] = settings.slate.x;
    vrp[1] = settings.slate.y;
    vrp[2] = settings.slate.z;
    vrp[3] = 1;

    vup[0] = 0; vup[1] = 0; vup[2] = 1; vup[3] = 1;

    vpn[0] = vrp[0] - cop[0];
    vpn[1] = vrp[1] - cop[1];
    vpn[2] = vrp[2] - cop[2];
    vpn[3] = 1;

    dist = sqrt(sqr(vpn[0]) + sqr(vpn[1]) + sqr(vpn[2]));

    aov = settings.zoom;
    umin = (-sin(pi * aov / 360) / cos(pi * aov / 360)) * dist;
    vmin = umin;
    umax = -umin;
    vmax = umax;

    get_transf_matrix(Nper);

    XSetForeground(theDisplay,cameraGC,YellowPen);
    XClearWindow(theDisplay,cameraWindow);
    for (obs=0;obs<num_objects;obs++) {
        for (i=0;i<8;i++) {
            old[i][0] = objects[obs].verts[i][0];
            old[i][1] = objects[obs].verts[i][1];
            old[i][2] = objects[obs].verts[i][2];
            old[i][3] = 1;
            vec_mult(old[i],Nper,new[i]); 
            new[i][0] = ((zview + (new[i][0] / new[i][3])) / 
                    (2 * zview) * WINDOW_H); 
            new[i][1] = WINDOW_V - ((zview + (new[i][1] / new[i][3])) /
                    (2 * zview) * WINDOW_V-1); 
        }
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[0][0],new[0][1],new[1][0],new[1][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[0][0],new[0][1],new[3][0],new[3][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[0][0],new[0][1],new[5][0],new[5][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[6][0],new[6][1],new[3][0],new[3][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[6][0],new[6][1],new[5][0],new[5][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[6][0],new[6][1],new[7][0],new[7][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[4][0],new[4][1],new[1][0],new[1][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[4][0],new[4][1],new[7][0],new[7][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[4][0],new[4][1],new[5][0],new[5][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[2][0],new[2][1],new[7][0],new[7][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[2][0],new[2][1],new[1][0],new[1][1]);
        XDrawLine(theDisplay,cameraWindow,cameraGC,new[2][0],new[2][1],new[3][0],new[3][1]);
    }
} 
