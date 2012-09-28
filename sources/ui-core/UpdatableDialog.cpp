 /****************************************************************
 *
 *  Created by Alexey A. Shabelnikov. Ukraine, Gorlovka 2008.
 *   ICQ: 405-791-931. e-mail: shabelnikov-stc@mail.ru
 *  Microprocessors systems - design & programming.
 *
 *****************************************************************/

#include "stdafx.h"
#include "UpdatableDialog.h"

CUpdatableDialog::CUpdatableDialog()
{
 //empty
}

CUpdatableDialog::CUpdatableDialog(UINT nIDTemplate, CWnd* pParentWnd /* = NULL*/)
: Super(nIDTemplate, pParentWnd)
, m_bDoIdle(TRUE)
{
 //empty
}

BOOL CUpdatableDialog::PreTranslateMessage(MSG* pMsg)
{
 //���� �������� ��� ����� ��� ������ �������������, ����� ��� �� ����� �������� � ��������!
 HACCEL hAccel = ((CFrameWnd*)AfxGetApp()->m_pMainWnd)->m_hAccelTable;
 if((hAccel && ::TranslateAccelerator(AfxGetApp()->m_pMainWnd->m_hWnd, hAccel,pMsg)))
  return TRUE;

 MSG msg;
 if(!::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) && m_bDoIdle)
 {
  //���������� ���� ��� ������ ����� ��� ��������� � �������
  UpdateDialogControls(this,TRUE);
  m_bDoIdle = FALSE;
 }
 else
 {
  if(AfxGetApp()->IsIdleMessage(pMsg) && pMsg->message != 0x3FC)
  {
   m_bDoIdle = TRUE;
  }
 }

 return Super::PreTranslateMessage(pMsg);
}

CModelessUpdatableDialog::CModelessUpdatableDialog()
{
 //empty
}

CModelessUpdatableDialog::CModelessUpdatableDialog(UINT nIDTemplate, CWnd* pParentWnd /* = NULL*/)
: Super(nIDTemplate, pParentWnd)
{
 //empty
}

void CModelessUpdatableDialog::OnOK()
{
 UpdateData(); //for DDX/DDV
 //�� �������� ���������� �������� ������ ����� ������ ������ ���� �������
}

void CModelessUpdatableDialog::OnCancel()
{
 //�� �������� ���������� �������� ������ ����� ������ ������ ���� �������
}
