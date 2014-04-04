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
#include <algorithm>
#include <limits>

#include "Resources/resource.h"
#include "FirmwareTabController.h"

#include "Application/CommunicationManager.h"
#include "common/FastDelegate.h"
#include "FirmwareFileUtils.h"
#include "FWImpExp/MPSZImpExpController.h"
#include "FWImpExp/SECUImpExpController.h"
#include "FWImpExp/S3FImpExpController.h"
#include "FWImpExp/EEPROMImpExpController.h"
#include "FWIORemappingController.h"
#include "FWRPMGridEditController.h"
#include "io-core/EEPROMDataMediator.h"
#include "io-core/FirmwareDataMediator.h"
#include "io-core/SECU3IO.h"
#include "io-core/ufcodes.h"
#include "MainFrame/StatusBarManager.h"
#include "ParamDesk/Params/ParamDeskDlg.h"
#include "Settings/ISettingsData.h"
#include "TabControllersCommunicator.h"
#include "TabDialogs/FirmwareTabDlg.h"
#include "TablDesk/MapIds.h"
#include "TablDesk/TablesSetPanel.h"

using namespace fastdelegate;

#undef max   //avoid conflicts with C++

#define EHKEY _T("FirmwareCntr")
#define DELAY_AFTER_BL_START 100

bool AskUserAboutVrefCompensation(void)
{
 return (IDYES == AfxMessageBox(MLL::GetString(IDS_ASK_USER_ABOUT_VREF_COMP).c_str(), MB_YESNO));
}

bool AskUserAboutTabLeaving(void)
{
 return (IDYES==AfxMessageBox(MLL::LoadString(IDS_FW_LEAVE_TAB_WARNING), MB_YESNO|MB_DEFBUTTON2));
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFirmwareTabController::CFirmwareTabController(CFirmwareTabDlg* i_view, CCommunicationManager* i_comm, CStatusBarManager* i_sbar, ISettingsData* ip_settings)
: m_view(i_view)
, m_comm(i_comm)
, m_sbar(i_sbar)
, mp_settings(ip_settings)
, m_current_funset_index(-1)
, m_bl_read_flash_mode(MODE_RD_FLASH_TO_FILE)
, m_lastSel(0)
, m_bl_started_emergency(false)
, mp_iorCntr(new CFWIORemappingController(i_view->mp_IORemappingDlg.get()))
, m_moreSize(0)
, m_clear_sbar_txt_on_conn(false)
, m_read_fw_sig_info_flag(false)
{
 PlatformParamHolder holder(ip_settings->GetECUPlatformType());
 m_fpp = holder.GetFlashParameters();
 m_epp = holder.GetEepromParameters();
 m_fwdm = new CFirmwareDataMediator(holder.GetFlashParameters());
 ASSERT(m_fwdm);
 m_edm = new EEPROMDataMediator(holder.GetEepromParameters());
 ASSERT(m_edm);

 m_bl_data = new BYTE[m_fpp.m_total_size + 1];
 ASSERT(m_bl_data);
 m_code_for_merge_with_overhead = new BYTE[m_fpp.m_total_size + 1];
 ASSERT(m_code_for_merge_with_overhead);

 //������������� �������� (����������� ������� �� �������������)
 m_view->setOnBootLoaderInfo(MakeDelegate(this, &CFirmwareTabController::OnBootLoaderInfo));
 m_view->setOnReadEepromToFile(MakeDelegate(this, &CFirmwareTabController::OnReadEepromToFile));
 m_view->setOnWriteEepromFromFile(MakeDelegate(this, &CFirmwareTabController::OnWriteEepromFromFile));
 m_view->setOnResetEeprom(MakeDelegate(this, &CFirmwareTabController::OnResetEeprom));
 m_view->setOnReadFlashToFile(MakeDelegate(this, &CFirmwareTabController::OnReadFlashToFile));
 m_view->setOnWriteFlashFromFile(MakeDelegate(this, &CFirmwareTabController::OnWriteFlashFromFile));
 m_view->setOnBLStartedEmergency(MakeDelegate(this, &CFirmwareTabController::OnBLStartedEmergency));
 m_view->setOnOpenFlashFromFile(MakeDelegate(this, &CFirmwareTabController::OnOpenFlashFromFile));
 m_view->setOnFWInformationTextChanged(MakeDelegate(this, &CFirmwareTabController::OnFWInformationTextChanged));
 m_view->setOnSaveFlashToFile(MakeDelegate(this, &CFirmwareTabController::OnSaveFlashToFile));
 m_view->setIsFirmwareOpened(MakeDelegate(this, &CFirmwareTabController::IsFirmwareOpened));
 m_view->setOnImportDataFromAnotherFW(MakeDelegate(this, &CFirmwareTabController::OnImportDataFromAnotherFW));
 m_view->setOnImportDataFromSECU3(MakeDelegate(this, &CFirmwareTabController::OnImportDataFromSECU3));
 m_view->setOnReadFlashFromSECU(MakeDelegate(this, &CFirmwareTabController::OnReadFlashFromSECU));
 m_view->setOnWriteFlashToSECU(MakeDelegate(this, &CFirmwareTabController::OnWriteFlashToSECU));
 m_view->setOnImportMapsFromMPSZ(MakeDelegate(this, &CFirmwareTabController::OnImportMapsFromMPSZ));
 m_view->setOnImportMapsFromSECU3(MakeDelegate(this, &CFirmwareTabController::OnImportMapsFromSECU3));
 m_view->setOnImportMapsFromS3F(MakeDelegate(this, &CFirmwareTabController::OnImportMapsFromS3F));
 m_view->setOnImportDefParamsFromEEPROMFile(MakeDelegate(this, &CFirmwareTabController::OnImportDefParamsFromEEPROMFile));
 m_view->setOnImportTablesFromEEPROMFile(MakeDelegate(this, &CFirmwareTabController::OnImportTablesFromEEPROMFile));
 m_view->setOnExportMapsToMPSZ(MakeDelegate(this, &CFirmwareTabController::OnExportMapsToMPSZ));
 m_view->setOnExportMapsToSECU3(MakeDelegate(this, &CFirmwareTabController::OnExportMapsToSECU3));
 m_view->setOnExportMapsToS3F(MakeDelegate(this, &CFirmwareTabController::OnExportMapsToS3F));
 m_view->setOnFirmwareInfo(MakeDelegate(this, &CFirmwareTabController::OnFirmwareInfo));
 m_view->setOnViewFWOptions(MakeDelegate(this, &CFirmwareTabController::OnViewFWOptions));
 m_view->setIsViewFWOptionsAvailable(MakeDelegate(this, &CFirmwareTabController::IsViewFWOptionsAvailable)); 
 m_view->setIsIORemappingAvailable(MakeDelegate(this, &CFirmwareTabController::IsIORemappingAvailable)); 
 m_view->setOnDropFile(MakeDelegate(this, &CFirmwareTabController::OnDropFile));

 m_view->mp_TablesPanel->setOnMapChanged(MakeDelegate(this, &CFirmwareTabController::OnMapChanged));
 m_view->mp_TablesPanel->setOnFunSetSelectionChanged(MakeDelegate(this, &CFirmwareTabController::OnFunSetSelectionChanged));
 m_view->mp_TablesPanel->setOnFunSetNamechanged(MakeDelegate(this, &CFirmwareTabController::OnFunSetNamechanged));
 m_view->mp_TablesPanel->setOnCloseMapWnd(MakeDelegate(this, &CFirmwareTabController::OnCloseMapWnd));
 m_view->mp_TablesPanel->setOnOpenMapWnd(MakeDelegate(this, &CFirmwareTabController::OnOpenMapWnd));
 m_view->mp_TablesPanel->setIsAllowed(MakeDelegate(this, &CFirmwareTabController::IsFirmwareOpened));
 m_view->mp_TablesPanel->setOnCTSXAxisEditChanged(MakeDelegate(this, &CFirmwareTabController::OnCTSXAxisEditChanged));
 m_view->mp_TablesPanel->setOnRPMGridEditButton(MakeDelegate(this, &CFirmwareTabController::OnEditRPMGrid));
 m_view->mp_TablesPanel->EnableAdvanceAngleIndication(false);

 m_view->mp_ParamDeskDlg->SetOnTabActivate(MakeDelegate(this, &CFirmwareTabController::OnParamDeskTabActivate));
 m_view->mp_ParamDeskDlg->SetOnChangeInTab(MakeDelegate(this, &CFirmwareTabController::OnParamDeskChangeInTab));

 //register controller!
 TabControllersCommunicator::GetInstance()->AddReference(this, TCC_FIRMWARE_TAB_CONTROLLER);
}

CFirmwareTabController::~CFirmwareTabController()
{
 delete m_fwdm;
 delete m_edm;
 delete m_bl_data;
 delete m_code_for_merge_with_overhead;
}

//���������� ���������
void CFirmwareTabController::OnSettingsChanged(void)
{
 //�������� ����������� ��� ������� ��������� ���������������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION, true);
}

void CFirmwareTabController::OnActivate(void)
{
 //�������� ����� ��������� ������� �� ������ ����������
 bool result = m_view->mp_ParamDeskDlg->SetCurSel(m_lastSel);

 //////////////////////////////////////////////////////////////////
 //���������� ���������� � ������ ������ ������ �� SECU-3
 m_comm->m_pAppAdapter->AddEventHandler(this,EHKEY);
 m_comm->m_pBldAdapter->SetEventHandler(this);
 m_comm->setOnSettingsChanged(MakeDelegate(this,&CFirmwareTabController::OnSettingsChanged));
 //////////////////////////////////////////////////////////////////

 //�������� ����������� ��� ������� ��������� ���������������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION);

 m_modification_check_timer.SetTimer(this,&CFirmwareTabController::OnModificationCheckTimer,250);

 //Activate children controllers
 mp_iorCntr->OnActivate();

 SetViewFirmwareValues();

 //���������� ��������� ��������� ��� ���������� ����������, ��� ��� OnConnection ���������� ������ ����
 //���������� ��� ����������� ������������� (����� ����������� ����������������� �����������)
 bool online_status = m_comm->m_pControlApp->GetOnlineStatus();
 OnConnection(online_status);
}

void CFirmwareTabController::OnDeactivate(void)
{
 //����������� �� ������ ������
 m_comm->m_pAppAdapter->RemoveEventHandler(EHKEY);
 m_sbar->SetInformationText(_T(""));
 m_modification_check_timer.KillTimer();
 //���������� ����� ��������� ��������� ������� �� ������ ����������
 m_lastSel = m_view->mp_ParamDeskDlg->GetCurSel();

 //Deactivate children controllers
 mp_iorCntr->OnDeactivate();
}

/////////////////////////////////////////////////////////////////////////////////
void CFirmwareTabController::OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet)
{
 if (i_descriptor == FWINFO_DAT)
 { //������� ����� � ����������� ����������� � ��������
  if (m_read_fw_sig_info_flag)
  {
   SECU3IO::FWInfoDat* p_packet = (SECU3IO::FWInfoDat*)ip_packet;
   TCHAR string[256];
   OemToChar(p_packet->info, string);
   m_sbar->SetInformationText(string);

   //display firmware options if present
   _ShowFWOptions(string, p_packet->options);
   m_read_fw_sig_info_flag = false;
  }
 }
 else if (i_descriptor == OP_COMP_NC)
 {
  const SECU3IO::OPCompNc* p_ndata = (SECU3IO::OPCompNc*)ip_packet;
  switch(p_ndata->opcode)
  {
   case SECU3IO::OPCODE_EEPROM_PARAM_SAVE: //��������� ���� ���������
    m_sbar->SetInformationText(MLL::LoadString(IDS_PM_PARAMS_HAS_BEEN_SAVED));
    return;
   case SECU3IO::OPCODE_SAVE_TABLSET:     //������� ���� ���������
    m_sbar->SetInformationText(MLL::LoadString(IDS_PM_TABLSET_HAS_BEEN_SAVED));    
    return;
   case SECU3IO::OPCODE_RESET_EEPROM:     //������� ������� ������ EEPROM
    if (p_ndata->opdata == 0x55)
    {
     m_sbar->SetInformationText(MLL::LoadString(IDS_FW_RESET_EEPROM_STARTED));
     m_clear_sbar_txt_on_conn = true;
    }
    return;
  } 
 }
}

