#include "DIALOG.h"
#include "Config.h"
#include "MainTask.h"
#include "sysConf.h"
#include "dlg.h"
#include "skinColor.h"
#include "str.h"
#include "HSD_SLIDER.h"
#include "28.h"
#include "GUI.h"
#include "pwm.h"
//#include "fish.c"

#define ID_WINDOW_0         (GUI_ID_USER + 0x00)

#define ID_TEXT_TITLE       (GUI_ID_USER + 0x10)
#define ID_TEXT_NIGHT       (GUI_ID_USER + 0x11)
#define ID_TEXT_VOL         (GUI_ID_USER + 0x12)
#define ID_TEXT_BRIGHT      (GUI_ID_USER + 0x13)
#define ID_TEXT_ARMSND      (GUI_ID_USER + 0x14)
#define ID_TEXT_KEYSND      (GUI_ID_USER + 0x15)
#define ID_TEXT_UNIT        (GUI_ID_USER + 0x16)
#define ID_TEXT_SHAPE       (GUI_ID_USER + 0x17)
#define ID_TEXT_UPDATE      (GUI_ID_USER + 0x18)
#define ID_TEXT_VERSION     (GUI_ID_USER + 0x19)
#define ID_TEXT_VER         (GUI_ID_USER + 0x1a)

#define ID_SLIDER_SKIN      (GUI_ID_USER + 0x20)
#define ID_SLIDER_BRT       (GUI_ID_USER + 0x21)
#define ID_SLIDER_VOL       (GUI_ID_USER + 0x22)
#define ID_SLIDER_SNDARM    (GUI_ID_USER + 0x23)
#define ID_SLIDER_SNDKEY    (GUI_ID_USER + 0x24)
#define ID_SLIDER_UNIT      (GUI_ID_USER + 0x25)
#define ID_SLIDER_SHAPE     (GUI_ID_USER + 0x26)

extern GUI_CONST_STORAGE GUI_BITMAP bmfish;
GUI_POINT pPoint[] = {
	{0,0},
	{25,0},
	{35,8},
	{25,16},
	{0,16}
};

static void sldListenter(WM_MESSAGE * pMsg);

static const SysWin_COLOR * pSkin  = &SysWinSkins[0];
static void _OnSkinChanged(WM_MESSAGE * pMsg,int val);
static void _OnBrtChanged(WM_MESSAGE * pMsg,int val);
static void _OnVolChanged(WM_MESSAGE * pMsg,int val);
static void _OnArmSndChanged(WM_MESSAGE * pMsg,int val);
static void _OnKeySndChanged(WM_MESSAGE * pMsg,int val);
static void _OnUnitChanged(WM_MESSAGE * pMsg,int val);
static void _OnShapeChanged(WM_MESSAGE * pMsg,int val);


static CONF_SYS agentConf;


