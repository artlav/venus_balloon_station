//#######################################################################################//
// Orbiter and generic util H
// Made in 2007-2009 by Artlav
//#######################################################################################//
#ifndef _util_H
#define _util_H
double sqr(double a);
double sgn(double a);
int sgn(int a);
double absd(double a);

bool inrect(double x,double y,double xh,double yh,double xl,double yl);
//#######################################################################################//
   bool operator !=(VECTOR3  a,VECTOR3  b);
VECTOR3 operator - (VECTOR3 &a,VECTOR3 &b);
VECTOR3 operator + (VECTOR3 &a,VECTOR3 &b);
VECTOR3 operator / (VECTOR3  a,double  b);
VECTOR3 operator * (VECTOR3 &a,double &b);
 
VECTOR3 vmulv(VECTOR3 v1,VECTOR3 v2);
 double smulv(VECTOR3 v1,VECTOR3 v2);
VECTOR3 dmulv(VECTOR3 v1,VECTOR3 v2);

VECTOR3 vrotz(VECTOR3 v,double e);
VECTOR3 vroty(VECTOR3 v,double e);
VECTOR3 vrotx(VECTOR3 v,double e);
 double modv (VECTOR3 v);
VECTOR3 nrvec(VECTOR3 iv);
//#######################################################################################//
#endif
