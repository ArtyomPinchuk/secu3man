
#include "stdafx.h"
#include "KnockChannelTabController.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKnockChannelTabController::CKnockChannelTabController(CKnockChannelTabDlg* i_view, CCommunicationManager* i_comm, CStatusBarManager* i_sbar)
: m_view(NULL)
, m_comm(NULL)
, m_sbar(NULL)
{
  //�������������� ��������� �� ��������������� �������
  m_view = i_view;
  m_comm = i_comm;
  m_sbar = i_sbar;
}


CKnockChannelTabController::~CKnockChannelTabController()
{  
}

//from MainTabController
void CKnockChannelTabController::OnActivate(void)
{
}

//from MainTabController
void CKnockChannelTabController::OnDeactivate(void)
{
}

bool CKnockChannelTabController::OnClose(void)
{
  return true;
}
