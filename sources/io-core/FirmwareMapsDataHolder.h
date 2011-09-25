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

#include "common/unicodesupport.h"
#include <vector>

//���������� ���������� ����� ������������ ��� ������ �������
#define F_WRK_POINTS_F         16
#define F_WRK_POINTS_L         16
#define F_TMP_POINTS           16
#define F_STR_POINTS           16
#define F_IDL_POINTS           16
#define F_NAME_SIZE            16
#define KC_ATTENUATOR_LOOKUP_TABLE_SIZE 128
#define COIL_ON_TIME_LOOKUP_TABLE_SIZE 32

//���������� ������� ������������� �������� � ������ ��������
#define TABLES_NUMBER          8
//���������� ������� ������������� �������� � EEPROM
#define TUNABLE_TABLES_NUMBER  2

struct SECU3FWMapsItem
{
 float f_str[F_STR_POINTS];    // ������� ��� �� ������
 float f_idl[F_IDL_POINTS];    // ������� ��� ��� ��
 float f_wrk[F_WRK_POINTS_L * F_WRK_POINTS_F];     // �������� ������� ��� (3D)
 float f_tmp[F_TMP_POINTS];    // ������� �������. ��� �� �����������
 _TSTRING name;                // ��� ������ �������������
};

//��������� ����������� ������������� ������ ������ �������� � �������� SECU-3
struct FWMapsDataHolder
{
 SECU3FWMapsItem  maps[TABLES_NUMBER];
 float attenuator_table[KC_ATTENUATOR_LOOKUP_TABLE_SIZE];
 float dwellcntrl_table[COIL_ON_TIME_LOOKUP_TABLE_SIZE];

 std::vector<_TSTRING> GetListOfNames(void)
 {
  std::vector<_TSTRING> list;
  for (int i = 0; i < TABLES_NUMBER; i++)
   list.push_back(maps[i].name);
  return list;
 };
};
