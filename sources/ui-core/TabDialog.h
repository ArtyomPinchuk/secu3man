
#pragma once

#include "UpdatableDialog.h"

class AFX_EXT_CLASS CTabDialog : public CUpdatableDialog 
{
	typedef CUpdatableDialog Super;

  public:
  	CTabDialog(UINT nIDTemplate, CWnd* pParentWnd);

	//���������� ������������� �������
	virtual LPCTSTR GetDialogID(void) const = 0;

	virtual ~CTabDialog();

	//���������� ������� � Tab-�������� ������ ��������� �� IDOK ��� IDCANCEL
	void OnOK();
	void OnCancel();
};


