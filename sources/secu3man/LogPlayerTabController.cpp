 /****************************************************************
 *       SECU-3  - An open source, free engine control unit
 *    Designed by Alexey A. Shabelnikov. Ukraine, Gorlovka 2007.
 *       Microprocessors systems - design & programming.
 *    contacts:
 *              http://secu-3.narod.ru
 *              ICQ: 405-791-931
 ****************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "LogPlayerTabDlg.h"
#include "LogPlayerTabController.h"
#include "common/FastDelegate.h"
#include "io-core/ufcodes.h"
#include "CommunicationManager.h"
#include "StatusBarManager.h"

using namespace fastdelegate;
using namespace SECU3IO;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define EHKEY _T("LogPlayerCntr")


CLogPlayerTabController::CLogPlayerTabController(CLogPlayerTabDlg* i_view, CCommunicationManager* i_comm, CStatusBarManager* i_sbar)
: m_view(NULL)
, m_comm(NULL)
, m_sbar(NULL)
{
 //�������������� ��������� �� ��������������� �������
 m_view = i_view;
 m_comm = i_comm;
 m_sbar = i_sbar;
}


CLogPlayerTabController::~CLogPlayerTabController()
{
 //na  
}

//���������� ��������� ���������!
void CLogPlayerTabController::OnSettingsChanged(void)
{
 //�������� ����������� ��� ������� ��������� ���������������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION, true);   
}


//from MainTabController
void CLogPlayerTabController::OnActivate(void)
{
 //////////////////////////////////////////////////////////////////
 //���������� ���������� � ������ ������ �� SECU-3
 m_comm->m_pAppAdapter->AddEventHandler(this,EHKEY); 
 m_comm->setOnSettingsChanged(MakeDelegate(this,&CLogPlayerTabController::OnSettingsChanged)); 
 //////////////////////////////////////////////////////////////////
 
 //�������� ����������� ��� ������� ��������� ���������������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION);

 //���������� ��������� ��������� ��� ���������� ����������, ��� ��� OnConnection ���������� ������ ����
 //���������� ��� ����������� ������������� (����� ����������� ����������������� �����������)
 bool online_status = m_comm->m_pControlApp->GetOnlineStatus();
 OnConnection(online_status);
}

//from MainTabController
void CLogPlayerTabController::OnDeactivate(void)
{
 m_comm->m_pAppAdapter->RemoveEventHandler(EHKEY);
 m_sbar->SetInformationText(_T(""));
}

//hurrah!!! ������� ����� �� SECU-3
void CLogPlayerTabController::OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet)
{
 //������ ������: ������ ����� � �������������� �����
 if (i_descriptor == OP_COMP_NC)
 {
  const OPCompNc* p_ndata = (OPCompNc*)ip_packet;
  switch(p_ndata->opcode)
  {
   case OPCODE_EEPROM_PARAM_SAVE:
    m_sbar->SetInformationText(MLL::LoadString(IDS_PM_PARAMS_HAS_BEEN_SAVED));
    return;
  }		
 }
}

//��������� ����������� ����������!
void CLogPlayerTabController::OnConnection(const bool i_online)
{
 int state;
 ASSERT(m_sbar);

 if (i_online) //������� � ������
  state = CStatusBarManager::STATE_ONLINE;	
 else
  state = CStatusBarManager::STATE_OFFLINE;  
 
 m_sbar->SetConnectionState(state);
}


bool CLogPlayerTabController::OnClose(void)
{
 return true;
}
