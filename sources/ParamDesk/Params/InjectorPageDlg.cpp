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
#include "InjectorPageDlg.h"
#include "ui-core/ToolTipCtrlEx.h"

const UINT CInjectorPageDlg::IDD = IDD_PD_INJECTOR_PAGE;

BEGIN_MESSAGE_MAP(CInjectorPageDlg, Super)
 ON_CBN_SELCHANGE(IDC_PD_INJECTOR_INJCONFIG_COMBO, OnChangeData)
 ON_CBN_SELCHANGE(IDC_PD_INJECTOR_SQUIRTNUM_COMBO, OnChangeData)
 ON_EN_CHANGE(IDC_PD_INJECTOR_CYLDISP_EDIT, OnChangeData)
 ON_EN_CHANGE(IDC_PD_INJECTOR_FLOWRATE_EDIT, OnChangeData)

 ON_UPDATE_COMMAND_UI(IDC_PD_INJECTOR_CYLDISP_EDIT,OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_INJECTOR_CYLDISP_SPIN,OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_INJECTOR_CYLDISP_CAPTION,OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_INJECTOR_CYLDISP_UNIT,OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_INJECTOR_FLOWRATE_EDIT,OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_INJECTOR_FLOWRATE_SPIN,OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_INJECTOR_FLOWRATE_CAPTION,OnUpdateControls)
 ON_UPDATE_COMMAND_UI(IDC_PD_INJECTOR_FLOWRATE_UNIT,OnUpdateControls)
END_MESSAGE_MAP()

CInjectorPageDlg::CInjectorPageDlg(CWnd* pParent /*=NULL*/)
: Super(CInjectorPageDlg::IDD, pParent)
, m_enabled(false)
, m_cyldisp_edit(CEditEx::MODE_FLOAT, true)
, m_flowrate_edit(CEditEx::MODE_FLOAT, true)
, m_fuel_density(0.71f) //petrol density (0.71 g/cc)
{
 m_params.inj_config = SECU3IO::INJCFG_SIMULTANEOUS;
 m_params.inj_squirt_num = 4;
 m_params.inj_flow_rate = 200.0f;
 m_params.inj_cyl_disp = 0.375f;
 m_params.inj_sd_igl_const = 0;
 m_params.cyl_num = 4; 
}

CInjectorPageDlg::~CInjectorPageDlg()
{
 //empty
}

LPCTSTR CInjectorPageDlg::GetDialogID(void) const
{
 return (LPCTSTR)IDD;
}

void CInjectorPageDlg::DoDataExchange(CDataExchange* pDX)
{
 Super::DoDataExchange(pDX);
 DDX_Control(pDX,IDC_PD_INJECTOR_CYLDISP_EDIT, m_cyldisp_edit);
 DDX_Control(pDX,IDC_PD_INJECTOR_CYLDISP_SPIN, m_cyldisp_spin);
 DDX_Control(pDX,IDC_PD_INJECTOR_FLOWRATE_EDIT, m_flowrate_edit);
 DDX_Control(pDX,IDC_PD_INJECTOR_FLOWRATE_SPIN, m_flowrate_spin);
 DDX_Control(pDX,IDC_PD_INJECTOR_INJCONFIG_COMBO, m_injcfg_combo);
 DDX_Control(pDX,IDC_PD_INJECTOR_SQUIRTNUM_COMBO, m_sqrnum_combo);

 m_flowrate_edit.DDX_Value(pDX, IDC_PD_INJECTOR_FLOWRATE_EDIT, m_params.inj_flow_rate);
 float engdisp = m_params.inj_cyl_disp * m_params.cyl_num; //convert cyl.disp. to eng.disp
 m_cyldisp_edit.DDX_Value(pDX, IDC_PD_INJECTOR_CYLDISP_EDIT, engdisp);
 m_params.inj_cyl_disp = engdisp / m_params.cyl_num; //convert eng.disp to cyl.disp
}

void CInjectorPageDlg::OnUpdateControls(CCmdUI* pCmdUI)
{
 pCmdUI->Enable(m_enabled);
}

/////////////////////////////////////////////////////////////////////////////
// CInjectorPageDlg message handlers

