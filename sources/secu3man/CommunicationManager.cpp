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
#include "CommunicationManager.h"
#include "io-core/CComPort.h"
#include "io-core/Bootloader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommunicationManager::CCommunicationManager()
: m_pComPort(NULL)
, m_pBootLoader(NULL)
, m_pControlApp(NULL)
, m_recv_buff_size(32768)
, m_send_buff_size(32768)
, m_pSettings(NULL)  //external!
, m_OnSettingsChanged(NULL)
{
  m_pSettings = (static_cast<CSecu3manApp*>(AfxGetApp()))->m_pAppSettingsManager;

  m_pComPort = new CComPort(_T("COM1"),m_recv_buff_size,m_send_buff_size);

  m_pBootLoader = new CBootLoader();
  m_pControlApp = new CControlApp();
}

CCommunicationManager::~CCommunicationManager()
{
  delete m_pComPort;
  delete m_pBootLoader;
  delete m_pControlApp;
}


bool CCommunicationManager::Terminate(void)
{
  //��������� ������ ������������ �����������
  m_pControlApp->Terminate();
  m_pBootLoader->Terminate();

  m_pComPort->Terminate();

  return true;
}


bool CCommunicationManager::Init(void)
{
  ASSERT(m_pSettings);  //�� ����� ���� ����������������!
  ASSERT(m_pBootLoader);
  ASSERT(m_pControlApp);

  bool status = true;

  //������������� ������ ������������ �����������, ���������� ���������� ����� �����������
  //����� - � ����������� �� �������� ��������� ���������
  m_pControlApp->SwitchOn(false); 
  m_pBootLoader->SwitchOn(false); 

  //��� ���� ����� ����� ���� ��������� ����� ��������� � �����, ���������� ��� ������ �������
  m_pComPort->Terminate();

  try
  {
    //�� �������� ���������� � ��� ������� �� ����� �������� �������� (��� ����� ����������� ���������� ������������ �����������)
	m_pComPort->Initialize(_TSTRING(m_pSettings->m_pModel->m_optPortName),9600,NOPARITY,ONESTOPBIT,0,1);
  }
  catch(CComPort::xInitialize e)
  {
    //������� ������������ ��� ��� �� ������� ������� ��������� ����!
    AfxMessageBox(e.GetDetailStr());
	status = false;
  }
  
  m_pComPort->Purge();

  //�������������� ����������� �����������
  try 
  { m_pControlApp->Initialize(m_pComPort,m_pSettings->m_pModel->m_optBaudRateApplication,500);
  }
  catch(CControlApp::xThread)
  {
    ASSERT(0); //because, strange situation!
	status = false;
  }

  try
  { m_pBootLoader->Initialize(m_pComPort,m_pSettings->m_pModel->m_optBaudRateBootloader);
  }
  catch (CBootLoader::xThread)
  {
    ASSERT(0);
	status = false;
  }

  m_pControlApp->SetSettings(m_pSettings->m_pModel->m_optMAPCurveOffset,m_pSettings->m_pModel->m_optMAPCurveSlope);

  //��������� ������� ��������� �� ��������� ��������
  if (m_OnSettingsChanged) 
	m_OnSettingsChanged();

  return status;
}
