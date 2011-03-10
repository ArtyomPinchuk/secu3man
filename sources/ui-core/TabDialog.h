
#pragma once

#include "UpdatableDialog.h"


//���� ����� ��������� ���������������� ������������� � ���, ����� ������ ������ ����
//������� �� IDOK ��� IDCANCEL.
class AFX_EXT_CLASS CTabDialog : public CUpdatableDialog
{
  typedef CUpdatableDialog Super;

 public:
  CTabDialog(UINT nIDTemplate, CWnd* pParentWnd);
  virtual ~CTabDialog();

  //���������� ������������� �������
  virtual LPCTSTR GetDialogID(void) const = 0;

  //���������� ������� � Tab-�������� ������ ��������� �� IDOK ��� IDCANCEL
  virtual void OnOK();
  virtual void OnCancel();
};
