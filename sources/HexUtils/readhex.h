
#ifdef HEXUTILS_EXPORTS
#define HEXUTILS_API __declspec(dllexport)
#else
#define HEXUTILS_API __declspec(dllimport)
#endif


enum EReadHexStatus
{
 RH_SUCCESS = 0,
 RH_INCORRECT_CHKSUM = 1,
 RH_UNEXPECTED_SYMBOL = 2,
};

// Intel HEX 80 �� �������� ������� ������ 0xFFFF ������� ���������������
// ��� �������� ����� ����� ������ �� ����� 0xFFFF
//
// ��������� ������ �� ������� Intel HEX 80 � ��������

//ip_buff - ������� ����� ���������� ������ HEX-�����
//i_size - ������ �������� ������ � ��������
//op_buff - �������� ����� � ������� ����� �������� ��������������� �������� ������
//o_size - ������ ��������� ������
//���������� ��������� ������������ �� ������� ������� ���� ����������� ��������������.
HEXUTILS_API EReadHexStatus HexUtils_ConvertHexToBin(BYTE* ip_buff, size_t i_size, BYTE* op_buff, size_t& o_size);
