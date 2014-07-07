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

#pragma once

#include "IMeasInstrument.h"
#include "ui-core/AnalogMeterCtrl.h"

class CToolTipCtrlEx;

class AFX_EXT_CLASS CMIVoltage : public IMeasInstrument
{
 public:
  CMIVoltage(const _TSTRING& ttText);
  virtual ~CMIVoltage();

  void DDX_Controls(CDataExchange* pDX, int nIDC_meter);

  //-------interface-----------------------
  virtual void Create(void);
  virtual void Scale(float i_x_factor, float i_y_factor);
  virtual void SetValue(float value);
  virtual float GetValue(void);
  virtual void Show(bool show);
  virtual void Enable(bool enable);
  virtual bool IsVisible(void);
  virtual bool IsEnabled(void);
  virtual void SetLimits(float loLimit, float upLimit);
  virtual void SetTicks(int number);
  //---------------------------------------

  void SetTitle(const _TSTRING& title);

 private:
  CAnalogMeterCtrl m_meter;
  std::auto_ptr<CToolTipCtrlEx> mp_ttc;
  _TSTRING m_ttText;
};
