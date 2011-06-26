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
#include "common/unicodesupport.h"
#include "io-core/ControlApp.h"
#include "io-core/ControlAppAdapter.h"
#include "io-core/ufcodes.h"
#include "TabsManagement/ITabController.h"

class CCommunicationManager;
class CParamMonTabDlg; //view
class CPMInitDataCollector;
class CPMMonitorController;
class CPMParamsController;
class CPMStateMachineState;
class CPMTablesController;
class CStatusBarManager;
class ISettingsData;

class CParamMonTabController : public ITabController, private IAPPEventHandler
{
 public:
  CParamMonTabController(CParamMonTabDlg* i_view, CCommunicationManager* i_comm, CStatusBarManager* i_sbar, ISettingsData* ip_settings);
  virtual ~CParamMonTabController();

 private:
  //���������/�������� ������� ���������� � ��������
  virtual void OnActivate(void);
  virtual void OnDeactivate(void);

  virtual bool OnClose(void);
  virtual bool OnAskFullScreen(void);
  virtual void OnFullScreen(bool i_what, const CRect& i_rect);

  //from IAPPEventHandler:
  virtual void OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet);
  virtual void OnConnection(const bool i_online);

  void OnSettingsChanged(void);

  //from "show raw sensors" check box
  void OnRawSensorsCheckBox(void);
  //from "edit tables" check box
  void OnEditTablesCheckBox(void);

  void OnPDRequestsDataCollection();

 private:
  CParamMonTabDlg*  m_view;
  CCommunicationManager* m_comm;
  CStatusBarManager*  m_sbar;
  ISettingsData* mp_settings;
  std::auto_ptr<CPMTablesController> mp_tabcntr;
  std::auto_ptr<CPMParamsController> mp_parcntr;
  std::auto_ptr<CPMMonitorController> mp_moncntr;
  std::auto_ptr<CPMInitDataCollector> mp_idccntr;

  void StartScenario(const std::vector<CPMStateMachineState*>& scenario);
  //��������� ������ ��������� (��� ������ ���������)
  std::vector<CPMStateMachineState*> m_scenario1;
  std::vector<CPMStateMachineState*> m_scenario2;
  std::vector<CPMStateMachineState*> m_scenario3;
  std::vector<CPMStateMachineState*> m_scenario4;
  //������ ���������
  std::vector<CPMStateMachineState*> m_state_machine;
  std::vector<CPMStateMachineState*>::iterator m_current_state;
};
