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
#include "common/FastDelegate.h"
#include "CheckEngineTabController.h"
#include "CheckEngineTabDlg.h"
#include "io-core/ce_errors.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace fastdelegate;
using namespace SECU3IO;

#define EHKEY _T("CheckEngineCntr")

const BYTE default_context = SENSOR_DAT;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckEngineTabController::CCheckEngineTabController(CCheckEngineTabDlg* i_view, CCommunicationManager* i_comm, CStatusBarManager* i_sbar)
: m_view(NULL)
, m_comm(NULL)
, m_sbar(NULL)
, m_real_time_errors_mode(false)
{
  //�������������� ��������� �� ��������������� �������
  m_view = i_view;
  m_comm = i_comm;
  m_sbar = i_sbar;

  m_view->setOnRealTimeErrors(MakeDelegate(this,&CCheckEngineTabController::OnRealTimeErrors));
  m_view->setOnReadSavedErrors(MakeDelegate(this,&CCheckEngineTabController::OnReadSavedErrors));
  m_view->setOnWriteSavedErrors(MakeDelegate(this,&CCheckEngineTabController::OnWriteSavedErrors)); 
  m_view->setOnListSetAllErrors(MakeDelegate(this,&CCheckEngineTabController::OnListSetAllErrors)); 
  m_view->setOnListClearAllErrors(MakeDelegate(this,&CCheckEngineTabController::OnListClearAllErrors)); 
  //��������� ����� ������� ����� ��������� ������ ������������ � ������. ����� ���� �������������� ������
  //������������ ��������� ������ � ���� ����� � ID-���� ��� ������������� ��������� ������.
  m_errors_ids.insert(ErrorsIDContainer::value_type(ECUERROR_CKPS_MALFUNCTION, _T("���� � ������ ���� - ������ �������������")));
  m_errors_ids.insert(ErrorsIDContainer::value_type(ECUERROR_EEPROM_PARAM_BROKEN, _T("��������� � EEPROM ���������� (�R�)")));
  m_errors_ids.insert(ErrorsIDContainer::value_type(ECUERROR_PROGRAM_CODE_BROKEN, _T("��� �������� ��������� (�R�)")));
  m_errors_ids.insert(ErrorsIDContainer::value_type(ECUERROR_KSP_CHIP_FAILED, _T("��������� ��������� �� ��������")));
}


CCheckEngineTabController::~CCheckEngineTabController()
{  
 //na
}

//���������� ��������� ���������!
void CCheckEngineTabController::OnSettingsChanged(void)
{
 //�������� ����������� ��� ������� ��������� ���������������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION, true);   
}

//from MainTabController
void CCheckEngineTabController::OnActivate(void)
{
 //��������� ������ ������ ������
 ErrorsIDContainer::const_iterator it = m_errors_ids.begin(); 
 for(; it != m_errors_ids.end(); ++it)
  m_view->AppendErrorsList((*it).first, (*it).second, false);

 m_view->EnableRWButtons(true);

 m_comm->m_pAppAdapter->AddEventHandler(this,EHKEY); 
 m_comm->SetOnSettingsChanged(MakeDelegate(this,&CCheckEngineTabController::OnSettingsChanged));

 m_real_time_errors_mode = false;

 //�������� ����������� ��� ������� ��������� ���������������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION);

 //���������� ��������� ��������� ��� ���������� ����������, ��� ��� OnConnection ���������� ������ ����
 //���������� ��� ����������� ������������� (����� ����������� ����������������� �����������)
 bool online_status = m_comm->m_pControlApp->GetOnlineStatus();
 OnConnection(online_status);
}

//from MainTabController
void CCheckEngineTabController::OnDeactivate(void)
{
  m_comm->m_pAppAdapter->RemoveEventHandler(EHKEY);
  m_sbar->SetInformationText(_T(""));
}

void CCheckEngineTabController::OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet)
{
 SECU3IO::SECU3Packet m_recepted_packet;

 ////////////////////////////////////////////////////////////////////////
 //������������ ������, ����������� ������, � ����� ������������ ������� 
 m_comm->m_pControlApp->EnterCriticalSection();
 memcpy(&m_recepted_packet,ip_packet,sizeof(SECU3IO::SECU3Packet));
 m_comm->m_pControlApp->LeaveCriticalSection();
  ////////////////////////////////////////////////////////////////////////

 //������ �������� � ���������� ������ ������
 _OnPacketReceived(i_descriptor,&m_recepted_packet);
}

