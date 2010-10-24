/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Gorlovka

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
              http://secu-3.narod.ru
              email: secu-3@yandex.ru
*/

#include "stdafx.h"
#include "secu3man.h"
#include "Resources\resource.h"

#include "CommunicationManager.h"
#include "DLLLinkedFunctions.h"
#include "io-core\ccomport.h"
#include "io-core\logwriter.h"
#include "MainFrame\MainFrame.h"
#include "MainFrame\MainFrameManager.h"
#include "Settings\AppSettingsDlg.h"
#include "Settings\AppSettingsManager.h"
#include "Settings\ISettingsData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSecu3manApp, CWinApp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecu3manApp construction

CSecu3manApp::CSecu3manApp()
: m_pMainFrameManager(NULL)
, m_pAppSettingsManager(NULL)
, m_pCommunicationManager(NULL)
{
 m_pAppSettingsManager = new CAppSettingsManager();
 m_pCommunicationManager = new CCommunicationManager();
 m_pLogWriter = new LogWriter();
 m_pMainFrameManager = new CMainFrameManager();
}

CSecu3manApp::~CSecu3manApp()
{
 delete m_pMainFrameManager;
 delete m_pAppSettingsManager;
 delete m_pCommunicationManager;
 delete m_pLogWriter;
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CSecu3manApp object

CSecu3manApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSecu3manApp initialization

BOOL CSecu3manApp::InitInstance()
{
 AfxEnableControlContainer();

 CoInitialize(NULL);

#if (_MSC_VER <1300)
 #ifdef _AFXDLL
  Enable3dControls();		    // Call this when using MFC in a shared DLL
 #else
  Enable3dControlsStatic();	// Call this when linking to MFC statically
 #endif
#endif

 SetRegistryKey(MLL::LoadString(IDS_APP_TITLE));

 //���������� ������� �� ����������� ����������� DLL
 DLL::LoadDLLsAndLinkToFunctions(); 
  
 //������ ���������
 m_pAppSettingsManager->ReadSettings();
 
 //�����������
 switch(m_pAppSettingsManager->GetSettings()->GetInterfaceLanguage())
 {
  case IL_ENGLISH:
   ::SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
   break;
  case IL_RUSSIAN:
   ::SetThreadLocale(MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_ENGLISH_US), SORT_DEFAULT));
   break; 
 }
 if (DLL::UOZ1_Chart2DSetLanguage)
  DLL::UOZ1_Chart2DSetLanguage(m_pAppSettingsManager->GetSettings()->GetInterfaceLanguage());
 if (DLL::UOZ2_Chart3DSetLanguage)
  DLL::UOZ2_Chart3DSetLanguage(m_pAppSettingsManager->GetSettings()->GetInterfaceLanguage());


 //������� ������� ����. ��� ������ ���� ������� ������ ��� ����� �����������
 //���������� �������������.
 m_pMainWnd = m_pMainFrameManager->GreateMainWindow();

 //�������������� ���������������� ��������
 m_pCommunicationManager->Init();

 //�������������� ���������� �������� ���� (�������� �����������).  
 m_pMainFrameManager->Init(m_pMainWnd);

 return TRUE;
}

BOOL CSecu3manApp::OnIdle(LONG lCount) 
{	
 return CWinApp::OnIdle(lCount);
}

int CSecu3manApp::ExitInstance() 
{	  
 //��������� ���������
 m_pAppSettingsManager->WriteSettings();

 //���������� ������ ��������� ������������
 m_pCommunicationManager->Terminate();

 return CWinApp::ExitInstance();
}

CAppSettingsManager* CSecu3manApp::GetAppSettingsManager(void) const
{
 return m_pAppSettingsManager;
}

CCommunicationManager* CSecu3manApp::GetCommunicationManager(void) const
{
 return m_pCommunicationManager;
}

CMainFrameManager* CSecu3manApp::GetMainFrameManager(void) const
{
 return m_pMainFrameManager;
}

LogWriter* CSecu3manApp::GetLogWriter(void) const
{
 return m_pLogWriter;
}
