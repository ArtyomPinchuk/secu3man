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
              http://secu-3.org
              email: shabelnikov@secu-3.org
*/

#include "stdafx.h"
#include "Resources\resource.h"
#include <limits>
#include <math.h>
#include <numeric>
#include "Application\CommunicationManager.h"
#include "common\fastdelegate.h"
#include "common\MathHelpers.h"
#include "FirmwareTabController.h"
#include "KnockChannelTabController.h"
#include "MainFrame\StatusBarManager.h"
#include "ParamDesk\KnockPageDlg.h"
#include "TabControllersCommunicator.h"
#include "TabDialogs\KnockChannelTabDlg.h"

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
, m_k_desired_level(1.0f)
{
 //�������������� ��������� �� ��������������� �������
 m_view = i_view;
 m_comm = i_comm;
 m_sbar = i_sbar;

 m_view->setOnSaveParameters(MakeDelegate(this,&CKnockChannelTabController::OnSaveParameters));
 m_view->mp_knock_parameters_dlg->setFunctionOnChange(MakeDelegate(this,&CKnockChannelTabController::OnParametersChange));
 m_view->setOnCopyToAttenuatorTable(MakeDelegate(this,&CKnockChannelTabController::OnCopyToAttenuatorTable));
 m_view->setOnClearFunction(MakeDelegate(this,&CKnockChannelTabController::OnClearFunction));

 _InitializeRPMKnockFunctionBuffer();
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

 //��������� ������ ���� �������� �� ������� �� ������� "������ ��������"
 CFirmwareTabController* p_controller = static_cast<CFirmwareTabController*>
 (TabControllersCommunicator::GetInstance()->GetReference(TCC_FIRMWARE_TAB_CONTROLLER));
 m_view->EnableCopyToAttenuatorTableButton(p_controller->IsFirmwareOpened());
 m_view->EnableClearFunctionButton(true);

 //��������������� �������� ������� �������
 m_view->SetDesiredLevel(m_k_desired_level);
}

//from MainTabController
void CKnockChannelTabController::OnDeactivate(void)
{
 m_params_changes_timer.KillTimer();
 m_comm->m_pAppAdapter->RemoveEventHandler(EHKEY);
 m_sbar->SetInformationText(_T(""));

 //��������� �������� ������� �������
 m_k_desired_level = m_view->GetDesiredLevel();
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
   m_sbar->SetInformationText(MLL::LoadString(IDS_KC_PARAMETERS_HAS_BEEN_SAVED));
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
    m_view->mp_knock_parameters_dlg->Enable(state);
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
    _HandleSample((SensorDat*)(ip_packet), true);
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
    _HandleSample((SensorDat*)(ip_packet), false);
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
  m_view->mp_knock_parameters_dlg->Enable(i_online);
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
 m_sbar->SetInformationText(MLL::LoadString(IDS_KC_READING_PARAMETERS));

 switch(m_operation_state)
 {
  case 0:  //������� ������ � ���������� ���� �����������
  {
   if (i_descriptor!=kparams_context)
   {
    m_comm->m_pControlApp->ChangeContext(kparams_context); //!!!
   }
   else
   {//��� ��� ����!
    m_view->mp_knock_parameters_dlg->SetValues((SECU3IO::KnockPar*)i_packet_data);

    //������� ������������� �������
    m_operation_state = -1; //������� �� - �������� ���������
    m_sbar->SetInformationText(MLL::LoadString(IDS_KC_READY));
    return true; //�������� ���������
   }
  }
  break;
 }//switch

 return false; //�� ���������� ������
}

bool CKnockChannelTabController::OnClose(void)
{
 return true;
}

bool CKnockChannelTabController::OnAskFullScreen(void)
{
 return false;
}

void CKnockChannelTabController::OnFullScreen(bool i_what, const CRect& i_rect)
{
 //na
}

void CKnockChannelTabController::OnSaveParameters(void)
{
 m_sbar->SetInformationText(MLL::LoadString(IDS_KC_WRITING_PARAMETERS));
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
  m_view->mp_knock_parameters_dlg->GetValues(&packet_data);

  //������� ���������� ������������� ������ (��� �������� ��������� �����, ������� �� ������ � ����� ����� �� ������������)
  m_comm->m_pControlApp->SendPacket(kparams_context, &packet_data);

  m_parameters_changed = false; //���������� ������� - ���������� �������
 }

 //������������ ������ �� ������ � ������ �� �������������
 std::vector<float> values;
 _PerformAverageOfRPMKnockFunctionValues(values);
 m_view->SetRPMKnockSignal(values);
}