void CCheckEngineTabController::_OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet)
{
  //������ ������: ������ ����� � �������������� �����
  if (i_descriptor == OP_COMP_NC)
  {
   const OPCompNc* p_ndata = (OPCompNc*)ip_packet;
   switch(p_ndata->opcode)
   {
    case OPCODE_CE_SAVE_ERRORS: //��������� ����� ���� ������ ���� ��������� � EEPROM
     m_sbar->SetInformationText("���� ������ ���� ������� ���������!");
     return;
   }		
  }
 
 if (i_descriptor == CE_ERR_CODES && m_real_time_errors_mode)
 { //������ ����� ���������� ���� ������ (� �������� �������)
  CEErrors* errors = reinterpret_cast<CEErrors*>(ip_packet);
  _SetErrorsToList(errors);
 }

 if (i_descriptor == CE_SAVED_ERR)
 { //������� ������ ����������� �� EEPROM
  CEErrors* errors = reinterpret_cast<CEErrors*>(ip_packet);
  m_sbar->SetInformationText("����������� ���� ������ ������� ���������!");
  _SetErrorsToList(errors); 
 }
}


void CCheckEngineTabController::OnConnection(const bool i_online)
{
 int state;
  ASSERT(m_sbar);

  if (i_online) //������� � ������
  {
   state = CStatusBarManager::STATE_ONLINE;
   //���� ���������� ������� ������ ��������� �������, �� ������ ��������
   m_comm->m_pControlApp->ChangeContext(m_real_time_errors_mode ? CE_ERR_CODES : default_context);  
   m_view->EnableAll(true);
  }
  else
  {
	state = CStatusBarManager::STATE_OFFLINE;  
  }
 
  //����� �� ����� ������ ��������� ������, � ��������� �� ����� ������ �����, ����� ��������� ������������ 
  if (i_online==false) 
  {
   m_view->EnableAll(false); 
  }

  m_sbar->SetConnectionState(state);
}

//����������� ������� ��� ���������� ����� ������� � ����������, ����� �� ��� �������.
bool CCheckEngineTabController::OnClose(void)
{
  return true;
}

//������������� "������" ��� ���� "��������� ������ � �������� �������".
void CCheckEngineTabController::OnRealTimeErrors(void) 
{
  bool checked  = m_view->GetRealTimeErrorsCheck();
  m_real_time_errors_mode = checked;
  m_view->EnableRWButtons(!checked);

  if (checked)
  {
   m_sbar->SetInformationText("");
   m_comm->m_pControlApp->ChangeContext(CE_ERR_CODES);  //change context!
  }
  else
  {
   //������������� �������� �� ���������
   m_comm->m_pControlApp->ChangeContext(default_context);    
  }
}


//������� ������� ������� �������� SECU-3 ��������� ���� ������ �� EEPROM 
//� �������� �� � �������� (��� ����� � ������)
void CCheckEngineTabController::OnReadSavedErrors(void) 
{
 m_sbar->SetInformationText("������ ����� ������ �� EEPROM...");
 OPCompNc packet_data;
 packet_data.opcode = OPCODE_CE_SAVE_ERRORS;
 m_comm->m_pControlApp->SendPacket(OP_COMP_NC,&packet_data);
}

//������ �� ������ ��������� ��� ������ � �������� ������ � ������ ������ � SECU-3.
//����� ��������� ����� SECU-3 �������� ���� ������ � EEPROM.
void CCheckEngineTabController::OnWriteSavedErrors(void) 
{
 m_sbar->SetInformationText("������ ����� ������ � EEPROM...");
 CEErrors packet_data;
 _GetErrorsFromList(&packet_data);
 m_comm->m_pControlApp->SendPacket(CE_SAVED_ERR,&packet_data);
}

//��������� ���� ��� ������ � ������
void CCheckEngineTabController::OnListSetAllErrors(void)
{
 ErrorsIDContainer::const_iterator it = m_errors_ids.begin(); 
 for(; it != m_errors_ids.end(); ++it)
  m_view->SetErrorState((*it).first, true); 
}

//������� ���� ��� ������ � ������
void CCheckEngineTabController::OnListClearAllErrors(void)
{
 ErrorsIDContainer::const_iterator it = m_errors_ids.begin(); 
 for(; it != m_errors_ids.end(); ++it)
  m_view->SetErrorState((*it).first, false); 
}

//������������ ������ ����� ������ �� ��������� ������ � ��� ����� ������
void CCheckEngineTabController::_SetErrorsToList(const CEErrors* ip_errors)
{
 ErrorsIDContainer::const_iterator it = m_errors_ids.begin(); 
 for(; it != m_errors_ids.end(); ++it)
 {
  DWORD bit = 1 << ((*it).first);   
  bool state = ((bit & ip_errors->flags)!=0) ? true : false;
  m_view->SetErrorState((*it).first, state); 
 }
}

//������������ ��������� ��������� ������ � ������ ��������� ������
void CCheckEngineTabController::_GetErrorsFromList(SECU3IO::CEErrors* op_errors)
{
 op_errors->flags = 0;
 ErrorsIDContainer::const_iterator it = m_errors_ids.begin(); 
 for(; it != m_errors_ids.end(); ++it)
 {
  DWORD value = m_view->GetErrorState((*it).first) ? 0x00000001 : 0x00000000; 
  DWORD bit = value << ((*it).first);   
  op_errors->flags|= bit;
 }
}

