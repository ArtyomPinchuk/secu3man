

#pragma once

#include "BootLoader.h"
#include "common/unicodesupport.h"
#include <vector>

//���� ����� ��������� ��� ���������� ��� �������� �������� ������ � �������� 
//(������ �������� ����������������), � ��� �� ��� �������� ���� ������.

class AFX_EXT_CLASS CFirmwareDataMediator
{
  public:
     CFirmwareDataMediator();
	~CFirmwareDataMediator();

	//�������� ������ �������� �� ���������� ������
	void LoadBytes(const BYTE* i_bytes);
	void LoadDataBytesFromAnotherFirmware(const BYTE* i_source_bytes);
	
	//���������� ������ �������� � ��������� �����
	void StoreBytes(BYTE* o_bytes);

	//���� �� �������� �������� ����� ��������� �������� 
	bool IsModified(void);

	//���������� true ���� �������� ��������� (���� ��� �������������)
	bool IsLoaded(void) { return m_is_opened;}

	//�������� ��� ������� ��� ������ �������� �����������, ������
	//����� ���� ��� ������ ���� ������� ���������.
	void ResetModified(void);

    static void CalculateAndPlaceFirmwareCRC(BYTE* io_data);

	//-----------------------------------------------------------------------
    _TSTRING GetSignatureInfo(void);
	void SetSignatureInfo(_TSTRING i_string);
  
    void GetStartMap(int i_index, float* o_values, bool i_original = false);
	void SetStartMap(int i_index, float* i_values);	
    void GetIdleMap(int i_index, float* o_values, bool i_original = false);
	void SetIdleMap(int i_index, float* i_values);	
    void GetWorkMap(int i_index, float* o_values, bool i_original = false);
	void SetWorkMap(int i_index, float* i_values);	
    void GetTempMap(int i_index, float* o_values, bool i_original = false);
	void SetTempMap(int i_index, float* i_values);	


	std::vector<_TSTRING> GetFunctionsSetNames(void);
	void SetFunctionsSetName(int i_index, _TSTRING i_new_name);

	void SetFWFileName(const _TSTRING i_fw_file_name);
	_TSTRING GetFWFileName(void);

    unsigned int CalculateCRC16OfActiveFirmware(void);
	unsigned int GetCRC16StoredInActiveFirmware(void);
	void CalculateAndPlaceFirmwareCRC(void);

	//-----------------------------------------------------------------------


  private:
    const size_t m_firmware_size;
	BYTE m_bytes_active[CBootLoader::FLASH_TOTAL_SIZE + 1]; //����� �������� (����� ��� �����������)
	BYTE m_bytes_original[CBootLoader::FLASH_TOTAL_SIZE + 1]; //����� �������� (����� ��� ���������) 
	bool m_is_opened;
	_TSTRING m_fw_file_name;
};