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
              http://secu-3.narod.ru
              email: secu-3@yandex.ru
*/

#pragma once

#include "common\ObjectTimer.h"
#include "IMIView.h"
#include "io-core\SECU3IO.h"

#include "MIAirFlow.h"
#include "MIDwellAngle.h"
#include "MIGasValve.h"
#include "MIPressure.h"
#include "MIShutoffValve.h"
#include "MITachometer.h"
#include "MITemperature.h"
#include "MIThrottleGate.h"
#include "MIVoltmeter.h"

#include "ui-core\DialogWithAccelerators.h"

/////////////////////////////////////////////////////////////////////////////
// CMIDeskDlg dialog

class AFX_EXT_CLASS CMIDeskDlg : public CModelessDialog, public IMIView
{
  typedef CModelessDialog Super;

 public:
  CMIDeskDlg(CWnd* pParent = NULL);   // standard constructor
  static const UINT IDD;

  //--------interface implementation---------------
  virtual void Show(bool show);
  virtual void Enable(bool enable);
  virtual void SetValues(const SECU3IO::SensorDat* i_values);
  virtual void GetValues(SECU3IO::SensorDat* o_values);
  //-----------------------------------------------

  //��������� ������� ���������� � �������������
  void SetUpdatePeriod(unsigned int i_period);

  //Set maximum value of tachometer displayed on the fixture
  void SetTachometerMax(int i_max);

  //Set maximum value of pressure meter displayed on the fixture
  void SetPressureMax(int i_max);

  //��������� �������� ����
  void Resize(const CRect& i_rect);

 protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog(); //activate
  afx_msg void OnDestroy();    //deactivate
  DECLARE_MESSAGE_MAP()

  void OnUpdateTimer(void);

  // Implementation
 private:
  CMITachometer	m_tachometer;
  CMIPressure     m_pressure;
  CMIVoltmeter    m_voltmeter;
  CMIDwellAngle   m_dwell_angle;
  CMITemperature  m_temperature;

  CMIAirFlow      m_air_flow;
  CMIGasValve     m_gas_valve;
  CMIThrottleGate m_throttle_gate;
  CMIShutoffValve m_shutoff_valve;

  SECU3IO::SensorDat m_values;
  CObjectTimer<CMIDeskDlg> m_update_timer;
  unsigned int m_update_period;
  bool m_was_initialized;
};

/////////////////////////////////////////////////////////////////////////////
