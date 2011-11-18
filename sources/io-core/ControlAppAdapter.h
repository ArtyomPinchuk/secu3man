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

#include "ControlApp.h"
#include "common/unicodesupport.h"
#include <map>


//���� ��������� ������ ��������������� �������-������������ �������
class IAPPEventHandler
{
 public:
  //event handlers
  virtual void OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet) = 0;
  virtual void OnConnection(const bool i_online) = 0;
};


////////////////////////////////////////////////////////////////////////

//���� ������� ��������� ��� ���������� �������� ������ MFC ������� �� "���������" ������

//ACHTUNG! CWnd ������ ������������� ������, ����� ����� ����� ����������� ��������!
class AFX_EXT_CLASS CControlAppAdapter : public CWnd, public IAPPThreadEventHandler
{
 public:
  CControlAppAdapter();
  virtual ~CControlAppAdapter();

  //��������� ���� ���������� ��� ������ ��� ���� ����� �������� ����� ���� ���������!
  BOOL Create(CWnd* pParentWnd);

  //��������/��������� �������. ���� ������� ��������, �� �� �� ��������������
  //������ � ������� ����������
  void SwitchOn(bool state);

  //���������� ������������
  bool AddEventHandler(IAPPEventHandler* i_pEventHandler, const _TSTRING &i_observer_key);

  //�������� ������������
  bool RemoveEventHandler(const _TSTRING &i_observer_key);

 protected:
  afx_msg LRESULT msgOnConnection(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT msgOnPacketReceived(WPARAM wParam, LPARAM lParam);

  DECLARE_MESSAGE_MAP();

 private:
  typedef std::map<_TSTRING, IAPPEventHandler*> Observers;
  typedef std::map<_TSTRING, IAPPEventHandler*>::iterator ObserversIterator;

  Observers m_observers;                //������ ������������� �������
  bool m_switch_on;
  volatile bool m_switch_on_thread_side;

  inline void _UpdateInternalState(void);

  //from IAPPEventHandler, called by thread:
  virtual void OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet);
  virtual void OnConnection(const bool i_online);
};
////////////////////////////////////////////////////////////////////////
