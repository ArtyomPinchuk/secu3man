 /****************************************************************
 *       SECU-3  - An open source, free engine control unit
 *    Designed by Alexey A. Shabelnikov. Ukraine, Gorlovka 2007.
 *       Microprocessors systems - design & programming.
 *    contacts:
 *              http://secu-3.narod.ru
 *              ICQ: 405-791-931
 ****************************************************************/

#include "stdafx.h"
#include "StatusBarManager.h"
#include "resource.h"
#include "ui-core/MPCLogoPane.h"


static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,      
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL
};


CStatusBarManager::CStatusBarManager()
: m_pParentWnd(NULL)
, m_CurrentConnectionState(STATE_OFFLINE)
, m_LogWrIcon(NULL)
{
  for(int i = 0; i < 3; i++)
  {
    m_ConnIcons[i] = NULL;
	m_ConnStrings[i] = _T("");
  }
}

CStatusBarManager::~CStatusBarManager()
{
 //��� ������������� ������� ��������� ����������� � StatusBar
}


bool CStatusBarManager::Create(CWnd* pParentWnd)
{
  if (!pParentWnd)
  {
    ASSERT(pParentWnd); //���� �������� ������������ ����!
	return false;
  }

  m_pParentWnd = pParentWnd;

  //������� StatusBar � ��������� � ���� ����������� ���� (����������)  
  if (!m_wndStatusBar.Create(pParentWnd,WS_CHILD|WS_VISIBLE|CBRS_BOTTOM,ID_MAIN_STATUS_BAR) ||
	  !m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
  {
	TRACE0("Failed to create status bar\n");
	return false;      // fail to create
  }

  //���������� �� �������� ������ � ������ 
  m_ConnIcons[0] = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_CONN_CONNECTED));
  m_ConnIcons[1] = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_CONN_DISCONNECTED));
  m_ConnIcons[2] = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_CONN_BOOTLOADER));
  m_LogWrIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_LOGWR_WRITE));

  m_ConnStrings[0].LoadString(IDS_CONN_CONNECTED);
  m_ConnStrings[1].LoadString(IDS_CONN_DISCONNECTED);
  m_ConnStrings[2].LoadString(IDS_CONN_BOOTLOADER);

  return true;
}


//���������� � StatusBar �������������� �����
void CStatusBarManager::AddContent(void)
{
  int idx = 0;

  //������� SECU-3
  m_wndStatusBar.AddIndicator(0,ID_MSB_INDICATOR_LOGO);
  idx = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_LOGO);
  m_wndStatusBar.SetPaneWidth(idx,80);
  m_wndStatusBar.SetPaneStyle(idx, m_wndStatusBar.GetPaneStyle(idx) | SBPS_NOBORDERS );
  MPCLogoPane* pLogo = new MPCLogoPane;
  pLogo->Create(MLL::LoadString(IDS_STATUS_BAR_LOGO),WS_CHILD|WS_VISIBLE,&m_wndStatusBar,ID_MSB_INDICATOR_LOGO);
  pLogo->SetLogoFont(_T("Arial"), 18);
  m_wndStatusBar.AddControl(pLogo,ID_MSB_INDICATOR_LOGO);

  //�������������� ���� (���������� ID_SEPARATOR)
  idx = m_wndStatusBar.CommandToIndex(ID_SEPARATOR);
  m_wndStatusBar.SetPaneWidth(idx,80);
  m_wndStatusBar.SetPaneStyle(idx, m_wndStatusBar.GetPaneStyle(idx) | SBPS_NOBORDERS );
  m_wndStatusBar.SetPaneText(idx,_T(" "));

  //�������� ���
  m_wndStatusBar.AddIndicator(2,ID_MSB_INDICATOR_PROGRESS);
  idx = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_PROGRESS);
  m_wndStatusBar.SetPaneWidth(idx,80);
  m_wndStatusBar.SetPaneStyle(idx, m_wndStatusBar.GetPaneStyle(idx) | SBPS_NOBORDERS );
  CProgressCtrl* progress = new CProgressCtrl;
  progress->Create(WS_CHILD,CRect(0,0,0,0),&m_wndStatusBar,ID_MSB_INDICATOR_PROGRESS);  
 // progress->ShowWindow(SW_SHOW);
  m_wndStatusBar.AddControl(progress,ID_MSB_INDICATOR_PROGRESS);

  //��������
  m_wndStatusBar.AddIndicator(3,ID_MSB_INDICATOR_PERCENT);
  idx = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_PERCENT);
  m_wndStatusBar.SetPaneWidth(idx,30);
  m_wndStatusBar.SetPaneStyle(idx, m_wndStatusBar.GetPaneStyle(idx) | SBPS_NOBORDERS );

  //������ ��������
  m_wndStatusBar.AddIndicator(4,ID_MSB_INDICATOR_ICON);
  idx = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_ICON);
  m_wndStatusBar.SetPaneWidth(idx,16);
  m_wndStatusBar.SetPaneStyle(idx, SBPS_NORMAL);

  //����� ��������
  m_wndStatusBar.AddIndicator(5,ID_MSB_INDICATOR_CONNTXT);
  idx = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_CONNTXT);
  m_wndStatusBar.SetPaneWidth(idx,80);
  m_wndStatusBar.SetPaneStyle(idx, SBPS_NORMAL);

  //������� ������� ������������
  m_wndStatusBar.AddIndicator(6,ID_MSB_INDICATOR_EMPTY);
  idx = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_EMPTY);
  m_wndStatusBar.SetPaneWidth(idx,10);
  m_wndStatusBar.SetPaneStyle(idx, m_wndStatusBar.GetPaneStyle(idx) | SBPS_NOBORDERS );

  //������ ������������ ������ ������ �����
  m_wndStatusBar.AddIndicator(7,ID_MSB_INDICATOR_LOGWR);
  idx = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_LOGWR);
  m_wndStatusBar.SetPaneWidth(idx,16);
  m_wndStatusBar.SetPaneStyle(idx, SBPS_NORMAL);

  //������ �����������: CAP, NUM, SCRL
}