void CFirmwareTabController::OnConnection(const bool i_online)
{
 int state;
 ASSERT(m_sbar);

 if (i_online) //������� � ������
 {
  state = CStatusBarManager::STATE_ONLINE;
  m_view->EnableBLStartedEmergency(false);

  //� ������� �������� ���� ��������� � ����������� ������ ���������
  m_view->EnableBLItems(true);

  //���� �� ������� � ������, �� ������� ������� - ������������ ��� ����������:
  //�� ������ ��� ��������� ������� �������� � � ���� ����� ������� Application
  m_view->SetBLStartedEmergency(false);

  m_view->EnableAppItems(true);
  if (m_clear_sbar_txt_on_conn)
  {
   m_sbar->SetInformationText(_T(""));
   m_clear_sbar_txt_on_conn = false;
  }
 }
 else
 { //������� � �������
  if (!m_comm->m_pBootLoader->GetWorkState()) //��������� ������� ������ ���� �� � �������� ����� ����� �������
   m_view->EnableBLStartedEmergency(true);

  //� �������� ��������� ��������� ���� ��������� � ����������� ������� �� ��������� ��������
  bool enable = m_view->IsBLStartedEmergency();
  m_view->EnableBLItems(enable);

  if (enable)
   state = CStatusBarManager::STATE_BOOTLOADER; //����� ������ ���������� �� ��������� ����� ���������� ��������
  else
   state = CStatusBarManager::STATE_OFFLINE;

  m_view->EnableAppItems(false);
 }

 //���� ��������� ������� (����������� ��������� �� ���� ��������), �� ����� ���������� ������
 //������ ����������
 if (m_comm->m_pBootLoader->GetWorkState())
  state = CStatusBarManager::STATE_BOOTLOADER;

 m_sbar->SetConnectionState(state);
}

/////////////////////////////////////////////////////////////////////////////////
void CFirmwareTabController::OnUpdateUI(IBLDEventHandler::poolUpdateUI* ip_data)
{
 ASSERT(ip_data);
 IBLDEventHandler::poolUpdateUI data;

 /////////////////////////////////////////////////////////////
 //������������ ������
 m_comm->m_pBootLoader->EnterCriticalSection();
 data = *ip_data;
 m_comm->m_pBootLoader->LeaveCriticalSection();
 /////////////////////////////////////////////////////////////

 if (data.opcode!=CBootLoader::BL_OP_EXIT) //��� �������� ������ �� ���������� �� ���������� �������� �������� ����
 {
  m_sbar->SetProgressRange(0, data.total);
  m_sbar->SetProgressPos(data.current);
 }
}

void CFirmwareTabController::OnBegin(const int opcode,const int status)
{
 if (opcode == CBootLoader::BL_OP_READ_SIGNATURE)
  m_sbar->SetInformationText(MLL::LoadString(IDS_FW_READING_SIGNATURE));
 if (opcode == CBootLoader::BL_OP_READ_EEPROM)
  m_sbar->SetInformationText(MLL::LoadString(IDS_FW_READING_EEPROM));
 if (opcode == CBootLoader::BL_OP_WRITE_EEPROM)
  m_sbar->SetInformationText(MLL::LoadString(IDS_FW_WRITING_EEPROM));
 if (opcode == CBootLoader::BL_OP_READ_FLASH)
  m_sbar->SetInformationText(MLL::LoadString(IDS_FW_READING_FW));
 if (opcode == CBootLoader::BL_OP_WRITE_FLASH)
  m_sbar->SetInformationText(MLL::LoadString(IDS_FW_WRITING_FW));
 if (opcode == CBootLoader::BL_OP_EXIT)
 {
  //Exiting from boot loader...
 }

 m_view->EnableBLItems(false);
 m_view->EnableBLStartedEmergency(false);
}

