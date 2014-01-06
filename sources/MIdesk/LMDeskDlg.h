/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Gorlovka

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

#pragma once

#include <memory>
#include "ui-core/DialogWithAccelerators.h"

class CToolTipCtrlEx;

/////////////////////////////////////////////////////////////////////////////
// CLMDeskDlg dialog

class AFX_EXT_CLASS CLMDeskDlg : public CModelessDialog
{
  typedef CModelessDialog Super;

 public:
  CLMDeskDlg(CWnd* pParent = NULL);   // standard constructor
  static const UINT IDD;

  void Show(bool show);
  void Enable(bool enable);
  void SetValues(bool k1, bool k2, bool k3);

  //��������� �������� ����
  void Resize(const CRect& i_rect);

  // Implementation
 protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  DECLARE_MESSAGE_MAP()

 private:
  int  m_enabled;
  CStatic m_key_text[3];
  
  //std::auto_ptr<CToolTipCtrlEx> mp_ttc;
};

/////////////////////////////////////////////////////////////////////////////
