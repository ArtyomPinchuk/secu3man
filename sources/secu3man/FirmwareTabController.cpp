 /****************************************************************
 *       SECU-3  - An open source, free engine control unit
 *    Designed by Alexey A. Shabelnikov. Ukraine, Gorlovka 2007.
 *       Microprocessors systems - design & programming.
 *    contacts:
 *              http://secu-3.narod.ru
 *              ICQ: 405-791-931
 ****************************************************************/

#include "stdafx.h"
#include "secu3man.h"
#include "FirmwareTabController.h"

#include "common/FastDelegate.h"
#include "FirmwareTabDlg.h"
#include "io-core/FirmwareDataMediator.h"

using namespace fastdelegate;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFirmwareTabController::CFirmwareTabController(CFirmwareTabDlg* i_view, CCommunicationManager* i_comm, CStatusBarManager* i_sbar)
: m_view(NULL)
, m_comm(NULL)
, m_sbar(NULL)
, m_pAppAdapter(NULL)
, m_pBldAdapter(NULL)
{
  m_view = i_view;
  m_comm = i_comm;
  m_sbar = i_sbar;

  CWnd* pParent = AfxGetApp()->m_pMainWnd;

  m_pAppAdapter = new CControlAppAdapter(this);
  m_pAppAdapter->Create(pParent);


  m_pBldAdapter = new CBootLoaderAdapter(this);
  m_pBldAdapter->Create(pParent);

  m_fwdm = new CFirmwareDataMediator(); 

  //������������� �������� (����������� ������� �� �������������)
  m_view->setOnBootLoaderInfo(MakeDelegate(this,&CFirmwareTabController::OnBootLoaderInfo));
  m_view->setOnReadEepromToFile(MakeDelegate(this,&CFirmwareTabController::OnReadEepromToFile));
  m_view->setOnWriteEepromFromFile(MakeDelegate(this,&CFirmwareTabController::OnWriteEepromFromFile));
  m_view->setOnReadFlashToFile(MakeDelegate(this,&CFirmwareTabController::OnReadFlashToFile));
  m_view->setOnWriteFlashFromFile(MakeDelegate(this,&CFirmwareTabController::OnWriteFlashFromFile));
  m_view->setOnBLStartedEmergency(MakeDelegate(this,&CFirmwareTabController::OnBLStartedEmergency));
  m_view->setOnOpenFlashFromFile(MakeDelegate(this,&CFirmwareTabController::OnOpenFlashFromFile));
  m_view->setOnFWInformationTextChanged(MakeDelegate(this,&CFirmwareTabController::OnFWInformationTextChanged));
  m_view->setOnSaveFlashToFile(MakeDelegate(this,&CFirmwareTabController::OnSaveFlashToFile));
  m_view->setIsFirmwareOpened(MakeDelegate(this,&CFirmwareTabController::IsFirmwareOpened));


}

CFirmwareTabController::~CFirmwareTabController()
{
  delete m_pAppAdapter;
  delete m_pBldAdapter;
  delete m_fwdm;
}

  
//���������� ��������� 
void CFirmwareTabController::OnSettingsChanged(void)
{
 //�������� ����������� ��� ������� ��������� ���������������� ����������
 m_comm->m_pControlApp->SwitchOn(true);
 m_comm->m_pBootLoader->SwitchOn(false);  
}


void CFirmwareTabController::OnActivate(void)
{
  //AfxMessageBox(_T("CFirmwareTabController::OnActivate"));

 //////////////////////////////////////////////////////////////////
 //������������� ����������� ������� ����������� ��� ��������� ��������� � ������� �������� ����������
 m_comm->m_pControlApp->SetEventHandler(m_pAppAdapter); 
 m_comm->m_pBootLoader->SetEventHandler(m_pBldAdapter); 
 m_comm->SetOnSettingsChanged(MakeDelegate(this,&CFirmwareTabController::OnSettingsChanged)); 
 //////////////////////////////////////////////////////////////////

 m_pAppAdapter->SwitchOn(true); 

 //�������� ����������� ��� ������� ��������� ���������������� ����������
 m_comm->m_pControlApp->SwitchOn(true); //true
 m_comm->m_pBootLoader->SwitchOn(false);

 SetViewFirmwareValues();
}

void CFirmwareTabController::OnDeactivate(void)
{
  m_pAppAdapter->SwitchOn(false); 
  m_sbar->SetInformationText(_T(""));
}


