/****************************************************************
 *       SECU-3  - An open source, free engine control unit
 *    Designed by Alexey A. Shabelnikov. Ukraine, Gorlovka 2007.
 *       Microprocessors systems - design & programming.
 *    contacts:
 *              http://secu-3.narod.ru
 *              ICQ: 405-791-931
 ****************************************************************/

#include "stdafx.h"
#include "KnockChannelTabController.h"
#include "common\fastdelegate.h"
#include "KnockChannelTabDlg.h"
#include "CommunicationManager.h"
#include "StatusBarManager.h"

using namespace fastdelegate;
using namespace SECU3IO;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define EHKEY _T("KnockChanCntr")

const BYTE default_context = SENSOR_DAT;
const BYTE kparams_context = KNOCK_PAR;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKnockChannelTabController::CKnockChannelTabController(CKnockChannelTabDlg* i_view, CCommunicationManager* i_comm, CStatusBarManager* i_sbar)
: m_view(NULL)
, m_comm(NULL)
, m_sbar(NULL)
, m_operation_state(-1)
, m_packet_processing_state(PPS_READ_MONITOR_DATA)
, m_parameters_changed(false)
{
  //�������������� ��������� �� ��������������� �������
  m_view = i_view;
  m_comm = i_comm;
  m_sbar = i_sbar;

  m_view->setOnSaveParameters(MakeDelegate(this,&CKnockChannelTabController::OnSaveParameters));
  m_view->m_knock_parameters_dlg.setFunctionOnChange(MakeDelegate(this,&CKnockChannelTabController::OnParametersChange));
}


CKnockChannelTabController::~CKnockChannelTabController()
{  
 //na
}

//���������� ��������� ���������!
void CKnockChannelTabController::OnSettingsChanged(void)
{
  //�������� ����������� ��� ������� ��������� ���������������� ����������
  m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION, true);   
}

//from MainTabController
void CKnockChannelTabController::OnActivate(void)
{
 m_comm->m_pAppAdapter->AddEventHandler(this,EHKEY); 
 m_comm->setOnSettingsChanged(MakeDelegate(this,&CKnockChannelTabController::OnSettingsChanged));

 //�������� ����������� ��� ������� ��������� ���������������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION);

 //��������� ������ �� �������� ����� �������������� ������� ������� ������ � SECU-3
 m_params_changes_timer.SetTimer(this,&CKnockChannelTabController::OnParamsChangesTimer,500);

 //���������� ��������� ��������� ��� ���������� ����������, ��� ��� OnConnection ���������� ������ ����
 //���������� ��� ����������� ������������� (����� ����������� ����������������� �����������)
 bool online_status = m_comm->m_pControlApp->GetOnlineStatus();
 OnConnection(online_status);
}

//from MainTabController
void CKnockChannelTabController::OnDeactivate(void)
{
 m_comm->m_pAppAdapter->RemoveEventHandler(EHKEY);
 m_sbar->SetInformationText(_T(""));
}

void CKnockChannelTabController::OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet)
{
 //������ ������: ������ ����� � �������������� �����
  if (i_descriptor == OP_COMP_NC)
  {
   const OPCompNc* p_ndata = (OPCompNc*)ip_packet;
   switch(p_ndata->opcode)
   {
    case OPCODE_EEPROM_PARAM_SAVE:
     m_sbar->SetInformationText("��������� ���� ���������.");
     return;
   }		
  }

  //��������� ���������� ������� � ����������� �� �������� ������
  switch(m_packet_processing_state)
  {
	case PPS_READ_NECESSARY_PARAMETERS:  //������ ��������� ����������
	  if (ReadNecessaryParametersFromSECU(i_descriptor,ip_packet))
	  {
	    m_packet_processing_state = PPS_BEFORE_READ_MONITOR_DATA;

	    //������������ ��������� - ����� ��������� ������
        bool state = m_comm->m_pControlApp->GetOnlineStatus();
        m_view->m_knock_parameters_dlg.Enable(state);
		m_view->EnableAll(state);
	  }
	  break;
	
    case PPS_BEFORE_READ_MONITOR_DATA: //� ���� ����� �� �������� ������ ���� ���
	  if (i_descriptor!=default_context)
	  {
       m_comm->m_pControlApp->ChangeContext(default_context); //!!!		  		
	  }
	  else
	  {
       //������������� �������� ��������, ��������� �� � ��������� � �������� �����
	  /* m_view->m_MIDeskDlg.SetValues((SensorDat*)(ip_packet)); 	
       bool state = m_comm->m_pControlApp->GetOnlineStatus();
	   m_view->m_MIDeskDlg.Enable(state);*/
	   m_packet_processing_state = PPS_READ_MONITOR_DATA;
	  }
	  break;

	case PPS_READ_MONITOR_DATA:  //��������� ������ ��� ��������       
	  if (i_descriptor!=default_context)
	  {
       m_comm->m_pControlApp->ChangeContext(default_context); //!!!		  		
	  }
	  else
	  {
	   /*m_view->m_MIDeskDlg.SetValues((SensorDat*)(ip_packet)); 	*/
	  }
	  break;	
	}//switch
}

