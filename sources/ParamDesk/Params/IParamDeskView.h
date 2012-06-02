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

#include "common/FastDelegate.h"
#include "common/unicodesupport.h"
#include "IDeskView.h"

//����� ���� ��������� ����� ������� � ������� ���������� (��������������)
//�����������: ����� �� ��������� ������� ���������� ����� ���� ������ ����������
//�� ���� - ����� ������� ��� ��� (����� SetCurSel()). ��� ������ � �������������� ��� ������� ������������ ������������

class IParamDeskView : public IDeskView
{
 public:
  virtual bool SetValues(BYTE i_descriptor, const void* i_values) = 0;  //������� ������ � �������������
  virtual bool GetValues(BYTE i_descriptor, void* o_values) = 0;        //������� ������ �� �������������
  virtual void ShowSaveButton(bool i_show) = 0;

  //���������/��������� ���� �������� ������������� (��� ������� FunSet...)
  //������ �������� ����� ���� �������� ������������� �������� � �������� SECU-3
  virtual void SetFunctionsNames(const std::vector<_TSTRING>& i_names) = 0;
  virtual const std::vector<_TSTRING>& GetFunctionsNames(void) = 0;

  virtual void EnableIgnitionCogs(bool enable) = 0;
  virtual void EnableUseVentPwm(bool enable) = 0;
  virtual void EnableUseCTSCurveMap(bool enable) = 0;
  virtual void EnableHallOutputParams(bool enable) = 0;
  virtual void SetCrankType(int type) = 0;
  virtual void SetMaxCylinders(int number) = 0;
  virtual void EnableOddCylinders(bool enable) = 0;

  virtual BYTE GetCurrentDescriptor(void) = 0; //�������� ���������� ��������������� ������� (������������) �������

  virtual void SetOnTabActivate(EventHandler OnTabActivate) = 0; //���������� ����� ���������� ��� ��������� �������
  virtual void SetOnChangeInTab(EventHandler OnChangeInTab) = 0; //���������� ����� ���������� ��� ��������� ������������� ������ �������
  virtual void SetOnSaveButton(EventHandler OnSaveButton) = 0; //���������� ����� ���������� ��� ������� ������ "���������"

  //�������� ������ ��� �������� ������!!!
  virtual bool SetCurSel(int sel) = 0;
  virtual int GetCurSel(void) = 0;
};
