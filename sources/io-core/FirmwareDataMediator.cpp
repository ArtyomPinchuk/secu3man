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


//��������� ��������� �������
typedef struct
{
  _uchar tmp_use;                        //������� ������������ ����-��
  _uchar carb_invers;                    //�������� ��������� �� �����������
  _uchar idl_regul;                      //������������ �������� ������� �� �������������� ���
  _uchar fn_benzin;                      //����� ������ ������������� ������������ ��� �������
  _uchar fn_gas;                         //����� ������ ������������� ������������ ��� ����
  _uint  map_lower_pressure;             //������ ������� ��� �� ��� �������(���)
  _uint  ephh_lot;                       //������ ����� ���� (���-1)
  _uint  ephh_hit;                       //������� ����� ���� (���-1)
  _uint  starter_off;                    //����� ���������� �������� (���-1)
  _int   map_upper_pressure;             //������� ������� ��� �� ��� ������� (���)
  _uint  smap_abandon;                   //������� �������� � �������� ����� �� �������  (���-1) 
  _int   max_angle;                      //����������� ������������� ���
  _int   min_angle;                      //����������� ������������ ���
  _int   angle_corr;                     //�����-��������� ���    
  _uint  idling_rpm;                     //�������� ������� �� ��� ����������� �������������� ���   
  _int   ifac1;                          //������������ �-���������� �������� ��, ��� ������������� �
  _int   ifac2;                          //������������� ������ ��������������.
  _int   MINEFR;                         //���� ������������������ ���������� (�������)
  _int   vent_on;                        //����������� ��������� �����������
  _int   vent_off;                       //����������� ���������� �����������  

  _int  map_adc_factor;
  _long map_adc_correction;
  _int  ubat_adc_factor;
  _long ubat_adc_correction;
  _int  temp_adc_factor;
  _long temp_adc_correction;
  
  _uchar ckps_edge_type;                
  _uchar ckps_cogs_btdc;
  _uchar ckps_ignit_cogs;

  //��� ����������������� ����� ���������� ��� ���������� �������� �������������
  //����� ������ �������� � ����� ������� ��������. ��� ���������� ����� ������
  //� ���������, ���������� ����������� ��� �����.
  _uchar reserved[24];

  _ushort crc;                           //����������� ����� ������ ���� ��������� (��� �������� ������������ ������ ����� ���������� �� EEPROM)  
}params;


#define FW_SIGNATURE_INFO_SIZE 48

//��������� �������������� ������ �������� � ��������
typedef struct
{
  _uchar fw_signature_info[FW_SIGNATURE_INFO_SIZE];

  //��� ����������������� ����� ���������� ��� ���������� �������� �������������
  //����� ������ �������� � ����� ������� ��������. ��� ���������� ����� ������
  //� ���������, ���������� ����������� ��� �����.
  _uchar reserved[128];  
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

//---��� ������ ����������� ���������� ��� ��������/������� ������ ����� ����������--
//��������� ����� ���� ������ �������� � ������
#define FIRMWARE_ALL_DATA_START (FIRMWARE_DATA_START)
//������ ���� ������ �������� ��� ����� ������ ����������� ����� ��������
#define FIRMWARE_ALL_DATA_SIZE  ( (sizeof(F_data)*TABLES_NUMBER) + sizeof(params) + sizeof(FirmwareData))  



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


void CFirmwareDataMediator::LoadBytes(const BYTE* i_bytes)
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
  /*CharToOem(raw_string,string);*/
  OemToChar(raw_string,string);
  return _TSTRING(string);
}

void CFirmwareDataMediator::SetSignatureInfo(_TSTRING i_string)
{
  char raw_string[256];
  memset(raw_string,0,FW_SIGNATURE_INFO_SIZE+1);
  BYTE* addr = &m_bytes_active[FIRMWARE_DATA_START];
  /*OemToChar(i_string.c_str(),raw_string);*/
  CharToOem(i_string.c_str(),raw_string);
  memcpy(addr,raw_string,FW_SIGNATURE_INFO_SIZE);    
}


void CFirmwareDataMediator::GetStartMap(int i_index,float* o_values, bool i_original /* = false */)
{
  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL;
  ASSERT(o_values);

  if (i_original)	  
	p_bytes = m_bytes_original;
  else
    p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 
   
  
  for (int i = 0; i < F_STR_POINTS; i++ )
    o_values[i] = ((float)p_maps[i_index].f_str[i]) / 2.0f;

}

void CFirmwareDataMediator::SetStartMap(int i_index,float* i_values)
{
  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL;
  ASSERT(i_values);

  p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 
   
  
  for (int i = 0; i < F_STR_POINTS; i++ )
	p_maps[i_index].f_str[i] = CNumericConv::Round((i_values[i]*2.0f));
}

void CFirmwareDataMediator::GetIdleMap(int i_index,float* o_values, bool i_original /* = false */)
{
  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL;
  ASSERT(o_values);

  if (i_original)	  
	p_bytes = m_bytes_original;
  else
    p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 
   
  
  for (int i = 0; i < F_IDL_POINTS; i++ )
    o_values[i] = ((float)p_maps[i_index].f_idl[i]) / 2.0f;

}

