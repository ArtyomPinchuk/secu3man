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
#include "resource.h"
#include "EEPROMImpExpController.h"
#include <algorithm>

#include "common/FastDelegate.h"
#include "io-core/EEPROMDataMediator.h"
#include "io-core/FirmwareMapsDataHolder.h"
#include "io-core/PlatformParamHolder.h"
#include "MapImpExpDlg.h"

using namespace fastdelegate;

EEPROMImportController::EEPROMImportController(FWMapsDataHolder* ip_fwd)
: mp_fwd(ip_fwd)
, m_eedm(NULL)
, mp_view(new CMapImpExpDlg())
, m_eeprom_file_name(_T(""))
{
 ASSERT(mp_fwd);
 ASSERT(mp_view);

 //��������� ����������� ������� �� �������������
 mp_view->setOnOkButton(MakeDelegate(this,&EEPROMImportController::OnOkPressed));
 mp_view->setOnCancelButton(MakeDelegate(this,&EEPROMImportController::OnCancelPressed));
 mp_view->setOnExchangeButton(MakeDelegate(this,&EEPROMImportController::OnExchangePressed));
 mp_view->setOnActivate(MakeDelegate(this,&EEPROMImportController::OnViewActivate));
 mp_view->setIsExchengeButtonAllowed(MakeDelegate(this,&EEPROMImportController::IsExchangeButtonAllowed));
 mp_view->setOnFWDCurrentListNamechanged(MakeDelegate(this,&EEPROMImportController::OnCurrentListNameChanged));
 mp_view->setOnFWDOtherListNamechanged(MakeDelegate(this,&EEPROMImportController::OnOtherListNameChanged));
}

void EEPROMImportController::setFileReader(Delegate1 delegate)
{
 m_FileReader = delegate;
}

int EEPROMImportController::DoImport(void)
{
 if (!m_FileReader)
  return IDCANCEL;

 std::vector<int> sizes = PlatformParamHolder::GetEEPROMSizes();
 std::vector<int>::const_iterator it = std::max_element(sizes.begin(), sizes.end());
 std::vector<BYTE> buffer((*it));
 int selected_size = 0;
 _TSTRING file_name;
 bool result = m_FileReader(&buffer[0], sizes, &selected_size, &file_name);
 if (true==result)
 {
  m_eeprom_file_name = file_name;
  EECUPlatform platform_id;
  if (!PlatformParamHolder::GetPlatformIdByEEPROMMagic(&buffer[0], selected_size, platform_id))
  {
   if (!PlatformParamHolder::GetPlatformIdByEEPROMSize(selected_size, platform_id))
    return IDCANCEL; //error
  }
  PlatformParamHolder params(platform_id);
  m_eedm = new EEPROMDataMediator(params.GetEepromParameters());
  m_eedm->LoadBytes(&buffer[0]);
 }
 else
  return IDCANCEL; //����� ������������ ��� ������

 return mp_view->DoModal();
}

bool EEPROMImportController::IsExchangeButtonAllowed(void)
{
 int current_sel = mp_view->GetFWDCurrentListSelection();
 int other_sel   = mp_view->GetFWDOtherListSelection();
 return (current_sel != -1) && (other_sel != -1);
}

EEPROMImportController::~EEPROMImportController()
{
 delete mp_view; //avoid memory leak
 delete m_eedm;
}

void EEPROMImportController::OnOkPressed(void)
{
 //empty
}

void EEPROMImportController::OnCancelPressed(void)
{
 //empty
}

