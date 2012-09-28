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
#include "Resources/resource.h"
#include "AppSettingsDlg.h"
#include "ui-core/XBrowseForFolder.h"

const UINT CAppSettingsDlg::IDD = IDD_APP_SETTINGS;

CAppSettingsDlg::CAppSettingsDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAppSettingsDlg::IDD, pParent)
, m_OnOk(NULL)
, m_OnCancel(NULL)
, m_OnActivate(NULL)
, m_midesk_update_period_edit(CEditEx::MODE_INT)
, m_dv_update_period_edit(CEditEx::MODE_INT)
, m_tachometer_max_edit(CEditEx::MODE_INT)
, m_pressure_max_edit(CEditEx::MODE_INT)
{
 m_app_baudrate = -1;
 m_bl_baudrate = -1;
 m_port_number = -1;
 m_log_csv_sepsymbol_index = -1;
 m_log_files_folder = _T("");
 m_use_app_folder = BST_UNCHECKED;
 m_midesk_update_period = 40;
 m_tachometer_max = 0;
 m_pressure_max = 0;
 m_use_dv_features = BST_UNCHECKED;
 m_dv_update_period = 40;
}

void CAppSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
 CDialog::DoDataExchange(pDX);
 DDX_Control(pDX, IDC_APP_SETTINGS_PORT_SELECTION_COMBO, m_port_selection_combo);
 DDX_Control(pDX, IDC_APP_SETTINGS_BL_BAUDRATE_SELECTION_COMBO, m_bl_baudrate_selection_combo);
 DDX_Control(pDX, IDC_APP_SETTINGS_APP_BAUDRATE_SELECTION_COMBO, m_app_baudrate_selection_combo);
 DDX_Control(pDX, IDC_APP_SETTINGS_LOG_CSV_SEPSYMBOL_COMBO, m_log_csv_sepsymbol_combo);
 DDX_Control(pDX, IDC_APP_SETTINGS_PLATFORM_SEL_COMBO, m_ecu_platform_selection_combo);
 DDX_Control(pDX, IDC_APP_SETTINGS_LANG_SEL_COMBO, m_iface_lang_selection_combo);

 DDX_Control(pDX, IDC_APP_SETTINGS_LOGFOLDER_EDITBOX, m_log_files_folder_edit);
 DDX_Control(pDX, IDC_APP_SETTINGS_LOGFOLDER_BUTTON, m_log_files_folder_button);
 DDX_Control(pDX, IDC_APP_SETTINGS_LOGFOLDER_USEAPPFOLDER, m_use_app_folder_button);
 DDX_Control(pDX, IDC_APP_SETTINGS_USEDEBUG_FEATURES, m_use_dv_features_button);

 DDX_CBIndex(pDX, IDC_APP_SETTINGS_APP_BAUDRATE_SELECTION_COMBO, m_app_baudrate);
 DDX_CBIndex(pDX, IDC_APP_SETTINGS_BL_BAUDRATE_SELECTION_COMBO, m_bl_baudrate);
 DDX_CBIndex(pDX, IDC_APP_SETTINGS_PORT_SELECTION_COMBO, m_port_number);
 DDX_CBIndex(pDX, IDC_APP_SETTINGS_LOG_CSV_SEPSYMBOL_COMBO, m_log_csv_sepsymbol_index);

 DDX_CBIndex(pDX, IDC_APP_SETTINGS_PLATFORM_SEL_COMBO, m_ecu_platform_selection);
 DDX_CBIndex(pDX, IDC_APP_SETTINGS_LANG_SEL_COMBO, m_iface_lang_selection);

 DDX_Text(pDX, IDC_APP_SETTINGS_LOGFOLDER_EDITBOX, m_log_files_folder);
 DDX_Check(pDX, IDC_APP_SETTINGS_LOGFOLDER_USEAPPFOLDER, m_use_app_folder);
 DDX_Check(pDX, IDC_APP_SETTINGS_USEDEBUG_FEATURES, m_use_dv_features);

 DDX_Control(pDX, IDC_APP_SETTINGS_MIDESK_UPDATE_PERIOD_SPIN, m_midesk_update_period_spin);
 DDX_Control(pDX, IDC_APP_SETTINGS_MIDESK_UPDATE_PERIOD_EDIT, m_midesk_update_period_edit);
 m_midesk_update_period_edit.DDX_Value(pDX, IDC_APP_SETTINGS_MIDESK_UPDATE_PERIOD_EDIT, m_midesk_update_period);

 DDX_Control(pDX, IDC_APP_SETTINGS_DBGPANEL_UPDATE_PERIOD_SPIN, m_dv_update_period_spin);
 DDX_Control(pDX, IDC_APP_SETTINGS_DBGPANEL_UPDATE_PERIOD_EDIT, m_dv_update_period_edit);
 m_dv_update_period_edit.DDX_Value(pDX, IDC_APP_SETTINGS_DBGPANEL_UPDATE_PERIOD_EDIT, m_dv_update_period);

 DDX_Control(pDX, IDC_APP_SETTINGS_TACHOMETER_MAX_SPIN, m_tachometer_max_spin);
 DDX_Control(pDX, IDC_APP_SETTINGS_TACHOMETER_MAX_EDIT, m_tachometer_max_edit);
 m_tachometer_max_edit.DDX_Value(pDX, IDC_APP_SETTINGS_TACHOMETER_MAX_EDIT, m_tachometer_max);

 DDX_Control(pDX, IDC_APP_SETTINGS_PRESSURE_MAX_SPIN, m_pressure_max_spin);
 DDX_Control(pDX, IDC_APP_SETTINGS_PRESSURE_MAX_EDIT, m_pressure_max_edit);
 m_pressure_max_edit.DDX_Value(pDX, IDC_APP_SETTINGS_PRESSURE_MAX_EDIT, m_pressure_max);

 DDX_Control(pDX, IDC_APP_SETTINGS_DBGPANEL_UPDATE_PERIOD_CAPTION, m_dv_update_period_caption);
 DDX_Control(pDX, IDC_APP_SETTINGS_INFO_TEXT, m_info_text);
}