CString CFirmwareTabController::GenerateErrorStr(void)
{
 switch(m_comm->m_pBootLoader->GetLastError())
 {
  case CBootLoader::BL_ERROR_NOANSWER:
   return MLL::LoadString(IDS_BL_ERROR_NOANSWER);
  case CBootLoader::BL_ERROR_CHKSUM:
   return MLL::LoadString(IDS_BL_ERROR_CRC);
  case CBootLoader::BL_ERROR_WRONG_DATA:
   return MLL::LoadString(IDS_BL_ERROR_WRONG_DATA);
 }
 ASSERT(0); //��� �� ������?
 return CString(_T(""));
}

void CFirmwareTabController::OnEnd(const int opcode,const int status)
{
 switch(opcode)
 {
  //////////////////////////////////////////////////////////////////////
  case CBootLoader::BL_OP_EXIT: //�� ������������ ����� ��������� ������� ��������
  {
   //����� ���������� ���������������� ���������� ����������
   m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION);
   break;
  }

  //////////////////////////////////////////////////////////////////////
  case CBootLoader::BL_OP_READ_SIGNATURE:
  {
   if (status==1)
   {
    m_bl_data[CBootLoader::BL_SIGNATURE_STR_LEN] = 0;
    m_sbar->SetInformationText(m_bl_data);
   }
   else
   {
    m_sbar->SetInformationText(GenerateErrorStr());
   }

   //���� ���� �� ���������� ���������� ��������
   while(!m_comm->m_pBootLoader->IsIdle());

   //Achtung! ����� ��������
   ExitBootLoader();

   Sleep(250);
   m_sbar->ShowProgressBar(false);
   break;
  }

  //////////////////////////////////////////////////////////////////////
  case CBootLoader::BL_OP_READ_EEPROM:
  {
   if (status==1)
   { //OK
   	m_sbar->SetInformationText(MLL::LoadString(IDS_FW_EEPROM_READ_SUCCESSFULLY));
    FirmwareFileUtils::SaveEEPROMToFile(m_bl_data, m_epp.m_size);
   }
   else
   {
    m_sbar->SetInformationText(GenerateErrorStr());
   }

   //���� ���� �� ���������� ���������� ��������
   while(!m_comm->m_pBootLoader->IsIdle());

   //Achtung! ����� ��������
   ExitBootLoader();

   Sleep(250);
   m_sbar->ShowProgressBar(false);
   break;
  }

  //////////////////////////////////////////////////////////////////////
  case CBootLoader::BL_OP_WRITE_EEPROM:
  {
   if (status==1)
    m_sbar->SetInformationText(MLL::LoadString(IDS_FW_EEPROM_WRITTEN_SUCCESSFULLY));
   else
   {
    m_sbar->SetInformationText(GenerateErrorStr());
   }

   //���� ���� �� ���������� ���������� ��������
   while(!m_comm->m_pBootLoader->IsIdle());

   //Achtung! ����� ��������
   ExitBootLoader();

   Sleep(250);
   m_sbar->ShowProgressBar(false);
   break;
  }

  //////////////////////////////////////////////////////////////////////
  case CBootLoader::BL_OP_READ_FLASH:
  {
   if (status==1)
   {
    m_sbar->SetInformationText(MLL::LoadString(IDS_FW_FW_READ_SUCCESSFULLY));
    if (m_bl_read_flash_mode == MODE_RD_FLASH_TO_FILE)
    {
     FirmwareFileUtils::SaveFLASHToFile(m_bl_data, m_fpp.m_total_size, m_fwdm);
    }
    else if (m_bl_read_flash_mode == MODE_RD_FLASH_TO_BUFF_FOR_LOAD)
    {
     if (_CheckFirmwareCompatibilityAndAskUser(m_bl_data) && _CheckQuartzCompatibilityAndAskUser(m_bl_data))
      PrepareOnLoadFLASH(m_bl_data, _T(""));
    }
    else if (m_bl_read_flash_mode == MODE_RD_FLASH_FOR_IMPORT_DATA)
    {
     if (_CheckFirmwareCompatibilityAndAskUser(m_bl_data) && _CheckQuartzCompatibilityAndAskUser(m_bl_data))
     {
      m_fwdm->LoadDataBytesFromAnotherFirmware(m_bl_data);      
      PrepareOnLoadFLASH(NULL, m_fwdm->GetFWFileName());
     }
    }
    else if (m_bl_read_flash_mode == MODE_RD_FLASH_TO_BUFF_MERGE_DATA)
    {
     //���� ���� �� ���������� ���������� ��������
     while(!m_comm->m_pBootLoader->IsIdle());

     //����������� ������ ������. ������ ���������� ���������� ����������� ������ � ������� ��� ������,
     //�������� ����������� ����� � ��������� ������� ���������������� FLASH.
     size_t dataSize = m_fpp.m_app_section_size - m_code_for_merge_size;
     BYTE* dataPtr = m_code_for_merge_with_overhead + m_code_for_merge_size;
     //��� ��� �� ������������� ������ ���, � �� �������� ��������� ������, �� �� ������
     //"���������" ��� ������ �� ��������� ������� ����. m_code_for_merge_with_overhead ������ ��� ������ ���������
     //� ����� �����.
     size_t srcSize = (m_fpp.m_app_section_size - m_code_for_merge_size) + m_moreSize;
     m_fwdm->LoadCodeData(m_bl_data, srcSize, m_code_for_merge_with_overhead);
     //��������� ������ ������
     memset(dataPtr, 0, dataSize);
     memcpy(dataPtr, m_bl_data + m_moreSize, dataSize);
     //����������� � ���������� ����������� ����� ���� ��������
     m_fwdm->CalculateAndPlaceFirmwareCRC(m_code_for_merge_with_overhead);

     Sleep(250);
     m_sbar->SetProgressPos(0);
     m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_WRITE_FLASH, m_code_for_merge_with_overhead, m_fpp.m_app_section_size);

     //�� ������� �� ���������� � �� ������������ ���������������� ����������, ��� ��� ������
     //����������� ���������� ��������.
     return;
    }
    else
    {
     ASSERT(0); //what is it?
    }
   }
   else
   {
    m_sbar->SetInformationText(GenerateErrorStr());
   }

   //���� ���� �� ���������� ���������� ��������
   while(!m_comm->m_pBootLoader->IsIdle());

   //Achtung! ����� ��������
   ExitBootLoader();

   Sleep(250);
   m_sbar->ShowProgressBar(false);
   break;
  }

  //////////////////////////////////////////////////////////////////////
  case CBootLoader::BL_OP_WRITE_FLASH:
  {
   if (status==1)
    m_sbar->SetInformationText(MLL::LoadString(IDS_FW_FW_WRITTEN_SUCCESSFULLY));
   else
   {
    m_sbar->SetInformationText(GenerateErrorStr());
   }

   //���� ���� �� ���������� ���������� ��������
   while(!m_comm->m_pBootLoader->IsIdle());

   //Achtung! ����� ��������
   ExitBootLoader();

   Sleep(250);
   m_sbar->ShowProgressBar(false);
   break;
  }
  //////////////////////////////////////////////////////////////////////
 }//switch

 //���� ��������� ��� ������� ��������, �� ���������� ����������������
 //���������� ����������.  � �����???  TODO.
 if (m_bl_started_emergency)
 {
  m_view->EnableBLItems(true);
  m_view->EnableBLStartedEmergency(true);

  m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_APPLICATION);
 }
}

