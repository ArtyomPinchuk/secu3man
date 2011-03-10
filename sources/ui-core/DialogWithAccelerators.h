
#pragma once

// ����� ��������� ������������ (�� �������� ����)
class AFX_EXT_CLASS CDialogWithAccelerators : public CDialog
{
 typedef CDialog Super;

 public:
  CDialogWithAccelerators(); //modeless
  CDialogWithAccelerators(UINT nIDTemplate, CWnd* pParentWnd = NULL );

 protected:
  //����� ����� ������� ���!
  virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//������ � �������������� ������� ������ ������� �� IDOK ��� IDCANCEL
class AFX_EXT_CLASS CModelessDialog : public CDialogWithAccelerators
{
 typedef CDialogWithAccelerators Super;

 public:
  CModelessDialog(); //modeless
  CModelessDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL );

 protected:
  virtual void OnOK();
  virtual void OnCancel();
};
