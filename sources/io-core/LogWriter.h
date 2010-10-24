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

#include "ControlAppAdapter.h"
#include "common/unicodesupport.h"


class AFX_EXT_CLASS LogWriter : public IAPPEventHandler
{
 public:
  LogWriter();
  virtual ~LogWriter();

  //���������� �������� ������. ���� ��������� ������ ��������, �� ���������� false.
  //i_folder - ������ ���� � �������� ��� ����� �������� ���-����.
  //o_full_file_name - ���������� ������� ������ ��� ����� � ������� �������� ������
  bool BeginLogging(const _TSTRING& i_folder, _TSTRING* o_full_file_name = NULL);

  //������������ �������� ������
  void EndLogging(void);

  //���������� true, ���� � ������� ������ ���� ������ 
  bool IsLoggingInProcess(void);

  //Separating symbol for CSV 
  void SetSeparatingSymbol(char i_sep_symbol);

 public:

  //����� ��� ����������� ������ ��������� � �������� ������
  virtual void OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet);          
  virtual void OnConnection(const bool i_online);

 private:

  //����� ����� � ������� ���� ������
  FILE* m_out_handle;

  //true - ���� ������� ������, false - ������ ����, � ������ ����. 
  bool  m_is_busy;

  char m_csv_separating_symbol;
  char m_csv_data_template[1024];
};
