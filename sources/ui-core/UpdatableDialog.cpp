/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Kiev

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   contacts:
              http://secu-3.org
              email: shabelnikov@secu-3.org
*/

/** \file UpdatableDialog.cpp
 * \author Alexey A. Shabelnikov
 */

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
