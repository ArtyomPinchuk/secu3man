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

/** \file MiscPageDlg.cpp
 * \author Alexey A. Shabelnikov
 */

#include "stdafx.h"
#include "Resources/resource.h"
#include "MiscPageDlg.h"
#include "ui-core/ToolTipCtrlEx.h"
#include "ui-core/ddx_helpers.h"

const UINT CMiscPageDlg::IDD = IDD_PD_MISC_PAGE;

BEGIN_MESSAGE_MAP(CMiscPageDlg, Super)
 ON_EN_CHANGE(IDC_PD_MISC_PACKET_PERIOD_EDIT, OnChangeData)
 ON_EN_CHANGE(IDC_PD_MISC_IGNCUTOFF_RPM_EDIT, OnChangeData)
 ON_EN_CHANGE(IDC_PD_MISC_HALL_OUTPUT_START_EDIT, OnChangeData)
 ON_EN_CHANGE(IDC_PD_MISC_HALL_OUTPUT_DURAT_EDIT, OnChangeData)
 ON_CBN_SELCHANGE(IDC_PD_MISC_UART_SPEED_COMBO, OnChangeData)
 ON_BN_CLICKED(IDC_PD_MISC_IGNCUTOFF_CHECK, OnIgncutoffCheck)
 ON_BN_CLICKED(IDC_PD_MISC_FLPMP_OFFONGAS_CHECK, OnChangeData)
 ON_BN_CLICKED(IDC_PD_MISC_INJ_OFFONGAS_CHECK, OnChangeData)

 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_UART_SPEED_COMBO_CAPTION, OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_UART_SPEED_COMBO, OnUpdateControls)

 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_PACKET_PERIOD_EDIT, OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_PACKET_PERIOD_CAPTION, OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_PACKET_PERIOD_UNIT, OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_PACKET_PERIOD_SPIN, OnUpdateControls)

 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_IGNCUTOFF_CHECK, OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_IGNCUTOFF_RPM_EDIT, OnUpdateControlsIgncutoff)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_IGNCUTOFF_RPM_UNIT, OnUpdateControlsIgncutoff)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_IGNCUTOFF_RPM_SPIN, OnUpdateControlsIgncutoff)

 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_HALL_OUTPUT_GROUP, OnUpdateControlsHOP)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_HALL_OUTPUT_START_EDIT, OnUpdateControlsHOP)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_HALL_OUTPUT_START_SPIN, OnUpdateControlsHOP)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_HALL_OUTPUT_START_UNIT, OnUpdateControlsHOP)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_HALL_OUTPUT_DURAT_EDIT, OnUpdateControlsHOP)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_HALL_OUTPUT_DURAT_SPIN, OnUpdateControlsHOP)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_HALL_OUTPUT_DURAT_UNIT, OnUpdateControlsHOP)

 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_FLPMP_OFFONGAS_CHECK, OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_MISC_INJ_OFFONGAS_CHECK, OnUpdateControlsInjTurnoffOnGas)
END_MESSAGE_MAP()

CMiscPageDlg::CMiscPageDlg(CWnd* pParent /*=NULL*/)
: Super(CMiscPageDlg::IDD, pParent)
, m_enabled(false)
, m_hall_output_enabled(false)
, m_inj_turnoff_enabled(false)
, m_uart_speed_cb_index(0)
, m_packet_period_edit(CEditEx::MODE_INT, true)
, m_igncutoff_rpm_edit(CEditEx::MODE_INT, true)
, m_hop_start_edit(CEditEx::MODE_INT, true)
, m_hop_durat_edit(CEditEx::MODE_INT, true)
{
 m_params.baud_rate = CBR_9600;
 m_params.period_ms = 0;
 m_params.ign_cutoff = 0;
 m_params.ign_cutoff_thrd = 0;
 m_params.hop_start_cogs = 0;
 m_params.hop_durat_cogs = 0;
 m_params.flpmp_offongas = true;
 m_params.inj_offongas = false;
}

LPCTSTR CMiscPageDlg::GetDialogID(void) const
{
 return (LPCTSTR)IDD;
}

