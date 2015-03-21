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

/** \file HotKeysToCmdRouter.h
 * \author Alexey A. Shabelnikov
 */

#pragma once

#include "WndSubclasser.h"
#include "../common/unicodesupport.h"
#include <map>

class HotKeysManager;

//��������! ������ ������ ����������� ������ � �������������� heap!
class AFX_EXT_CLASS CHotKeysToCmdRouter : private CWndSubclasser
{
 public:
  CHotKeysToCmdRouter();
  virtual ~CHotKeysToCmdRouter();

  //E��� Init() �� ������, �� ������� �� ��������������. ������� ������ Init() ������
  //��������������� ����� Close().
  bool Init(CWnd* ip_wnd);

  //������������ �� ����, �������� ������������������ ������� ������. Close() �������������
  //���������� ������������.
  bool Close(void);

  // i_command_id - ID ������� (�������� IDM_OPEN_FILE)
  // i_vk - ��� ������� �������.
  // i_fsModifiers - ����� ������������� ������������ ������� ������� ������ ���� ������ ������
  // � ������� ��������. ��������: MOD_ALT, MOD_CONTROL, MOD_SHIFT, MOD_WIN.
  // �� ��������� ����������� ����������� � �������� �. RegisterHotKey() � MSDN.
  bool RegisterCommand(UINT i_command_id, UINT i_vk, UINT i_fsModifiers = 0);

  //�������� ������������������ ������� �������.
  //i_command_id - ID-��� ������� ��� ������ � �����������. ���� ���������������� ���������
  //������ � ����������� ID-�����, �� ��� ��� ����� ����� � �����������.
  bool UnregisterCommand(UINT i_command_id);

  //�������� (������ � �����������) ���� ������������������ ������� ������.
  bool UnregisterAllCommands();

 private:
  friend HotKeysManager;

  //from CWndSubclasser
  virtual LRESULT WndProcSub(UINT uMsg, WPARAM wParam, LPARAM lParam);

  struct HotKeyInfo
  {
   UINT m_id_command; //ID-��� ������� ��� WM_COMMNAND
   UINT m_fsModifiers;
   UINT m_vk;
  };

  //<ID-��� ������� �������, info>
  typedef std::map<int, HotKeyInfo> HotKeyMap;
  HotKeyMap::iterator _FindCommandID(UINT i_command_id);

  //pointer to original window
  CWnd*   mp_OriginalWnd;

  //���� ������ ID-���
  HotKeyMap m_hot_key_map;

  _TSTRING m_thread_id_string;
};