static  void (* const ProcChanging[7])(WM_MESSAGE *, int)  = {
   _OnSkinChanged, 
   _OnBrtChanged,
   _OnVolChanged,
   _OnArmSndChanged,
   _OnKeySndChanged,
   _OnUnitChanged,
   _OnShapeChanged
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[]  = 
{
 { WINDOW_CreateIndirect,  "Window",  ID_WINDOW_0,  SubWin_X,  SubWin_Y, SubWin_WIDTH, SubWin_HEIGHT,  0, 0, 0},
// { TEXT_CreateIndirect, "系统设置" ,ID_TEXT_TITLE,  0,   0,                                                             120,  30,  0, 0, 0},
// { TEXT_CreateIndirect, "夜间模式", ID_TEXT_NIGHT,  0,   Win_SysSet_txOrg,                                              120,  30,  0, 0, 0},
// { TEXT_CreateIndirect, "亮度设置", ID_TEXT_BRIGHT  ,  0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap),   120,  30,  0, 0, 0},
// { TEXT_CreateIndirect, "音量设置", ID_TEXT_VOL, 0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2, 120,  30,  0, 0, 0},
// { TEXT_CreateIndirect, "报警  音", ID_TEXT_ARMSND, 0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3, 120,  30,  0, 0, 0},
// { TEXT_CreateIndirect, "按键  音", ID_TEXT_KEYSND, 0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4, 120,  30,  0, 0, 0},
// { TEXT_CreateIndirect, "单位设置", ID_TEXT_UNIT,   0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5, 120,  30,  0, 0, 0},
// { TEXT_CreateIndirect, "船位设置", ID_TEXT_SHAPE,  0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6, 120,  30,  0, 0, 0},
// { TEXT_CreateIndirect, "软件更新", ID_TEXT_UPDATE, 0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7, 120,  30,  0, 0, 0},
// { TEXT_CreateIndirect, "系统版本", ID_TEXT_VERSION,0,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*8, 120,  30,  0, 0, 0},
 { TEXT_CreateIndirect, "左右",       ID_TEXT_VER,    75, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7+10, 120,  30, 0,  0, 0},
// 
 { HSD_SLIDER_CreateIndirect, " " ,ID_SLIDER_SKIN,   180,   Win_SysSet_txOrg,                                              120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, " ", ID_SLIDER_BRT  ,  180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap),   120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, " ", ID_SLIDER_VOL,    180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2, 120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, " ", ID_SLIDER_SNDARM, 180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3, 120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, " ", ID_SLIDER_SNDKEY, 180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4, 120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, " ", ID_SLIDER_UNIT,   180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5, 120,  30,  0, 0, 0},
 { HSD_SLIDER_CreateIndirect, " ", ID_SLIDER_SHAPE,  180,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6, 120,  30,  0, 0, 0}
// { HSD_SLIDER_CreateIndirect, " ", ID_BUTTON_UPDATE, 120,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7, 120,  30,  0, 0, 0},
};







static WM_HWIN Slideres[7];