void CKnockChannelTabController::_HandleSample(SECU3IO::SensorDat* p_packet, bool i_first_time)
{
 //��������� ����� �������� � �����������
 m_view->AppendPoint(p_packet->knock_k);

 //��������� ����� ��������� ����� ������� ������� �� �� ��������
 //1. ��������� ������ � �������. 200 - ������� � ������ �����, 60 - ��� �� ��������.
 //2. ���� ������ ������� �� ��������� ���������� - ��������� ��������. ���� ������ �������
 //��������� ����������, �� ��������� ����� �������� ������ � ������������ � ������� ��������.
 int index_unchecked = MathHelpers::Round((p_packet->frequen - 200.f) / 60.f);
 if (index_unchecked < 0)
  index_unchecked = 0;
 if (index_unchecked > (CKnockChannelTabDlg::RPM_KNOCK_SIGNAL_POINTS - 1))
  index_unchecked = (CKnockChannelTabDlg::RPM_KNOCK_SIGNAL_POINTS - 1);
 size_t index = (size_t)index_unchecked;

 if (m_rpm_knock_signal[index].size() < RPM_KNOCK_SAMPLES_PER_POINT)
   m_rpm_knock_signal[index].push_back(p_packet->knock_k);
 else
 {
  size_t &ii = m_rpm_knock_signal_ii[index];
  m_rpm_knock_signal[index][ii] = p_packet->knock_k;
  ii = ii < (RPM_KNOCK_SAMPLES_PER_POINT - 1) ? ii + 1 : 0;
 }
}

void CKnockChannelTabController::_PerformAverageOfRPMKnockFunctionValues(std::vector<float> &o_function)
{
 o_function.clear();
 std::vector<std::vector<float> >::const_iterator it = m_rpm_knock_signal.begin();
 for(; it != m_rpm_knock_signal.end(); ++it)
 {
  float value = std::accumulate((it)->begin(), (it)->end(), 0.f);
  value = it->size() ? value / it->size() : 0; //avoid divide by zero!
  o_function.push_back(value);
 }
}

void CKnockChannelTabController::_InitializeRPMKnockFunctionBuffer(void)
{
 m_rpm_knock_signal.clear();
 m_rpm_knock_signal_ii.clear();
 for(size_t i = 0; i < CKnockChannelTabDlg::RPM_KNOCK_SIGNAL_POINTS; ++i)
 {
  m_rpm_knock_signal.push_back(std::vector<float>());
  m_rpm_knock_signal_ii.push_back(0);
 }
}

void CKnockChannelTabController::OnCopyToAttenuatorTable(void)
{
 CFirmwareTabController* p_controller = static_cast<CFirmwareTabController*>
 (TabControllersCommunicator::GetInstance()->GetReference(TCC_FIRMWARE_TAB_CONTROLLER));

 //�������� ������� ����������� ������� ���� �� ��������
 std::vector<float> values;
 _PerformAverageOfRPMKnockFunctionValues(values);

 //�������� ����������� ������� (���������� ���� 5 �������)
 float function_out[CKnockChannelTabDlg::RPM_KNOCK_SIGNAL_POINTS];
 float function_inp[CKnockChannelTabDlg::RPM_KNOCK_SIGNAL_POINTS];
 std::copy(values.begin(), values.end(), function_inp);
 VERIFY(MathHelpers::Smooth1D(function_inp, function_out, CKnockChannelTabDlg::RPM_KNOCK_SIGNAL_POINTS, 5));

 //�������� �������� ��������� ������� �������
 float level = m_view->GetDesiredLevel();

 //�������� ������� ������� �����������
 float array[CKnockChannelTabDlg::RPM_KNOCK_SIGNAL_POINTS];
 p_controller->GetAttenuatorMap(array);

 //�������� ������ ������������� �������� � ���������� �� � ������� �����������
 //�����. �������� ��� ������ ������� ������������� ������ �� ������� ��������
 //������� � �������� ��������� ������ �������.
 for(size_t i =0; i < CKnockChannelTabDlg::RPM_KNOCK_SIGNAL_POINTS; ++i)
 {
  //����� �� ������� ��� ����� �������� �����. ��������
  float correcting_gain;
  if (function_out[i]!=0)
    correcting_gain = level / function_out[i];
  else
    correcting_gain = SECU3IO::hip9011_attenuator_gains[0];

  //��������� ����� ����������� ��������
  size_t old_gain_index = MathHelpers::Round(array[i]);
  float new_gain = SECU3IO::hip9011_attenuator_gains[old_gain_index] * correcting_gain;

  //���� ��������� ����������� �������� � ������� ������������� ��������
  size_t new_gain_index = 0;
  float smaller_diff = FLT_MAX;
  for(size_t j = 0; j < SECU3IO::GAIN_FREQUENCES_SIZE; ++j)
  {
   float gain = SECU3IO::hip9011_attenuator_gains[j];
   float diff = fabs(gain - new_gain);
   if (diff < smaller_diff)
   {
    smaller_diff = diff;
    new_gain_index = j;
   }
  }

  //�������� ����������� �������� ������ ��� ��� �����, ��� ������� ���� �������� ����������
  if (m_rpm_knock_signal[i].size() > 0)
    array[i] = (float)new_gain_index;
 }

 //������������� ����������� ������� �����������
 p_controller->SetAttenuatorMap(array);
}

void CKnockChannelTabController::OnClearFunction(void)
{
 _InitializeRPMKnockFunctionBuffer();
 //������������ ������ �� ������ � ������ �� �������������
 std::vector<float> values;
 _PerformAverageOfRPMKnockFunctionValues(values);
 m_view->SetRPMKnockSignal(values);
}