/////////////////////////////////////////////////////////////////////////////////
//��������� ���������� � ����������
void CFirmwareTabController::OnBootLoaderInfo(void)
{
 if (!m_comm->m_pBootLoader->IsIdle())
  return;

 //��������� ��������� (���� �����)
 StartBootLoader();

 //���������� ���������������� ���������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_BOOTLOADER);

 //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
 m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_READ_SIGNATURE,m_bl_data,0);

 m_sbar->ShowProgressBar(true);
 m_sbar->SetProgressPos(0);
}

void CFirmwareTabController::OnReadEepromToFile(void)
{
 if (!m_comm->m_pBootLoader->IsIdle())
  return;

 //��������� ��������� �� ������� �� ����������
 StartBootLoader();

 //���������� ���������������� ���������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_BOOTLOADER);

 //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
 m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_READ_EEPROM,m_bl_data,0);

 m_sbar->ShowProgressBar(true);
 m_sbar->SetProgressPos(0);
}

void CFirmwareTabController::OnWriteEepromFromFile(void)
{
 std::vector<int> sizes;
 sizes.push_back(m_epp.m_size);
 bool result = FirmwareFileUtils::LoadEEPROMFromFile(m_bl_data, sizes);

 if (!result)
  return; //cancel

 ASSERT(m_comm);

 //��������� ��������� �� ������� �� ����������
 StartBootLoader();

 //���������� ���������������� ���������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_BOOTLOADER);

 //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
 m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_WRITE_EEPROM,m_bl_data,0);

 m_sbar->ShowProgressBar(true);
 m_sbar->SetProgressPos(0);
}

void CFirmwareTabController::OnResetEeprom(void)
{
 if (IDYES==AfxMessageBox(MLL::GetString(IDS_RESET_EEPROM_COMFIRMATION).c_str(), MB_YESNO|MB_DEFBUTTON2|MB_ICONEXCLAMATION))
 {
  SECU3IO::OPCompNc packet_data;
  packet_data.opcode = SECU3IO::OPCODE_RESET_EEPROM;
  packet_data.opdata = 0xAA;
  m_comm->m_pControlApp->SendPacket(OP_COMP_NC, &packet_data);
 }
}

void CFirmwareTabController::OnReadFlashToFile(void)
{
 m_bl_read_flash_mode = MODE_RD_FLASH_TO_FILE;
 _OnReadFlashToFile();
}

void CFirmwareTabController::_OnReadFlashToFile(void)
{
 //��������� ��������� �� ������� �� ����������
 StartBootLoader();

 //���������� ���������������� ���������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_BOOTLOADER);

 //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
 m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_READ_FLASH,m_bl_data, m_fpp.m_total_size);

 m_sbar->ShowProgressBar(true);
 m_sbar->SetProgressPos(0);
}

bool CFirmwareTabController::_CheckFirmwareCompatibilityAndAskUser(BYTE* i_buff, const PlatformParamHolder* p_pph /*=NULL*/)
{
 if (!i_buff)
  return false;

 if (!m_fwdm->CheckCompatibility(i_buff, p_pph ? &p_pph->GetFlashParameters() : NULL))
 {
  if (IDNO==AfxMessageBox(MLL::LoadString(IDS_INCOMPATIBLE_FIRMWARE), MB_YESNO | MB_ICONEXCLAMATION))
   return false; //aborted by user
 }

 return true; //compatible or/and user argee
}

bool CFirmwareTabController::_CheckQuartzCompatibilityAndAskUser(BYTE* ip_buff, size_t fwSize /*=0*/)
{
 if (0==fwSize)
  fwSize = m_fpp.m_total_size;

 EECUPlatform platform_id;
 if (PlatformParamHolder::GetPlatformIdByFirmwareMagic(ip_buff, fwSize, platform_id))
 {
  if (platform_id != m_fpp.m_platform_id && platform_id == EP_ATMEGA644)
  {
   if (IDNO==AfxMessageBox(MLL::LoadString(IDS_INCOMPATIBLE_QUARTZ), MB_YESNO | MB_ICONEXCLAMATION))
    return false; //aborted by user
  }
 }

 return true; //compatible or compatibility can't be checked
}

void CFirmwareTabController::OnWriteFlashFromFile(void)
{
 std::vector<int> sizes;
 sizes.push_back(m_fpp.m_total_size);
 bool result = FirmwareFileUtils::LoadFLASHFromFile(m_bl_data, sizes);

 if (!result || !_CheckQuartzCompatibilityAndAskUser(m_bl_data))
  return; //cancel

 StartWritingOfFLASHFromBuff(m_bl_data);
}

void CFirmwareTabController::StartWritingOfFLASHFromBuff(BYTE* io_buff)
{
 //��������� ����������� ����� � ��������� �� � ������ � ���������. ��� ���������� �������� �����
 //�� ���������� ����� ���������������� ��������, ������� ����� �� ��������� ����������� �����
 m_fwdm->CalculateAndPlaceFirmwareCRC(io_buff);

 ASSERT(m_comm);

 //��������� ��������� �� ������� �� ���������� (���� ��� ����� ��� �� ������� �������)
 StartBootLoader();

 //���������� ���������������� ���������� ����������
 m_comm->SwitchOn(CCommunicationManager::OP_ACTIVATE_BOOTLOADER);

 //���� ���������� ����� �������� ������ ���� (��� ������), �� ��� ��������� �������
 if (m_view->IsProgrammeOnlyCode())
 {
  //�� ������������� ������ ���, ����� ����������� ����� ��������� ���������� ��� ������ ������. ������� ��� ����������
  //��������� ������, ��������� �� � ����� �����, �������� ����������� ����� � ������ ����� ���������������.
  m_bl_read_flash_mode = MODE_RD_FLASH_TO_BUFF_MERGE_DATA;

  //��������� ������ ��� ����, ����� ����� ���������� �� � ������������ "��������" �������
  m_code_for_merge_size = m_fwdm->GetOnlyCodeSize(io_buff);
  memcpy(m_code_for_merge_with_overhead, io_buff, m_fpp.m_app_section_size);

  m_moreSize = (m_fwdm->HasCodeData(io_buff) ? 0x400 : 0); //1024 bytes more

  //������ ������� ������ ����, ��� ����, ����� ������������� ��������� ������ ����������� � ����  
  size_t reducedSize = m_code_for_merge_size - m_moreSize;
  //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
  m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_READ_FLASH, m_bl_data,
  m_fpp.m_app_section_size - reducedSize, //������ ������ ������ ��� ����� ���������
  reducedSize);                           //����� ������ "�������" ������
 }
 else
 {//��� ����� ������
  m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_WRITE_FLASH,io_buff, m_fpp.m_app_section_size);
 }

 m_sbar->ShowProgressBar(true);
 m_sbar->SetProgressPos(0);
}

//�� ��������...
void CFirmwareTabController::OnBLStartedEmergency(void)
{
 if (m_comm->m_pControlApp->GetOnlineStatus()==true)
  return;

 //���� �������, �� ��������� ��������� ���� ������� �� ��������� ��������
 bool emergency = m_view->IsBLStartedEmergency();
 m_view->EnableBLItems(emergency);

 //���� �� ��� �������, �� ��� ������������ �������� � ������� ����� ������ ������ ����������
 if (emergency)
  m_sbar->SetConnectionState(CStatusBarManager::STATE_BOOTLOADER);
 else
  m_sbar->SetConnectionState(CStatusBarManager::STATE_OFFLINE);
}

bool CFirmwareTabController::IsBLStartedEmergency(void)
{
 //���� ������� �������� �� ������ ���������� ������������� ���������, � �������� �������
 //���� �� � ������� ��� ������� ���������
 return m_view->IsBLStartedEmergency() && m_view->IsBLStartedEmergencyEnabled();
}

