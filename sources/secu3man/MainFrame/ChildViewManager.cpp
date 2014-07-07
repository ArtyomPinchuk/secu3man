/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Kiev

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   contacts:
              http://secu-3.org
              email: shabelnikov@secu-3.org
*/

#include "stdafx.h"
#include "Resources/resource.h"
#include "ChildViewManager.h"

#include "Application/ISECU3Man.h"
#include "ChildView.h"
#include "MainFrame.h"
#include "MainFrameManager.h"
#include "TabsManagement/MainTabController.h"

#include "TabControllers/CheckEngineTabController.h"
#include "TabControllers/DevDiagnostTabController.h"
#include "TabControllers/FirmwareTabController.h"
#include "TabControllers/KnockChannelTabController.h"
#include "TabControllers/LogPlayerTabController.h"
#include "TabControllers/ParamMonTabController.h"

#include "TabDialogs/CheckEngineTabDlg.h"
#include "TabDialogs/DevDiagnostTabDlg.h"
#include "TabDialogs/FirmwareTabDlg.h"
#include "TabDialogs/KnockChannelTabDlg.h"
#include "TabDialogs/LogPlayerTabDlg.h"
#include "TabDialogs/ParamMonTabDlg.h"

#include "Settings/AppSettingsManager.h"

CChildViewManager::CChildViewManager()
: mp_wndView(new CChildView)
, mp_MainTabController(new CMainTabController(mp_wndView.get()))
, m_pParamMonTabController(NULL)
, m_pFirmwareTabController(NULL)
, m_pCheckEngineTabController(NULL)
, m_pKnockChannelTabController(NULL)
, m_pLogPlayerTabController(NULL)
, m_pDevDiagnostTabController(NULL)
, m_pParamMonTabDlg(NULL)
, m_pFirmwareTabDlg(NULL)
, m_pCheckEngineTabDlg(NULL)
, m_pKnockChannelTabDlg(NULL)
, m_pLogPlayerTabDlg(NULL)
, m_pDevDiagnostTabDlg(NULL)
{
 //empty
}

CChildViewManager::~CChildViewManager()
{
 delete m_pParamMonTabController;
 delete m_pFirmwareTabController;
 delete m_pCheckEngineTabController;
 delete m_pKnockChannelTabController;
 delete m_pLogPlayerTabController;
 delete m_pDevDiagnostTabController;

 delete m_pParamMonTabDlg;
 delete m_pFirmwareTabDlg;
 delete m_pCheckEngineTabDlg;
 delete m_pKnockChannelTabDlg;
 delete m_pLogPlayerTabDlg;
 delete m_pDevDiagnostTabDlg;
}

bool CChildViewManager::Init(CMainFrame* i_pMainFrame)
{
 ASSERT(i_pMainFrame);
 // create a view to occupy the client area of the frame
 if (!mp_wndView->Create(i_pMainFrame))
 {
  TRACE0("Failed to create view window\n");
  return false;
 }
 //Set standard ID for this window can be attached to framework.
 mp_wndView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);

 i_pMainFrame->SetView(mp_wndView.get());

 mp_wndView->SetEventListener(mp_MainTabController.get()); //���������� ����� ��������� ��������� �� View

 CCommunicationManager* p_comm = ISECU3Man::GetSECU3Man()->GetCommunicationManager();
 CStatusBarManager*     p_sbar = ISECU3Man::GetSECU3Man()->GetMainFrameManager()->GetStatusBarManager();
 ISettingsData*         p_settings = ISECU3Man::GetSECU3Man()->GetAppSettingsManager()->GetSettings();

 //������� ����������� ������� � ��������� Tab control ���������
 m_pParamMonTabDlg = new CParamMonTabDlg(); //view
 m_pParamMonTabController = new CParamMonTabController(m_pParamMonTabDlg, p_comm, p_sbar, p_settings);

 mp_MainTabController->AddTabController(m_pParamMonTabController);
 mp_wndView->AddView(MLL::GetString(IDS_TAB_PARAMETERS_AND_MONOTOR), m_pParamMonTabDlg, 0);

 m_pFirmwareTabDlg = new CFirmwareTabDlg(); //view
 m_pFirmwareTabController = new CFirmwareTabController(m_pFirmwareTabDlg, p_comm, p_sbar, p_settings);

 mp_MainTabController->AddTabController(m_pFirmwareTabController);
 mp_wndView->AddView(MLL::GetString(IDS_TAB_FIRMWARE_DATA),m_pFirmwareTabDlg,1);

 m_pCheckEngineTabDlg = new CCheckEngineTabDlg(); //view
 m_pCheckEngineTabController = new CCheckEngineTabController(m_pCheckEngineTabDlg, p_comm, p_sbar);

 mp_MainTabController->AddTabController(m_pCheckEngineTabController);
 mp_wndView->AddView(MLL::GetString(IDS_TAB_CHECK_ENGINE),m_pCheckEngineTabDlg,2);

 m_pKnockChannelTabDlg = new CKnockChannelTabDlg(); //view
 m_pKnockChannelTabController = new CKnockChannelTabController(m_pKnockChannelTabDlg, p_comm, p_sbar);

 mp_MainTabController->AddTabController(m_pKnockChannelTabController);
 mp_wndView->AddView(MLL::GetString(IDS_TAB_KNOCK_SETTINGS),m_pKnockChannelTabDlg,3);

 m_pLogPlayerTabDlg = new CLogPlayerTabDlg(); //view
 m_pLogPlayerTabController = new CLogPlayerTabController(m_pLogPlayerTabDlg, p_comm, p_sbar, p_settings);

 mp_MainTabController->AddTabController(m_pLogPlayerTabController);
 mp_wndView->AddView(MLL::GetString(IDS_TAB_LOG_PLAYER),m_pLogPlayerTabDlg,4);

 m_pDevDiagnostTabDlg = new CDevDiagnostTabDlg(); //view
 m_pDevDiagnostTabController = new CDevDiagnostTabController(m_pDevDiagnostTabDlg, p_comm, p_sbar, p_settings);

 mp_MainTabController->AddTabController(m_pDevDiagnostTabController);
 mp_wndView->AddView(MLL::GetString(IDS_TAB_DEV_DIAGNOSTICS), m_pDevDiagnostTabDlg, 5);

 mp_wndView->SetCurSel(0);
 mp_wndView->SetFocus();
 return true;
}

bool CChildViewManager::OnClose(void)
{
 std::vector<ITabController*>& list = mp_MainTabController->GetControllersList();
 for (size_t i = 0; i < list.size(); i++)
 {
  if (list[i]->OnClose()==false)
   return false;
 }
 return true;
}

bool CChildViewManager::OnAskFullScreen(void)
{
 ASSERT(mp_MainTabController->GetActiveController());
 return mp_MainTabController->GetActiveController()->OnAskFullScreen();
}

void CChildViewManager::OnFullScreen(bool i_what)
{
 //��������� ���-������� �� ����� ���� ���� ���������� �� ���� �����. ��� ����� � ������������� �����
 //������ ����� �� �������� �������, ��� ������, ������ ����� �� ���-�������.
 mp_wndView->EnableTabControl(!i_what);
 mp_wndView->SetFocus(i_what);

 //��������� ���������� �������� ������� � ���������/���������� �������������� ������
 CRect rect;
 mp_wndView->GetClientRect(rect); 
 ASSERT(mp_MainTabController->GetActiveController());
 mp_MainTabController->GetActiveController()->OnFullScreen(i_what, rect);
}