BEGIN_MESSAGE_MAP(CAppSettingsDlg, CDialog)
 ON_BN_CLICKED(IDC_APP_SETTINGS_LOGFOLDER_BUTTON, OnAppSettingsLogfolderButton)
 ON_BN_CLICKED(IDC_APP_SETTINGS_LOGFOLDER_USEAPPFOLDER, OnAppSettingsLogfolderUseappfolder)
 ON_BN_CLICKED(IDC_APP_SETTINGS_USEDEBUG_FEATURES, OnAppSettingsLogfolderUseDVFeatures)
 ON_WM_CTLCOLOR()
 ON_CBN_SELENDOK(IDC_APP_SETTINGS_LANG_SEL_COMBO, OnSelendokRestartPerameters)
 ON_CBN_SELENDOK(IDC_APP_SETTINGS_PLATFORM_SEL_COMBO, OnSelendokRestartPerameters)
END_MESSAGE_MAP()

HBRUSH CAppSettingsDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
 if (pWnd->m_hWnd == m_info_text.m_hWnd && nCtlColor == CTLCOLOR_STATIC)
 {
  pDC->SetTextColor(RGB(255, 0, 0));
  pDC->SetBkMode(TRANSPARENT);
  return (HBRUSH)GetStockObject(NULL_BRUSH);
 }

 return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CAppSettingsDlg::OnSelendokRestartPerameters()
{
 UpdateData();
 bool program_restart_required = m_iface_lang_selection_orig != m_iface_lang_selection || 
                                 m_ecu_platform_selection_orig != m_ecu_platform_selection;
 
 m_info_text.SetWindowText(program_restart_required ? MLL::GetString(IDS_APP_SETTINGS_PRG_RESTART_REQ).c_str() : _T(""));
 m_info_text.ShowWindow(program_restart_required ? SW_SHOW : SW_HIDE); 
}

