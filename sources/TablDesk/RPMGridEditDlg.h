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

#include <vector>
#include "common/FastDelegate.h"
#include "common/UnicodeSupport.h"

class CEditEx;

class AFX_EXT_CLASS CRPMGridEditDlg : public CDialog
{
  typedef CDialog Super;
  typedef fastdelegate::FastDelegate0<> EventHandler;
  typedef fastdelegate::FastDelegate2<size_t, float> EventOnChange;

 public:
  CRPMGridEditDlg(CWnd* pParent = NULL);   // standard constructor
 ~CRPMGridEditDlg();
  static const UINT IDD;

  void SetValues(const float* ip_values);
  void GetValues(float* op_values);
  float GetValue(size_t index);
  void setOnChange(EventOnChange OnFunction);
  void setOnLoadDefVal(EventHandler OnFunction);
  void SetErrMessage(const _TSTRING& str);
  void SetItemError(size_t index, bool i_error);

 // Implementation
 protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnOK();
  virtual void OnCancel();
  virtual BOOL OnInitDialog();
  afx_msg void OnChangeEdit(UINT nID);
  afx_msg void OnLoadDefValBtn();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor);
  DECLARE_MESSAGE_MAP()

 private:
  CStatic m_errMsg;
  std::vector<CEditEx*> m_edits;
  std::vector<float> m_values;
  std::vector<bool> m_errflags;
  EventOnChange m_onChange;
  EventHandler m_onLoadDefVal;
};
