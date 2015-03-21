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

/** \file LogReader.h
 * \author Alexey A. Shabelnikov
 */

#pragma once

#include "common/unicodesupport.h"
#include "iocore_api.h"
#include "SECU3IO.h"

//��� ������ � ����� ������ ����� ���������� �����!
class IOCORE_API LogReader
{
 public:
  LogReader();
 ~LogReader();

  enum FileError
  {
   FE_OPEN,
   FE_FORMAT,
   FE_NA
  };

  //��������� ����
  bool OpenFile(const _TSTRING& i_file_name, FileError& o_error);

  //��������� ����
  bool CloseFile(void);

  //���������� true ���� � ������ ������ ������ ����
  bool IsOpened(void) const;

  //�������� ������ �� ������� ������
  bool GetRecord(SYSTEMTIME& o_time, SECU3IO::SensorDat& o_data, int& o_marks);

  //����������� � ��������� ������
  //���������� false ���� ��������� ����� �������
  bool Next(void);

  //����������� � ���������� ������
  //���������� false ���� ���������� ������ �������
  bool Prev(void);

  //���������� ���-�� ������� � �����
  unsigned long GetCount(void) const;

  //Separating symbol for CSV
  void SetSeparatingSymbol(char i_sep_symbol);

  //�������� �� ������/�����
  bool IsNextPossible(void) const;
  bool IsPrevPossible(void) const;

  //���������� ������� �������
  unsigned long GetCurPos(void) const;

 private:

  _TSTRING m_file_name;         //��� ��������� �����
  unsigned long m_record_count; //���-�� ������� � �����
  FILE* m_file_handle;          //��������� �� ����

  //����� ������� ������
  unsigned long m_current_record;
  //���-�� �������� � ����� ������, ������� ������� �������� ������
  unsigned long m_record_size;

  char m_csv_separating_symbol;
  char m_csv_data_template[1024];
};
