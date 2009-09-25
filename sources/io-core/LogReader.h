
#pragma once

#include "common\unicodesupport.h"
#include "SECU3IO.h"

//��� ������ � ����� ������ ����� ���������� �����!
class AFX_EXT_CLASS LogReader
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
	bool GetRecord(SYSTEMTIME& o_time, SECU3IO::SensorDat *op_data);

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