/////////////////////////////////////////////////////////////////////////////////
void CFirmwareTabController::OnPacketReceived(const BYTE i_descriptor, const void* i_packet_data)
{
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
  }
  else
  { //������� � �������
    if (m_comm->m_pBootLoader->IsStoped()) //��������� ������� ������ ���� �� � �������� ����� ����� ������� 
	  m_view->EnableBLStartedEmergency(true);

	//� �������� ��������� ��������� ���� ��������� � ����������� ������� �� ��������� ��������
	bool enable = m_view->IsBLStartedEmergency();
  	m_view->EnableBLItems(enable);

	if (enable)
	  state = CStatusBarManager::STATE_BOOTLOADER; //����� ������ ���������� �� ��������� ����� ���������� ��������
	else
	  state = CStatusBarManager::STATE_OFFLINE;  
  }

  //���� ��������� ������� (����������� ��������� �� ���� ��������), �� ����� ���������� ������ 
  //������ ���������� 
  if (!m_comm->m_pBootLoader->IsStoped()) 
    state = CStatusBarManager::STATE_BOOTLOADER;

  m_sbar->SetConnectionState(state);
}



/////////////////////////////////////////////////////////////////////////////////
void CFirmwareTabController::OnUpdateUI(const int opcode,const int total,const int current)
{
	if (opcode!=CBootLoader::BL_OP_EXIT) //��� �������� ������ �� ���������� �� ���������� �������� �������� ����
	{
	  m_sbar->SetProgressRange(0,total);
	  m_sbar->SetProgressPos(current);
	}
}

void CFirmwareTabController::OnBegin(const int opcode,const int status)
{
  if (opcode == CBootLoader::BL_OP_READ_SIGNATURE)
    m_sbar->SetInformationText("������ ���������...");
  if (opcode == CBootLoader::BL_OP_READ_EEPROM)
    m_sbar->SetInformationText("������ EEPROM...");
  if (opcode == CBootLoader::BL_OP_WRITE_EEPROM)
    m_sbar->SetInformationText("������ EEPROM...");
  if (opcode == CBootLoader::BL_OP_READ_FLASH)
    m_sbar->SetInformationText("������ ��������...");
  if (opcode == CBootLoader::BL_OP_WRITE_FLASH)
    m_sbar->SetInformationText("������ ��������...");
  if (opcode == CBootLoader::BL_OP_EXIT)
  {
    //m_sbar->SetInformationText("����� �� BL...");  
  }

  m_view->EnableBLItems(false);
  m_view->EnableBLStartedEmergency(false);
}


CString CFirmwareTabController::GenerateErrorStr(void)
{
  switch(m_comm->m_pBootLoader->GetLastError())
  {
  case CBootLoader::BL_ERROR_NOANSWER:
	return CString("������: ��� ������!");
  case CBootLoader::BL_ERROR_CHKSUM:
	return CString("������: ����������� ����� �� ���������!");
  case CBootLoader::BL_ERROR_WRONG_DATA:
    return CString("������: ������������ ������!");
  }
 ASSERT(0); //��� �� ������?
 return CString(_T(""));
}



void CFirmwareTabController::OnEnd(const int opcode,const int status)
{
  //////////////////////////////////////////////////////////////////////
  if (opcode == CBootLoader::BL_OP_EXIT) //�������������� ����� ��������� ������� ��������
  {
    // m_sbar->SetInformationText("��������� ��������.");

    //����� ���������� ���������������� ���������� ����������
    m_comm->m_pControlApp->SwitchOn(true);
    m_comm->m_pBootLoader->SwitchOn(false);  
   
    //m_view->EnableBLItems(true);
    //m_view->EnableBLStartedEmergency(true);
  }
  //////////////////////////////////////////////////////////////////////
  if (opcode == CBootLoader::BL_OP_READ_SIGNATURE) 
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
  }

  //////////////////////////////////////////////////////////////////////
  else if (opcode == CBootLoader::BL_OP_READ_EEPROM)
  {
    if (status==1)
    { //OK
      m_sbar->SetInformationText("EEPROM ������� ���������!");
      SaveEEPROMToFile(m_bl_data,CBootLoader::EEPROM_SIZE);
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
  }
  //////////////////////////////////////////////////////////////////////
  else if (opcode == CBootLoader::BL_OP_WRITE_EEPROM)
  {
    if (status==1) 
      m_sbar->SetInformationText("EEPROM ������� ��������!");
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
  }
  //////////////////////////////////////////////////////////////////////
  else if (opcode == CBootLoader::BL_OP_READ_FLASH)
  {
    if (status==1)
    { 	
 	  m_sbar->SetInformationText("�������� ������� ���������!");
      SaveFLASHToFile(m_bl_data,CBootLoader::FLASH_TOTAL_SIZE);
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
  }
  //////////////////////////////////////////////////////////////////////
  else if (opcode == CBootLoader::BL_OP_WRITE_FLASH)
  {
    if (status==1) 
      m_sbar->SetInformationText("�������� ������� ��������!");
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
  }
  //////////////////////////////////////////////////////////////////////

  if (m_bl_started_emergency)
  {
    m_view->EnableBLItems(true);
    m_view->EnableBLStartedEmergency(true);

    m_comm->m_pControlApp->SwitchOn(true);
    m_comm->m_pBootLoader->SwitchOn(false);  
  }

}   

/////////////////////////////////////////////////////////////////////////////////

//��������� ���������� � ����������
void CFirmwareTabController::OnBootLoaderInfo(void)
{
  //��������� ��������� (���� �����) 	
  StartBootLoader(); 

  //���������� ���������������� ���������� ����������
  m_comm->m_pControlApp->SwitchOn(false);
  m_comm->m_pBootLoader->SwitchOn(true);  

  //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
  m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_READ_SIGNATURE,m_bl_data,0);

  m_sbar->ShowProgressBar(true);
  m_sbar->SetProgressPos(0);
}


