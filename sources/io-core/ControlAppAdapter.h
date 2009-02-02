
#pragma once

#include "ControlApp.h"
#include "common/unicodesupport.h"
#include <map>

////////////////////////////////////////////////////////////////////////

//���� ������� ��������� ��� ���������� �������� ������ MFC ������� �� "���������" ������

//ACHTUNG! CWnd ������ ������������� ������, ����� ����� ����� ����������� ��������!
class AFX_EXT_CLASS CControlAppAdapter : public CWnd, public IAPPEventHandler
{
  public:
	  CControlAppAdapter();	  
      virtual ~CControlAppAdapter();

	  //��������� ���� ���������� ��� ������ ��� ���� ����� �������� ����� ���� ���������!
      BOOL Create(CWnd* pParentWnd);	  

	  //��������/��������� �������. ���� ������� ��������, �� �� �� �������������� 
	  //������ � ������� ����������
	  void SwitchOn(bool state);

	  //���������� ������������
      bool AddEventHandler(IAPPEventHandler* i_pEventHandler, const _TSTRING &i_observer_key);

	  //�������� ������������
      bool RemoveEventHandler(const _TSTRING &i_observer_key);

  protected:
	  afx_msg LRESULT msgOnConnection(WPARAM wParam, LPARAM lParam);
	  afx_msg LRESULT msgOnPacketReceived(WPARAM wParam, LPARAM lParam);

	  DECLARE_MESSAGE_MAP();           

  private:
      typedef std::map<_TSTRING, IAPPEventHandler*> Observers;
      typedef std::map<_TSTRING, IAPPEventHandler*>::iterator ObserversIterator;

      Observers m_observers;                //������ ������������� �������      
	  bool m_switch_on;
	  bool m_switch_on_thread_side;

      inline void _UpdateInternalState(void);

  	  //from IAPPEventHandler, called by thread:
	  virtual void OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet);
	  virtual void OnConnection(const bool i_online);
};
////////////////////////////////////////////////////////////////////////
