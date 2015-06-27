#include "main.h"
#define sqr(X)   ((X)*(X))
#define cube(X)  ((X)*(X)*(X))
extern struct Mover *framelist;
extern Settings settings;

do_spline(start,end,n)
int start,end,n;
{
    float points[20][3],points1[20][3];
    int pointlist[20];
    int loop,i,j,k,ind,cmax;
    float f[4][3],m[20][23];
    float t1,t,pivot,max,temp,l[20],l1[20];
    
    ind = 0;
    if (settings.moving == CAMERA) {
        for (i=start; i<=end; i++) {
            if (framelist[i].movetype == SPLINESTART 
                || framelist[i].movetype == SPLINEPOINT
                || framelist[i].movetype == SPLINEEND
                || framelist[i].movetype == SPLINESTARTEND) {
                pointlist[ind] = i;
                   points[ind][0] = framelist[i].camera.x;
                   points[ind][1] = framelist[i].camera.y;
                   points[ind++][2] = framelist[i].camera.z;
            }
        }
    }
    else {
        for (i=start; i<=end; i++) {
            if (framelist[i].smovetype == SPLINESTART
                || framelist[i].smovetype == SPLINEPOINT
                || framelist[i].smovetype == SPLINEEND
                || framelist[i].smovetype == SPLINESTARTEND) {
                pointlist[ind] = i;
                   points[ind][0] = framelist[i].slate.x;
                   points[ind][1] = framelist[i].slate.y;
                   points[ind++][2] = framelist[i].slate.z;
            }
        }
    }

    for (i=0;i<n-1;i++) {
        l[i] = sqrt(sqr(points[i+1][0]-points[i][0])+
                    sqr(points[i+1][1]-points[i][1])+
                    sqr(points[i+1][2]-points[i][2]));
    }

    for (i=0;i<n;i++)
        for (j=0;j<n+3;j++)
            m[i][j] = 0;
            
    m[0][0] = 1.0;
    m[0][1] = 0.5;
    m[n-1][n-2] = 2;
    m[n-1][n-1] = 4;

    for (j=1;j<n-1;j++) {
        m[j][j-1] = l[j];
        m[j][j]   = 2*(l[j]+l[j-1]);
        m[j][j+1] = l[j-1];
        for (k=0;k<3;k++) {
            m[j][n+k] = 3*(sqr(l[j-1])*(points[j+1][k]-points[j][k])
                         + sqr(l[j])*(points[j][k]-points[j-1][k]));
            m[j][n+k] = m[j][n+k]/(l[j]*l[j-1]);
        }
    }
    
    for (i=0;i<3;i++) {
        m[0][i+n] = (3/(2*l[0]))*(points[1][i]-points[0][i]);
        m[n-1][i+n] = (6/l[n-2])*(points[n-1][i]-points[n-2][i]);
    }
    
    for (i = 0; i < n; i++) {   /* scale every row */ 
        max = m[i][0]; 
        for (j = 1; j < n+3; j++) /* first find largest magnitude in row */ 
            if (abs(m[i][j]) > abs(max)) max = m[i][j]; 
        for (j = 0; j < n+3; j++)   /* divide each row by scale factor */ 
            if (max != 0) m[i][j] = m[i][j] / max; 
    } 

    for (i = 0; i < n + 3 - 1; i++) {  
        max = m[i][i]; cmax = i; 
        for (j = i + 1; j < n; j++) { ; 
            if (abs(m[j][i]) > abs(max)) { ; 
                max = m[j][i]; 
                cmax = j; 
            } 
        } 
        if (cmax != i) { ;   /* swap rows, if necessary */ 
            for (j = 0; j < n+3; j++) { ; 
                temp = m[cmax][j]; 
                m[cmax][j] = m[i][j]; 
                m[i][j] = temp; 
            } 
        } 

    } 
    for (k = 0; k < n-1; k++) { 
        for (i = k + 1; i < n; i++) { 
            pivot = m[i][k]; 
            for (j = 0; j < n+3; j++) 
                if (m[k][k] != 0) m[i][j] = m[i][j] + (-1 * pivot / m[k][k]) *
                    (m[k][j]); 
        } 
    } 
    for (k = n-1; k > 0; k--) {
        for (i = k - 1; i > -1; i--) {  
            pivot = m[i][k]; 
            for (j = k; j < n+3; j++) {  
                if (m[k][k] != 0) m[i][j] = m[i][j] + (-1 * pivot / m[k][k]) *
                    (m[k][j]); 
            } 
        } 
    } 
    for (i = 0; i < n; i++) { 
        pivot = m[i][i]; 
        for (j = i; j < n+3; j++) 
           if (pivot != 0) m[i][j] = m[i][j] / pivot; 
    } 

    for (j=0;j<n-1;j++) {
        for (k=0;k<3;k++) {
            f[0][k] = points[j][k];
            f[1][k] = m[j][k+n];
            f[2][k] = ( (3.0 * (points[j+1][k]-points[j][k]))/ sqr(l[j])) -
                     (m[j+1][k+n]/l[j]) - ((2.0*m[j][k+n]) /l[j]);
            f[3][k] = ( (-2.0*(points[j+1][k]-points[j][k]))/cube(l[j]))
                      + (m[j+1][k+n]/sqr(l[j])) + (m[j][k+n]/sqr(l[j]));
        }
        t1 = l[j]/(pointlist[j+1]-pointlist[j]);
        for (k=pointlist[j]; k<pointlist[j+1];k++) {
            t=(float)(k-pointlist[j])*t1;
            if (settings.moving == CAMERA) {
                if (framelist[k].movetype == NONE)
                    framelist[k].movetype = SPLINE;
                framelist[k].camera.x = f[0][0]+(f[1][0]*t)+(f[2][0]*t*t)+
                                        (f[3][0]*t*t*t);
                framelist[k].camera.y = f[0][1]+(f[1][1]*t)+(f[2][1]*t*t)+
                                        (f[3][1]*t*t*t);
                framelist[k].camera.z = f[0][2]+(f[1][2]*t)+(f[2][2]*t*t)+
                                        (f[3][2]*t*t*t);
            }
            else {
                if (framelist[k].smovetype == NONE)
                    framelist[k].smovetype = SPLINE;
                framelist[k].slate.x = f[0][0]+(f[1][0]*t)+(f[2][0]*t*t)+
                                        (f[3][0]*t*t*t);
                framelist[k].slate.y = f[0][1]+(f[1][1]*t)+(f[2][1]*t*t)+
                                        (f[3][1]*t*t*t);
                framelist[k].slate.z = f[0][2]+(f[1][2]*t)+(f[2][2]*t*t)+
                                        (f[3][2]*t*t*t);
            }
        }
    }
} 