bool CFirmwareTabController::StartBootLoader(void)
{
 ASSERT(m_comm);

 //�� ���������� �������� ���� ������������ ������ ���� ����������
 m_bl_started_emergency = IsBLStartedEmergency();

 //��������� ��������� �� ������� �� ���������� (���� ��� ����� ��� �� ������� �������)
 if (!m_bl_started_emergency)
 {
  bool result = m_comm->m_pControlApp->StartBootLoader();
  Sleep(DELAY_AFTER_BL_START); //����������� ����� ��������� �� ����� 50 ��, ����� ����� �������� ����������� ������� ��� ������ ������ �� ����������
  return result;
 }

 return true; //��������� ��������� �� ���� ���� - �� ������� ��������
}

bool CFirmwareTabController::ExitBootLoader(void)
{
 ASSERT(m_comm);

 if (!m_bl_started_emergency)
 {
  bool result = m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_EXIT,NULL,0);
  return result;
 }

 //�������� �� ���������� �� ���� ���� - �� ������� ��������.
 //�� �������: ���� ��������� ������� �������� � ����������� �������� ������ �������� (���������� �������� :-)),
 //�� �� ���� ����� �����... �� ��� �� ������� ������������. � ������ �������� � ���� - ���������� ���.
 return true;
}

bool CFirmwareTabController::CheckChangesAskAndSaveFirmware(void)
{
 bool modified = m_fwdm->IsModified();
 if (modified)
 {
  int result = AfxMessageBox(MLL::LoadString(IDS_FW_MODIFICATION_WARNING), MB_YESNOCANCEL);
  if (result==IDCANCEL)
  {
   return false; //������������ ������� ��������
  }
  if (result==IDNO)
  {
   return true; //������������ ����� ���������� ��� ���������
  }
  if (result==IDYES)
  { //���������� ��������� ������! (� ����������)
   OnSaveFlashToFile();
   return true;
  }
 }
 //������ �� ���� �������� - ����������� ��� ��������
 return true;
}

//��� ������� ���������� ��� ������ �� ����������. ��� ������� ����� ��������� �����, ���� ������ false
bool CFirmwareTabController::OnClose(void)
{
 //��������� ������� �������� ����!
 OnCloseMapWnd(m_view->mp_TablesPanel->GetMapWindow(TYPE_MAP_DA_START), TYPE_MAP_DA_START);
 OnCloseMapWnd(m_view->mp_TablesPanel->GetMapWindow(TYPE_MAP_DA_IDLE),  TYPE_MAP_DA_IDLE);
 OnCloseMapWnd(m_view->mp_TablesPanel->GetMapWindow(TYPE_MAP_DA_WORK),  TYPE_MAP_DA_WORK);
 OnCloseMapWnd(m_view->mp_TablesPanel->GetMapWindow(TYPE_MAP_DA_TEMP_CORR), TYPE_MAP_DA_TEMP_CORR);
 OnCloseMapWnd(m_view->mp_TablesPanel->GetMapWindow(TYPE_MAP_ATTENUATOR), TYPE_MAP_ATTENUATOR);
 OnCloseMapWnd(m_view->mp_TablesPanel->GetMapWindow(TYPE_MAP_DWELLCNTRL), TYPE_MAP_DWELLCNTRL);
 OnCloseMapWnd(m_view->mp_TablesPanel->GetMapWindow(TYPE_MAP_CTS_CURVE), TYPE_MAP_CTS_CURVE);
 OnCloseMapWnd(m_view->mp_TablesPanel->GetMapWindow(TYPE_MAP_CHOKE_OP), TYPE_MAP_CHOKE_OP);
 OnCloseMapWnd(m_view->mp_TablesPanel->GetMapWindow(TYPE_MAP_GME_WND), TYPE_MAP_GME_WND);

 if (!m_comm->m_pBootLoader->IsIdle())
  if (!AskUserAboutTabLeaving())
   return false;

 return CheckChangesAskAndSaveFirmware();
}

bool CFirmwareTabController::OnAskFullScreen(void)
{
 return false;
}

void CFirmwareTabController::OnFullScreen(bool i_what, const CRect& i_rect)
{
 //empty
}

bool CFirmwareTabController::OnAskChangeTab(void)
{
 if (m_comm->m_pBootLoader->IsIdle())
  return true; //allows
 return AskUserAboutTabLeaving();
}

void CFirmwareTabController::PrepareOnLoadFLASH(const BYTE* i_buff, const _TSTRING& i_file_name)
{
 if (i_buff) //Do we need to load?
  m_fwdm->LoadBytes(i_buff);
 if (i_file_name!=_T(""))
  m_fwdm->SetFWFileName(_TSTRING(i_file_name));
 else
 {
  CString string;
  SYSTEMTIME time;
  GetLocalTime(&time);
  string.Format(_T("FW%02u%02u%02u-%02u%02u%04u.bin"),
    time.wHour,time.wMinute,time.wSecond,time.wDay,time.wMonth,time.wYear);
  m_fwdm->SetFWFileName(_TSTRING(string));
 }

 //��������� ��� ��������� ������������ ������� � ����������� �� ����� ��������
 m_view->mp_TablesPanel->EnableDwellControl((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_DWELL_CONTROL)) > 0);
 m_view->mp_TablesPanel->EnableCTSCurve((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_THERMISTOR_CS)) > 0);
 m_view->mp_TablesPanel->EnableChokeOp((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_SM_CONTROL)) > 0);
 m_view->mp_ParamDeskDlg->EnableIgnitionCogs(!(m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_DWELL_CONTROL)) && !(m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_CKPS_2CHIGN)));
 m_view->mp_ParamDeskDlg->EnableUseVentPwm((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_COOLINGFAN_PWM)) > 0);
 m_view->mp_ParamDeskDlg->EnableUseCTSCurveMap((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_THERMISTOR_CS)) > 0);
 m_view->mp_ParamDeskDlg->EnableHallOutputParams(((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_HALL_OUTPUT)) > 0) && ((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_HALL_SYNC)) == 0));
 m_view->mp_ParamDeskDlg->EnableSECU3TItems((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_SECU3T)));
 if ((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_SECU3T)))
  //SECU-3T has two additional ignition outputs
  m_view->mp_ParamDeskDlg->SetMaxCylinders((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_PHASED_IGNITION)) > 0 ? 8 : 8);
 else
  m_view->mp_ParamDeskDlg->SetMaxCylinders((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_PHASED_IGNITION)) > 0 ? 4 : 8);

 //in full-sequential ignition mode odd cylinder number engines are also supported,
 //also if hall sensor synchronization is used
 m_view->mp_ParamDeskDlg->EnableOddCylinders((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_PHASED_IGNITION)) > 0 || (m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_HALL_SYNC)) > 0);

 this->mp_iorCntr->EnableSECU3TFeatures((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_SECU3T)) > 0);
 this->mp_iorCntr->Enable(m_fwdm->HasCodeData());

 m_view->mp_ParamDeskDlg->EnableCKPSItems((m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_HALL_SYNC)) == 0);
 m_view->mp_ParamDeskDlg->EnableInputsMerging(!(m_fwdm->GetFWOptions() & (1 << SECU3IO::COPT_CKPS_2CHIGN)));

 SetViewFirmwareValues();
}

void CFirmwareTabController::OnOpenFlashFromFile(void)
{
 bool result;
 std::vector<BYTE> buff_container(m_fpp.m_total_size, 0);
 BYTE *buff = &buff_container[0];
 _TSTRING opened_file_name = _T("");
 
 if (!m_comm->m_pBootLoader->IsIdle())
  if (!AskUserAboutTabLeaving())
   return;
  
 bool is_continue = CheckChangesAskAndSaveFirmware();
 if (!is_continue)
  return;  //������������ ���������

 //!!! ��� ���������� � ������ ����������� ����� � �����
 std::vector<int> sizes;
 sizes.push_back(m_fpp.m_total_size);
 result  = FirmwareFileUtils::LoadFLASHFromFile(buff, sizes, NULL, NULL, &opened_file_name);
 if (result && _CheckFirmwareCompatibilityAndAskUser(buff) && _CheckQuartzCompatibilityAndAskUser(buff)) //user OK?
 {
  PrepareOnLoadFLASH(buff, _TSTRING(opened_file_name));
 }
}