void CFirmwareTabController::OnReadEepromToFile(void)
{
  //��������� ��������� �� ������� �� ���������� 	
  StartBootLoader(); 

  //���������� ���������������� ���������� ����������
  m_comm->m_pControlApp->SwitchOn(false);
  m_comm->m_pBootLoader->SwitchOn(true);  

  //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
  m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_READ_EEPROM,m_bl_data,0);

  m_sbar->ShowProgressBar(true);
  m_sbar->SetProgressPos(0);
}


void CFirmwareTabController::OnWriteEepromFromFile(void)
{
  bool result = LoadEEPROMFromFile(m_bl_data,CBootLoader::EEPROM_SIZE);

  if (!result)
	  return; //cancel

  ASSERT(m_comm);

  //��������� ��������� �� ������� �� ���������� 	
  StartBootLoader(); 

  //���������� ���������������� ���������� ����������
  m_comm->m_pControlApp->SwitchOn(false);
  m_comm->m_pBootLoader->SwitchOn(true);  

  //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
  m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_WRITE_EEPROM,m_bl_data,0);

  m_sbar->ShowProgressBar(true);
  m_sbar->SetProgressPos(0);
}

void CFirmwareTabController::OnReadFlashToFile(void)
{
  //��������� ��������� �� ������� �� ���������� 	
  StartBootLoader(); 

  //���������� ���������������� ���������� ����������
  m_comm->m_pControlApp->SwitchOn(false);
  m_comm->m_pBootLoader->SwitchOn(true);  

  //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
  m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_READ_FLASH,m_bl_data,CBootLoader::FLASH_TOTAL_SIZE);

  m_sbar->ShowProgressBar(true);
  m_sbar->SetProgressPos(0);
}

void CFirmwareTabController::OnWriteFlashFromFile(void)
{
  bool result = LoadFLASHFromFile(m_bl_data,CBootLoader::FLASH_TOTAL_SIZE);

  if (!result)
	  return; //cancel

  //��������� ����������� ����� � ��������� �� � ������ � ���������. ��� ���������� �������� �����
  //�� ���������� ����� ���������������� ��������, ������� ����� �� ��������� ����������� �����
  CFirmwareDataMediator::CalculateAndPlaceFirmwareCRC(m_bl_data);

  ASSERT(m_comm);

  //��������� ��������� �� ������� �� ���������� (���� ��� ����� ��� �� ������� �������) 	
  StartBootLoader(); 

  //���������� ���������������� ���������� ����������
  m_comm->m_pControlApp->SwitchOn(false);
  m_comm->m_pBootLoader->SwitchOn(true);  

  //�������� �� ��������� ����� - �������� ���������� �� ���������� ������!
  m_comm->m_pBootLoader->StartOperation(CBootLoader::BL_OP_WRITE_FLASH,m_bl_data,CBootLoader::FLASH_APP_SECTION_SIZE);

  m_sbar->ShowProgressBar(true);
  m_sbar->SetProgressPos(0); 
}



void CFirmwareTabController::SaveEEPROMToFile(const BYTE* p_data, const int size)
{
  HANDLE   hFile=0;    
  static TCHAR BASED_CODE szFilter[] = _T("BIN Files (*.bin)|*.bin|All Files (*.*)|*.*||");
  CFileDialog save(FALSE,NULL,NULL,NULL,szFilter,NULL);
  if (save.DoModal()==IDOK)
  {          
    CFile f;   
    CFileException ex;
    TCHAR szError[1024];
    if(!f.Open(save.GetFileName(),CFile::modeWrite|CFile::modeCreate,&ex))
    {
      ex.GetErrorMessage(szError, 1024);
	  AfxMessageBox(szError);
      return;
    }
    f.Write(p_data,size);
    f.Close();	     	  	  
	return;
  }
  else
	return;
}