static void  _cbDialog(WM_MESSAGE * pMsg)
{
   const WM_KEY_INFO * pInfo;
   WM_MESSAGE myMsg;
   
   WM_HWIN hItem;
   int Id;
   int i;
   GUI_RECT r;
      
   switch(pMsg->MsgId)
   {   
      case GUI_KEY_PWM_INC:
INFO("case pwm_inc");      
           if(agentConf.Brt < 5)        
           {
              agentConf.Brt++;
              PWM_SET(agentConf.Brt*2);
              HSD_SLIDER_SetValue(Slideres[1],agentConf.Brt);
           }
           break;
      case GUI_KEY_PWM_DEC:
INFO("case pwm_dec");      
           if(agentConf.Brt > 1)
           {
              agentConf.Brt--;
              PWM_SET(agentConf.Brt*2);
              HSD_SLIDER_SetValue(Slideres[1],agentConf.Brt);              
           }
           break;
           
      case USER_MSG_SKIN:
INFO("case msg skin");      
           pSkin  = &(SysWinSkins[pMsg->Data.v]);
           
           WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);
           for(i=7; i;)
           {
              i--;
              HSD_SLIDER_SetBkColor(Slideres[i], pSkin->sldBk);
						        HSD_SLIDER_SetFocusBkColor (Slideres[i], pSkin->sldBk);		
              HSD_SLIDER_SetSlotColor(Slideres[i], pSkin->sldSlot);
              HSD_SLIDER_SetFocusSlotColor(Slideres[i], pSkin->sldSlot); 
						 
              HSD_SLIDER_SetSliderColor(Slideres[i], pSkin->sldSlider);
              HSD_SLIDER_SetFocusSliderColor(Slideres[i], pSkin->sldFocusSlider);
           }
           
           break;
           
      case WM_INIT_DIALOG:
           pSkin  = &(SysWinSkins[SysConf.Skin]);
                //text
           hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VER);
           TEXT_SetFont(hItem,&GUI_Font24);
           TEXT_SetTextColor(hItem, pSkin->sldSlider);
           
						//WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);
			
           Slideres[0]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_SKIN);
           WM_SetCallback(Slideres[0], &sldListenter);
           HSD_SLIDER_SetRange(Slideres[0], SKIN_Day, SKIN_Night); 
           HSD_SLIDER_SetValue(Slideres[0], SysConf.Skin);
           
           Slideres[1]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_BRT);
           WM_SetCallback(Slideres[1], &sldListenter);
           HSD_SLIDER_SetNumTicks(Slideres[1], 5);
           HSD_SLIDER_SetRange(Slideres[1], 1, 5);
           HSD_SLIDER_SetValue(Slideres[1], SysConf.Brt);
           
           Slideres[2]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_VOL);
           WM_SetCallback(Slideres[2], &sldListenter);
           HSD_SLIDER_SetNumTicks(Slideres[2], 5);
           HSD_SLIDER_SetRange(Slideres[2], 0, 4);
           HSD_SLIDER_SetValue(Slideres[2], SysConf.Snd.Vol);
           
           Slideres[3]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_SNDARM);
           WM_SetCallback(Slideres[3], &sldListenter);
           HSD_SLIDER_SetNumTicks(Slideres[3],2);
           HSD_SLIDER_SetRange(Slideres[3], 1 , 2);
           HSD_SLIDER_SetValue(Slideres[3], SysConf.Snd.ArmSnd);
           
           Slideres[4]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_SNDKEY);
           WM_SetCallback(Slideres[4], &sldListenter);
           HSD_SLIDER_SetNumTicks(Slideres[4], 2);
           HSD_SLIDER_SetRange(Slideres[4], 1,  2);
           HSD_SLIDER_SetValue(Slideres[4], SysConf.Snd.KeySnd);
           
           Slideres[5]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_UNIT);
           WM_SetCallback(Slideres[5], &sldListenter);
           HSD_SLIDER_SetRange(Slideres[5], UNIT_nm,  UNIT_km);
           HSD_SLIDER_SetValue(Slideres[5], SysConf.Unit);
           
           Slideres[6]  = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_SHAPE);
           WM_SetCallback(Slideres[6], &sldListenter);
           HSD_SLIDER_SetRange(Slideres[6], SHAPE_Boat,  SHAPE_Fish);
           HSD_SLIDER_SetValue(Slideres[6], SysConf.Shape);
           
           WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);
           
           for(i=7; i; )
           {
              i--;
              HSD_SLIDER_SetBkColor(Slideres[i], pSkin->sldBk);
						        HSD_SLIDER_SetFocusBkColor (Slideres[i], pSkin->sldBk);		
						 
              HSD_SLIDER_SetSlotColor(Slideres[i], pSkin->sldSlot);
              HSD_SLIDER_SetFocusSlotColor(Slideres[i], pSkin->sldSlot); 
						 
              HSD_SLIDER_SetSliderColor(Slideres[i], pSkin->sldSlider);
              HSD_SLIDER_SetFocusSliderColor(Slideres[i], pSkin->sldFocusSlider);
           }           
           
           agentConf.Skin              = SysConf.Skin;
           agentConf.Brt               = SysConf.Brt;
           agentConf.Snd.Vol           = SysConf.Snd.Vol;
           agentConf.Snd.ArmSnd        = SysConf.Snd.ArmSnd;
           agentConf.Snd.KeySnd        = SysConf.Snd.KeySnd;
           agentConf.Unit              = SysConf.Unit;
           agentConf.Shape             = SysConf.Shape;
                 
           break;
           
           
       case USER_MSG_REPLY:
            if(pMsg->Data.v == REPLY_OK)
            {
               if(agentConf.Skin != SysConf.Skin)
               {
                  myMsg.MsgId  = USER_MSG_SKIN;
                  myMsg.Data.v  = agentConf.Skin;
                  WM_BroadcastMessage(&myMsg);
               }
               if(agentConf.Shape != SysConf.Shape)
               {
                  myMsg.hWin  = mapWin;
                  myMsg.MsgId  = USER_MSG_SHAPE;
                  myMsg.Data.v  = agentConf.Shape;
                  WM_SendMessage(myMsg.hWin, &myMsg);
               }
               if(agentConf.Unit != SysConf.Unit)
               {              
                  myMsg.MsgId  = USER_MSG_UNIT;
                  myMsg.Data.v  = agentConf.Unit;
                  WM_SendMessage(mapWin, &myMsg);
                  WM_SendMessage(WM_GetClientWindow(mntSettingWin), &myMsg);
               }
               
               SysConf.Brt             = agentConf.Brt;
               SysConf.Shape           = agentConf.Shape;
               SysConf.Skin            = agentConf.Skin;
               SysConf.Snd.Vol         = agentConf.Snd.Vol;
               SysConf.Snd.ArmSnd      = agentConf.Snd.ArmSnd;
               SysConf.Snd.KeySnd      = agentConf.Snd.KeySnd;
               SysConf.Unit            = agentConf.Unit;
               
               sysStore();
            }
            else 
            {
               /// Roll back
               HSD_SLIDER_SetValue(Slideres[0], agentConf.Skin);
               HSD_SLIDER_SetValue(Slideres[1], agentConf.Brt);
               HSD_SLIDER_SetValue(Slideres[2], agentConf.Snd.Vol);
               HSD_SLIDER_SetValue(Slideres[3], agentConf.Snd.ArmSnd);
               HSD_SLIDER_SetValue(Slideres[4], agentConf.Snd.KeySnd);
               HSD_SLIDER_SetValue(Slideres[5], agentConf.Unit);
               HSD_SLIDER_SetValue(Slideres[6], agentConf.Shape);
              
               ProcChanging[0](NULL, SysConf.Skin);
               ProcChanging[1](NULL, SysConf.Brt);
               ProcChanging[2](NULL, SysConf.Snd.Vol);
               ProcChanging[3](NULL, SysConf.Snd.ArmSnd);
               ProcChanging[4](NULL, SysConf.Snd.KeySnd);
               ProcChanging[5](NULL, SysConf.Unit);
               ProcChanging[6](NULL, SysConf.Shape);

               PWM_SET(agentConf.Brt * 2);
            }
            WM_SetFocus(menuWin);
            break;
       
       case WM_NOTIFY_PARENT:       
            switch(pMsg->Data.v)  
            {
               case WM_NOTIFICATION_VALUE_CHANGED:
                    Id  = WM_GetId(pMsg->hWinSrc);
                    Id  = Id - ID_SLIDER_SKIN;
                    if(Id >= 0  &&  Id <= 6)
                    {
                       ProcChanging[Id](pMsg, HSD_SLIDER_GetValue(Slideres[Id]));
                    }
                    else
                    {                   
                    }
                    break;                   
                             
               default:
//                    WINDOW_Callback(pMsg);
                    break;
            }
            break;
          
       case WM_PAINT:
						
            WM_GetClientRectEx(pMsg->hWin, &r);
            GUI_SetColor(pSkin->bkColor);
            GUI_FillRectEx(&r);
            GUI_SetColor(pSkin->ClientbkColor);
            r.x0 = r.x0 + 10;
            r.x1 = r.x1 - 10;
            r.y0 = r.y0 + 40;
            r.y1 = r.y1 - 40;
            GUI_FillRectEx(&r);
			 
            GUI_SetFont(&GUI_Font30);
					      	GUI_SetTextMode(GUI_TEXTMODE_TRANS);
            GUI_SetColor(pSkin->txColor);
            GUI_DispStringAt("夜间模式:", 30,   Win_SysSet_txOrg );
            GUI_DispStringAt("亮度设置:", 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap));
            GUI_DispStringAt("音量设置:", 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*2);
            GUI_DispStringAt("报警音:"  , 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*3);
            GUI_DispStringAt("按键音:"  , 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*4);
            GUI_DispStringAt("单位设置:", 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5);
            GUI_DispStringAt("船位设置:", 30,   Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6);
            
            GUI_SetFont(&GUI_Font30);
            
            GUI_DispStringAt("关闭", 130,Win_SysSet_txOrg);
            GUI_DispStringAt("开启",  310,Win_SysSet_txOrg);
						      GUI_SetFont(&GUI_Font24);
            GUI_DispStringAt("使用",30, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7+10);
	      					GUI_DispStringAt("选择选项及调整音量亮度数字",118, Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*7+10);
         
            GUI_SetFont (&GUI_Font32_1);
            GUI_SetColor(pSkin->txColor);
            GUI_DrawPolygon(&pPoint[0],5,310,Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6+8);
            GUI_DispStringAt("nm", 140,Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5);           
            GUI_DispStringAt("km", 310,Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*5);
            GUI_DrawBitmap(&bmfish,130,Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6+7);
						
						
