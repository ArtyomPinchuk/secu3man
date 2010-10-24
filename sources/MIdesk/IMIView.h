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

namespace SECU3IO
{
 struct SensorDat;
}

//��������� ��� ���������� ������� ������������� ������������ (��������������)
//TODO: ���� � ������� �������� ������������� ������������ UI ������� �� ������������� (��� ������ ��������)
//�� ���� �������� � ��������� ��������������� ������� ���������� ���������. ��� ����� ����� ������������
//boost::bind ��� FastDelegate.

class IMIView  
{
 public:
  virtual void Show(bool show) = 0;                                //��������/�������� ������� �������������
  virtual void Enable(bool enable) = 0;                            //����������/���������� �������������
  virtual void SetValues(const SECU3IO::SensorDat* i_values) = 0;  //�������� ������ � �������������
  virtual void GetValues(SECU3IO::SensorDat* o_values) = 0;        //��������� ������ �� �������������	
};
