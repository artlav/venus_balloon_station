//#######################################################################################//
// Orbiter and generic util C
// Made in 2007-2009 by Artlav
//#######################################################################################//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "orbitersdk.h"
#include "util.h"
double sqr (double a){return a*a;}
double sgn(double a){if(a>=0)return 1; else return -1;}
int sgn(int a){if(a>=0)return 1; else return -1;}
double absd(double a){if(a<0)return -a; else return a;}
bool inrect(double x,double y,double xh,double yh,double xl,double yl){return ((x>=xh)&&(x<=xl)&&(y>=yh)&&(y<=yl));}
//#######################################################################################//
bool operator !=(VECTOR3 a,VECTOR3 b) {if((a.x!=b.x)||(a.y!=b.y)||(a.z!=b.z)) return true; else return false;}
VECTOR3 operator -(VECTOR3 &a,VECTOR3 &b){return _V(a.x-b.x,a.y-b.y,a.z-b.z);}
VECTOR3 operator +(VECTOR3 &a,VECTOR3 &b){return _V(a.x+b.x,a.y+b.y,a.z+b.z);}    
VECTOR3 operator /(VECTOR3 a,double b){return _V(a.x/b,a.y/b,a.z/b);}     
VECTOR3 operator *(VECTOR3 &a,double &b){return _V(a.x*b,a.y*b,a.z*b);}                     
VECTOR3 vmulv(VECTOR3 v1,VECTOR3 v2){return _V(v1.y*v2.z-v1.z*v2.y,-v1.x*v2.z+v1.z*v2.x,v1.x*v2.y-v1.y*v2.x);}
double  smulv(VECTOR3 v1,VECTOR3 v2){return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;}                
VECTOR3 dmulv(VECTOR3 v1,VECTOR3 v2){return _V(v1.x*v2.x,v1.y*v2.y,v1.z*v2.z);}
double  modv(VECTOR3 v){return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);}
VECTOR3 vrotz(VECTOR3 v,double e){double c,s;if(e==0)return v;c=cos(e);s=sin(e);return _V(v.x*c-v.y*s,v.x*s+v.y*c,v.z);} 
VECTOR3 vroty(VECTOR3 v,double e){double c,s;if(e==0)return v;c=cos(e);s=sin(e);return _V(v.x*c+v.z*s,v.y        ,-v.x*s+v.z*c);} 
VECTOR3 vrotx(VECTOR3 v,double e){double c,s;if(e==0)return v;c=cos(e);s=sin(e);return _V(v.x        ,v.y*c-v.z*s, v.y*s+v.z*c);} 
//#######################################################################################//
VECTOR3 nrvec(VECTOR3 iv)
{
 double md;
 VECTOR3 re;

 md=modv(iv);
 if (absd(md)<0.001) {re=iv; return re;}

 re.x=iv.x/md;
 re.y=iv.y/md;
 re.z=iv.z/md;
 return re;
}
//#######################################################################################//
