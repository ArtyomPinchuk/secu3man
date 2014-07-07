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

//������� ����� ��� ������� �������������� ��������� ��������� ��������
class CPMStateMachineState
{
 public:
  CPMStateMachineState() : m_operation_state(0){}
  virtual ~CPMStateMachineState() {}

  //������ ����� ������
  virtual void StartDataCollection(void) = 0
  {
   m_operation_state = 0;
  }

  //������ ���� ����������� ������ ����� �������� (�������� �������)
  //���������� true ����� ��� ������ ���������. ��� ������� ����� �������� ��� �������
  //������ ����������� �� SECU-3
  virtual bool CollectData(const BYTE i_descriptor, const void* i_packet_data) = 0;

 protected:
  int  m_operation_state;
};

//������� ����� ��� ������������ ������� "��������� � �������"
template <class T>
class CPMControllerBase : public CPMStateMachineState
{
 public:
  typedef T VIEW;

  CPMControllerBase(T* ip_view) : mp_view(ip_view) {}
  virtual ~CPMControllerBase() {}

  //������ ������ �����������
  virtual void OnActivate(void) = 0;

  //����� ������ �����������
  virtual void OnDeactivate(void) = 0;

 protected:
  T*  mp_view;       //view
};


