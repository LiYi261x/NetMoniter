/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.24                          *
*        Compiled Jan 27 2014, 11:28:24                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "MainTask.h"
#include "Config.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)

#define ID_TEXT_0 (GUI_ID_USER + 0x01)
#define ID_TEXT_1 (GUI_ID_USER + 0x02)
#define ID_TEXT_2 (GUI_ID_USER + 0x03)
#define ID_TEXT_3 (GUI_ID_USER + 0x04)
#define ID_TEXT_4 (GUI_ID_USER + 0x05)
#define ID_TEXT_5 (GUI_ID_USER + 0x06)
#define ID_TEXT_6 (GUI_ID_USER + 0x07)
#define ID_TEXT_7 (GUI_ID_USER + 0x08)

#define ID_EDIT_0 (GUI_ID_USER + 0x10)
#define ID_EDIT_1 (GUI_ID_USER + 0x11)
#define ID_EDIT_2 (GUI_ID_USER + 0x12)
#define ID_EDIT_3 (GUI_ID_USER + 0x13)
#define ID_EDIT_4 (GUI_ID_USER + 0x14)
#define ID_EDIT_5 (GUI_ID_USER + 0x15)
#define ID_EDIT_6 (GUI_ID_USER + 0x16)

WM_HWIN etWin ;
WM_HWIN hEts[7];

extern short N_monitedBoat;


extern WM_HWIN subWins[4];
extern unsigned char * pStrBuf;


extern WM_HWIN confirmWin;

static int fdDist  = 0;
static int zmDist  = 0;

static void myEditListener(WM_MESSAGE* pMsg);
static void myEditAddKeyEx(EDIT_Handle hObj, int key);
// USER START (Optionally insert additional defines)


//void printMoniteSetting(void);
//void printSetting(MNT_SETTING * p_setting);
//extern void mntSetting_init(void);
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*   
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "ETWin", ID_WINDOW_0, ETWin_X, SubWin_Y, ETWin_WIDHT, ETWin_HEIGHT, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Set info",  ID_TEXT_0, 0,   0,                  ETWin_WIDHT,30, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "1.xsAlarm", ID_TEXT_1, 0,   LV_MoniteSet_Y,     120,        40, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "2.fdAlarm", ID_TEXT_2, 0,   LV_MoniteSet_Y+40,  180,        40, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "Dist:",     ID_TEXT_3, 160, LV_MoniteSet_Y+80,  60,        40, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "Sound",     ID_TEXT_4, 40,  LV_MoniteSet_Y+160, 60,       40, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "3.zmAlarm", ID_TEXT_5, 0,   LV_MoniteSet_Y+200, 180,       40, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "Dist:",     ID_TEXT_6, 160, LV_MoniteSet_Y+240, 60,       40, 0, 0x0, 0},
 { TEXT_CreateIndirect, "Sound",     ID_TEXT_7, 40,  LV_MoniteSet_Y+280, 60,       40, 0, 0x0, 0},
	
 { EDIT_CreateIndirect, "et_0", ID_EDIT_0, 200, LV_MoniteSet_Y,     80, 40, 0, 0xa,  0 },
 { EDIT_CreateIndirect, "et_1", ID_EDIT_1, 200, LV_MoniteSet_Y+40,  80, 40, 0, 0x64, 0 },
 { EDIT_CreateIndirect, "et_2", ID_EDIT_2, 200, LV_MoniteSet_Y+80,  80, 40, 0, 0x64, 0 },
 { EDIT_CreateIndirect, "et_3", ID_EDIT_3, 200, LV_MoniteSet_Y+160, 80, 40, 0, 0x64, 0 },
 { EDIT_CreateIndirect, "et_4", ID_EDIT_4, 200, LV_MoniteSet_Y+200, 80, 40, 0, 0x64, 0 },
 { EDIT_CreateIndirect, "et_5", ID_EDIT_5, 200, LV_MoniteSet_Y+240, 80, 40, 0, 0x64, 0},
	{ EDIT_CreateIndirect, "et_6", ID_EDIT_6, 200, LV_MoniteSet_Y+280, 80, 40, 0, 0x64, 0}
	
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
	
	const WM_KEY_INFO* pInfo;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'et_0'
    //
    hEts[0] = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
//    EDIT_SetText(hEts[0], mntSetting.DSP_Setting.isEnable>DISABLE?"Enable":"Disable");   
	   WM_SetCallback(hEts[0],&myEditListener);
    EDIT_SetpfAddKeyEx(hEts[0],myEditAddKeyEx);
    
    //
    // Initialization of 'et_1'
    //
    hEts[1] = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