BOOL CInjectorPageDlg::OnInitDialog()
{
 Super::OnInitDialog();

 m_cyldisp_spin.SetBuddy(&m_cyldisp_edit);
 m_cyldisp_edit.SetLimitText(6);
 m_cyldisp_edit.SetDecimalPlaces(4);
 m_cyldisp_spin.SetRangeAndDelta(0.05f, 3.0000f, 0.0001);
 m_cyldisp_edit.SetRange(0.05f, 3.0000f);

 m_flowrate_spin.SetBuddy(&m_flowrate_edit);
 m_flowrate_edit.SetLimitText(6);
 m_flowrate_edit.SetDecimalPlaces(2);
 m_flowrate_spin.SetRangeAndDelta(50.00f, 1000.00f, 0.02f);
 m_flowrate_edit.SetRange(50.00f, 1000.00f);

 //create a tooltip control and assign tooltips
 mp_ttc.reset(new CToolTipCtrlEx());
 VERIFY(mp_ttc->Create(this, WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON));
 VERIFY(mp_ttc->AddWindow(&m_cyldisp_edit, MLL::GetString(IDS_PD_INJECTOR_CYLDISP_EDIT_TT)));

 mp_ttc->SetMaxTipWidth(100); //Enable text wrapping
 mp_ttc->ActivateToolTips(true);

 _FillInjCfgComboBox();
 _FillSqrNumComboBox();

 UpdateData(FALSE);
 UpdateDialogControls(this, TRUE);
 return TRUE;  // return TRUE unless you set the focus to a control
}

void CInjectorPageDlg::OnChangeData()
{
 UpdateData();
 OnChangeNotify(); //notify event receiver about change in view content(see class ParamPageEvents)
}

//����������/���������� ���������� (���� ���������)
void CInjectorPageDlg::Enable(bool enable)
{
 if (m_enabled == enable)
  return; //already has needed state
 m_enabled = enable;
 if (::IsWindow(m_hWnd))
  UpdateDialogControls(this,TRUE);
}

//��� � �����������?
bool CInjectorPageDlg::IsEnabled(void)
{
 return m_enabled;
}

//��� ������� ���������� ������������ ����� ���� �������� ������ �� �������
void CInjectorPageDlg::GetValues(SECU3IO::InjctrPar* o_values)
{
 ASSERT(o_values);
 UpdateData(TRUE); //�������� ������ �� ������� � ����������

 m_params.inj_config = _GetInjCfgComboBoxSelection();
 m_params.inj_squirt_num = _GetSqrNumComboBoxSelection();

 //----------------------------------------------------------------------------
 //convert inj.flow rate from cc/min to g/min
 float mifr = m_params.inj_flow_rate * m_fuel_density;

 int inj_num = m_params.cyl_num;
 if (m_params.inj_config == SECU3IO::INJCFG_TROTTLEBODY)
  inj_num = 1; //single injector
 else if (m_params.inj_config == SECU3IO::INJCFG_SIMULTANEOUS ||
          m_params.inj_config == SECU3IO::INJCFG_SEMISEQUENTIAL ||
          m_params.inj_config == SECU3IO::INGCFG_FULLSEQUENTIAL)
  inj_num = m_params.cyl_num; //= N cylinders
 else
  ASSERT(0);

 //calculate constant (calculation of this value related to the ideal gas law, see firmware code for more information)
 m_params.inj_sd_igl_const = ((m_params.inj_cyl_disp * 3.482f * 18750000.0f) / mifr) * (float(m_params.cyl_num) / (float(inj_num) * float(m_params.inj_squirt_num)));
 //----------------------------------------------------------------------------

 if (m_params.inj_sd_igl_const > 131072)
 {
  m_params.inj_sd_igl_const = 131072;
  AfxMessageBox(_T("Overflow detected when calculating constant for the ideal gas law equation! Change configuration to eliminate this error."));
 } 

 memcpy(o_values,&m_params, sizeof(SECU3IO::InjctrPar));
}