void CMiscPageDlg::DoDataExchange(CDataExchange* pDX)
{
 Super::DoDataExchange(pDX);

 DDX_Control(pDX,IDC_PD_MISC_UART_SPEED_COMBO_CAPTION, m_uart_speed_label);
 DDX_Control(pDX,IDC_PD_MISC_UART_SPEED_COMBO, m_uart_speed_combo);

 DDX_Control(pDX,IDC_PD_MISC_PACKET_PERIOD_EDIT, m_packet_period_edit);
 DDX_Control(pDX,IDC_PD_MISC_PACKET_PERIOD_CAPTION, m_packet_period_label);
 DDX_Control(pDX,IDC_PD_MISC_PACKET_PERIOD_UNIT, m_packet_period_unit);
 DDX_Control(pDX,IDC_PD_MISC_PACKET_PERIOD_SPIN, m_packet_period_spin);

 DDX_Control(pDX,IDC_PD_MISC_IGNCUTOFF_CHECK, m_igncutoff_check);
 DDX_Control(pDX,IDC_PD_MISC_IGNCUTOFF_RPM_EDIT, m_igncutoff_rpm_edit);
 DDX_Control(pDX,IDC_PD_MISC_IGNCUTOFF_RPM_UNIT, m_igncutoff_rpm_unit);
 DDX_Control(pDX,IDC_PD_MISC_IGNCUTOFF_RPM_SPIN, m_igncutoff_rpm_spin);

 DDX_Control(pDX,IDC_PD_MISC_HALL_OUTPUT_START_EDIT, m_hop_start_edit);
 DDX_Control(pDX,IDC_PD_MISC_HALL_OUTPUT_START_UNIT, m_hop_start_unit);
 DDX_Control(pDX,IDC_PD_MISC_HALL_OUTPUT_START_SPIN, m_hop_start_spin);

 DDX_Control(pDX,IDC_PD_MISC_HALL_OUTPUT_DURAT_EDIT, m_hop_durat_edit);
 DDX_Control(pDX,IDC_PD_MISC_HALL_OUTPUT_DURAT_UNIT, m_hop_durat_unit);
 DDX_Control(pDX,IDC_PD_MISC_HALL_OUTPUT_DURAT_SPIN, m_hop_durat_spin);

 DDX_Control(pDX,IDC_PD_MISC_FLPMP_OFFONGAS_CHECK, m_flpmp_offongas_check);
 DDX_Control(pDX,IDC_PD_MISC_INJ_OFFONGAS_CHECK, m_inj_offongas_check);

 DDX_CBIndex(pDX, IDC_PD_MISC_UART_SPEED_COMBO, m_uart_speed_cb_index);
 m_packet_period_edit.DDX_Value(pDX, IDC_PD_MISC_PACKET_PERIOD_EDIT, m_params.period_ms);
 m_igncutoff_rpm_edit.DDX_Value(pDX, IDC_PD_MISC_IGNCUTOFF_RPM_EDIT, m_params.ign_cutoff_thrd);
 m_hop_start_edit.DDX_Value(pDX, IDC_PD_MISC_HALL_OUTPUT_START_EDIT, m_params.hop_start_cogs);
 m_hop_durat_edit.DDX_Value(pDX, IDC_PD_MISC_HALL_OUTPUT_DURAT_EDIT, m_params.hop_durat_cogs);
 DDX_Check_UCHAR(pDX, IDC_PD_MISC_IGNCUTOFF_CHECK, m_params.ign_cutoff);
 DDX_Check_bool(pDX, IDC_PD_MISC_FLPMP_OFFONGAS_CHECK, m_params.flpmp_offongas);
 DDX_Check_bool(pDX, IDC_PD_MISC_INJ_OFFONGAS_CHECK, m_params.inj_offongas);
}

/////////////////////////////////////////////////////////////////////////////
// CMiscPageDlg message handlers

//Update controls
void CMiscPageDlg::OnUpdateControls(CCmdUI* pCmdUI)
{
 pCmdUI->Enable(m_enabled);
}

void CMiscPageDlg::OnUpdateControlsHOP(CCmdUI* pCmdUI)
{
 pCmdUI->Enable(m_enabled && m_hall_output_enabled);
}

void CMiscPageDlg::OnUpdateControlsInjTurnoffOnGas(CCmdUI* pCmdUI)
{
 pCmdUI->Enable(m_enabled && m_inj_turnoff_enabled);
}

void CMiscPageDlg::OnUpdateControlsIgncutoff(CCmdUI* pCmdUI)
{
 pCmdUI->Enable(m_enabled && m_igncutoff_check.GetCheck() == BST_CHECKED);
}

void CMiscPageDlg::OnChangeData()
{
 UpdateData();
 OnChangeNotify(); //notify event receiver about change of view content(see class ParamPageEvents)
}

