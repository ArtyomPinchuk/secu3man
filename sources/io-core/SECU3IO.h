 /****************************************************************
 *       SECU-3  - An open source, free engine control unit
 *    Designed by Alexey A. Shabelnikov. Ukraine, Gorlovka 2007.
 *       Microprocessors systems - design & programming.
 *    contacts:
 *              http://secu-3.narod.ru
 *              ICQ: 405-791-931
 ****************************************************************/


//������������� ������ ������������ ����� SECU � ����������
#include <string>

#pragma once

namespace SECU3IO
{
//��������� ��� ����� ������� - �� ����������� � ������������ ��������
 struct SensorDat
 {
  int frequen;                                        //������� �������� ��������� (�����������)
  float pressure;                                     //�������� �� �������� ���������� (�����������)
  float voltage;                                      //���������� �������� ���� (�����������)
  float temperat;                                     //����������� ����������� �������� (�����������)
  float adv_angle;                                    //������� ���
  unsigned char carb;                                 //��������� ��������� ����������� 
  unsigned char gas;                                  //��������� �������� ������� 
  unsigned char air_flow;                             //������ �������
  unsigned char ephh_valve;                           //��������� ������� ����
 };
 
 struct FnNameDat
 {
  unsigned char tables_num;
  unsigned char index;
  std::string   name;
 };

 struct StartrPar
 {
   int  starter_off;                                //����� ���������� �������� (�������)
   int  smap_abandon;                               //������� �������� � �������� ����� �� �������   
 };
 
struct AnglesPar
{
  float  max_angle;
  float  min_angle;
  float  angle_corr;
};


struct FunSetPar
{
  unsigned char fn_benzin;
  unsigned char fn_gas;
  float map_grad;
  float press_swing;
};

struct IdlRegPar

{
  unsigned char idl_regul;
  float  ifac1;
  float  ifac2;
  int  MINEFR;
  int  idl_turns;
};


struct CarburPar
{
  int  ephh_lot;
  int  ephh_hit;
  unsigned char carb_invers;
};

struct TemperPar
{
  unsigned char tmp_use;
  float  vent_on;
  float  vent_off;
};

};