//             GUI_DispStringAt("Boat",140,Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6+8);
//             GUI_DispStringAt("Fish",310,Win_SysSet_txOrg+(Win_SysSet_Text_HEIGHT+Win_SysSet_txGrap)*6+8);
            break;
       default:
           WM_DefaultProc(pMsg);
           break;
   }
}


WM_HWIN _sub3WinCreate()
{
   WM_HWIN hWin;
   hWin  = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
   return hWin;
}


static void sldListenter(WM_MESSAGE * pMsg)
{
   const WM_KEY_INFO * pInfo;
   WM_MESSAGE myMsg;
   
   
   switch(pMsg->MsgId)
   {
      case WM_KEY:
           pInfo  = (WM_KEY_INFO*)(pMsg->Data.p);
           switch(pInfo->Key)
           {
              case GUI_KEY_BACKSPACE:
                   myMsg.hWin  = WM_GetClientWindow(confirmWin);
                   myMsg.hWinSrc  = subWins[3];
                   myMsg.MsgId  = USER_MSG_CHOOSE;
                   myMsg.Data.v  = SYS_SETTING;
                   WM_SendMessage(myMsg.hWin, &myMsg);
                   
                   WM_BringToTop(confirmWin);
                   WM_SetFocus(WM_GetDialogItem (confirmWin,GUI_ID_BUTTON0)); 
                   break;
              default:
                   HSD_SLIDER_Callback(pMsg);
                   break;
           }
           break;
           
      default:
           HSD_SLIDER_Callback(pMsg);
           break;
   }
}



