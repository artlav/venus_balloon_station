//############################################################################//
HHOOK trap=0;
int pks[256];
void (*kskeydown)(int)=0;
void (*kskeyup)(int)=0;
//############################################################################//
LRESULT CALLBACK keyspy(int Code,WPARAM wParam,LPARAM lParam)
{
 if(!trap) return 1;
 if(Code<0) return CallNextHookEx(trap,Code,wParam,lParam);

 //Keydown
 if ((((lParam)>>30)&1)==0){pks[wParam]=1;if(kskeydown)kskeydown(wParam);}

 //Keyup
 if ((((lParam)>>31)&1)==1){pks[wParam]=0;if(kskeyup)kskeyup(wParam);}
 return CallNextHookEx(trap,Code,wParam,lParam);
}
//############################################################################//
void InitKeys(HINSTANCE hDLL)
{
 for(int i=0;i<255;i++) pks[i]=0;
 trap=SetWindowsHookEx(WH_KEYBOARD,HOOKPROC(keyspy),hDLL,GetCurrentThreadId());
}
//############################################################################//
int nsh()
{
 if (pks[VK_CONTROL]) return 0;
 if (pks[VK_SHIFT])   return 0;
 if (pks[VK_MENU])    return 0;
 return 1;
}
//############################################################################//
