

#pragma once

#include "BootLoader.h"
#include "common/unicodesupport.h"

//���� ����� ��������� ��� ���������� ��� �������� �������� ������ � �������� 
//(������ �������� ����������������), � ��� �� ��� �������� ���� ������.

class AFX_EXT_CLASS CFirmwareDataMediator
{
  public:
     CFirmwareDataMediator();
	~CFirmwareDataMediator();

	//�������� ������ �������� �� ���������� ������
	void LoadBytes(BYTE* i_bytes);
	
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

    _TSTRING GetSignatureInfo(void);
	void SetSignatureInfo(_TSTRING i_string);
  

  private:
    const size_t m_firmware_size;
	BYTE m_bytes_active[CBootLoader::FLASH_TOTAL_SIZE + 1]; //����� �������� (����� ��� �����������)
	BYTE m_bytes_original[CBootLoader::FLASH_TOTAL_SIZE + 1]; //����� �������� (����� ��� ���������) 
	bool m_is_opened;
};