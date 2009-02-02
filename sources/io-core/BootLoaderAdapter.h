
#pragma once

#include "BootLoader.h"

//���� ������� ��������� ��� ���������� �������� ������ MFC ������� �� "���������" ������

//ACHTUNG! CWnd ������ ������������� ������, ����� ����� ����� ����������� ��������!
class AFX_EXT_CLASS CBootLoaderAdapter  : public CWnd, public IBLDEventHandler
{
  public:
    CBootLoaderAdapter();
	virtual ~CBootLoaderAdapter();

    //��������� ���� ���������� ��� ������ ��� ���� ����� �������� ����� ���� ���������!
    BOOL Create(CWnd* pParentWnd);

	//��������� ����������� �������
	void SetEventHandler(IBLDEventHandler* ip_destination_handler);

  private:
    IBLDEventHandler* m_destination_handler;

	virtual void OnUpdateUI(IBLDEventHandler::poolUpdateUI* ip_data);  
    virtual void OnBegin(const int opcode,const int status);
    virtual void OnEnd(const int opcode,const int status);    

  protected:

	afx_msg LRESULT msgOnUpdateUI(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT msgOnBegin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT msgOnEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP();    
};

