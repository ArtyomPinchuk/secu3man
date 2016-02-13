/* Common utilities - object oriented timer
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
             e-mail: shabelnikov@secu-3.org
             Microprocessor systems - design & programming.
*/

/** \file ObjectTimer.h
 * \author Alexey A. Shabelnikov
 */

#pragma once

#include <map>

//��� ��������� ���������� ���������� ��� �������� � ����������� ����������� this, ����� ���������
//������������ �� �������������� �������, ������ ������� �������� ��������� �� CObjectTimer
static std::map<int,void*> g_object_timer_instance_map;

template<class T> class CObjectTimer
{
 public:
  typedef void (T::*msgHandlerType) (void);

  CObjectTimer(T* ip) : m_pDispatcher(ip), msgHandler(NULL), m_interval_ms(0), m_timer_id(0) {};
  CObjectTimer()      : m_pDispatcher(NULL), msgHandler(NULL), m_interval_ms(0), m_timer_id(0) {};

  //��� �������� ���������� �������-������� ���� ���������� ������ �� ���� �������
  virtual ~CObjectTimer()
  {
   KillTimer();
  };

  //set all necessary paramaters and runs timer
  void SetTimer(T* object, msgHandlerType i_function, int interval)
  {
   SetMsgHandler(i_function);
   m_pDispatcher = object;
   SetTimer(interval);
   m_interval_ms = interval;
  };

  //��������� ������ ���� �������� ����� ������ ���� �������������� � ����������� ��� �������
  //��������� �� ������ � ���� ������� ������� SetMsgHandler() ��� ��������� �����������
  void SetTimer(int interval)
  {
   ASSERT(m_timer_id == 0);
   if (m_timer_id != 0)
    return; //already installed!

   m_timer_id = ::SetTimer(NULL,0,interval,(TIMERPROC)&TimerProc);
   ASSERT(m_timer_id); //failed to set timer!
   g_object_timer_instance_map[m_timer_id] = this;
   m_interval_ms = interval;
  }

  //Kill timer using this function
  void KillTimer(void)
  {
   ::KillTimer(NULL,m_timer_id);
   if (!g_object_timer_instance_map.empty())
   {
    if (g_object_timer_instance_map.find(m_timer_id)!=g_object_timer_instance_map.end())
     g_object_timer_instance_map.erase(m_timer_id);
    else
     ASSERT(m_timer_id == 0); //timer id is non-zero but we can not find it in map!
   }
   m_timer_id = 0; //indicate that we reset the timer
  }

  //��� ������� ��������� �� ����� ����� �������-�����: void T::Function(void)
  inline void SetMsgHandler(msgHandlerType i_function)
  {
   msgHandler = i_function;
  }

  //������ ������� ������� ��������� ����� ������� � ����� �������
  inline void SetMsgHandler(T* ip_this, msgHandlerType i_function)
  {
   m_pDispatcher = ip_this;
   msgHandler = i_function;
  }

  //returns current period of timer events
  int GetPeriod(void) const
  {
   return m_interval_ms;
  }

 private:

  static inline VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
  {
   //welcome to hell :-D ... � ��� ������ ��� � �++ ���� ��������!?
   CObjectTimer* p_this = reinterpret_cast<CObjectTimer*>(g_object_timer_instance_map[idEvent]);
   ASSERT(p_this);
   if (!p_this)  return;

   T *x = p_this->m_pDispatcher;
   ASSERT(x);
   if (!x) return;

   //���������� ���������� �������� ���������� ������ ������ T, ������ ���� ������� ������������������
   if (p_this->msgHandler!=NULL)
   {
    (x->*(p_this->msgHandler))();
   }
  };

 private: //internal data
  msgHandlerType msgHandler;
  T*   m_pDispatcher;
  UINT_PTR m_timer_id;
  int  m_interval_ms;
};