//��� ������� ���������� ������������ ����� ���� ������� ������ � ������
void CInjectorPageDlg::SetValues(const SECU3IO::InjctrPar* i_values)
{
 ASSERT(i_values);
 memcpy(&m_params,i_values, sizeof(SECU3IO::InjctrPar));

 _SetInjCfgComboBoxSelection(m_params.inj_config);
 _SetSqrNumComboBoxSelection(m_params.inj_squirt_num);

 UpdateData(FALSE); //�������� ������ �� ���������� � ������
}

void CInjectorPageDlg::_FillInjCfgComboBox(void)
{
 m_injcfgs.clear();
 m_injcfgs.push_back(std::make_pair(SECU3IO::INJCFG_TROTTLEBODY, MLL::GetString(IDS_INJ_CFG_THROTTLEBODY))); 
 m_injcfgs.push_back(std::make_pair(SECU3IO::INJCFG_SIMULTANEOUS, MLL::GetString(IDS_INJ_CFG_SIMULTANEOUSLY))); 
 m_injcfgs.push_back(std::make_pair(SECU3IO::INJCFG_SEMISEQUENTIAL, MLL::GetString(IDS_INJ_CFG_SEMISEQUENTIAL))); 
 m_injcfgs.push_back(std::make_pair(SECU3IO::INGCFG_FULLSEQUENTIAL, MLL::GetString(IDS_INJ_CFG_FULLSEQUENTIAL))); 

 m_injcfg_combo.ResetContent();
 for(size_t i = 0; i < m_injcfgs.size(); i++)
 {
  int index = m_injcfg_combo.AddString(m_injcfgs[i].second.c_str());
  if (index==CB_ERR)
  {
   ASSERT(0);
   continue;
  }
  m_injcfg_combo.SetItemData(index, i);
 }
}

int  CInjectorPageDlg::_GetInjCfgComboBoxSelection(void)
{
 int index = m_injcfg_combo.GetCurSel();
 if (index==CB_ERR)
 {
  ASSERT(0);
  return 0;
 }
 int cfg_index = m_injcfg_combo.GetItemData(index);
 return m_injcfgs[cfg_index].first;
}

void CInjectorPageDlg::_SetInjCfgComboBoxSelection(int i_sel)
{
 for(size_t i = 0; i < m_injcfgs.size(); i++)
 {
  if (m_injcfgs[i].first != i_sel) //find index in conntainer for configuration
   continue;
  //find related index and select corresponding item
  int count = m_injcfg_combo.GetCount();
  for(int ii = 0; ii < count; ii++)
  if (m_injcfg_combo.GetItemData(ii) == i)
  {
   m_injcfg_combo.SetCurSel(ii);
   return;
  }
 }
 ASSERT(0);
}

void CInjectorPageDlg::_FillSqrNumComboBox(void)
{
 m_sqrnum.clear();
 m_sqrnum.push_back(std::make_pair(1, _T("1"))); 
 m_sqrnum.push_back(std::make_pair(2, _T("2"))); 
 m_sqrnum.push_back(std::make_pair(4, _T("4"))); 

 m_sqrnum_combo.ResetContent();
 for(size_t i = 0; i < m_sqrnum.size(); i++)
 {
  int index = m_sqrnum_combo.AddString(m_sqrnum[i].second.c_str());
  if (index==CB_ERR)
  {
   ASSERT(0);
   continue;
  }
  m_sqrnum_combo.SetItemData(index, i);
 }

}

int CInjectorPageDlg::_GetSqrNumComboBoxSelection(void)
{
 int index = m_sqrnum_combo.GetCurSel();
 if (index==CB_ERR)
 {
  ASSERT(0);
  return 0;
 }
 int num_index = m_sqrnum_combo.GetItemData(index);
 return m_sqrnum[num_index].first;
}

void CInjectorPageDlg::_SetSqrNumComboBoxSelection(int i_sel)
{
 for(size_t i = 0; i < m_sqrnum.size(); i++)
 {
  if (m_sqrnum[i].first != i_sel) //find index in container
   continue;
  //find related index and select corresponding item
  int count = m_sqrnum_combo.GetCount();
  for(int ii = 0; ii < count; ii++)
  if (m_sqrnum_combo.GetItemData(ii) == i)
  {
   m_sqrnum_combo.SetCurSel(ii);
   return;
  }
 }
 ASSERT(0);
}