//    EDIT_SetText(hEts[1], mntSetting.BGL_Setting.isEnable>DISABLE?"Enable":"Disable"); 
	   WM_SetCallback(hEts[1],&myEditListener);
    EDIT_SetpfAddKeyEx(hEts[1], myEditAddKeyEx);    
    //
    // Initialization of 'et_2'
    //
    hEts[2] = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
//	  	sprintf(pStrBuf, "%d",mntSetting.BGL_Setting.dist);
//	  	EDIT_SetText(hEts[2], pStrBuf);  
	   WM_SetCallback(hEts[2],&myEditListener);	
    EDIT_SetpfAddKeyEx(hEts[2], myEditAddKeyEx);
    //
    // Initialization of 'et_3'
    //
    hEts[3] = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
//    EDIT_SetText(hEts[3], mntSetting.BGL_Setting.isSndEnable>DISABLE?"Enable":"Disable");
	   WM_SetCallback(hEts[3] ,&myEditListener);	
    EDIT_SetpfAddKeyEx(hEts[3], myEditAddKeyEx);    
    //
    // Initialization of 'et_4'
    //
    hEts[4]  = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
//    EDIT_SetText(hEts[4], mntSetting.DRG_Setting.isEnable>DISABLE?"Enable":"Disable");
	   WM_SetCallback(hEts[4],&myEditListener);
    EDIT_SetpfAddKeyEx(hEts[4], myEditAddKeyEx);    
   
    //
    // Initialization of 'et_5'
    //
    hEts[5]  = WM_GetDialogItem(pMsg->hWin, ID_EDIT_5);   
//    sprintf(pStrBuf, "%d",mntSetting.DRG_Setting.dist);
//    EDIT_SetText(hEts[5], pStrBuf);     
    WM_SetCallback(hEts[5],&myEditListener);
    EDIT_SetpfAddKeyEx(hEts[5], myEditAddKeyEx);
    
    
    hEts[6]  = WM_GetDialogItem(pMsg->hWin, ID_EDIT_6);
//    EDIT_SetText(hEts[6], mntSetting.DRG_Setting.isSndEnable>DISABLE?"Enable":"Disable");
    WM_SetCallback(hEts[6], &myEditListener);
    EDIT_SetpfAddKeyEx(hEts[6], myEditAddKeyEx);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
	case WM_KEY:
		pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
	
	  switch(pInfo->Key)
		{
			case GUI_KEY_LEFT:
				GUI_StoreKeyMsg(GUI_KEY_BACKTAB,1);
			break;
			case GUI_KEY_RIGHT:
				GUI_StoreKeyMsg(GUI_KEY_TAB,1);
			break;

			
			default:
				break;
		}
		break;
	
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_EDIT_0: // Notifications sent by 'et_0'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_1: // Notifications sent by 'et_1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_2: // Notifications sent by 'et_2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_3: // Notifications sent by 'et_3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_EDIT_4: // Notifications sent by 'et_4'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateETWin
*/
WM_HWIN etWinCreate(void);
WM_HWIN etWinCreate(void) {

//  etWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, subWins[1], 0, 0);
etWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return etWin;
}

// USER START (Optionally insert additional public code)

