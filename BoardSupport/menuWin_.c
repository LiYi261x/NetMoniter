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

#include "MainTask.h"
#include "DIALOG.h"
#include "Config.h"
#include "WM.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_BUTTON_2 (GUI_ID_USER + 0x03)
#define ID_BUTTON_3 (GUI_ID_USER + 0x04)
#define ID_TEXT_0   (GUI_ID_USER + 0x05)
//#define ID_LISTVIEW_0 (GUI_ID_USER + 0x06)
//#define ID_TEXT_0 (GUI_ID_USER + 0x07)
//#define ID_TEXT_1 (GUI_ID_USER + 0x08)
//#define ID_TEXT_2 (GUI_ID_USER + 0x09)
//#define ID_TEXT_3 (GUI_ID_USER + 0x0A)
//#define ID_EDIT_0 (GUI_ID_USER + 0x0B)


// USER START (Optionally insert additional defines)
extern  void  UpdateListViewContent(WM_HWIN thisHandle);
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
static void myButtonListener(WM_MESSAGE * pMsg);
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, Win_Main_WIDTH, Win_Main_HEIGHT, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "bt_0", ID_BUTTON_0, MenuLabel_X, MenuLabel_Y,                    MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "bt_1", ID_BUTTON_1, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT,  MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "bt_2", ID_BUTTON_2, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT*2,MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "bt_3", ID_BUTTON_3, MenuLabel_X, MenuLabel_Y+MenuButton_HEIGHT*3,MenuButton_WIDTH, MenuButton_HEIGHT, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};


WM_HWIN subWins[4];
WM_HWIN hButtons[4];

extern WM_HWIN  menuWin;
extern WM_HWIN etWin;
//extern WM_HWIN winTest;
extern WM_HWIN hDlg_FishMap;
extern WM_HWIN confirmWin;
extern char editindex;
extern WM_HWIN sub0WinCreate(void);
extern WM_HWIN sub1WinCreate(void);
extern WM_HWIN sub2WinCreate(void);
extern WM_HWIN sub3WinCreate(void);



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
	const WM_KEY_INFO * pInfo;
//  WM_HWIN hItem;
  int     NCode;
  int     Id;
	char    i;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
			case WM_PAINT:
				GUI_SetBkColor (GUI_LIGHTBLUE);
				GUI_Clear();
				GUI_SetTextMode(GUI_TEXTMODE_TRANS);
				GUI_SetColor (GUI_WHITE);
				GUI_SetFont (&GUI_Font28);
				GUI_DispStringAt ("主菜单",15,10);
			break;
//		case WM_KEY:
//			pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
//			switch(pInfo->Key)
//			{
//				case GUI_KEY_DOWN:

//          WM_SetFocus(WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0));				
//					break;
//			}
//			break;
  case WM_INIT_DIALOG:
		
    //
    // Initialization of 'bt_0'
    //
    hButtons[0] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hButtons[0], "监控列表");
    //
    // Initialization of 'bt_1'
    //
    hButtons[1] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetText(hButtons[1], "监控设置");
    //
    // Initialization of 'bt_2'
    //
    hButtons[2] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetText(hButtons[2], "船舶列表");
    //
    // Initialization of 'bt_3'
    //
    hButtons[3] = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetText(hButtons[3], "系统设置");	
		
		for (i = 0; i < 4; i++)
		{		
			BUTTON_SetBkColor(hButtons[i],GUI_LIGHTRED,BUTTON_CI_DISABLED);
			BUTTON_SetBkColor(hButtons[i],GUI_LIGHTBLUE,BUTTON_CI_UNPRESSED);	
			WM_SetCallback(hButtons[i],&myButtonListener);
		}
		//
		// initialization of subWindow
		//
		
	  	subWins[0]  = sub0WinCreate();
		 	subWins[1]  = sub1WinCreate();				
			subWins[2]  = sub2WinCreate();
	  	subWins[3]  = sub3WinCreate();

    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
		
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'bt_0'
				
      switch(NCode) {     
      break;
      case WM_NOTIFICATION_CLICKED:
			
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'bt_1'
		
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'bt_2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'bt_3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
//    case ID_LISTVIEW_0: // Notifications sent by 'thisListview'
//      switch(NCode) {
//      case WM_NOTIFICATION_CLICKED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      case WM_NOTIFICATION_RELEASED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      case WM_NOTIFICATION_SEL_CHANGED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      // USER START (Optionally insert additional code for further notification handling)
//      // USER END
//      }
//      break;
//    case ID_EDIT_0: // Notifications sent by 'Edit'
//      switch(NCode) {
//      case WM_NOTIFICATION_CLICKED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      case WM_NOTIFICATION_RELEASED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      case WM_NOTIFICATION_VALUE_CHANGED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      // USER START (Optionally insert additional code for further notification handling)
//      // USER END
//      }
//      break;
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
*       CreateWindow
*/
WM_HWIN menuWinCreate(void);
WM_HWIN menuWinCreate(void) {
  WM_HWIN hWin;
	
	
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	
  return hWin;
}

// USER START (Optionally insert additional public code)

static void myButtonListener(WM_MESSAGE * pMsg)
{
	const WM_KEY_INFO * pInfo;
	WM_HWIN thisButton  = pMsg->hWin;
	WM_HWIN handle;
	int i  = 0;
	
	
	///判断当前焦点落在哪个button 上
	handle  = WM_GetFocussedWindow();
	
	if(handle  == hButtons[0])
	{
		
		i  = 0;
    WM_BringToTop(subWins[0]);	
    
	}
	else if(handle == hButtons[1])
	{		
		
    i  = 1;		
  UpdateListViewContent(WM_GetDialogItem(subWins[1],GUI_ID_USER + 0x01));
		WM_BringToTop(subWins[1]);
		WM_BringToTop(etWin);
	}
	else if(handle == hButtons[2])
	{
		
		i  = 2;
		WM_BringToTop(subWins[2]);		
	}
	else if(handle == hButtons[3])
	{
		
		i  = 3;
	  WM_BringToTop(subWins[3]);
	}
	else 
	{
     ;
	}
	
	
	switch(pMsg->MsgId)
	{
		case WM_KEY:
			pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
		  switch(pInfo->Key)
			{
				case GUI_KEY_DOWN:
         GUI_StoreKeyMsg(GUI_KEY_TAB,1);		
					if (i == 0)
					{
						WM_EnableWindow(hButtons[3]);
					}
					else if (i == 1)
					{
						WM_EnableWindow(hButtons[0]);
					}
					else if (i == 2)
					{
						WM_EnableWindow(hButtons[1]);
					}
					else if (i == 3)
					{
						WM_EnableWindow(hButtons[2]);
					}
				
					break;
				
				case GUI_KEY_UP:
					GUI_StoreKeyMsg(GUI_KEY_BACKTAB,1);
					break;
				
				case GUI_KEY_RIGHT:
         if(i!=3)
          WM_SetFocus(subWins[i]);
				 else 
				 {						WM_SetFocus (WM_GetDialogItem(subWins[i], GUI_ID_EDIT0));
						WM_EnableWindow(WM_GetFocussedWindow());
						editindex = 0;
					}
				  break;
				case GUI_KEY_MENU:       
		    		 WM_BringToTop(hDlg_FishMap);
         WM_BringToBottom(menuWin);
         WM_HideWindow(subWins[0]);
         WM_HideWindow(subWins[1]);
         WM_HideWindow(subWins[2]);
         WM_HideWindow(subWins[3]);
         WM_HideWindow(etWin);
					WM_SetFocus(hDlg_FishMap);
					break;
				
				default:
					BUTTON_Callback(pMsg);
				break;
			}
			break;
				default:
					BUTTON_Callback(pMsg);
				break;
	}
}

// USER END

/*************************** End of file ****************************/