//������������� ������� ������ � ����� (��������� ����������)
void CStatusBarManager::SetConnectionState(int i_State)
{	
  int idx_icon = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_ICON);
  int idx_str =  m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_CONNTXT);

  CStatusBarCtrl& status = m_wndStatusBar.GetStatusBarCtrl();

  switch(i_State)
  {
    case STATE_ONLINE:
      status.SetIcon(idx_icon,m_ConnIcons[0]);
	  m_wndStatusBar.SetPaneText(idx_str,m_ConnStrings[0]);
      m_CurrentConnectionState = i_State;
	  break;
    case STATE_OFFLINE:
      status.SetIcon(idx_icon,m_ConnIcons[1]);
	  m_wndStatusBar.SetPaneText(idx_str,m_ConnStrings[1]);
      m_CurrentConnectionState = i_State;
	  break;
    case STATE_BOOTLOADER:
      status.SetIcon(idx_icon,m_ConnIcons[2]);
	  m_wndStatusBar.SetPaneText(idx_str,m_ConnStrings[2]);
      m_CurrentConnectionState = i_State;
	  break;
  }
}

//����������/������ ProgressCtrl
void CStatusBarManager::ShowProgressBar(bool show)
{
  MPCStatusBarPaneControlInfo* info = m_wndStatusBar.GetPanControl(ID_MSB_INDICATOR_PROGRESS);
  ASSERT(info);
  int nCmdShow = ((show) ? SW_SHOW : SW_HIDE);  
  info->m_hWnd->ShowWindow(nCmdShow);
  if (nCmdShow == SW_HIDE)
  { //������� ��������� ��������
    int idx = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_PERCENT);
    m_wndStatusBar.SetPaneText(idx,_T(""));  
  }
}

//������������� �������� ��� ProgressCtrl
void CStatusBarManager::SetProgressRange(short nLower, short nUpper)
{
  MPCStatusBarPaneControlInfo* info = m_wndStatusBar.GetPanControl(ID_MSB_INDICATOR_PROGRESS);
  ASSERT(info);
  ((CProgressCtrl*)info->m_hWnd)->SetRange(nLower, nUpper);
}

//������������� ������� �������� ��� ProgressCtrl � ��������� ��������
void CStatusBarManager::SetProgressPos(int nPos)
{
  MPCStatusBarPaneControlInfo* info = m_wndStatusBar.GetPanControl(ID_MSB_INDICATOR_PROGRESS);
  ASSERT(info);
  CProgressCtrl* pProgress = static_cast<CProgressCtrl*>(info->m_hWnd);
  pProgress->SetPos(nPos);
  int idx = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_PERCENT);

  int nLower,nUpper;
  pProgress->GetRange(nLower,nUpper);

  int percents = ((100 * nPos) / (nUpper - nLower));
  CString str;
  str.Format(_T("%d%s"),percents,_T("%"));
  m_wndStatusBar.SetPaneText(idx,str);  
}

//������������� �������������� �����
void CStatusBarManager::SetInformationText(const CString& i_text)
{
  int idx = m_wndStatusBar.CommandToIndex(ID_SEPARATOR);
  m_wndStatusBar.SetPaneText(idx,i_text); 
}

//������������� ������� ������ � ����� (��������� ����������)
void CStatusBarManager::SetLoggerState(int i_state)
{	
  int idx_icon = m_wndStatusBar.CommandToIndex(ID_MSB_INDICATOR_LOGWR);  
  CStatusBarCtrl& status = m_wndStatusBar.GetStatusBarCtrl();

  switch(i_state)
  {
    case LOG_STATE_WRITING:
      status.SetIcon(idx_icon,m_LogWrIcon);
	  break;
    case LOG_STATE_STOPPED:
      status.SetIcon(idx_icon,NULL);
	  break;
  }
}