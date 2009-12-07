
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

//���������� ������� ������������� �������� � ������ ��������
#define TABLES_NUMBER          8   

struct SECU3FWMapsItem
{
  float f_str[F_STR_POINTS];          // ������� ��� �� ������
  float f_idl[F_IDL_POINTS];          // ������� ��� ��� ��
  float f_wrk[F_WRK_POINTS_L * F_WRK_POINTS_F];     // �������� ������� ��� (3D)
  float f_tmp[F_TMP_POINTS];  // ������� �������. ��� �� �����������
  _TSTRING name;              // ��� ������ �������������
};

//��������� ����������� ������������� ������ ������ �������� � �������� SECU-3
struct FWMapsDataHolder
{
  SECU3FWMapsItem  maps[TABLES_NUMBER];

  std::vector<_TSTRING> GetListOfNames(void)
  {
   std::vector<_TSTRING> list;
   for (int i = 0; i < TABLES_NUMBER; i++)
    list.push_back(maps[i].name);
   return list;
  };
};