void CFirmwareDataMediator::SetIdleMap(int i_index,float* i_values)
{
  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL;
  ASSERT(i_values);

  p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 
   
  
  for (int i = 0; i < F_IDL_POINTS; i++ )
	p_maps[i_index].f_idl[i] = CNumericConv::Round((i_values[i]*2.0f));
}

std::vector<_TSTRING> CFirmwareDataMediator::GetFunctionsSetNames(void)
{
  std::vector<_TSTRING> names(TABLES_NUMBER);

  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL;  

  p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 

  for (int i = 0; i < TABLES_NUMBER; i++)
  {         
    char raw_string[256];
    memset(raw_string,0,F_NAME_SIZE+1);
    BYTE* addr = p_maps[i].name; 
    memcpy(raw_string,addr,F_NAME_SIZE);  
    TCHAR string[256];
    OemToChar(raw_string,string);
	names[i] = string;
  }

return names;
}


void CFirmwareDataMediator::SetFunctionsSetName(int i_index, _TSTRING i_new_name)
{
  if (i_index >= TABLES_NUMBER)       
    return;

  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL; 

  p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 
 

  char raw_string[256];
  memset(raw_string,0,FW_SIGNATURE_INFO_SIZE+1);  
  CharToOem(i_new_name.c_str(),raw_string);
  memcpy(p_maps[i_index].name,raw_string,F_NAME_SIZE);     
}

void CFirmwareDataMediator::SetFWFileName(const _TSTRING i_fw_file_name)
{
  m_fw_file_name = i_fw_file_name;
}

_TSTRING CFirmwareDataMediator::GetFWFileName(void)
{
  return m_fw_file_name;
}


unsigned int CFirmwareDataMediator::CalculateCRC16OfActiveFirmware(void)
{
  unsigned short crc_16 = crc16(m_bytes_active,CODE_SIZE);
  return crc_16;
}

unsigned int CFirmwareDataMediator::GetCRC16StoredInActiveFirmware(void)
{
 unsigned short* crc_16_addr = (unsigned short*)(&m_bytes_active[CODE_CRC_ADDR]);
 return *crc_16_addr;
}

void CFirmwareDataMediator::CalculateAndPlaceFirmwareCRC(void)
{
  CalculateAndPlaceFirmwareCRC(m_bytes_active);  
}

void CFirmwareDataMediator::LoadDataBytesFromAnotherFirmware(const BYTE* i_source_bytes)
{
 if (false==IsLoaded())
   return; //������ ���������...
 memcpy(m_bytes_active + FIRMWARE_ALL_DATA_START,i_source_bytes + FIRMWARE_ALL_DATA_START,FIRMWARE_ALL_DATA_SIZE);
 /*???memcpy(m_bytes_original + FIRMWARE_ALL_DATA_START,i_source_bytes + FIRMWARE_ALL_DATA_START,FIRMWARE_ALL_DATA_SIZE);*/
}

void CFirmwareDataMediator::GetWorkMap(int i_index, float* o_values, bool i_original /* = false*/)
{
  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL;
  ASSERT(o_values);

  if (i_original)	  
	p_bytes = m_bytes_original;
  else
    p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 
   
  
  for (int i = 0; i < (F_WRK_POINTS_F * F_WRK_POINTS_L); i++ )
  {
	_char *p = &(p_maps[i_index].f_wrk[0][0]);
	o_values[i] = ((float) *(p + i)) / 2.0f;
  }
}

void CFirmwareDataMediator::SetWorkMap(int i_index, float* i_values)
{
  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL;
  ASSERT(i_values);

  p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 
   
  
  for (int i = 0; i < (F_WRK_POINTS_F * F_WRK_POINTS_L); i++ )
  {
    _char *p = &(p_maps[i_index].f_wrk[0][0]);
	*(p + i) = CNumericConv::Round((i_values[i]*2.0f));
  }	
}

void CFirmwareDataMediator::GetTempMap(int i_index,float* o_values, bool i_original /* = false */)
{
  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL;
  ASSERT(o_values);

  if (i_original)	  
	p_bytes = m_bytes_original;
  else
    p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 
   
  
  for (int i = 0; i < F_TMP_POINTS; i++ )
    o_values[i] = ((float)p_maps[i_index].f_tmp[i]) / 2.0f;

}

void CFirmwareDataMediator::SetTempMap(int i_index,float* i_values)
{
  BYTE* p_bytes = NULL;
  F_data* p_maps = NULL;
  ASSERT(i_values);

  p_bytes = m_bytes_active; 

  //�������� ����� ������ ������ �������� �������������
  p_maps = (F_data*)(p_bytes + TABLES_START); 
   
  for (int i = 0; i < F_TMP_POINTS; i++ )
	p_maps[i_index].f_tmp[i] = CNumericConv::Round((i_values[i]*2.0f));
}
