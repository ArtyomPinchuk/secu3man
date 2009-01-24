 /****************************************************************
 *       SECU-3  - An open source, free engine control unit
 *    Designed by Alexey A. Shabelnikov. Ukraine, Gorlovka 2007.
 *       Microprocessors systems - design & programming.
 *    contacts:
 *              http://secu-3.narod.ru
 *              ICQ: 405-791-931
 ****************************************************************/


//������������� ������ ������������ ����� SECU � ����������
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
  float knock_k;                                      //������� ������� ��������� (����������� �� ����� �������� ����)
 };
 
 struct FnNameDat
 {
  unsigned char tables_num;
  unsigned char index;
  char   name[64];
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
  float  dec_spead;
  float  inc_spead;
};


struct FunSetPar
{
  unsigned char fn_benzin;
  unsigned char fn_gas;
  float map_lower_pressure;
  float map_upper_pressure;
  float map_curve_offset;
  float map_curve_gradient;
};

struct IdlRegPar

{
  unsigned char idl_regul;
  float  ifac1;
  float  ifac2;
  int  MINEFR;
  int  idling_rpm;
  float min_angle;
  float max_angle;
};


struct CarburPar
{
  int  ephh_lot;
  int  ephh_hit;
  unsigned char carb_invers;
  float epm_ont; //����� ��������� ���
  int  ephh_lot_g;
  int  ephh_hit_g;
  float shutoff_delay; //�������� ���������� �������
};

struct TemperPar
{
  unsigned char tmp_use;
  float  vent_on;
  float  vent_off;
};

struct ADCCompenPar
{
  float  map_adc_factor;      //����������� ������������ �����������
  float  map_adc_correction;  //����� � �������
  float  ubat_adc_factor;
  float  ubat_adc_correction;
  float  temp_adc_factor;
  float  temp_adc_correction;
};

struct RawSensDat
 {
  float map_value;
  float ubat_value; 
  float temp_value;
  float knock_value;
 };

struct CKPSPar
 {
 unsigned char ckps_edge_type;
 unsigned char ckps_cogs_btdc;
 unsigned char ckps_ignit_cogs;
 };

struct OPCompNc //����������� ���� ���� ������ ������� ��� ������� ������������ ���
{
 unsigned char opcode;
};

#define OPCODE_EEPROM_PARAM_SAVE 1


struct KnockPar
{
 unsigned char knock_use_knock_channel;    
 float knock_bpf_frequency;     
 float knock_k_wnd_begin_angle;        
 float knock_k_wnd_end_angle;          
};


//������� ������������� ���� ������� �� � �������
static float hip9011_gain_frequences[64] = 
{
 01.22f,01.26f,01.31f,01.35f,01.40f,01.45f,01.51f,01.57f,
 01.63f,01.71f,01.78f,01.87f,01.96f,02.07f,02.18f,02.31f,
 02.46f,02.54f,02.62f,02.71f,02.81f,02.92f,03.03f,03.15f,
 03.28f,03.43f,03.59f,03.76f,03.95f,04.16f,04.39f,04.66f,
 04.95f,05.12f,05.29f,05.48f,05.68f,05.90f,06.12f,06.37f,
 06.64f,06.94f,07.27f,07.63f,08.02f,08.46f,08.95f,09.50f,
 10.12f,10.46f,10.83f,11.22f,11.65f,12.10f,12.60f,13.14f,
 13.72f,14.36f,15.07f,15.84f,16.71f,17.67f,18.76f,19.98f
}; 



union SECU3Packet
{
  SECU3IO::SensorDat    m_SensorDat;
  SECU3IO::FnNameDat    m_FnNameDat;
  SECU3IO::StartrPar    m_StartrPar; 
  SECU3IO::AnglesPar    m_AnglesPar;
  SECU3IO::FunSetPar    m_FunSetPar;
  SECU3IO::IdlRegPar    m_IdlRegPar;
  SECU3IO::CarburPar    m_CarburPar;
  SECU3IO::TemperPar    m_TemperPar;
  SECU3IO::ADCCompenPar m_ADCCompenPar;
  SECU3IO::RawSensDat   m_RawSensDat;
  SECU3IO::CKPSPar      m_CKPSPar;
  SECU3IO::OPCompNc     m_OPCompNc;
  SECU3IO::KnockPar     m_KnockPar;
};


};
