
#pragma once

// ����� ��������� ������������ (�� �������� ����)
class AFX_EXT_CLASS CDialogWithAccelerators : public CDialog
{
 typedef CDialog Super;

 public:
  CDialogWithAccelerators(UINT nIDTemplate, CWnd* pParentWnd = NULL );      

 protected:
  //����� ����� ������� ���! 
  virtual BOOL PreTranslateMessage(MSG* pMsg);
};
