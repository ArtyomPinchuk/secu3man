#include "stdafx.h"
#include "FirmwareDataMediator.h"
#include "CRC16.h"
#include "BootLoader.h"


typedef unsigned short _uint;
typedef signed short   _int;
typedef unsigned short _ushort;
typedef signed long    _long;
typedef signed char    _char;
typedef unsigned char  _uchar;


//���� ���� �������� ������������ ��������������� ��������� ����
#pragma pack(1)

//���������� ���������� ����� ������������ ��� ������ �������
#define F_WRK_POINTS_F         16  
#define F_WRK_POINTS_L         16  
#define F_TMP_POINTS           16
#define F_STR_POINTS           16                            
#define F_IDL_POINTS           16     
#define F_NAME_SIZE            16

typedef struct 
{
  _char f_str[F_STR_POINTS];                       // ������� ��� �� ������
  _char f_idl[F_IDL_POINTS];                       // ������� ��� ��� ��
  _char f_wrk[F_WRK_POINTS_L][F_WRK_POINTS_F];     // �������� ������� ��� (3D)
  _char f_tmp[F_TMP_POINTS];                       // ������� �������. ��� �� �����������
  _uchar name[F_NAME_SIZE];                         // ��������������� ��� (��� ���������)
}F_data;


#pragma pack(1)
//��������� ��������� �������
typedef struct
{
  _uchar tmp_use;                        //������� ������������ ����-��
  _uchar carb_invers;                    //�������� ��������� �� �����������
  _uchar idl_regul;                      //������������ �������� ������� �� �������������� ���
  _uchar fn_benzin;                      //����� ������ ������������� ������������ ��� �������
  _uchar fn_gas;                         //����� ������ ������������� ������������ ��� ����
  _uint  map_grad;                       //������ ����� ������� ������� (���)
  _uint  ephh_lot;                       //������ ����� ���� (���-1)
  _uint  ephh_hit;                       //������� ����� ���� (���-1)
  _uint  starter_off;                    //����� ���������� �������� (���-1)
  _int  press_swing;                    //������� �������� ��� ��������� �������� ��������   (���)
  _uint  smap_abandon;                   //������� �������� � �������� ����� �� �������  (���-1) 
  _int  max_angle;                      //����������� ������������� ���
  _int  min_angle;                      //����������� ������������ ���
  _int  angle_corr;                     //�����-��������� ���    
  _uint  idl_turns;                      //�������� ������� �� ��� ����������� �������������� ���   
  _int  ifac1;                          //������������ �-���������� �������� ��, ��� ������������� �
  _int  ifac2;                          //������������� ������ ��������������, 1...100 
  _int  MINEFR;                         //���� ������������������ ���������� (�������)
  _int  vent_on;                        //����������� ��������� �����������
  _int  vent_off;                       //����������� ���������� �����������  

  _int  map_adc_factor;
  _long map_adc_correction;
  _int  ubat_adc_factor;
  _long ubat_adc_correction;
  _int  temp_adc_factor;
  _long temp_adc_correction;
  
  _ushort crc;                           //����������� ����� ������ ���� ��������� (��� �������� ������������ ������ ����� ���������� �� EEPROM)  
}params;


#define FW_SIGNATURE_INFO_SIZE 48

#pragma pack(1)
//��������� �������������� ������ �������� � ��������
typedef struct
{
  _uchar fw_signature_info[FW_SIGNATURE_INFO_SIZE];
}FirmwareData;


#define BOOT_START 0x3E00
#define CODE_CRC_ADDR (BOOT_START-sizeof(unsigned short))
#define CODE_SIZE (BOOT_START-sizeof(unsigned short)) //������ ���� �������� ��� ����� ������ ����������� �����

//���������� ������� ������������� �������� � ������ ��������
#define TABLES_NUMBER          8   

//����� ������� ������ - �������� �������������
#define TABLES_START (CODE_CRC_ADDR-(sizeof(F_data)*TABLES_NUMBER))

//����� ��������� ���������� ���������� (���������� EEPROM �� ���������)
#define DEFPARAM_START (TABLES_START-sizeof(params))

//����� �������������� ����������
#define FIRMWARE_DATA_START (DEFPARAM_START-sizeof(FirmwareData))


CFirmwareDataMediator::CFirmwareDataMediator()
: m_firmware_size(CBootLoader::FLASH_TOTAL_SIZE)
, m_is_opened(false)
{
  memset(m_bytes_active,0x00,m_firmware_size);
  memset(m_bytes_original,0x00,m_firmware_size);
}

CFirmwareDataMediator::~CFirmwareDataMediator()
{
}

//������� ����������� ����� � ���������� ��������� �� ���������������� ������
//io_data - ������ ������ �������� 
void CFirmwareDataMediator::CalculateAndPlaceFirmwareCRC(BYTE* io_data)
{
  unsigned short crc = crc16(io_data,CODE_SIZE);
  unsigned short* crc_addr = (unsigned short*)(&io_data[CODE_CRC_ADDR]);
  *crc_addr = crc; //��������� ����������� �����
}


void CFirmwareDataMediator::LoadBytes(BYTE* i_bytes)
{
  ASSERT(i_bytes);
  memset(m_bytes_active,0x00,m_firmware_size);
  memset(m_bytes_original,0x00,m_firmware_size);

  memcpy(m_bytes_active,i_bytes,m_firmware_size);
  memcpy(m_bytes_original,i_bytes,m_firmware_size);  

  m_is_opened = true;
}
	
void CFirmwareDataMediator::StoreBytes(BYTE* o_bytes)
{
  ASSERT(o_bytes);
  memcpy(o_bytes,m_bytes_active,m_firmware_size);
}

bool CFirmwareDataMediator::IsModified(void)
{
  return (0 != memcmp(m_bytes_active,m_bytes_original,m_firmware_size));
}

void CFirmwareDataMediator::ResetModified(void)
{
  memcpy(m_bytes_original,m_bytes_active,m_firmware_size);
}

_TSTRING CFirmwareDataMediator::GetSignatureInfo(void)
{
  char raw_string[256];
  memset(raw_string,0,FW_SIGNATURE_INFO_SIZE+1);
  BYTE* addr = &m_bytes_active[FIRMWARE_DATA_START];
  memcpy(raw_string,addr,FW_SIGNATURE_INFO_SIZE);  
  TCHAR string[256];
  CharToOem(raw_string,string);
  return _TSTRING(string);
}

void CFirmwareDataMediator::SetSignatureInfo(_TSTRING i_string)
{
  char raw_string[256];
  memset(raw_string,0,FW_SIGNATURE_INFO_SIZE+1);
  BYTE* addr = &m_bytes_active[FIRMWARE_DATA_START];
  OemToChar(i_string.c_str(),raw_string);
  memcpy(addr,raw_string,FW_SIGNATURE_INFO_SIZE);    
}