static void _OnSkinChanged(WM_MESSAGE * pMsg,int val)
{
   int i  = 0;
	 WM_MESSAGE myMsg;
   if(agentConf.Skin  != val)
   {
		 
      agentConf.Skin  = val;
      pSkin  = &(SysWinSkins[val]);
      
      WINDOW_SetBkColor(pMsg->hWin, pSkin->bkColor);
      for(i=0; i<7; i++)
      {
         HSD_SLIDER_SetBkColor(Slideres[i], pSkin->sldBk);
				     HSD_SLIDER_SetFocusBkColor(Slideres[i], pSkin->sldBk);
         HSD_SLIDER_SetSlotColor(Slideres[i],pSkin->sldSlot);
         HSD_SLIDER_SetFocusSliderColor(Slideres[i], pSkin->sldFocusSlider);
         HSD_SLIDER_SetFocusSlotColor(Slideres[i], pSkin->sldSlot);
      }
      
	     myMsg.hWin = WM_GetClientWindow(menuWin);		
      myMsg.Data.v = val;
      myMsg.MsgId = USER_MSG_SKIN;
      WM_SendMessage (myMsg.hWin, &myMsg);
      
      myMsg.hWin  = confirmWin;
      WM_SendMessage (myMsg.hWin, &myMsg);
			
   }

INFO("Skin changed .skin:%d",agentConf.Skin);
}

static void _OnBrtChanged(WM_MESSAGE * pMsg,int val)
{
   if(agentConf.Brt != val)
   {
      agentConf.Brt  = val;
      PWM_SET(val * 2);
   }
INFO("Brightness changed .brt:%d",agentConf.Brt);
}

static void _OnVolChanged(WM_MESSAGE * pMsg,int val)
{
   if(agentConf.Snd.Vol != val)
   {
      agentConf.Snd.Vol  = val;
   }
INFO("Vol changed .Vol:%d",agentConf.Snd.Vol);
}

static void _OnArmSndChanged(WM_MESSAGE * pMsg,int val)
{
   if(agentConf.Snd.ArmSnd  != val)
   {
      agentConf.Snd.ArmSnd  = val;
   }
INFO("arm snd changed . armsnd:%d",agentConf.Snd.ArmSnd);
}

static void _OnKeySndChanged(WM_MESSAGE * pMsg,int val)
{
   if(agentConf.Snd.KeySnd  != val)
   {
      agentConf.Snd.KeySnd  = val;
   }
INFO("key snd changed . keysnd:%d",agentConf.Snd.KeySnd);
}

static void _OnUnitChanged(WM_MESSAGE * pMsg,int val)
{
   agentConf.Unit  = val;
INFO("unit changed  . Unit:%d",agentConf.Unit);
}

static void _OnShapeChanged(WM_MESSAGE * pMsg,int val)
{
agentConf.Shape  = val;
INFO("Shape changed . Shape:%d",agentConf.Shape);
}









































