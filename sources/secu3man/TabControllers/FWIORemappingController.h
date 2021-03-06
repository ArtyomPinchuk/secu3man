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

/** \file FWIORemappingController.h
 * \author Alexey A. Shabelnikov
 */

#pragma once

#include <map>
#include "io-core/FirmwareDataMediator.h"

class CIORemappingDlg;

class CFWIORemappingController
{
  typedef CIORemappingDlg IORVIEW;
  typedef CFirmwareDataMediator FWDM;

 public:
  CFWIORemappingController(IORVIEW* ip_view);
  virtual ~CFWIORemappingController();

  //Must be called each time when user loads firmware
  void AttachFWDM(FWDM* ip_fwdm);

  //������ ������ �����������
  void OnActivate(void);

  //����� ������ �����������
  void OnDeactivate(void);

  void Enable(bool state);

  void EnableSECU3TFeatures(bool i_enable);

 private:
  FWDM::IOPid _IOPStart(void);
  FWDM::IOPid _IOPCount(void);
  FWDM::IOSid _IOSStart(void);
  FWDM::IOSid _IOSCount(void);
  bool _CheckForErrors(void);
  void _UpdateView(void);
  void _AttachPlug(FWDM::IOPid iopId);
  void _AttachPlug(FWDM::IOPid iopId, FWDM::IOSid iosId, bool inv = false);
  void _AttachPlugToSpecifiedSlot(FWDM::IOPid iopId, FWDM::IOSid iosId);
  void _DetachPlugsFromSpecifiedSlot(FWDM::IOSid iosId);
  void _AttachFreeSlotsToDefaultPlugs(void);
  bool _IsSlotFree(FWDM::IOSid iosId);
  void _PrepareLogic(void);
  bool _FixRedundantConnections(void);
  bool _FixInputsVSOutputs(void);
  bool _IsIOPInput(FWDM::IOPid iopId) const;
  bool _IsIOSInput(FWDM::IOSid iosId) const;
  FWDM::IOSid _GetConnectedSlot(FWDM::IOPid iopId, bool init, bool& inv);
  void _EnableInversionItems(void);
  void _DisplayPlugs(void);
  void _SetInvFlag(FWDM::IOSid iosId, bool inv);
  bool _CmpPnS(FWDM::IOPid pid, FWDM::IOSid sid);
  void OnItemSelected(FWDM::IOSid iosId, FWDM::IOPid iopId);
  void OnItemInverted(FWDM::IOSid iosId, bool i_invert);

  CIORemappingDlg* mp_view;
  FWDM* mp_fwdm;
  std::map<FWDM::IOSid, FWDM::IOPid> m_defValMap;
  std::map<FWDM::IOSid, bool> m_invFlags;
};
