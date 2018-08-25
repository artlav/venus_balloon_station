//############################################################################//
#define ORBITER_MODULE
//############################################################################//
#include <math.h>
#include <stdio.h>
#include "orbitersdk.h" 
#include "UMmuSDK.h"
#include "orbitersdk.h"
//############################################################################//
#include "meshres.h" 
#include "vbs.h"
#include "util.h"
#include "keys.hpp"
//############################################################################//
#define ACTION_RAD 2
#define NOTE_TIME 2
//############################################################################//
vbs::vbs(OBJHANDLE hObj,int fmodel):VESSEL2(hObj,fmodel)
{
 int i;  
  
 for(i=0;i<10;i++)lndvo[i]=0;
 frun=1; 
 fix=0;
  
 polySides=24;
 polyX[ 0]=-128;polyZ[ 0]=  74;polyX[ 1]=-114;polyZ[ 1]=  82;polyX[ 2]= -85;polyZ[ 2]=  82;
 polyX[ 3]= -38;polyZ[ 3]=  55;polyX[ 4]=   0;polyZ[ 4]=  45;polyX[ 5]=  38;polyZ[ 5]=  55;
 polyX[ 6]=  85;polyZ[ 6]=  82;polyX[ 7]= 114;polyZ[ 7]=  82;polyX[ 8]= 128;polyZ[ 8]=  74; 
 polyX[ 9]= 129;polyZ[ 9]=  58;polyX[10]= 114;polyZ[10]=  33;polyX[11]=  66;polyZ[11]=   5;
 polyX[12]=  38;polyZ[12]= -23;polyX[13]=  28;polyZ[13]= -60;polyX[14]=  28;polyZ[14]=-115;
 polyX[15]=  15;polyZ[15]=-140;polyX[16]=   0;polyZ[16]=-148;polyX[17]= -15;polyZ[17]=-140;
 polyX[18]= -28;polyZ[18]=-115;polyX[19]= -28;polyZ[19]= -60;polyX[20]= -38;polyZ[20]= -23;
 polyX[21]= -66;polyZ[21]=   5;polyX[22]=-144;polyZ[22]=  33;polyX[23]=-129;polyZ[23]=  58;
 
 for(i=0;i<3;i++){
  anim_proc[i]=0.0;anim_status[i]=0;
 }
 anim_t[0]=100;
 anim_t[1]=100; 
 anim_t[2]=10;
 

 ccam=bccam=0;
 
 camdefpos[0]=_V(0,2,-93);
 camdefrot[0]=_V(1,0,0);
 
 camdefpos[1]=_V(0,19,-93);
 camdefrot[1]=_V(1,0,0);
 
 camdefpos[2]=_V(0,-330.3,-143.7);
 camdefrot[2]=_V(0,0,1);
 camdefpos[3]=_V(0,0,500);
 camdefrot[3]=_V(0,0,1);
 
 cama=0;
 campos=_V(0,0,0);
 camrot=_V(1,0,0);
 
 alpos[0]=_V(39,1,36);
 arst=true;
 
 actv[0]=_V(37.45,1,-19.1);//RW
 actv[1]=_V(-3.26,1,35.1); //Probe
 actv[2]=_V(1.87,1,38.55); //Dropper
 actv[3]=_V(-32.42,1,44.8);//Rocket Launcher
 actv[4]=_V(39,1,36);      //Airlock B
 actd[0]=_V(1,0,0);
 actd[1]=_V(0,0,1);
 actd[2]=_V(0,0,1);
 actd[3]=_V(1,0,0);
 actd[4]=_V(1,0,0);
 act_dt=0;
 
 DefineAnimations();
 
 note=oapiCreateAnnotation(0,0.6,_V(1,1,0));
 note_dt=0;
}
//############################################################################//
vbs::~vbs(){}
//############################################################################//
void vbs::clbkSetClassCaps (FILEHANDLE cfg)
{
 int i;

 SetSize(305.0);
 SetPMI(_V(36950,22800,37300));
 SetEmptyMass (EMPTY_MASS);
 SetCW(0.2,0.2,1.5,1.5);
 SetCrossSections (_V(18249,44244,25924));
 SetRotDrag (_V(0.3, 0.3, 0.3));
 SetSurfaceFrictionCoeff(0.7,0.7);
 SetCameraOffset(_V(0,0,0));
     
 SetTouchdownPoints(_V(0,-50,10),_V(-10,-50,-7),_V(10,-50,-7));
 EnableTransponder(true); 
 bool render_cockpit=true;

 InitNavRadios(1);

 ph_main=CreatePropellantResource(MAX_MAIN_FUEL);//BST tank
 ph_rcs =CreatePropellantResource(MAX_RCS_FUEL); //RCS tank

 //BST
 th_main =CreateThruster(_V(0,0,0),_V(0,1,0),MAX_main_THRUST,ph_main,main_ISP);
 thg_main=CreateThrusterGroup(&th_main,1,THGROUP_MAIN);

 th_mainr =CreateThruster(_V(0,0,0),_V(0,-1,0),MAX_main_THRUST,ph_main,main_ISP);
 thg_mainr=CreateThrusterGroup(&th_mainr,1,THGROUP_RETRO);

 //RCS
 THRUSTER_HANDLE th_att_rot[4], th_att_lin[4];
 th_att_rot[0] = CreateThruster (_V(-6, 0.7,-0.4), _V(0,-1,0), MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_rot[1] = CreateThruster (_V( 6,-0.7,-0.4), _V(0, 1,0), MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_rot[2] = CreateThruster (_V(-6,-0.7,-0.4), _V(0, 1,0), MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_rot[3] = CreateThruster (_V( 6, 0.7,-0.4), _V(0,-1,0), MAX_RCS_THRUST, ph_rcs, ISP);
 CreateThrusterGroup (th_att_rot,   2, THGROUP_ATT_BANKLEFT);
 CreateThrusterGroup (th_att_rot+2, 2, THGROUP_ATT_BANKRIGHT);
 for (i = 0; i < 4; i++) AddExhaust (th_att_rot[i], 0.7, 0.08);
 th_att_rot[0] = th_att_lin[0] = CreateThruster (_V(0,0, 15), _V(0, 1,0), MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_rot[1] = th_att_lin[2] = CreateThruster (_V(0,0,-15), _V(0,-1,0), MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_rot[2] = th_att_lin[3] = CreateThruster (_V(0,0, 15), _V(0,-1,0), MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_rot[3] = th_att_lin[1] = CreateThruster (_V(0,0,-15), _V(0, 1,0), MAX_RCS_THRUST, ph_rcs, ISP);
 CreateThrusterGroup (th_att_rot,   2, THGROUP_ATT_PITCHUP);
 CreateThrusterGroup (th_att_rot+2, 2, THGROUP_ATT_PITCHDOWN);
 CreateThrusterGroup (th_att_lin,   2, THGROUP_ATT_UP);
 CreateThrusterGroup (th_att_lin+2, 2, THGROUP_ATT_DOWN);
 th_att_rot[0] = th_att_lin[0] = CreateThruster (_V(0,0, 15), _V( 1,0,0), MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_rot[1] = th_att_lin[2] = CreateThruster (_V(0,0,-15), _V(-1,0,0), MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_rot[2] = th_att_lin[3] = CreateThruster (_V(0,0, 15), _V(-1,0,0), MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_rot[3] = th_att_lin[1] = CreateThruster (_V(0,0,-15), _V( 1,0,0), MAX_RCS_THRUST, ph_rcs, ISP);
 CreateThrusterGroup (th_att_rot,   2, THGROUP_ATT_YAWRIGHT);
 CreateThrusterGroup (th_att_rot+2, 2, THGROUP_ATT_YAWLEFT);
 CreateThrusterGroup (th_att_lin,   2, THGROUP_ATT_RIGHT);
 CreateThrusterGroup (th_att_lin+2, 2, THGROUP_ATT_LEFT);
 th_att_lin[0] = CreateThruster (_V( 0,0,0), _V(0,0, 1), 2*MAX_RCS_THRUST, ph_rcs, ISP);
 th_att_lin[1] = CreateThruster (_V( 0,0,0), _V(0,0,-1), 2*MAX_RCS_THRUST, ph_rcs, ISP);
 CreateThrusterGroup (th_att_lin,   1, THGROUP_ATT_FORWARD);
 CreateThrusterGroup (th_att_lin+1, 1, THGROUP_ATT_BACK);

 SetMeshVisibilityMode(AddMesh(exmesh_tpl=oapiLoadMeshGlobal("shukra\\shukra")),MESHVIS_EXTERNAL|MESHVIS_COCKPIT); 
     
 for(i=0;i<10;i++){ 
  ltsp[i]=_V(-73+i*16.22,1.5,-33.1);ltsc[i]=_V(1,1,1);
  lts[i].shape=BEACONSHAPE_DIFFUSE;
  lts[i].pos=&ltsp[i];lts[i].col=&ltsc[i];
  lts[i].size=0.5;lts[i].falloff=0.6;
  lts[i].period=0;lts[i].duration=0.1;lts[i].tofs=0.1*i;
  lts[i].active=true;
  AddBeacon(&lts[i]); 
 }    
 for(i=10;i<20;i++){
  ltsp[i]=_V(-73+(i-10)*16.22,2.5,-33.1);ltsc[i]=_V(1,0,0);
  lts[i].shape=BEACONSHAPE_DIFFUSE;
  lts[i].pos=&ltsp[i];lts[i].col=&ltsc[i];
  lts[i].size=0.5;lts[i].falloff=0.6;
  lts[i].period=1;lts[i].duration=0.1;lts[i].tofs=0.1*(i-10);
  lts[i].active=true;
  AddBeacon(&lts[i]);
 }
 for(i=20;i<26;i++){
  ltsp[i]=_V(-73,1.5,-23-4*(i-20));ltsc[i]=_V(0,1,0);
  lts[i].shape=BEACONSHAPE_DIFFUSE;
  lts[i].pos=&ltsp[i];lts[i].col=&ltsc[i];
  lts[i].size=0.5;lts[i].falloff=0.6;
  lts[i].period=0;lts[i].duration=0;lts[i].tofs=0;
  lts[i].active=true;
  AddBeacon(&lts[i]);
 }
 for(i=26;i<32;i++){
  ltsp[i]=_V(73,1.5,-23-4*(i-26));ltsc[i]=_V(0,1,0);
  lts[i].shape=BEACONSHAPE_DIFFUSE;
  lts[i].pos=&ltsp[i];lts[i].col=&ltsc[i];
  lts[i].size=0.5;lts[i].falloff=0.6;
  lts[i].period=0;lts[i].duration=0;lts[i].tofs=0;
  lts[i].active=true;
  AddBeacon(&lts[i]);
 }
 
 ltsp[32]=_V(129,0,74);ltsc[32]=_V(1,0,0);
 lts[32].shape=BEACONSHAPE_DIFFUSE;lts[32].pos=&ltsp[32];lts[32].col=&ltsc[32];
 lts[32].size=0.5;lts[32].falloff=0.6;lts[32].period=1;lts[32].duration=0.5;lts[32].tofs=0;
 lts[32].active=true;
 AddBeacon(&lts[32]);
 
 ltsp[33]=_V(-129,0,74);ltsc[33]=_V(1,0,0);
 lts[33].shape=BEACONSHAPE_DIFFUSE;lts[33].pos=&ltsp[33];lts[33].col=&ltsc[33];
 lts[33].size=0.5;lts[33].falloff=0.6;lts[33].period=1;lts[33].duration=0.5;lts[33].tofs=0.33;
 lts[33].active=true;
 AddBeacon(&lts[33]);
 
 ltsp[34]=_V(0,0,-148);ltsc[34]=_V(1,0,0);
 lts[34].shape=BEACONSHAPE_DIFFUSE;lts[34].pos=&ltsp[34];lts[34].col=&ltsc[34];
 lts[34].size=0.5;lts[34].falloff=0.6;lts[34].period=1;lts[34].duration=0.5;lts[34].tofs=0.66;
 lts[34].active=true;
 AddBeacon(&lts[34]);
 
 
 //UMMUinit
 UmmuSdKInitReturnCode=Crew.InitUmmu(GetHandle()); 
 float UMmuVersion=Crew.GetUserUMmuVersion();
 // first parameter is airlock door state (OPEN=TRUE) and other are X,X1,Y,Y1 and Z,Z1 
 // min and max coordinate for this virtual box (vessel local coordinate)
 Crew.DefineAirLockShape(arst,(float)alpos[0].x-4,(float)alpos[0].x+4,(float)alpos[0].y-1,(float)alpos[0].y+5,(float)alpos[0].z-4,(float)alpos[0].z+4);   
 Crew.SetMembersPosRotOnEVA(alpos[0],_V(0,1,0)); 
 Crew.SetMaxSeatAvailableInShip(20);

 Crew.AddCrewMember("John Smith",41,65,74,"Capt");  //(for name and id a-z A-Z 0-9 characters only)
 Crew.AddCrewMember("Ivan Sarkashwilly",27,67,55,"Eng"); //(for name and id a-z A-Z 0-9 characters only)
 Crew.AddCrewMember("George McStitch",15,70,45,"Doc"); //(for name and id a-z A-Z 0-9 characters only)
}     
//############################################################################//
void vbs::DefineAnimations()
{
 static UINT AnimRwGrp1[1]={GRP_Strip_LR};
 static UINT AnimRwGrp2[1]={GRP_Strip_RR};
 static MGROUP_ROTATE AnimRwGrpa1(0,AnimRwGrp1,1,_V( 24,0,-33.108),_V(0,1,0),(float)(-53*RAD));
 static MGROUP_ROTATE AnimRwGrpa2(0,AnimRwGrp2,1,_V(-24,0,-33.108),_V(0,1,0),(float)( 53*RAD));
 anims[0]=CreateAnimation(0);
 AddAnimationComponent(anims[0],0,1,&AnimRwGrpa1);
 AddAnimationComponent(anims[0],0,1,&AnimRwGrpa2);
 
 static UINT AnimPrbGrp1[1]={GRP_Probe};
 static UINT AnimPrbGrp2[1]={GRP_Rope};
 static MGROUP_TRANSLATE AnimPrbGrpa1(0,AnimPrbGrp1,1,_V(0,-10000,0));
 static MGROUP_SCALE AnimPrbGrpa2(0,AnimPrbGrp2,1,_V(-2.685,2.045,48.201),_V(0,333.333,0));
 anims[1]=CreateAnimation(0);
 AddAnimationComponent(anims[1],0,1,&AnimPrbGrpa1);
 AddAnimationComponent(anims[1],0,1,&AnimPrbGrpa2);
 
 static UINT AnimDrpGrp1[1]={GRP_Platf};
 static MGROUP_TRANSLATE AnimDrpGrpa1(0,AnimDrpGrp1,1,_V(0,0.34,0));
 static MGROUP_TRANSLATE AnimDrpGrpa2(0,AnimDrpGrp1,1,_V(0,0,8.5));
 static MGROUP_ROTATE    AnimDrpGrpa3(0,AnimDrpGrp1,1,_V(2,1.37,50.56),_V(1,0,0),(float)(160*RAD));
 anims[2]=CreateAnimation(0);
 AddAnimationComponent(anims[2],  0,0.1,&AnimDrpGrpa1);
 AddAnimationComponent(anims[2],0.1,0.9,&AnimDrpGrpa2);
 AddAnimationComponent(anims[2],0.9,  1,&AnimDrpGrpa3);
};
//############################################################################//
void vbs::process_ummu()
{
 int ReturnCode=Crew.ProcessUniversalMMu();
 switch(ReturnCode){
 case UMMU_TRANSFERED_TO_OUR_SHIP: 
  //sprintf(SendFocusScreenMessage(),"%s \"%s\" aged %i was transfered to our ship",Crew.GetCrewMiscIdByName(Crew.GetLastEnteredCrewName()),Crew.GetLastEnteredCrewName(),Crew.GetCrewAgeByName(Crew.GetLastEnteredCrewName()));
  break;
 case UMMU_RETURNED_TO_OUR_SHIP:
  //sprintf(SendFocusScreenMessage(),"%s \"%s\" aged %i entered into our ship",Crew.GetCrewMiscIdByName(Crew.GetLastEnteredCrewName()),Crew.GetLastEnteredCrewName(),Crew.GetCrewAgeByName(Crew.GetLastEnteredCrewName()));
  break;
 }
}
//############################################################################//
void vbs::set_internal_camera()
{
 if(ccam==0){
  SetCameraOffset(camdefpos[0]+campos);
  SetCameraDefaultDirection(camrot,cama);
 }else if((ccam>0)&&(ccam<4)){
  SetCameraOffset(camdefpos[ccam]);
  SetCameraDefaultDirection(camdefrot[ccam]);
 }
}   
//############################################################################//
void vbs::clbkLoadStateEx(FILEHANDLE scn,void *vs)
{
 char *line;
 int n=0;

 while(oapiReadScenario_nextline(scn,line)){
  /*if(!strnicmp(line,"DT",2)){
   sscanf(line+2,"%lf",&dt);
  }else if(!strnicmp(line,"CNT",3)){
   sscanf(line+3,"%d",&cnt);
  }else if(!strnicmp(line,"LIM",3)){
   sscanf(line+3,"%d",&lim);
  }else*/ ParseScenarioLineEx(line,vs);
 }
}
//############################################################################//
void vbs::clbkSaveState(FILEHANDLE scn)
{
 //char cbuf[256];

 SaveDefaultState(scn);
 //sprintf(cbuf,"%.3f",dt);oapiWriteScenario_string(scn,"DT",cbuf);
 //sprintf(cbuf,"%d",cnt);oapiWriteScenario_string(scn,"CNT",cbuf);
 //sprintf(cbuf,"%d",lim);oapiWriteScenario_string(scn,"LIM",cbuf);
}
//############################################################################//
void vbs::clbkDrawHUD(int mode,const HUDPAINTSPEC *hps,HDC hDC)
{
 VESSEL2::clbkDrawHUD(mode,hps,hDC);
}   
//############################################################################//
int  vbs::clbkConsumeDirectKey(char *kstate)
{
 if(KEYMOD_ALT(kstate)){
  if(KEYDOWN(kstate,OAPI_KEY_J)){camrot=vroty(camrot,-PI/100);set_internal_camera();RESETKEY(kstate,OAPI_KEY_J);}
  if(KEYDOWN(kstate,OAPI_KEY_L)){camrot=vroty(camrot, PI/100);set_internal_camera();RESETKEY(kstate,OAPI_KEY_L);}
  if(KEYDOWN(kstate,OAPI_KEY_I)){campos=campos+(camrot*0.1);  set_internal_camera();RESETKEY(kstate,OAPI_KEY_I);}
  if(KEYDOWN(kstate,OAPI_KEY_K)){campos=campos-(camrot*0.1);  set_internal_camera();RESETKEY(kstate,OAPI_KEY_K);}
 }
 return 0;
}
//############################################################################//
void vbs::toggle_anim(int n)
{
 if(anim_status[n]==0){anim_status[n]=1;return;}
 if(anim_status[n]==3){anim_status[n]=2;return;}
 if(anim_status[n]==1){anim_status[n]=2;return;}
 if(anim_status[n]==2){anim_status[n]=1;return;}
}
//############################################################################//
int  vbs::clbkConsumeBufferedKey(DWORD key,bool down,char *kstate)
{
 if(!down)return 0;
 if (KEYMOD_SHIFT(kstate)){
 } else if(KEYMOD_CONTROL(kstate)){
  switch(key){
   case OAPI_KEY_DIVIDE:SetAttitudeMode(GetAttitudeMode()>= 1? 0:1);return 1;
  }
 }else if(KEYMOD_ALT(kstate)){
  switch(key){
  case OAPI_KEY_1:ccam=0;set_internal_camera();return 1;
  case OAPI_KEY_2:ccam=1;set_internal_camera();return 1;
  case OAPI_KEY_3:ccam=2;set_internal_camera();return 1;
  case OAPI_KEY_4:ccam=3;set_internal_camera();return 1;
  case OAPI_KEY_U:
   if(ccam==0){
    cama=0;
    campos=_V(0,0,0);
    camrot=_V(1,0,0);
    set_internal_camera();
   }
   return 1;
  case OAPI_KEY_C:toggle_anim(0);return 1;
  case OAPI_KEY_V:toggle_anim(1);return 1;
  case OAPI_KEY_B:toggle_anim(2);return 1;
  }
 }else{
  switch(key){
   case OAPI_KEY_DIVIDE:ToggleAttitudeMode();return 1;
  }
 }
 return 0;
}
//############################################################################//
//Land-attach vessels
void vbs::doattach(VESSEL* vo)
{
 VECTOR3 v1,gp,vp,go,vd,gf,vf;
 int i,c;
 
 c=-1;      
 for (i=0;i<10;i++) {
  if (lndvo[i]==vo->GetHandle()) return;
  if (lndvo[i]==0) {c=i;break;}
 }
 if (c==-1) return; 
 
 GetGlobalPos(gp);
 vo->GetGlobalPos(vp); 
 Local2Global(_V(0,1,0),go); 
 Local2Global(_V(1,0,0),gf); 
  
 vd=go-gp+vp;
 vo->Global2Local(vd,vd); 
 vf=gf-gp+vp;
 vo->Global2Local(vf,vf); 
 Global2Local(vp,v1);  
 
 lndvo[c]=vo->GetHandle();
 lndvg[c]=vo->CreateAttachment(true,_V(0,0,0),-vd,vf, "VBS");
 lndvh[c]=CreateAttachment(false,v1,_V(0,1,0),_V(1,0,0), "VBS"); 
 
 AttachChild(vo->GetHandle(),lndvh[c],lndvg[c]);
}
//############################################################################//
//Detach vessels
void vbs::dodeattach(VESSEL* vo,double vl)
{
 int i,c;  
 c=-1;      
 for (i=0;i<10;i++) if (lndvo[i]==vo->GetHandle()) {c=i;break;}
 if (c==-1) return; 
 
 DetachChild(lndvh[c],vl);

 lndvo[c]=0;
}
//############################################################################//
//Move vessels
void vbs::moveatt(VESSEL* vo,int d,double simdt)
{
 int i,c;  
 VECTOR3 pos,dir,rot,ps,gp,vp;
 
 c=-1;      
 for (i=0;i<10;i++) if (lndvo[i]==vo->GetHandle()) {c=i;break;}
 if (c==-1) return; 
               
 vo->GetGlobalPos(vp); 
 GetGlobalPos(gp); 
  
 if (d==0) ps=_V(0,0, 1);
 if (d==1) ps=_V(0,0,-1);
 
 if ((d==0)||(d==1)) {
  vo->Local2Global(ps,ps);
  ps=ps-vp+gp; 
  Global2Local(ps,ps);
  ps=ps*simdt*4;
  ps.y=0;
 }
 
 GetAttachmentParams(lndvh[c],pos,dir,rot);
 if ((d==0)||(d==1)) pos=pos+ps;
 if (d==2) rot=vroty(rot,-simdt*(PI/2));
 if (d==3) rot=vroty(rot, simdt*(PI/2));
 SetAttachmentParams(lndvh[c],pos,dir,rot);
 
 if ((!floordet(pos.x,pos.z))) {
  dodeattach(vo,3);
  if (d==0) vo->AddForce(_V(0,0, vo->GetMass()*4/simdt),_V(0,0,0));
  if (d==1) vo->AddForce(_V(0,0,-vo->GetMass()*4/simdt),_V(0,0,0));
 }
}
//############################################################################//
//Actions
void vbs::doact(VESSEL* vo)
{
 int i,c;  
 VECTOR3 pos,dir,rot;
 
 c=-1;      
 for (i=0;i<10;i++) if (lndvo[i]==vo->GetHandle()) {c=i;break;}
 if (c==-1) return;     
 if (act_dt>0) return; 
 
 GetAttachmentParams(lndvh[c],pos,dir,rot);
 
 for (i=0;i<5;i++) if (modv(pos-actv[i])<ACTION_RAD) {
  if (i==0) toggle_anim(0);
  if (i==1) toggle_anim(1);
  if (i==2) toggle_anim(2);
  act_dt=1;
 }                      
}
//############################################################################//
//Notes for actions
void vbs::checkloc(VESSEL* vo)
{
 int i,c;  
 VECTOR3 pos,dir,rot;
 
 c=-1;      
 for (i=0;i<10;i++) if (lndvo[i]==vo->GetHandle()) {c=i;break;}
 if (c==-1) return;      
 
 GetAttachmentParams(lndvh[c],pos,dir,rot);
                       
 for (i=0;i<5;i++) if (modv(pos-actv[i])<ACTION_RAD) {
  if (i==0) {note_dt=NOTE_TIME;oapiAnnotationSetText(note,"Press E to stow/retract runway.");}
  if (i==1) {note_dt=NOTE_TIME;oapiAnnotationSetText(note,"Press E to stow/retract athmospheric probe.\n");}
  if (i==2) {note_dt=NOTE_TIME;oapiAnnotationSetText(note,"Press E to cycle dropper.\n");}
  if (i==3) {note_dt=NOTE_TIME;oapiAnnotationSetText(note,"Press E to launch probe rocket.\n");}
  if (i==4) {note_dt=NOTE_TIME;oapiAnnotationSetText(note,"Press E to enter Hab B.\n");}
 }                      
}      
//############################################################################//
//Polygon detection
bool vbs::pointinpolygon(double x,double z)
{
 int i,j;
 bool oddNodes;
 
 oddNodes=false;
 j=polySides-1;

 for (i=0;i<polySides;i++) {
  if ((polyZ[i]<z)&&(polyZ[j]>=z)||(polyZ[j]<z)&&(polyZ[i]>=z)) {
   if (polyX[i]+(z-polyZ[i])/(polyZ[j]-polyZ[i])*(polyX[j]-polyX[i])<x) {
    oddNodes=!oddNodes;
   }
  }
  j=i; 
 }
 return oddNodes;
}
//############################################################################//
//Floor detection
bool vbs::floordet(double x,double z)
{
 if (sqrt(sqr(x)+sqr(z))<17) return false;
 if (pointinpolygon(x,z)) return true;
 if (inrect(x,z,-73,-43,73,-23)) return true;
 return false;
}
//############################################################################//
//Collision detection      
void vbs::colldet(double simt,double simdt,double mjd)
{
 VESSEL *vo;
 OBJHANDLE vob;
 VECTOR3 gpos,tgpos,goff,gvel,vpos,vvel,gvv,lvv,p[3],gp1,gp2,gp3,rv,d1,d2,dvv;      
 VECTOR3 dir,posp[3],pos,pmi;
 double dstp[3],dstc,pow,siz,f,prd,pm,r,g,alt;;
 int is_col,is_inn[3],is_oru,c,i,j,is_att; 
 
 VECTOR3 vel,iwit,ip,tv;
 double inn,iden,vm;             

 c=oapiGetVesselCount();
 if (c!=0) for (i=0;i<c;i++) {                               
  vob=oapiGetVesselByIndex(i); 
  vo=oapiGetVesselInterface(vob); 
  vo->GetGlobalVel(vvel);
  vo->GetGlobalPos(vpos); 
    
  GetGlobalVel(gvel);
  GetGlobalPos(gpos);
  Local2Global(_V(0,1,0),goff);
  tgpos=gpos;   
                    
  is_inn[0]=0;is_inn[1]=0;is_inn[2]=0;is_col=0;is_oru=0;is_att=0;
  if (vo->GetClassName()!=NULL) if (!strcmp(vo->GetClassName(),"DynamicPlanet")) is_oru=1;  
  for (j=0;j<10;j++) if (lndvo[j]==vo->GetHandle()) is_att=1;
  
  
  vo->GetTouchdownPoints(p[0],p[1],p[2]);  
  vo->Local2Global(p[0],gp1);vo->Local2Global(p[1],gp2);vo->Local2Global(p[2],gp3);    
  siz=vo->GetSize();    
  vm=vo->GetMass();  
  vo->GetPMI(pmi);   
  vo->GetAngularVel(rv);
  rv=-rv; 
  iwit=_V(3/(vm*pmi.x),3/(vm*pmi.y),3/(vm*pmi.z));   
  prd=oapiGetSize(GetSurfaceRef());
  pm=oapiGetMass(GetSurfaceRef());     
  oapiGetAltitude(GetHandle(),&alt);
  r=prd+alt;
  g=pm*GGRAV/(r*r);
  
    
  dstc=modv(vpos-gpos);
  dstp[0]=modv(gp1-gpos);dstp[1]=modv(gp2-gpos);dstp[2]=modv(gp3-gpos);
  
  Global2Local(vpos,pos);
  Global2Local(gp1,posp[0]);
  Global2Local(gp2,posp[1]);
  Global2Local(gp3,posp[2]);
  if ((dstc<200)&&(pos.y<(1+siz))&&(pos.y>(-1-siz))&&(absd(pos.x)<200)&&(absd(pos.z)<200)) {
   if ((posp[0].y<1)&&(posp[0].y>-1)&&floordet(posp[0].x,posp[0].z)) is_inn[0]=1;
   if ((posp[1].y<1)&&(posp[1].y>-1)&&floordet(posp[1].x,posp[1].z)) is_inn[1]=1;
   if ((posp[2].y<1)&&(posp[2].y>-1)&&floordet(posp[2].x,posp[2].z)) is_inn[2]=1;
   f=is_inn[0]+is_inn[1]+is_inn[2];
  }
                                   
  
  vo->Global2Local(vvel-gvel+vpos,dvv);                
  Global2Local(vvel-gvel+gpos,gvv);
  lvv=-gvv;
             
  Local2Global(_V(0,0,1),d1);d1=d1-gpos+vpos;vo->Global2Local(d1,d1);
  Local2Global(_V(1,0,0),d2);d2=d2-gpos+vpos;vo->Global2Local(d2,d2); 
  
  gpos=goff-gpos+vpos;
  vo->Global2Local(gpos,dir);
   
  if ((vo!=this)&&(!is_oru)&&(!is_att)) {
   //sprintf(oapiDebugString(),"Coldet: gvv.y=%.2lf",gvv.y);       
   //sprintf(oapiDebugString(),"Coldet: pos.x=%.2lf pos.y=%.2lf pos.z=%.2lf",pos.x,pos.y,pos.z);          

   for (j=0;j<3;j++) {
    vel=gvv+vmulv(rv,p[j]);
    inn=(-(1+0))*smulv(vel,dir);
    iden=1/vm+smulv(vmulv(dmulv(iwit,vmulv(p[j],dir)),p[j]),dir);
    ip=dir*(inn/iden);
    tv=ip*(1/simdt);
    //osh:=addv(ra[ii].sh,osh);
    if ((modv(tv)!=0)&&(is_inn[j])&&(gvv.y<0)) vo->AddForce(tv,p[j]);
   }
   if (is_inn[0]||is_inn[1]||is_inn[2]) {  
    r=1.1;
    if (f<3) r=0.9;      
    pow=vm*g*r;
    vo->AddForce(dir*pow,_V(0,0,0));  
     
    pow=vm*(-dvv.z/simdt)*(0.001+0.2*pks[190]);
    vo->AddForce(_V(0,0,1)*pow,_V(0,0,0));
    pow=vm*(-dvv.x/simdt)*(0.001+0.2*pks[190]);
    vo->AddForce(_V(1,0,0)*pow,_V(0,0,0));
    
    if ((modv(gvv)<0.5)&&(f==3)) { 
     vo->GetThrustVector(dir);   
     if (modv(dir)==0) doattach(vo);
    }
   }
   
   /*
   if(inn[0]){  
    pow=vo->GetMass()*g*1.01+vo->GetMass()*(-gvv.y/simdt);
    vo->AddForce(dir*(pow/f),p1);
   }
   if(inn[1]){   
    pow=vo->GetMass()*g*1.01+vo->GetMass()*(-gvv.y/simdt);
    vo->AddForce(dir*(pow/f),p2);
   }
   if(inn[2]){   
    pow=vo->GetMass()*g*1.01+vo->GetMass()*(-gvv.y/simdt);
    vo->AddForce(dir*(pow/f),p3);
   }
   */
  }else if (is_att) {
   vo->GetThrustVector(dir);
   if (modv(dir)>0) dodeattach(vo,0);
  }
 }
}
//############################################################################//
//Frame update
void vbs::clbkPostStep(double simt,double simdt,double mjd)
{
 VECTOR3 nv,dv;
 double prd,pm,r,g,n,alt;
 VECTOR3 v1,v2,v3,gp,cgp,clp;
 VESSEL *vo;
 int i;
 
 //Init
 if (frun) {
  set_internal_camera();    
  for (i=0;i<3;i++){ 
   if (anim_status[i]==0) {anim_status[i]=2;anim_proc[i]=simdt/(anim_t[i]*0.98);}
   if (anim_status[i]==3) {anim_status[i]=1;anim_proc[i]=1-simdt/(anim_t[i]*0.98);}  
  }
  frun=0;
 }
 if (bccam!=ccam) {bccam=ccam;set_internal_camera();}  
 
 act_dt=act_dt-simdt;  
 note_dt=note_dt-simdt;  
 if (note_dt<=0) oapiAnnotationSetText(note,"");
 
 //Beacons
 GetGlobalPos(gp);
 oapiCameraGlobalPos(&cgp);
 Global2Local(cgp,clp); 
 if (clp.x <0) for (i=10;i<20;i++) {lts[i].pos=&ltsp[29-i];lts[i].col=&ltsc[29-i];}
 if (clp.x>=0) for (i=10;i<20;i++) {lts[i].pos=&ltsp[   i];lts[i].col=&ltsc[   i];}          
 for (i=0;i<32;i++) lts[i].active=(anim_status[0]==0);
 
 //Animation
 for (i=0;i<3;i++) {
  if (anim_status[i]==1) {anim_proc[i]+=simdt/anim_t[i];if (anim_proc[i]>1.0) {anim_proc[i]=1;anim_status[i]=3;}}
  if (anim_status[i]==2) {anim_proc[i]-=simdt/anim_t[i];if (anim_proc[i]<0.0) {anim_proc[i]=0;anim_status[i]=0;}}
  SetAnimation(anims[i],anim_proc[i]);
 }
 
 //Accel
 if (oapiGetTimeAcceleration()>=10) {
  if (fix) {  
   //DefSetStateEx(&vcs);  
  }else{
   fix=1;
   vcs.version=2;
   vcs.flag=0;
   
   
   GetTouchdownPoints(v1,v2,v3);  
   oapiGetEquPos(GetHandle(),&clon,&clat,&calt);
   oapiGetAltitude(GetHandle(),&alt);
   oapiGetSize(GetSurfaceRef());
                                 
   SetTouchdownPoints(_V(0,-alt,10),_V(-10,-alt,-7),_V(10,-alt,-7));
   SetSize(alt*1.5);
   
   
   GetStatusEx(&vcs);
   
   vcs.surf_lat=clat;
   vcs.surf_lng=clon;
   vcs.status=1;  
   DefSetStateEx(&vcs);
  } 
 }else{
  if (fix) { 
   fix=0;  
   vcs.version=2;
   vcs.flag=0;
   
   GetStatusEx(&vcs);  
   vcs.status=0; 
   DefSetStateEx(&vcs);   
   
   SetTouchdownPoints(_V(0,-50,10),_V(-10,-50,-7),_V(10,-50,-7));
  }else{
  }  
 }
 
 //Hover
 oapiGetGlobalPos(GetSurfaceRef(),&nv);
 Global2Local(nv,dv);
 dv=nrvec(dv);
  
 prd=oapiGetSize(GetSurfaceRef());
 pm=oapiGetMass(GetSurfaceRef());

 oapiGetAltitude(GetHandle(),&alt);
  
 r=prd+alt;
 g=pm*GGRAV/(r*r);
 n=GetMass()*g;

 if (!fix) AddForce(dv*(-n),_V(0,0,0));
 
 colldet(simt,simdt,mjd);
       
            
 //38-40=up-down
 //35-34=left-right
 
 //65-68 A D
 //87-83 W S
 //69 E 77 M
 if (pks[38]||pks[40]||pks[35]||pks[34]||pks[65]||pks[68]||pks[87]||pks[83]||pks[69]) {  
  vo=oapiGetFocusInterface();   
  if (vo) if (vo->GetClassName()!=NULL) if (!strcmp(vo->GetClassName(),"UMmu")) {
   if (pks[38]||pks[87]) moveatt(vo,0,simdt);
   if (pks[40]||pks[83]) moveatt(vo,1,simdt);
   if (pks[35]||pks[65]) moveatt(vo,2,simdt);
   if (pks[34]||pks[68]) moveatt(vo,3,simdt);   
   if (pks[69]) doact(vo);
  } 
 }    
 vo=oapiGetFocusInterface();   
 if (vo) if (vo->GetClassName()!=NULL) if (!strcmp(vo->GetClassName(),"UMmu")) checkloc(vo);
      
 for (i=0;i<10;i++) if (lndvo[i]) if (!oapiIsVessel(lndvo[i])) lndvo[i]=0;

 process_ummu();
}
//############################################################################//
DLLCLBK void InitModule(HINSTANCE hDLL){InitKeys(hDLL);}
DLLCLBK void ExitModule(HINSTANCE hModule){}
DLLCLBK VESSEL *ovcInit(OBJHANDLE hvessel,int flightmodel){return new vbs(hvessel,flightmodel);}
DLLCLBK void ovcExit(VESSEL *vessel){if(vessel)delete(vbs*)vessel;}
//############################################################################//