void CFirmwareTabController::OnDropFile(_TSTRING fileName)
{
 bool result;
 std::vector<BYTE> buff_container(m_fpp.m_total_size, 0);
 BYTE *buff = &buff_container[0];
 _TSTRING opened_file_name = _T("");

 bool is_continue = CheckChangesAskAndSaveFirmware();
 if (!is_continue)
  return;  //������������ ���������

 std::vector<int> sizes;
 sizes.push_back(m_fpp.m_total_size);
 result  = FirmwareFileUtils::LoadFLASHFromFile(buff, sizes, NULL, NULL, &opened_file_name, &fileName);
 if (result && _CheckFirmwareCompatibilityAndAskUser(buff) && _CheckQuartzCompatibilityAndAskUser(buff)) //user OK?
 {
  PrepareOnLoadFLASH(buff, _TSTRING(opened_file_name));
 }
}

void CFirmwareTabController::OnSaveFlashToFile(void)
{
 std::vector<BYTE> buff_container(m_fpp.m_total_size, 0);
 BYTE *buff = &buff_container[0];
 CString opened_file_name = _T("");

 m_fwdm->StoreBytes(buff);

 //� ������ ������������� ������������, ����� ������
 //��������� ����������� ����� � ��������� � ������ � ���������
 bool result = FirmwareFileUtils::SaveFLASHToFile(buff, m_fpp.m_total_size, m_fwdm, &opened_file_name,true);
 if (result)
 {
  //����������� ����� ���� ��������� ������ ������� � ��������� ������� �����������,
  //��� ��� ���������� ���� ������������, �� ������ ����� �������� � ������ � �������� ���������
  m_fwdm->CalculateAndPlaceFirmwareCRC();

  //������ ���� ������� ��������� - ����� ���������� ������� �����������
  m_fwdm->ResetModified();

  //����� ���������� "Save As" ��������� ��� ��������� �����
  m_fwdm->SetFWFileName(_TSTRING(opened_file_name));
  m_view->SetFirmwareName(m_fwdm->GetFWFileName());

  //������������� �������� ������ � �������
  SetViewChartsValues();
  m_view->mp_TablesPanel->UpdateOpenedCharts();
  m_view->mp_TablesPanel->UpdateOpenedChartsAxisLabels();

  m_view->SetFirmwareCRCs(m_fwdm->GetCRC16StoredInActiveFirmware(),m_fwdm->CalculateCRC16OfActiveFirmware());
 }
}

void CFirmwareTabController::OnFWInformationTextChanged(void)
{
 CString string = m_view->GetFWInformationText();
 m_fwdm->SetSignatureInfo(_TSTRING(string));
}

bool CFirmwareTabController::IsFirmwareOpened()
{
 return m_fwdm->IsLoaded();
}

//��� ������� �� ��������� �������, ����� ��� �������� UpdateOpenedCharts()!
void CFirmwareTabController::SetViewChartsValues(void)
{
 m_fwdm->GetAttenuatorMap(m_view->mp_TablesPanel->GetAttenuatorMap(false),false);
 m_fwdm->GetAttenuatorMap(m_view->mp_TablesPanel->GetAttenuatorMap(true),true);

 m_fwdm->GetDwellCntrlMap(m_view->mp_TablesPanel->GetDwellCntrlMap(false),false);
 m_fwdm->GetDwellCntrlMap(m_view->mp_TablesPanel->GetDwellCntrlMap(true),true);

 m_fwdm->GetCTSCurveMap(m_view->mp_TablesPanel->GetCTSCurveMap(false),false);
 m_fwdm->GetCTSCurveMap(m_view->mp_TablesPanel->GetCTSCurveMap(true),true);
 m_view->mp_TablesPanel->SetCTSXAxisEdits(m_fwdm->GetCTSMapVoltageLimit(0), m_fwdm->GetCTSMapVoltageLimit(1));

 m_fwdm->GetChokeOpMap(m_view->mp_TablesPanel->GetChokeOpMap(false),false);
 m_fwdm->GetChokeOpMap(m_view->mp_TablesPanel->GetChokeOpMap(true),true);

 m_fwdm->GetRPMGridMap(m_view->mp_TablesPanel->GetRPMGrid());

 if (m_current_funset_index==-1)
  return;
 m_fwdm->GetStartMap(m_current_funset_index,m_view->mp_TablesPanel->GetStartMap(false),false);
 m_fwdm->GetStartMap(m_current_funset_index,m_view->mp_TablesPanel->GetStartMap(true),true);

 m_fwdm->GetIdleMap(m_current_funset_index,m_view->mp_TablesPanel->GetIdleMap(false),false);
 m_fwdm->GetIdleMap(m_current_funset_index,m_view->mp_TablesPanel->GetIdleMap(true),true);

 m_fwdm->GetWorkMap(m_current_funset_index,m_view->mp_TablesPanel->GetWorkMap(false),false);
 m_fwdm->GetWorkMap(m_current_funset_index,m_view->mp_TablesPanel->GetWorkMap(true),true);

 m_fwdm->GetTempMap(m_current_funset_index,m_view->mp_TablesPanel->GetTempMap(false),false);
 m_fwdm->GetTempMap(m_current_funset_index,m_view->mp_TablesPanel->GetTempMap(true),true);
}

void CFirmwareTabController::SetViewFirmwareValues(void)
{
 if (m_fwdm->IsLoaded()==false)
  return;

 CString string = m_fwdm->GetSignatureInfo().c_str();
 m_view->SetFWInformationText(string);

 SetViewChartsValues();

 std::vector<_TSTRING> funset_names = m_fwdm->GetFunctionsSetNames();
 m_view->mp_TablesPanel->SetFunSetListBox(funset_names);

 m_view->mp_TablesPanel->UpdateOpenedCharts();
 m_view->mp_TablesPanel->UpdateOpenedChartsAxisLabels();

 //���� ���� ��������� � ������, �� �������������� ���
 m_view->mp_TablesPanel->SetFunSetListBoxSelection(m_current_funset_index);

 m_view->SetFirmwareName(m_fwdm->GetFWFileName());

 m_view->SetFirmwareCRCs(m_fwdm->GetCRC16StoredInActiveFirmware(),m_fwdm->CalculateCRC16OfActiveFirmware());

 m_view->mp_ParamDeskDlg->SetFunctionsNames(funset_names);
 BYTE descriptor = m_view->mp_ParamDeskDlg->GetCurrentDescriptor();
 BYTE paramdata[256];
 m_fwdm->GetDefParamValues(descriptor,paramdata);
 m_view->mp_ParamDeskDlg->SetValues(descriptor,paramdata);

 //Attach fwdm to children controllers
 if (m_fwdm->HasCodeData())
  this->mp_iorCntr->AttachFWDM(m_fwdm);
}