void CKnockChannelTabController::OnConnection(const bool i_online)
{
 int state;
 ASSERT(m_sbar);

 if (i_online) //������� � ������
 {
  state = CStatusBarManager::STATE_ONLINE;
  StartReadingNecessaryParameters();
 }
 else
 {
  state = CStatusBarManager::STATE_OFFLINE;  
 }
 
 if (i_online==false) 
 { //����� ������ ���������, �������� � ������ ����� ����� ���������� ���������������� ��������
  m_view->m_knock_parameters_dlg.Enable(i_online);
  m_view->EnableAll(i_online);
 }

 m_sbar->SetConnectionState(state);
}

void CKnockChannelTabController::StartReadingNecessaryParameters(void) 
{
  m_comm->m_pControlApp->ChangeContext(kparams_context);  //change context!	  
  m_packet_processing_state = PPS_READ_NECESSARY_PARAMETERS;
  m_operation_state = 0;
}


//���������� true ����� ������ �������� ���������
//m_operation_state = 0 ��� �������
bool CKnockChannelTabController::ReadNecessaryParametersFromSECU(const BYTE i_descriptor, const void* i_packet_data)
{
  m_sbar->SetInformationText("������ ���������� ��..."); 

  switch(m_operation_state)
  {
    case 0:  //������� ������ � ���������� ���� �����������
	{
      if (i_descriptor!=kparams_context)
	  {
        m_comm->m_pControlApp->ChangeContext(kparams_context);	//!!!	  		  
	  }
	  else
	  {//��� ��� ����!
	    m_view->m_knock_parameters_dlg.SetValues((SECU3IO::KnockPar*)i_packet_data);
           
	    //������� ������������� �������
        m_operation_state = -1; //������� �� - �������� ���������
 	    m_sbar->SetInformationText("������.");
        return true; //�������� ���������
	  }
	}	
    break;
  }//switch

  return false; //�� ���������� ������...
}


bool CKnockChannelTabController::OnClose(void)
{
  return true;
}

void CKnockChannelTabController::OnSaveParameters(void)
{
 m_sbar->SetInformationText("���������� ���������� ��...");
 OPCompNc packet_data;
 packet_data.opcode = OPCODE_EEPROM_PARAM_SAVE;
 m_comm->m_pControlApp->SendPacket(OP_COMP_NC,&packet_data);
}

void CKnockChannelTabController::OnParametersChange(void)
{
  m_parameters_changed = true;
}

//�������� ������� � ����������� � SECU ����� ����������� �� ���� ��� ����� ����� �����������
void CKnockChannelTabController::OnParamsChangesTimer(void)
{ 
  if (m_parameters_changed)
  {
    //�������� ������ �� view � ��������� �� �� ��������� ����� 
    SECU3IO::KnockPar packet_data;   
    m_view->m_knock_parameters_dlg.GetValues(&packet_data);

    //������� ���������� ������������� ������ (��� �������� ��������� �����, ������� �� ������ � ����� ����� �� ������������)
    m_comm->m_pControlApp->SendPacket(kparams_context, &packet_data);

    m_parameters_changed = false; //���������� ������� - ���������� �������
  }
}