//������ ������ ����������� ������
void EEPROMImportController::OnExchangePressed(void)
{
 int current_sel = mp_view->GetFWDCurrentListSelection();
 int other_sel   = mp_view->GetFWDOtherListSelection();

 std::vector<_TSTRING> names = m_eedm->GetFunctionsSetNames();

 if (names[other_sel] != _T(""))
  mp_fwd->maps[current_sel].name = names[other_sel];
 else
 { //���� ������ ������, �� ���������� "�����c�������" ���
  TCHAR name[32];
  _stprintf(name,MLL::GetString(IDS_MAP_NO_NAME).c_str(), other_sel+1);
  mp_fwd->maps[current_sel].name = name;
 }

 mp_view->FillFWDCurrentList(mp_fwd->GetListOfNames());
 mp_view->SetFWDCurrentListSelection(current_sel);

 //ignition
 if (mp_view->GetFWDFlag(FLAG_START_MAP))
  m_eedm->GetStartMap(other_sel, mp_fwd->maps[current_sel].f_str);

 if (mp_view->GetFWDFlag(FLAG_IDLE_MAP))
  m_eedm->GetIdleMap(other_sel, mp_fwd->maps[current_sel].f_idl);

 if (mp_view->GetFWDFlag(FLAG_WORK_MAP))
  m_eedm->GetWorkMap(other_sel, mp_fwd->maps[current_sel].f_wrk);

 if (mp_view->GetFWDFlag(FLAG_TEMP_MAP))
  m_eedm->GetTempMap(other_sel, mp_fwd->maps[current_sel].f_tmp);

 //fuel injection
 if (mp_view->GetFWDFlag(FLAG_VE_MAP))
  m_eedm->GetTempMap(other_sel, mp_fwd->maps[current_sel].inj_ve);

 if (mp_view->GetFWDFlag(FLAG_AFR_MAP))
  m_eedm->GetAFRMap(other_sel, mp_fwd->maps[current_sel].inj_afr);

 if (mp_view->GetFWDFlag(FLAG_CRNK_MAP))
  m_eedm->GetCrnkMap(other_sel, mp_fwd->maps[current_sel].inj_cranking);

 if (mp_view->GetFWDFlag(FLAG_WRMP_MAP))
  m_eedm->GetWrmpMap(other_sel, mp_fwd->maps[current_sel].inj_warmup);

 if (mp_view->GetFWDFlag(FLAG_DEAD_MAP))
  m_eedm->GetDeadMap(other_sel, mp_fwd->maps[current_sel].inj_dead_time);

 if (mp_view->GetFWDFlag(FLAG_IDLR_MAP))
  m_eedm->GetIdlrMap(other_sel, mp_fwd->maps[current_sel].inj_iac_run_pos);

 if (mp_view->GetFWDFlag(FLAG_IDLC_MAP))
  m_eedm->GetIdlcMap(other_sel, mp_fwd->maps[current_sel].inj_iac_crank_pos);

 if (mp_view->GetFWDFlag(FLAG_AETPS_MAP))
  m_eedm->GetIdlcMap(other_sel, mp_fwd->maps[current_sel].inj_ae_tps);

 if (mp_view->GetFWDFlag(FLAG_AERPM_MAP))
  m_eedm->GetIdlcMap(other_sel, mp_fwd->maps[current_sel].inj_ae_rpm);

 if (mp_view->GetFWDFlag(FLAG_AFTSTR_MAP))
  m_eedm->GetAftstrMap(other_sel, mp_fwd->maps[current_sel].inj_aftstr);
}

//��������� ���� �������������� - �������� ��� �������������
void EEPROMImportController::OnViewActivate(void)
{
 mp_view->SetFWDCurrentListTitle(MLL::GetString(IDS_SECU3_CURRENT_FW));
 CString title;
 title.Format(MLL::GetString(IDS_SECU3_FW_FILE).c_str(), m_eeprom_file_name.c_str());
 mp_view->SetFWDOtherListTitle(_TSTRING(title));

 mp_view->SetExchangeButtonCaption(_T("<"));
 mp_view->SetWindowText(MLL::LoadString(IDS_IMPORT_SECU3_TABLES_EE));

 mp_view->FillFWDCurrentList(mp_fwd->GetListOfNames());
 std::vector<_TSTRING> strings = m_eedm->GetFunctionsSetNames();
 mp_view->FillFWDOtherList(strings);
  
 //ignition mpa flags
 mp_view->SetFWDFlag(FLAG_START_MAP, true);
 mp_view->SetFWDFlag(FLAG_IDLE_MAP, true);
 mp_view->SetFWDFlag(FLAG_WORK_MAP, true);
 mp_view->SetFWDFlag(FLAG_TEMP_MAP, true);
 //fuel injection map flags
 mp_view->SetFWDFlag(FLAG_VE_MAP, true);
 mp_view->SetFWDFlag(FLAG_AFR_MAP, true);
 mp_view->SetFWDFlag(FLAG_CRNK_MAP, true);
 mp_view->SetFWDFlag(FLAG_WRMP_MAP, true);
 mp_view->SetFWDFlag(FLAG_DEAD_MAP, true);
 mp_view->SetFWDFlag(FLAG_IDLR_MAP, true);
 mp_view->SetFWDFlag(FLAG_IDLC_MAP, true);
 mp_view->SetFWDFlag(FLAG_AETPS_MAP, true);
 mp_view->SetFWDFlag(FLAG_AERPM_MAP, true);
 mp_view->SetFWDFlag(FLAG_AFTSTR_MAP, true);
 //disable separate map flags
 mp_view->SetFWDFlag(FLAG_DWLCNTR_MAP, false);
 mp_view->EnableFWDFlag(FLAG_DWLCNTR_MAP, false);
 mp_view->SetFWDFlag(FLAG_ATTEN_MAP, false);
 mp_view->EnableFWDFlag(FLAG_ATTEN_MAP, false);
 mp_view->SetFWDFlag(FLAG_CTS_MAP, false);
 mp_view->EnableFWDFlag(FLAG_CTS_MAP, false);
 mp_view->SetFWDFlag(FLAG_CHOKE_MAP, false);
 mp_view->EnableFWDFlag(FLAG_CHOKE_MAP, false);
 mp_view->SetFWDFlag(FLAG_ATS_MAP, false);
 mp_view->EnableFWDFlag(FLAG_ATS_MAP, false);
 mp_view->SetFWDFlag(FLAG_ATSAAC_MAP, false);
 mp_view->EnableFWDFlag(FLAG_ATSAAC_MAP, false);
}

void EEPROMImportController::OnCurrentListNameChanged(int item, CString text)
{
 mp_fwd->maps[item].name = text;
}

void EEPROMImportController::OnOtherListNameChanged(int item, CString text)
{
 m_eedm->SetFunctionsSetName(item, text.GetBuffer(1000));
}

//////////////////////////////////////////////////////////////////////////////////////////
