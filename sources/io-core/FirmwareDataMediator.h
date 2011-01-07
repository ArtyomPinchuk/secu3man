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

#include "iocore_api.h"
#include "common/unicodesupport.h"
#include "PlatformParamHolder.h"
#include <vector>

//���� ����� ��������� ��� ���������� ��� �������� �������� ������ � �������� 
//(������ �������� ����������������), � ����� ��� �������� ���� ������.

class IOCORE_API CFirmwareDataMediator
{
 public:
  CFirmwareDataMediator(const PPFlashParam& i_fpp);
 ~CFirmwareDataMediator();

  //�������� ������ �������� �� ���������� ������
  void LoadBytes(const BYTE* i_bytes);
  void LoadDataBytesFromAnotherFirmware(const BYTE* i_source_bytes);
  void LoadDefParametersFromBuffer(const BYTE* i_source_bytes);
	
  //���������� ������ �������� � ��������� �����
  void StoreBytes(BYTE* o_bytes);

  //���� �� �������� �������� ����� ��������� �������� 
  bool IsModified(void);

  //���������� true ���� �������� ��������� (���� ��� �������������)
  bool IsLoaded(void) { return m_is_opened;}

  //�������� ��� ������� ��� ������ �������� �����������, ������
  //����� ���� ��� ������ ���� ������� ���������.
  void ResetModified(void);

  void CalculateAndPlaceFirmwareCRC(BYTE* io_data);

  //checks compatibility of specified firmware with this version of management software
  //returns false if incompatible
  bool CheckCompatibility(const BYTE* i_data) const;

	//-----------------------------------------------------------------------
  _TSTRING GetSignatureInfo(void);
  void SetSignatureInfo(_TSTRING i_string);
  
  void GetStartMap(int i_index, float* o_values, bool i_original = false);
  void SetStartMap(int i_index,const float* i_values);	
  void GetIdleMap(int i_index,  float* o_values, bool i_original = false);
  void SetIdleMap(int i_index, const float* i_values);	
  void GetWorkMap(int i_index, float* o_values, bool i_original = false);
  void SetWorkMap(int i_index, const float* i_values);	
  void GetTempMap(int i_index, float* o_values, bool i_original = false);
  void SetTempMap(int i_index, const float* i_values);	

  void GetMapsData(struct FWMapsDataHolder* op_fwd);
  void SetMapsData(const struct FWMapsDataHolder* ip_fwd);

  std::vector<_TSTRING> GetFunctionsSetNames(void);
  void SetFunctionsSetName(int i_index, _TSTRING i_new_name);

  void SetFWFileName(const _TSTRING i_fw_file_name);
  _TSTRING GetFWFileName(void);

  void GetAttenuatorMap(float* o_values, bool i_original = false);
  void SetAttenuatorMap(const float* i_values);

  void GetCoilRegulMap(float* o_values, bool i_original = false);
  void SetCoilRegulMap(const float* i_values);

  unsigned int CalculateCRC16OfActiveFirmware(void);
  unsigned int GetCRC16StoredInActiveFirmware(void);
  void CalculateAndPlaceFirmwareCRC(void);

  //-----------------------------------------------------------------------

  bool SetDefParamValues(BYTE i_descriptor, const void* i_values);   //������� ��������� �������.�����. � ��������
  bool GetDefParamValues(BYTE i_descriptor, void* o_values);         //������� ��������� �������.�����. �� ��������

  //-----------------------------------------------------------------------

 private:
  PPFlashParam m_fpp;
  const size_t m_firmware_size;
  BYTE* m_bytes_active;   //����� �������� (����� ��� �����������)
  BYTE* m_bytes_original; //����� �������� (����� ��� ���������) 
  bool m_is_opened;
  _TSTRING m_fw_file_name;
};