//������� ����� ���� �������, � ������ ����� ���� �� �������� � �������� ��������� � �������.
//� ���� ������ ������ ������� ����� ���������� � ��� �������� �������.
void CFirmwareTabController::OnMapChanged(int i_type)
{
 switch(i_type)
 {
  case TYPE_MAP_DA_START:
   ASSERT(m_current_funset_index!=-1);
   m_fwdm->SetStartMap(m_current_funset_index,m_view->mp_TablesPanel->GetStartMap(false));
   break;
  case TYPE_MAP_DA_IDLE:
   ASSERT(m_current_funset_index!=-1);
   m_fwdm->SetIdleMap(m_current_funset_index,m_view->mp_TablesPanel->GetIdleMap(false));
   break;
  case TYPE_MAP_DA_WORK:
   ASSERT(m_current_funset_index!=-1);
   m_fwdm->SetWorkMap(m_current_funset_index,m_view->mp_TablesPanel->GetWorkMap(false));
   break;
  case TYPE_MAP_DA_TEMP_CORR:
   ASSERT(m_current_funset_index!=-1);
   m_fwdm->SetTempMap(m_current_funset_index,m_view->mp_TablesPanel->GetTempMap(false));
   break;
  case TYPE_MAP_ATTENUATOR:
   m_fwdm->SetAttenuatorMap(m_view->mp_TablesPanel->GetAttenuatorMap(false));
   break;
  case TYPE_MAP_DWELLCNTRL:
   m_fwdm->SetDwellCntrlMap(m_view->mp_TablesPanel->GetDwellCntrlMap(false));
   break;
  case TYPE_MAP_CTS_CURVE:
   m_fwdm->SetCTSCurveMap(m_view->mp_TablesPanel->GetCTSCurveMap(false));
   break;
  case TYPE_MAP_CHOKE_OP:
   m_fwdm->SetChokeOpMap(m_view->mp_TablesPanel->GetChokeOpMap(false));
   break;
 }
}

//��������� ��������� � ������ �������� ������������� ����������.
// i_selected_index = -1 if there are no selected item
void CFirmwareTabController::OnFunSetSelectionChanged(int i_selected_index)
{
 m_current_funset_index = i_selected_index;

 if (m_current_funset_index != -1)
 { //������ ���� � ������ �������
  SetViewChartsValues();
  m_view->mp_TablesPanel->UpdateOpenedCharts();
 }
}

void CFirmwareTabController::OnFunSetNamechanged(int i_index_of_item, CString i_new_name)
{
 m_fwdm->SetFunctionsSetName(i_index_of_item,_TSTRING(i_new_name));
}

void CFirmwareTabController::OnCTSXAxisEditChanged(int i_type, float i_value)
{
 m_fwdm->SetCTSMapVoltageLimit(i_type, i_value);
}

void CFirmwareTabController::OnModificationCheckTimer(void)
{
 bool modified = m_fwdm->IsModified();
 m_view->SetModified(modified);
 if (m_fwdm->IsLoaded())
  m_view->SetFirmwareCRCs(m_fwdm->GetCRC16StoredInActiveFirmware(),m_fwdm->CalculateCRC16OfActiveFirmware());
}

void CFirmwareTabController::OnReadFlashFromSECU(void)
{
 if (!m_comm->m_pBootLoader->IsIdle())
  return;
 //I don't like "copy/paste" paradigm of programming...
 m_bl_read_flash_mode = MODE_RD_FLASH_TO_BUFF_FOR_LOAD;
 _OnReadFlashToFile();
}

void CFirmwareTabController::OnWriteFlashToSECU(void)
{
 if (!m_comm->m_pBootLoader->IsIdle())
  return;
 m_fwdm->StoreBytes(m_bl_data);
 StartWritingOfFLASHFromBuff(m_bl_data);
}

void CFirmwareTabController::OnImportDataFromAnotherFW()
{
 bool is_continue = CheckChangesAskAndSaveFirmware();
 if (!is_continue)
  return;  //������������ ���������

 bool result;
 std::vector<int> sizes = PlatformParamHolder::GetFirmwareSizes();
 std::vector<int>::const_iterator it = std::max_element(sizes.begin(), sizes.end());
 std::vector<BYTE> buff_container((*it), 0);
 BYTE *buff = &buff_container[0];
 _TSTRING opened_file_name = _T("");
 int selected_size = 0;

 //!!! ��� ���������� � ������ ����������� ����� � �����
 result  = FirmwareFileUtils::LoadFLASHFromFile(buff, sizes, NULL, &selected_size, &opened_file_name);
 //Get platform information
 EECUPlatform platform_id;
 if (!PlatformParamHolder::GetPlatformIdByFirmwareMagic(buff, selected_size, platform_id))
 {
  if (!PlatformParamHolder::GetPlatformIdByFirmwareSize(selected_size, platform_id))
   return; //error
 }
 PlatformParamHolder params(platform_id);
 if (result && _CheckFirmwareCompatibilityAndAskUser(buff, &params) && _CheckQuartzCompatibilityAndAskUser(buff, selected_size)) //user OK?
 {
  m_fwdm->LoadDataBytesFromAnotherFirmware(buff, &params.GetFlashParameters());
  PrepareOnLoadFLASH(NULL, m_fwdm->GetFWFileName());
 }
}

void CFirmwareTabController::OnImportDataFromSECU3(void)
{
 m_bl_read_flash_mode = MODE_RD_FLASH_FOR_IMPORT_DATA;
 _OnReadFlashToFile();
}

void CFirmwareTabController::OnParamDeskTabActivate(void)
{
 BYTE descriptor = m_view->mp_ParamDeskDlg->GetCurrentDescriptor();
 BYTE paramdata[256];
 m_fwdm->GetDefParamValues(descriptor,paramdata);
 m_view->mp_ParamDeskDlg->SetValues(descriptor,paramdata);
}

//from ParamDesk
void CFirmwareTabController::OnParamDeskChangeInTab(void)
{
 BYTE descriptor = m_view->mp_ParamDeskDlg->GetCurrentDescriptor();
 BYTE paramdata[256];
 m_view->mp_ParamDeskDlg->GetValues(descriptor,paramdata);
 m_fwdm->SetDefParamValues(descriptor,paramdata);
}

void CFirmwareTabController::OnImportMapsFromMPSZ(void)
{
 FWMapsDataHolder data;
 MPSZImportController import(&data);
 m_fwdm->GetMapsData(&data);
 int result = import.DoImport();
 if (result == IDOK)
 {
  m_fwdm->SetMapsData(&data);
  SetViewFirmwareValues();
 }
}

void CFirmwareTabController::OnImportMapsFromSECU3(void)
{
 FWMapsDataHolder data;
 SECU3ImportController import(&data);
 import.setFileReader(&FirmwareFileUtils::LoadFLASHFromFile);
 m_fwdm->GetMapsData(&data);
 int result = import.DoImport();
 if (result == IDOK)
 {
  m_fwdm->SetMapsData(&data);
  SetViewFirmwareValues();
 }
}

void CFirmwareTabController::OnImportMapsFromS3F(void)
{
 FWMapsDataHolder data;
 S3FImportController import(&data);
 m_fwdm->GetMapsData(&data);
 int result = import.DoImport();
 if (result == IDOK)
 {
  m_fwdm->SetMapsData(&data);
  SetViewFirmwareValues();
 }
}

void CFirmwareTabController::OnImportDefParamsFromEEPROMFile()
{
 std::vector<BYTE> eeprom_buffer(m_epp.m_size, 0x00);
 BYTE *eeprom = &eeprom_buffer[0];

 std::vector<int> sizes;
 sizes.push_back(m_epp.m_size);
 bool result = FirmwareFileUtils::LoadEEPROMFromFile(eeprom, sizes);

 if (!result)
  return; //cancel

 //�������� ����������� ����� ����������� ���������� � ����� ��������������
 if (!m_edm->VerifyParamsCheckSum(eeprom))
 {
  if (IDCANCEL==AfxMessageBox(IDS_FW_EEPROM_DEF_PARAMS_CRC_INVALID, MB_OKCANCEL))
   return; //user canceled
 }

 m_fwdm->LoadDefParametersFromBuffer(eeprom + m_edm->GetParamsStartAddr(), AskUserAboutVrefCompensation);
 SetViewFirmwareValues(); //Update!
}

