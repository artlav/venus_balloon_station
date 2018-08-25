//############################################################################//
const double EMPTY_MASS   =1417275*1.2;
const double MAX_MAIN_FUEL=1000;
const double MAX_RCS_FUEL =1000;
const double FULL_MASS    =EMPTY_MASS+MAX_MAIN_FUEL+MAX_RCS_FUEL;
const double MAX_main_THRUST=FULL_MASS*9.8;
const double MAX_RCS_THRUST =FULL_MASS*1.5;
const double main_ISP=(400.5e3*MAX_main_THRUST)/(MAX_MAIN_FUEL*0.2);
const double ISP     =(2.0e3*MAX_RCS_THRUST)/(MAX_RCS_FUEL*1.5);
#define bcnt 35
//############################################################################//
class vbs:public VESSEL2{
public:
 vbs(OBJHANDLE hObj,int fmodel);
 ~vbs();
 void clbkSetClassCaps(FILEHANDLE cfg);
 void clbkLoadStateEx(FILEHANDLE scn,void *vs);
 void clbkSaveState(FILEHANDLE scn);
 void clbkPostStep(double simt,double simdt,double mjd);
 void clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, HDC hDC);
 int  clbkConsumeBufferedKey (DWORD key, bool down, char *kstate); 
 int  clbkConsumeDirectKey(char *kstate);
 void set_internal_camera(); 
 void DefineAnimations();
 void process_ummu();
 void doattach(VESSEL* vo);
 void dodeattach(VESSEL* vo,double vl); 
 void colldet(double simt,double simdt,double mjd);          
 bool pointinpolygon(double x,double z);
 bool floordet(double x,double z);
 void moveatt(VESSEL* vo,int d,double simdt); 
 void toggle_anim(int n);  
 void doact(VESSEL* vo);   
 void checkloc(VESSEL* vo);

 PROPELLANT_HANDLE ph_main,ph_rcs;
 THRUSTER_HANDLE th_main,th_mainr;
 THGROUP_HANDLE thg_main,thg_mainr;
 
 BEACONLIGHTSPEC lts[bcnt];
 VECTOR3 ltsp[bcnt],ltsc[bcnt];

 MESHHANDLE exmesh,exmesh_tpl;  //local external mesh and global template

 double dt,et,ft,stv;
 int inita,frun,fix;

 OBJHANDLE lnsr;
 double lnlat,lnlon;   
 
 VESSELSTATUS2 vcs;
 double calt,clon,clat;
 
 
 NOTEHANDLE note;
 double note_dt,act_dt;  
 
 
 double anim_proc[3],anim_t[3];
 UINT anim_status[3];
 UINT anims[3];
 
 VECTOR3 actv[5],actd[5];
 
 
 
 int ccam,bccam;
 double cama;
 VECTOR3 camdefpos[4],camdefrot[4],campos,camrot;//,phtpos,dhdpos[2],*ara;
 
 VECTOR3 alpos[3],alrot[3];
 int UmmuSdKInitReturnCode;
 UMMUCREWMANAGMENT Crew;
 int arst;
 
 ATTACHMENTHANDLE lndvh[10],lndvg[10];
 OBJHANDLE lndvo[10];
 
 
 int polySides;
 float polyX[24],polyZ[24];
};
//############################################################################//
int cnt=0,lim=128;
double gdt=0;
VESSEL* foc;
//############################################################################//
