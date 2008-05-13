
#pragma once

#include "BootLoader.h"

//���� ������� ��������� ��� ���������� �������� ������ MFC ������� �� "���������" ������

//ACHTUNG! CWnd ������ ������������� ������, ����� ����� ����� ����������� ��������!
class AFX_EXT_CLASS CBootLoaderAdapter  : public CWnd, public IBLDEventHandler
{
  public:
    CBootLoaderAdapter(IBLDEventHandler* i_destination_handler)
    : m_destination_handler(i_destination_handler)
	{ 
	  ASSERT(i_destination_handler);	  
	};

	virtual ~CBootLoaderAdapter() {};

    //��������� ���� ���������� ��� ������ ��� ���� ����� �������� ����� ���� ���������!
    BOOL Create(CWnd* pParentWnd)
	{
      ASSERT(pParentWnd);		
	  return CWnd::Create(NULL,_T("CBootLoader_Adapter_Wnd"),0,CRect(0,0,0,0),pParentWnd,0);  
	}


  private:
    IBLDEventHandler* m_destination_handler;

	virtual void OnUpdateUI(const int opcode,const int total,const int current);  
    virtual void OnBegin(const int opcode,const int status);
    virtual void OnEnd(const int opcode,const int status);    

  protected:

	afx_msg LRESULT msgOnUpdateUI(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT msgOnBegin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT msgOnEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP();

    struct poolUpdateUI
	{
	  poolUpdateUI(int i_opcode,int i_total,int i_current)
	  : opcode(i_opcode), total(i_total), current(i_current){};
  	  const int opcode;
	  const int total;
	  const int current;
	};

};