BOOL CAppSettingsDlg::OnInitDialog()
{
 CDialog::OnInitDialog();

 m_midesk_update_period_edit.SetLimitText(4);
 m_midesk_update_period_spin.SetBuddy(&m_midesk_update_period_edit);
 m_midesk_update_period_spin.SetRangeAndDelta(10,1000,10);

 m_dv_update_period_edit.SetLimitText(4);
 m_dv_update_period_spin.SetBuddy(&m_dv_update_period_edit);
 m_dv_update_period_spin.SetRangeAndDelta(10,1000,10);

 m_tachometer_max_edit.SetLimitText(5);
 m_tachometer_max_spin.SetBuddy(&m_tachometer_max_edit);
 m_tachometer_max_spin.SetRangeAndDelta(1000, 15000, 10);

 m_pressure_max_edit.SetLimitText(5);
 m_pressure_max_spin.SetBuddy(&m_pressure_max_edit);
 m_pressure_max_spin.SetRangeAndDelta(50, 500, 10);

 if (m_OnActivate)
  m_OnActivate(); //����������� ��������� � ��� ��� �� ������ � ������ ������

 UpdateData(FALSE);

 OnAppSettingsLogfolderUseappfolder();
 OnAppSettingsLogfolderUseDVFeatures();

 return TRUE;  // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}

void CAppSettingsDlg::OnCancel()
{
 UpdateData();
 if (m_OnCancel) m_OnCancel();
  CDialog::OnCancel();
}

void CAppSettingsDlg::OnOK()
{
 UpdateData();
 if (m_OnOk) m_OnOk();
  CDialog::OnOK();
}

//����� ��������
void CAppSettingsDlg::OnAppSettingsLogfolderButton()
{
 UpdateData();
 TCHAR szFolder[MAX_PATH*2] = { _T('\0') };
 LPCTSTR lpszInitial = m_log_files_folder.GetBuffer(MAX_PATH);
 BOOL bEditBox = TRUE;

 BOOL bRet = XBrowseForFolder(m_hWnd,
                              lpszInitial,
                              -1,
                              MLL::LoadString(IDS_SELECT_FOLDER_FOR_LOG_FILES),
                              szFolder,
                              sizeof(szFolder)/sizeof(TCHAR)-2,
                              bEditBox);

 if (bRet)
 {
  m_log_files_folder = szFolder;
  UpdateData(FALSE);
 }
}

void CAppSettingsDlg::OnAppSettingsLogfolderUseappfolder()
{
 UpdateData();
 m_log_files_folder_edit.EnableWindow(m_use_app_folder==BST_UNCHECKED);
 m_log_files_folder_button.EnableWindow(m_use_app_folder==BST_UNCHECKED);
}

void CAppSettingsDlg::OnAppSettingsLogfolderUseDVFeatures()
{
 UpdateData();
 m_dv_update_period_edit.EnableWindow(m_use_dv_features==BST_CHECKED);
 m_dv_update_period_spin.EnableWindow(m_use_dv_features==BST_CHECKED);
 m_dv_update_period_caption.EnableWindow(m_use_dv_features==BST_CHECKED);
}

void CAppSettingsDlg::FillCtrlsWithAllowableBaudRates(std::vector<DWORD> i_AllowableBaudRates)
{
 for(size_t i = 0; i < i_AllowableBaudRates.size(); i++)
 {
  CString str;
  str.Format(_T("%d"),i_AllowableBaudRates[i]);
  m_bl_baudrate_selection_combo.AddString(str);
  m_app_baudrate_selection_combo.AddString(str);
 }
}

void CAppSettingsDlg::FillCtrlsWithAllowablePorts(std::vector<_TSTRING> i_AllowablePorts)
{
 for(size_t i = 0; i < i_AllowablePorts.size(); i++)
 {
  m_port_selection_combo.AddString(i_AllowablePorts[i].c_str());
 }
}

void CAppSettingsDlg::FillCtrlsWithAllowableCSVSepSymbols(std::vector<std::pair<_TSTRING, char> >  i_AllowableCSVSepSymbols)
{
 for(size_t i = 0; i < i_AllowableCSVSepSymbols.size(); i++)
 {
  int index = m_log_csv_sepsymbol_combo.AddString(i_AllowableCSVSepSymbols[i].first.c_str());
  ASSERT(index != LB_ERR);
  m_log_csv_sepsymbol_combo.SetItemData(index, i);
 }
}

void CAppSettingsDlg::FillCtrlsWithAllowableInterfaceLanguages(std::vector<std::pair<std::pair<_TSTRING, _TSTRING>, int> > i_AllowableInterfaceLanguages)
{
 for(size_t i = 0; i < i_AllowableInterfaceLanguages.size(); ++i)
 {
  int index = m_iface_lang_selection_combo.AddString(i_AllowableInterfaceLanguages[i].first.first.c_str());
  ASSERT(index != LB_ERR);
  m_iface_lang_selection_combo.SetItemData(i_AllowableInterfaceLanguages[i].second, i);
 }
}

void CAppSettingsDlg::FillCtrlsWithAllowableECUPlatformTypes(std::vector<std::pair<std::pair<_TSTRING, _TSTRING>, int> > i_AllowableECUPlatformTypes)
{
 for(size_t i = 0; i < i_AllowableECUPlatformTypes.size(); ++i)
 {
  int index = m_ecu_platform_selection_combo.AddString(i_AllowableECUPlatformTypes[i].first.first.c_str());
  ASSERT(index != LB_ERR);
  m_ecu_platform_selection_combo.SetItemData(i_AllowableECUPlatformTypes[i].second, i);
 }
}

//"Set" methods (model => view data transfer)
void CAppSettingsDlg::SetPortName(_TSTRING i_PortName)
{
 int result = m_port_selection_combo.FindString(-1,i_PortName.c_str());
 if (result!=LB_ERR)
  m_port_number = result;
}

void CAppSettingsDlg::SetBaudRateApplication(DWORD i_app_baud)
{
 CString str;
 str.Format(_T("%d"),i_app_baud);
 int result = m_app_baudrate_selection_combo.FindString(-1,str);
 if (result!=LB_ERR)
  m_app_baudrate = result;
}

void CAppSettingsDlg::SetBaudRateBootloader(DWORD i_bl_baud)
{
 CString str;
 str.Format(_T("%d"),i_bl_baud);
 int result = m_bl_baudrate_selection_combo.FindString(-1,str);
 if (result!=LB_ERR)
  m_bl_baudrate = result;
}

void CAppSettingsDlg::SetCSVSepSymbol(size_t i_index)
{
 int count = m_log_csv_sepsymbol_combo.GetCount();
 for (int i = 0; i < count; i++)
 {
  size_t index = m_log_csv_sepsymbol_combo.GetItemData(i);
  if (index == i_index)
  { //found!
   m_log_csv_sepsymbol_index = i;
   return;
  }
 }
 ASSERT(0); //WTF...
}

//"Get" methods (view => model data transfer)
_TSTRING CAppSettingsDlg::GetPortName(void)
{
 CString str;
 m_port_selection_combo.GetLBText(m_port_number,str);
 return _TSTRING(str);
}

DWORD CAppSettingsDlg::GetBaudRateApplication(void)
{
 CString str;
 m_app_baudrate_selection_combo.GetLBText(m_app_baudrate,str);
 return _ttoi(str);
}

DWORD CAppSettingsDlg::GetBaudRateBootloader(void)
{
 CString str;
 m_bl_baudrate_selection_combo.GetLBText(m_bl_baudrate,str);
 return _ttoi(str);
}

void CAppSettingsDlg::SetLogFilesFolder(const CString& i_folder)
{
 m_log_files_folder = i_folder;
}

CString CAppSettingsDlg::GetLogFilesFolder(void) const
{
 return m_log_files_folder;
}

void CAppSettingsDlg::SetUseAppFolder(bool i_use)
{
 m_use_app_folder = i_use ? BST_CHECKED : BST_UNCHECKED;
}

bool CAppSettingsDlg::GetUseAppFolder(void) const
{
 return (m_use_app_folder == BST_CHECKED) ? true : false;
}

size_t CAppSettingsDlg::GetCSVSepSymbol(void)
{
 return m_log_csv_sepsymbol_combo.GetItemData(m_log_csv_sepsymbol_index);
}

void CAppSettingsDlg::setFunctionOnOk(EventHandler OnOk)
{
 m_OnOk = OnOk;
}

void CAppSettingsDlg::setFunctionOnCancel(EventHandler OnCancel)
{
 m_OnCancel = OnCancel;
}

void CAppSettingsDlg::setFunctionOnActivate(EventHandler OnActivate)
{
 m_OnActivate = OnActivate;
}

int CAppSettingsDlg::ShowDialog(void)
{
 return DoModal();
}

void CAppSettingsDlg::SetMIDeskUpdatePeriod(int i_period)
{
 m_midesk_update_period = i_period;
}

int CAppSettingsDlg::GetMIDeskUpdatePeriod(void)
{
 return m_midesk_update_period;
}

void CAppSettingsDlg::SetInterfaceLanguage(int i_iface_lang)
{
 int count = m_iface_lang_selection_combo.GetCount();
 for (int i = 0; i < count; i++)
 {
  size_t id = m_iface_lang_selection_combo.GetItemData(i);
  if (id == i_iface_lang)
  { //found!
   m_iface_lang_selection = i;
   m_iface_lang_selection_orig = i;
   return;
  }
 }
 ASSERT(0); //WTF...
}

void CAppSettingsDlg::SetECUPlatformType(int i_platform_type)
{
 int count = m_ecu_platform_selection_combo.GetCount();
 for (int i = 0; i < count; i++)
 {
  size_t id = m_ecu_platform_selection_combo.GetItemData(i);
  if (id == i_platform_type)
  { //found!
   m_ecu_platform_selection = i;
   m_ecu_platform_selection_orig = i;
   return;
  }
 }
 ASSERT(0); //WTF...
}

int CAppSettingsDlg::GetInterfaceLanguage(void) const
{
 return m_iface_lang_selection_combo.GetItemData(m_iface_lang_selection);
}

int CAppSettingsDlg::GetECUPlatformType(void) const
{
 return m_ecu_platform_selection_combo.GetItemData(m_ecu_platform_selection);
}

void CAppSettingsDlg::SetTachometerMax(int i_max)
{
 m_tachometer_max = i_max;
}

void CAppSettingsDlg::SetPressureMax(int i_max)
{
 m_pressure_max = i_max;
}

int CAppSettingsDlg::GetTachometerMax(void) const
{
 return m_tachometer_max;
}

int CAppSettingsDlg::GetPressureMax(void) const
{
 return m_pressure_max;
}

void CAppSettingsDlg::SetUseDVFeatures(bool i_use)
{
 m_use_dv_features = i_use ? BST_CHECKED : BST_UNCHECKED;
}

void CAppSettingsDlg::SetDVDeskUpdatePeriod(int i_period)
{
 m_dv_update_period = i_period;
}

bool CAppSettingsDlg::GetUseDVFeatures(void)
{
 return (m_use_dv_features == BST_CHECKED) ? true : false;
}

int CAppSettingsDlg::GetDVDeskUpdatePeriod(void)
{
 return m_dv_update_period;
}
