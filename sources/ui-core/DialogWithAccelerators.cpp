 /****************************************************************
 *
 *  Created by Alexey A. Shabelnikov. Ukraine, Gorlovka 2008.
 *   ICQ: 405-791-931. e-mail: shabelnikov-stc@mail.ru
 *  Microprocessors systems - design & programming.
 *
 *****************************************************************/

#include "stdafx.h"
#include "DialogWithAccelerators.h"

CDialogWithAccelerators::CDialogWithAccelerators()
{
 //empty
}

CDialogWithAccelerators::CDialogWithAccelerators(UINT nIDTemplate, CWnd* pParentWnd /* = NULL*/)
: Super(nIDTemplate, pParentWnd)
{
 //empty
}

BOOL CDialogWithAccelerators::PreTranslateMessage(MSG* pMsg)
{
 //���� �������� ��� ����� ��� ������ �������������, ����� ��� �� ����� �������� � ��������!
 HACCEL hAccel = ((CFrameWnd*)AfxGetApp()->m_pMainWnd)->m_hAccelTable;
 if((hAccel && ::TranslateAccelerator(AfxGetApp()->m_pMainWnd->m_hWnd, hAccel, pMsg)))
  return TRUE;

 return Super::PreTranslateMessage(pMsg);
}

CModelessDialog::CModelessDialog()
{
 //empty
}

CModelessDialog::CModelessDialog(UINT nIDTemplate, CWnd* pParentWnd /* = NULL*/)
: Super(nIDTemplate, pParentWnd)
{
 //empty
}

void CModelessDialog::OnOK()
{
 UpdateData(); //for DDX/DDV
 //�� �������� ���������� �������� ������ ����� ������ ������ ���� �������
}

void CModelessDialog::OnCancel()
{
 //�� �������� ���������� �������� ������ ����� ������ ������ ���� �������
}