BOOL CMiscPageDlg::OnInitDialog()
{
 Super::OnInitDialog();

 m_packet_period_edit.SetLimitText(3);
 m_packet_period_edit.SetDecimalPlaces(3);
 m_packet_period_spin.SetBuddy(&m_packet_period_edit);
 m_packet_period_spin.SetRangeAndDelta(0, 500, 10);
 m_packet_period_edit.SetRange(0, 500);

 m_igncutoff_rpm_edit.SetLimitText(5);
 m_igncutoff_rpm_edit.SetDecimalPlaces(5);
 m_igncutoff_rpm_spin.SetBuddy(&m_igncutoff_rpm_edit);
 m_igncutoff_rpm_spin.SetRangeAndDelta(1000, 18000, 10);
 m_igncutoff_rpm_edit.SetRange(1000, 18000);
 
 m_hop_start_edit.SetLimitText(3);
 m_hop_start_edit.SetDecimalPlaces(3);
 m_hop_start_spin.SetBuddy(&m_hop_start_edit);
 m_hop_start_spin.SetRangeAndDelta(-6, 12, 1);
 m_hop_start_edit.SetRange(-6, 12);

 m_hop_durat_edit.SetLimitText(3);
 m_hop_durat_edit.SetDecimalPlaces(3);
 m_hop_durat_spin.SetBuddy(&m_hop_durat_edit);
 m_hop_durat_spin.SetRangeAndDelta(1, 30, 1);
 m_hop_durat_edit.SetRange(1, 30);

 BRCType br;
 for(size_t i = 0; i < SECU3IO::SECU3_ALLOWABLE_UART_DIVISORS_COUNT; ++i)
  br.push_back(SECU3IO::secu3_allowable_uart_divisors[i].first);
 FillUARTSpeedComboBox(br); //initialize combobox

 UpdateData(FALSE);  //initialize dialog controls with data

 UpdateDialogControls(this, TRUE);
 return TRUE;  // return TRUE unless you set the focus to a control
}

void CMiscPageDlg::OnIgncutoffCheck()
{
 UpdateData();
 OnChangeNotify();
 UpdateDialogControls(this, TRUE);
}

//����������/���������� ���������� (���� ���������)
void CMiscPageDlg::Enable(bool enable)
{
 if (m_enabled == enable)
  return; //already has needed state
 m_enabled = enable;
 if (::IsWindow(m_hWnd))
  UpdateDialogControls(this, TRUE);
}

//��� � �����������?
bool CMiscPageDlg::IsEnabled(void)
{
 return m_enabled;
}

void CMiscPageDlg::EnableHallOutputParams(bool enable)
{
 if (m_hall_output_enabled == enable)
  return; //already has needed state
 m_hall_output_enabled = enable;
 if (::IsWindow(this->m_hWnd))
  UpdateDialogControls(this, TRUE);
}

void CMiscPageDlg::EnableFuelInjection(bool i_enable)
{
 if (m_inj_turnoff_enabled == i_enable)
  return; //already
 m_inj_turnoff_enabled = i_enable;
 if (::IsWindow(this->m_hWnd))
  UpdateDialogControls(this, TRUE);
}

//��� ������� ���������� ������������ ����� ���� �������� ������ �� �������
void CMiscPageDlg::GetValues(SECU3IO::MiscelPar* o_values)
{
 ASSERT(o_values);
 UpdateData(TRUE); //copy data from dialog to variables
 m_params.baud_rate = _GetBRFromComboBoxByIndex(m_uart_speed_cb_index);
 memcpy(o_values, &m_params, sizeof(SECU3IO::MiscelPar));
}

//��� ������� ���������� ������������ ����� ���� ������� ������ � ������
void CMiscPageDlg::SetValues(const SECU3IO::MiscelPar* i_values)
{
 ASSERT(i_values);
 memcpy(&m_params, i_values, sizeof(SECU3IO::MiscelPar));
 m_uart_speed_cb_index = _GetIndexFromComboBoxByBR(m_params.baud_rate);
 UpdateData(FALSE); //copy data from variables to dialog
}

void CMiscPageDlg::FillUARTSpeedComboBox(const BRCType& i_baudrates)
{
 m_baudrates.clear();
 m_baudrates = i_baudrates; //make a copy

 m_uart_speed_combo.ResetContent();
 for(size_t i = 0; i < m_baudrates.size(); i++)
 {
  CString string;
  string.Format(_T("%d"), m_baudrates[i]);
  int index = m_uart_speed_combo.AddString(string);
  m_uart_speed_combo.SetItemData(index, static_cast<DWORD>(i));
 }
}

int CMiscPageDlg::_GetBRFromComboBoxByIndex(int i_cb_index)
{
 DWORD item_data = m_uart_speed_combo.GetItemData(i_cb_index);
 ASSERT(item_data < m_baudrates.size()); //invalid index?
 return m_baudrates[static_cast<size_t>(item_data)];
}

int CMiscPageDlg::_GetIndexFromComboBoxByBR(int i_baudrate)
{
 int count = m_uart_speed_combo.GetCount();
 for (int i = 0; i < count; i++)
 {
  DWORD item_data = m_uart_speed_combo.GetItemData(i);
  int baud = m_baudrates[static_cast<size_t>(item_data)];
  if (i_baudrate == baud)
   return i;
 }
 ASSERT(0); //WOW! Unknown baud rate?
 return 0;
}
