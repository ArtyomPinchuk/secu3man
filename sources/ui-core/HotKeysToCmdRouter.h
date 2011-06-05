/****************************************************************
 *
 *  Created by Alexey A. Shabelnikov. Ukraine, Gorlovka 2008.
 *   ICQ: 405-791-931. e-mail: shabelnikov-stc@mail.ru
 *  Microprocessors systems - design & programming.
 *
 *****************************************************************/

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
