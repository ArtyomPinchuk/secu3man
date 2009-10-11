
#pragma once

/*--------------------------------------------------------------------------------------

���� ���������������� �� ����� ������ �� � ����������� ������� ����� �������� ��������
ON_UPDATE_COMMAND_UI. ��� ��������� �������� ����� ������������ ������������������� ���������
WM_KICKIDLE ��� OnKickIdle()

�� ��������� ���������� (�������� Pocket PC) ����� �� ���� ��������� 0x3FC � ��������
������ �������� �� ������ ��������� (� ����� ������ ������������������). ��� ��������� 
�������� ���������� �������� � ����� ���������������� �� CWinApp ��������� ����������:

virtual BOOL IsIdleMessage(MSG* pMsg)
{
  if(CWinApp::IsIdleMessage(pMsg) == FALSE)
    return FALSE;

  return (pMsg->message != 0x3FC);
}

---------------------------------------------------------------------------------------*/

class AFX_EXT_CLASS CUpdatableDialog : public CDialog
{
 public:
  CUpdatableDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL );      

 protected:
  //����� ����� ������� ���! 
  virtual BOOL PreTranslateMessage(MSG* pMsg);
	
 private:
  BOOL m_bDoIdle;
};