void CFirmwareTabController::SaveFLASHToFile(const BYTE* p_data, const int size)
{
  HANDLE   hFile=0;    
  static TCHAR BASED_CODE szFilter[] = _T("BIN Files (*.bin)|*.bin|All Files (*.*)|*.*||");
  CFileDialog save(FALSE,NULL,NULL,NULL,szFilter,NULL);
  if (save.DoModal()==IDOK)
  {          
    CFile f;   
    CFileException ex;
    TCHAR szError[1024];
    if(!f.Open(save.GetFileName(),CFile::modeWrite|CFile::modeCreate,&ex))
    {
      ex.GetErrorMessage(szError, 1024);
	  AfxMessageBox(szError);
      return;
    }
	
    f.Write(p_data,size);
    f.Close();	     	  	  
	return;
  }
  else
	return;
}


//�� ������� ����� ������ ����� � EEPROM
bool CFirmwareTabController::LoadEEPROMFromFile(BYTE* p_data, const int size)
{
  HANDLE   hFile=0;    
  static TCHAR BASED_CODE szFilter[] = _T("BIN Files (*.bin)|*.bin|All Files (*.*)|*.*||");
  CFileDialog open(TRUE,NULL,NULL,NULL,szFilter,NULL);
  CString cs;

  if (open.DoModal()==IDOK)
  {     
    CFile f;   
    CFileException ex;
    TCHAR szError[1024];
    if(!f.Open(open.GetFileName(),CFile::modeRead,&ex))
    {
      ex.GetErrorMessage(szError, 1024);
	  AfxMessageBox(szError);
      return false;
    }

    //TODO: ������� �������� �� ������ ����� (��� ������ ������ ��������������� ������� EEPROM)

    f.Read(p_data,size);
    f.Close();	   
	return true; //������������� ������������
  }
  else
	return false; //����� ������������
}


//�� ������� ����� ������ ����� � FLASH
bool CFirmwareTabController::LoadFLASHFromFile(BYTE* p_data, const int size)
{
  HANDLE   hFile=0;    
  static TCHAR BASED_CODE szFilter[] = _T("BIN Files (*.bin)|*.bin|All Files (*.*)|*.*||");
  CFileDialog open(TRUE,NULL,NULL,NULL,szFilter,NULL);
  CString cs;

  if (open.DoModal()==IDOK)
  {     
    CFile f;   
    CFileException ex;
    TCHAR szError[1024];
    if(!f.Open(open.GetFileName(),CFile::modeRead,&ex))
    {
      ex.GetErrorMessage(szError, 1024);
	  AfxMessageBox(szError);
      return false;
    }

    //TODO: ������� �������� �� ������ ����� (��� ������ ������ ��������������� ����������� size!!!)

    f.Read(p_data,size);
    f.Close();	   

	return true; //������������� ������������
  }
  else
	return false; //����� ������������
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


//��� ������� ���������� ��� ������ �� ���������. ��� ������� ����� ��������� �����, ���� ������ false 
bool CFirmwareTabController::OnClose(void)
{
  bool modified = m_fwdm->IsModified();
  if (modified)
  {
   int result = AfxMessageBox(_T("�������� ���� ��������. ������ ���������?"),MB_YESNOCANCEL);
   if (result==IDCANCEL)
   {
     return false; //������������ ������� ����� �� ����������
   }
   if (result==IDNO)
   {
     return true; //������������ ����� ����� ��� ���������
   }
   if (result==IDYES)
   { //���������� ��������� ������!
     OnSaveFlashToFile();
     return true;
   }  
  }

  //������ �� ���� �������� - ��������� ����� �� ����������
  return true;
}


void CFirmwareTabController::OnOpenFlashFromFile(void)
{
  bool result;
  BYTE buff[65536];
  
  //!!! ��� ���������� � ������ ����������� ����� � �����
  result  = LoadFLASHFromFile(buff,CBootLoader::FLASH_TOTAL_SIZE);
  if (result) //user OK?
  {
   m_fwdm->LoadBytes(buff);
  
   SetViewFirmwareValues();
  }

}

void CFirmwareTabController::OnSaveFlashToFile(void)
{
  BYTE buff[65536];
 
  m_fwdm->StoreBytes(buff);

  //��������� ����������� ����� � ��������� �� � ������ � ���������	
  CFirmwareDataMediator::CalculateAndPlaceFirmwareCRC(buff);

  SaveFLASHToFile(buff,CBootLoader::FLASH_TOTAL_SIZE);

  //������ ���� ������� ��������� - ����� ���������� ������� �����������
  m_fwdm->ResetModified();
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

void CFirmwareTabController::SetViewFirmwareValues(void)
{
  if (m_fwdm->IsLoaded()==false)
	return;
  CString string = m_fwdm->GetSignatureInfo().c_str();
  m_view->SetFWInformationText(string);
}
