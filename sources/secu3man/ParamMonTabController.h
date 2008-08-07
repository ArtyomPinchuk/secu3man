
#pragma once

#include <map>
#include "ITabController.h"
#include "ParamMonTabDlg.h"
#include "CommunicationManager.h"
#include "StatusBarManager.h"
#include "io-core/ControlApp.h"
#include "io-core/ControlAppAdapter.h"
#include "io-core/ufcodes.h"
#include "common/ObjectTimer.h"
#include "common/unicodesupport.h"


class CParamMonTabController : public ITabController, private IAPPEventHandler 
{
  public:
	CParamMonTabController(CParamMonTabDlg* i_view, CCommunicationManager* i_comm, CStatusBarManager* i_sbar);
	virtual ~CParamMonTabController();

  private:
    CParamMonTabDlg*  m_view;
	CCommunicationManager* m_comm;
	CStatusBarManager*  m_sbar;
    CControlAppAdapter* m_pAdapter;
	
	CObjectTimer<CParamMonTabController> m_pd_changes_timer;

    //������� �� ������ ����������
    void OnParamDeskTabActivate(void); 
    void OnParamDeskChangeInTab(void);   

    //���������/�������� ������� ���������� � ��������
    virtual void OnActivate(void);
	virtual void OnDeactivate(void);
	virtual bool OnClose(void);

	//from IAPPEventHandler:
    virtual void OnPacketReceived(const BYTE i_descriptor, const void* i_packet_data);
    virtual void OnConnection(const bool i_online);

	void OnSettingsChanged(void);
    void OnParamDeskChangesTimer(void);


    enum //Packet Processing States
	{
	 PPS_COLLECT_INITIAL_DATA = 0,
     PPS_READ_NECESSARY_PARAMETERS = 1,
	 PPS_BEFORE_READ_MONITOR_DATA = 2,
	 PPS_READ_MONITOR_DATA = 3
	};

	////////////////////internal//////////////////////////
    bool CollectInitialDataFromSECU(const BYTE i_descriptor, const void* i_packet_data);
	void StartCollectingInitialData(void); 
    //////////////////////////////////////////////////////
    bool ReadNecessaryParametersFromSECU(const BYTE i_descriptor, const void* i_packet_data);
	void StartReadingNecessaryParameters(void);   
	//////////////////////////////////////////////////////

	void OnRawSensorsCheckBox(void);

  private:
    int  m_packet_processing_state;  //������ ��� ������� ��������, ���� ������� ������ �������� �� ����������� �� ������ ���������� PPS_READ_MONITOR_DATA
	int  m_operation_state;          //������ ��������� �������� ��������� ���������� ��������, ���� -1 -������ �� ����������

	int  m_lastSel;                  //������ ����� ������� ������ ���������� ������� ���� ������� � ��������� ���
	bool m_parameters_changed;       //���� ���� ��������������� ��� ��������� �� ����� �� ������� ���������� � ������������ ����� ������� ���������� ������ � SECU

};