void CFirmwareTabController::OnImportTablesFromEEPROMFile()
{
 FWMapsDataHolder data;
 EEPROMImportController import(&data);
 import.setFileReader(&FirmwareFileUtils::LoadEEPROMFromFile);
 m_fwdm->GetMapsData(&data);
 int result = import.DoImport();
 if (result == IDOK)
 {
  m_fwdm->SetMapsData(&data);
  SetViewFirmwareValues();
 }
}

void CFirmwareTabController::OnExportMapsToMPSZ(void)
{
 FWMapsDataHolder data;
 MPSZExportController export_cntr(&data);
 m_fwdm->GetMapsData(&data);
 export_cntr.DoExport();
}

void CFirmwareTabController::OnExportMapsToSECU3(void)
{
 FWMapsDataHolder data;
 SECU3ExportController export_cntr(&data);
 export_cntr.setFileReader(&FirmwareFileUtils::LoadFLASHFromFile);
 m_fwdm->GetMapsData(&data);
 export_cntr.DoExport();
}

void CFirmwareTabController::OnExportMapsToS3F(void)
{
 FWMapsDataHolder data;
 S3FExportController export_cntr(&data);
 m_fwdm->GetMapsData(&data);
 export_cntr.DoExport();
}

void CFirmwareTabController::OnEditRPMGrid(void)
{
 CFWRPMGridEditController cntr;
 cntr.AttachFWDM(m_fwdm);
 if (IDOK == cntr.Edit())
 {
  m_fwdm->GetRPMGridMap(m_view->mp_TablesPanel->GetRPMGrid());
  m_view->mp_TablesPanel->UpdateOpenedChartsAxisLabels();
 }
}

//������������ ������� �������� ���������� � ������� �� SECU-3
void CFirmwareTabController::OnFirmwareInfo(void)
{
 m_sbar->SetInformationText(MLL::LoadString(IDS_FW_READING_FW_SIGNATURE));
 SECU3IO::OPCompNc packet_data;
 packet_data.opcode = SECU3IO::OPCODE_READ_FW_SIG_INFO;
 m_comm->m_pControlApp->SendPacket(OP_COMP_NC,&packet_data);
 m_read_fw_sig_info_flag = true;
}

void CFirmwareTabController::OnViewFWOptions(void)
{
 _ShowFWOptions(m_fwdm->GetSignatureInfo(), m_fwdm->GetFWOptions());
}

bool CFirmwareTabController::IsViewFWOptionsAvailable(void)
{
 return m_fwdm->GetFWOptions() > 0;
}

bool CFirmwareTabController::IsIORemappingAvailable(void)
{
 return m_fwdm->HasCodeData();
}

void CFirmwareTabController::SetAttenuatorMap(const float* i_values)
{
 m_fwdm->SetAttenuatorMap(i_values);
 SetViewChartsValues();
 m_view->mp_TablesPanel->UpdateOpenedCharts();
}

void CFirmwareTabController::GetAttenuatorMap(float* o_values)
{
 ASSERT(o_values);
 m_fwdm->GetAttenuatorMap(o_values, false); //<--NOTE: modified
}

void CFirmwareTabController::OnCloseMapWnd(HWND i_hwnd, int i_mapType)
{
 if (!i_hwnd)
  return;

 RECT rc;
 GetWindowRect(i_hwnd, &rc);

 WndSettings ws;
 mp_settings->GetWndSettings(ws);

 switch(i_mapType)
 {
  case TYPE_MAP_DA_START:
   ws.m_StrtMapWnd_X = rc.left;
   ws.m_StrtMapWnd_Y = rc.top;
   break;
  case TYPE_MAP_DA_IDLE:
   ws.m_IdleMapWnd_X = rc.left;
   ws.m_IdleMapWnd_Y = rc.top;
   break;
  case TYPE_MAP_DA_WORK:
   ws.m_WorkMapWnd_X = rc.left;
   ws.m_WorkMapWnd_Y = rc.top;
   break;
  case TYPE_MAP_DA_TEMP_CORR:
   ws.m_TempMapWnd_X = rc.left;
   ws.m_TempMapWnd_Y = rc.top;
   break;
  case TYPE_MAP_ATTENUATOR:
   ws.m_AttenuatorMapWnd_X = rc.left;
   ws.m_AttenuatorMapWnd_Y = rc.top;
   break;
  case TYPE_MAP_DWELLCNTRL:
   ws.m_DwellCntrlMapWnd_X = rc.left;
   ws.m_DwellCntrlMapWnd_Y = rc.top;
   break;
  case TYPE_MAP_CTS_CURVE:
   ws.m_CTSCurveMapWnd_X = rc.left;
   ws.m_CTSCurveMapWnd_Y = rc.top;
   break;
  case TYPE_MAP_CHOKE_OP:
   ws.m_ChokeOpMapWnd_X = rc.left;
   ws.m_ChokeOpMapWnd_Y = rc.top;
   break;
  case TYPE_MAP_GME_WND: //pseudo map
   ws.m_GridMapWnd_X = rc.left;
   ws.m_GridMapWnd_Y = rc.top;
   break;
 };

 mp_settings->SetWndSettings(ws);
}

void CFirmwareTabController::OnOpenMapWnd(HWND i_hwnd, int i_mapType)
{
 if (!i_hwnd)
  return;

 WndSettings ws;
 mp_settings->GetWndSettings(ws);

 int X = 0, Y = 0;

 switch(i_mapType)
 {
  case TYPE_MAP_DA_START:
   X = ws.m_StrtMapWnd_X, Y = ws.m_StrtMapWnd_Y;
   break;
  case TYPE_MAP_DA_IDLE:
   X = ws.m_IdleMapWnd_X, Y = ws.m_IdleMapWnd_Y;
   break;
  case TYPE_MAP_DA_WORK:
   X = ws.m_WorkMapWnd_X, Y = ws.m_WorkMapWnd_Y;
   break;
  case TYPE_MAP_DA_TEMP_CORR:
   X = ws.m_TempMapWnd_X, Y = ws.m_TempMapWnd_Y;
   break;
  case TYPE_MAP_ATTENUATOR:
   X = ws.m_AttenuatorMapWnd_X, Y = ws.m_AttenuatorMapWnd_Y;
   break;
  case TYPE_MAP_DWELLCNTRL:
   X = ws.m_DwellCntrlMapWnd_X, Y = ws.m_DwellCntrlMapWnd_Y;
   break;
  case TYPE_MAP_CTS_CURVE:
   X = ws.m_CTSCurveMapWnd_X, Y = ws.m_CTSCurveMapWnd_Y;
   break;
  case TYPE_MAP_CHOKE_OP:
   X = ws.m_ChokeOpMapWnd_X, Y = ws.m_ChokeOpMapWnd_Y;
   break;
  case TYPE_MAP_GME_WND:
   X = ws.m_GridMapWnd_X, Y = ws.m_GridMapWnd_Y;
   break;
  default:
   return; //undefined case...
 };

 if (X != std::numeric_limits<int>::max() && Y != std::numeric_limits<int>::max())
  SetWindowPos(i_hwnd, NULL, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void CFirmwareTabController::_ShowFWOptions(const _TSTRING& info, DWORD options)
{
 if (options!=0)
 {
  _TSTRING str_options;
  str_options+=info;
  if (info.size() > 0)
   str_options+=_T("\n\n");
  int opt_count = 0;
  for(size_t i = 0; i < SECU3IO::SECU3_COMPILE_OPTIONS_BITS_COUNT; ++i)
  {
   if(options & 1 << SECU3IO::secu3_compile_options_bits[i].first)
   {
    str_options+= SECU3IO::secu3_compile_options_bits[i].second;
    str_options+=_T("\n");
    ++opt_count;
   }
  }
  str_options+=MLL::GetString(IDS_FW_TOTAL_NUMOF_OPT);
  _TSSTREAM str;  str << _T(" ") << opt_count;
  str_options+=str.str();
  AfxMessageBox(str_options.c_str(), MB_OK|MB_ICONINFORMATION);
 }
}