static void myEditListener(WM_MESSAGE* pMsg)
{
	const WM_KEY_INFO* pInfo;
	WM_MESSAGE msg;
	WM_HWIN thisEdit = pMsg->hWin;
	
	WM_HWIN focussedEdit  = WM_GetFocussedWindow();
	
	int i  = 0;
	int Step  = 0;

	
	switch(pMsg->MsgId)
	{
		case WM_KEY:
			pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
		
		  switch(pInfo->Key)
			{
				case GUI_KEY_UP:
					GUI_StoreKeyMsg(GUI_KEY_BACKTAB,1);
					break;
				
				case GUI_KEY_DOWN:
					GUI_StoreKeyMsg(GUI_KEY_TAB,1);
					break;
	
 /*
				case GUI_KEY_RIGHT:

					Step  = 2;
			
					//break;
				case GUI_KEY_LEFT:
					
					Step  = (Step>1)?1:-1;
		
					while(focussedEdit != hEts[i])
						i++;
					
					if(i<6)
					{
						//防盗报警距离设置
						if(2 == i)
						{
							fdDist  += Step;
							fdDist  = (fdDist+21)%21;
							
							sprintf(pStrBuf, "%d", fdDist);
							EDIT_SetText(hEts[2], pStrBuf);
						}
						///走锚报警距离设置
						else if(5 == i)
						{
							zmDist  += Step;
							zmDist  = (zmDist+21)%21;

							sprintf(pStrBuf, "%d", zmDist);
              EDIT_SetText(hEts[5], pStrBuf);							
						}
						else
						{
							if( MoniteSetFlags & (0x1<<i) )
							{
								MoniteSetFlags  &= (~(0x1<<i));
								EDIT_SetText(hEts[i],"Disable");
							}
							else
							{
								MoniteSetFlags  |= (0x1<<i);
								EDIT_SetText(hEts[i],"Enable");
							}
						}
					}
					else 
						printf("\r\nException occored!\r\n");
				  break;
*/		
    case GUI_KEY_RIGHT:
         Step  = 2;
    case GUI_KEY_LEFT:
         Step  = (Step>1)?1:-1;
         
         while( (focussedEdit != hEts[i])  &&  (i<7) )
              i++;
//         switch(i)
//         {
//            case 0:
//                 if(mntSetting.DSP_Setting.isEnable)
//                 {
//                    mntSetting.DSP_Setting.isEnable  = DISABLE;
//                    EDIT_SetText(hEts[0], "Disable");
//                 }
//                 else
//                 {
//                    mntSetting.DSP_Setting.isEnable  = ENABLE;
//                    EDIT_SetText(hEts[0], "Enable");
//                 }
//                 break;
//                 
//            case 1:
//                 if(mntSetting.BGL_Setting.isEnable)
//                 {
//                    mntSetting.BGL_Setting.isEnable  = DISABLE;
//                    EDIT_SetText(hEts[1], "Disable");
//                 }
//                 else
//                 {
//                    mntSetting.BGL_Setting.isEnable  = ENABLE;
//                    EDIT_SetText(hEts[1], "Enable");
//                 }
//                 break;
//                 
//            case 2:
//                 mntSetting.BGL_Setting.dist  += Step;
//                 mntSetting.BGL_Setting.dist  = (mntSetting.BGL_Setting.dist+21)%21;
//                 
//                 sprintf(pStrBuf, "%d", mntSetting.BGL_Setting.dist);
//                 EDIT_SetText(hEts[2], pStrBuf);
//                 break;
//                 
//            case 3:
//                 if(mntSetting.BGL_Setting.isSndEnable) 
//                 {                 
//                    mntSetting.BGL_Setting.isSndEnable = DISABLE;
//                    EDIT_SetText(hEts[3], "Disable");
//                 } 
//                 else
//                 {                
//                    mntSetting.BGL_Setting.isSndEnable  = ENABLE;
//                    EDIT_SetText(hEts[3], "Enable");
//                 }                   
//                 break;  
//            case 4:
//                 if(mntSetting.DRG_Setting.isEnable)
//                 { 
//                    mntSetting.DRG_Setting.isEnable  = DISABLE;
//                    EDIT_SetText(hEts[4], "Disable");
//                 }
//                 else
//                 {
//                    mntSetting.DRG_Setting.isEnable  = ENABLE;
//                    EDIT_SetText(hEts[4], "Enable");
//                 }
//                 break;            
//                 
//            case 5:
//                 mntSetting.DRG_Setting.dist  += Step;
//                 mntSetting.DRG_Setting.dist  = (mntSetting.DRG_Setting.dist+21)%21;
//                 
//                 sprintf(pStrBuf, "%d", mntSetting.DRG_Setting.dist);
//                 EDIT_SetText(hEts[5], pStrBuf);
//                 break; 
//            
//            case 6:
//                 if(mntSetting.DRG_Setting.isSndEnable)
//                 {
//                    mntSetting.DRG_Setting.isSndEnable  = DISABLE;
//                    EDIT_SetText(hEts[6], "Disable");
//                 }
//                 else
//                 {
//                    mntSetting.DRG_Setting.isSndEnable  = ENABLE;
//                    EDIT_SetText(hEts[6], "Enable");
//                 }
//                 break;            
//                 
//         }
    break;
    	
    case GUI_KEY_BACKSPACE:
    
         WM_BringToTop(confirmWin);
         WM_SetFocus(confirmWin);
         break;
     
				default:
					EDIT_Callback(pMsg);
				break;
			}
			
				default:
					EDIT_Callback(pMsg);
				break;
	}	 
  
}



static void myEditAddKeyEx(EDIT_Handle hObj, int key)
{
   WM_HWIN thisEdit;
   
   switch(key)
   {
      break;
   }
}
// USER END







/*************************** End of file ****************************/
