/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Kiev

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
              http://secu-3.org
              email: shabelnikov@secu-3.org
*/

//������������� ������ ������������ ����� SECU � ����������. ��������� ������ ������������ ��� ��������
//� ���������� ��������� - ��� ��������������
#pragma once

#include <utility>

namespace SECU3IO
{
 //��������� ��� ����� ������� - �� ����������� � ������������ ��������
 struct SensorDat
 {
  int frequen;                          //������� �������� ��������� (�����������)
  float pressure;                       //�������� �� �������� ���������� (�����������)
  float voltage;                        //���������� �������� ���� (�����������)
  float temperat;                       //����������� ����������� �������� (�����������)
  float adv_angle;                      //������� ���
  unsigned char carb;                   //��������� ��������� �����������
  unsigned char gas;                    //��������� �������� �������
  unsigned char air_flow;               //������ �������
  unsigned char ephh_valve;             //��������� ������� ����
  unsigned char epm_valve;              //��������� ������� ���
  unsigned char cool_fan;               //��������� ����������� (�������/��������)
  unsigned char st_block;               //��������� ������ ���������� ��������
  float knock_k;                        //������� ������� ��������� (����������� �� ����� �������� ����)
  float knock_retard;                   //������������� ��� ��� ���������
  unsigned char ce_state;               // !currently is not used!
  WORD ce_errors;                       //���� ������ ��
  float tps;                            //���� (TPS sensor value)
  float add_i1;                         //ADD_I1 voltage
  float add_i2;                         //ADD_I2 voltage
  float choke_pos;                      //choke position (%)
  float speed;                          //vehicle speed (Km/h)
  float distance;                       //passed distance since last ignition turn on (meters)
  float air_temp;                       //intake air temperature
  bool  add_i2_mode;                    //0 - show voltage, 1 - show IAT

  bool  knkret_use;
  float strt_aalt;                      //advance angle from start map
  bool  strt_use;
  float idle_aalt;                      //advance angle from idle map
  bool  idle_use;
  float work_aalt;                      //advance angle from work map
  bool  work_use;
  float temp_aalt;                      //advance angle from coolant temperature correction map
  bool  temp_use;
  float airt_aalt;                      //advance angle from air temperature correction map
  bool  airt_use;
  float idlreg_aac;                     //advance angle correction from idling RPM regulator
  bool idlreg_use;
  float octan_aac;                      //octane correction value
  bool octan_use;
  float lambda_corr;                    //lambda correction in %

  float inj_pw;                         //injector pulse width in ms
 };

 struct DbgvarDat
 {
  int var1;                             //���������� 1
  int var2;                             //���������� 2
  int var3;                             //���������� 3
  int var4;                             //���������� 4
 };

 struct FnNameDat
 {
  unsigned char tables_num;
  unsigned char index;
  char   name[64];
 };

 struct StartrPar
 {
  int  starter_off;                     //����� ���������� �������� (�������)
  int  smap_abandon;                    //������� �������� � �������� ����� �� �������
  float inj_cranktorun_time;            //sec.
  int   inj_aftstr_strokes;             //strokes
  float inj_prime_cold;                 //prime pulse width at -30C
  float inj_prime_hot;                  //prime pulse width at 70C
  float inj_prime_delay;                //prime pulse delay
 };

 struct AnglesPar
 {
  float  max_angle;
  float  min_angle;
  float  angle_corr;
  float  dec_spead;
  float  inc_spead;
  unsigned char zero_adv_ang;
 };


 struct FunSetPar
 {
  unsigned char fn_benzin;
  unsigned char fn_gas;
  float map_lower_pressure;
  float map_upper_pressure;
  float map_curve_offset;
  float map_curve_gradient;
  float tps_curve_offset;
  float tps_curve_gradient;
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
  float turn_on_temp;                   //idling regulator turn on temperature
 };

 struct CarburPar
 {
  int  ephh_lot;
  int  ephh_hit;
  unsigned char carb_invers;
  float epm_ont;                        //����� ��������� ���
  int  ephh_lot_g;
  int  ephh_hit_g;
  float shutoff_delay;                  //�������� ���������� �������
  float tps_threshold;                  //����� ������������ � ����� �� �� ����
  float fuelcut_map_thrd;               //fuel cut MAP threshold 
  float fuelcut_cts_thrd;               //fuel cut CTS threshold
 };

 struct TemperPar
 {
  unsigned char tmp_use;
  unsigned char vent_pwm;
  unsigned char cts_use_map;
  float  vent_on;
  float  vent_off;
 };

 struct ADCCompenPar
 {
  float  map_adc_factor;                //����������� ������������ �����������
  float  map_adc_correction;            //����� � �������
  float  ubat_adc_factor;
  float  ubat_adc_correction;
  float  temp_adc_factor;
  float  temp_adc_correction;
  float  tps_adc_factor;
  float  tps_adc_correction;
  float  ai1_adc_factor;
  float  ai1_adc_correction;
  float  ai2_adc_factor;
  float  ai2_adc_correction;
 };

 struct RawSensDat
 {
  float map_value;
  float ubat_value;
  float temp_value;
  float knock_value;
  float tps_value;
  float add_i1_value;
  float add_i2_value;
 };

 struct CKPSPar
 {
  unsigned char ckps_edge_type;
  unsigned char ckps_cogs_btdc;
  unsigned char ckps_ignit_cogs;
  unsigned char ckps_engine_cyl;
  unsigned char ckps_merge_ign_outs;
  unsigned char ckps_cogs_num;
  unsigned char ckps_miss_num;
  unsigned char ref_s_edge_type;
  unsigned char use_ckps_for_hall;      //Use CKPS input for Hall sensor
  float hall_wnd_width; 
 };

 struct OPCompNc                        //������������ ���� ���� ������ ������� ��� ������� ������������ ��� ��������
 {
  BYTE opcode;   //operation code
  BYTE opdata;   //operation data
 };

 const int OPCODE_EEPROM_PARAM_SAVE    = 1;
 const int OPCODE_CE_SAVE_ERRORS       = 2;
 const int OPCODE_READ_FW_SIG_INFO     = 3;
 const int OPCODE_LOAD_TABLSET         = 4;  //realtime tables
 const int OPCODE_SAVE_TABLSET         = 5;  //realtime tables
 const int OPCODE_DIAGNOST_ENTER       = 6;  //enter diagnostic mode
 const int OPCODE_DIAGNOST_LEAVE       = 7;  //leave diagnostic mode
 const int OPCODE_RESET_EEPROM         = 0xCF;//reset EEPROM

 struct KnockPar
 {
  unsigned char knock_use_knock_channel;
  unsigned char knock_bpf_frequency;
  float knock_k_wnd_begin_angle;
  float knock_k_wnd_end_angle;
  unsigned char knock_int_time_const;

  float knock_retard_step;              //��� �������� ��� ��� ���������
  float knock_advance_step;             //��� �������������� ���
  float knock_max_retard;               //������������ �������� ���
  float knock_threshold;                //� �������
  int knock_recovery_delay;             //� ������� ������ ���������
 };

 struct CEErrors
 {
  DWORD flags;
 };

 const int FW_SIGNATURE_INFO_SIZE = 48;
 struct FWInfoDat
 {
  char   info[FW_SIGNATURE_INFO_SIZE+1];//*one more byte for string's termination
  DWORD  options;
  BYTE   fw_version[2];                 //index: 0 - minor, 1 - major
 };

 struct MiscelPar
 {
  int baud_rate;                        //�������� UART-a
  int period_ms;                        //������ ������� ������� ��.
  unsigned char ign_cutoff;             //������� ������������� ������� ���������
  int ign_cutoff_thrd;                  //������� ������� ���������
  int hop_start_cogs;                   //����� ��: ������ �������� � ������ ����� �����. ��� 
  int hop_durat_cogs;                   //����� ��: ������������ �������� � ������ �����
 };

 struct EditTabPar
 {
  unsigned char tab_id;                 //������������� �������(������) � ������
  unsigned int address;                 //����� ������ ��������� ������ � �������
  float table_data[32];                 //�������� ������ (�� ����� 16-�� ����)
  TCHAR name_data[32];                  //�������� ��������� ����������
  unsigned int data_size;               //������ ��������� ������
 };

 const int ETMT_NAME_STR = 0;           //name of tables's set
 const int ETMT_STRT_MAP = 1;           //start map
 const int ETMT_IDLE_MAP = 2;           //idle map
 const int ETMT_WORK_MAP = 3;           //work map
 const int ETMT_TEMP_MAP = 4;           //temp.corr. map

 const int ETMT_VE_MAP   = 5;           //VE
 const int ETMT_AFR_MAP  = 6;           //AFR
 const int ETMT_CRNK_MAP = 7;           //Cranking PW
 const int ETMT_WRMP_MAP = 8;           //Warmup enrichment
 const int ETMT_DEAD_MAP = 9;           //Injector dead time
 const int ETMT_IDLR_MAP = 10;          //IAC run pos
 const int ETMT_IDLC_MAP = 11;          //IAC cranking pos
 const int ETMT_AETPS_MAP = 12;         //AE TPS map
 const int ETMT_AERPM_MAP = 13;         //AE RPM map
 const int ETMT_AFTSTR_MAP = 14;        //afterstart enrichment

 struct SepTabPar
 {
  unsigned int address;                 //����� ������ ��������� ������ � �������
  float table_data[32];                 //�������� ������ (�� ����� 16-�� ����)
  unsigned int data_size;               //������ ��������� ������
 };

 struct DiagInpDat
 {
  float voltage;                        //board voltage
  float map;                            //MAP sensor
  float temp;                           //coolant temperature
  float add_io1;                        //additional input 1 (analog)
  float add_io2;                        //additional input 2 (analog)
  float carb;                           //carburetor switch, throttle position sensor (analog)
  int gas;                              //gas valve state (digital)
  int ckps;                             //CKP sensor (digital)
  int ref_s;                            //VR type cam sensor (digital)
  int ps;                               //Hall-effect cam sensor (digital)
  int bl;                               //"Bootloader" jumper
  int de;                               //"Default EEPROM" jumper
  float ks_1;                           //knock sensor 1
  float ks_2;                           //knock sensor 2
 };

 struct DiagOutDat
 {
  int ign_out1;                         //ignition output 1
  int ign_out2;                         //ignition output 2
  int ign_out3;                         //ignition output 3
  int ign_out4;                         //ignition output 4
  int ie;                               //idle edconomizer
  int fe;                               //fuel economizer
  int ecf;                              //electric cooling fan
  int ce;                               //Check engine
  int st_block;                         //starter blocking
  int add_io1;                          //additional output 1
  int add_io2;                          //additional output 2
  int bl;                               //BL, 3-state output (0 - hiZ, 1 - "0", 2 - "1")
  int de;                               //DE, 3-state output (0 - hiZ, 1 - "0", 2 - "1")
 };

 struct ChokePar
 {
  int sm_steps;                         //number of stepper motor steps
  unsigned char testing;                //not a parameter, (flag) indicates that system is in choke testng mode
  signed char manual_delta;             //delta position value for manual choke control
  float strt_add;                       //Startup addition value used for choke (0...100%)  
  int choke_rpm[2];                     //two points which define RPM vs temperature function 
  float choke_rpm_if;                   //Integral choke RPM regulator factor
  float choke_corr_time;                //Startup corretion time
  float choke_corr_temp;                //Startup corretion temperature threshold
 };

 const int IBTN_KEYS_NUM = 2;           //Number of iButton keys
 const int IBTN_KEY_SIZE = 6;           //Size of iButton key (except CRC8 and family code)

 struct SecurPar
 {
  TCHAR bt_name[10];                    //bluetooth name, this parameter is not stored in EEPROM
  TCHAR bt_pass[10];                    //bluetooth password, this parameter is not stored in EEPROM
  unsigned char use_bt;                 //flag specifying to use or not to use bluetooth
  unsigned char set_btbr;               //flag indicating that bluetooth baud rate has to be set
  unsigned char use_imm;                //flag specifying to use or not to use immobilizer
  BYTE ibtn_keys[IBTN_KEYS_NUM][IBTN_KEY_SIZE];//iButton keys
 };

 //Describes a universal programmable output
 struct uni_output_par
 {
  unsigned char logicFunc;              //logic function between two conditions
  bool invers_1;                        //inversion flags for condition 1
  bool invers_2;                        //inversion flags for condition 2
  unsigned char condition1;             //code of condition 1
  unsigned char condition2;             //code of condition 2
  float on_thrd_1;                      //ON threshold (if value > on_thrd_1)
  float off_thrd_1;                     //OFF threshold (if value < off_thrd_1)
  float on_thrd_2;                      //same as on_thrd_1
  float off_thrd_2;                     //same as off_thrd_1
 };

 const int UNI_OUTPUT_NUM = 3;

 struct UniOutPar
 {
  uni_output_par out[UNI_OUTPUT_NUM];
  unsigned char logicFunc12;            //logic function between 1st and 2nd outputs
 };

 //logic functions
 const int UNIOUT_LF_OR = 0;
 const int UNIOUT_LF_AND = 1;
 const int UNIOUT_LF_XOR = 2;
 const int UNIOUT_LF_2ND = 3;
 const int UNIOUT_LF_NONE = 15;
 //conditions
 const int UNIOUT_COND_CTS = 0;       //coolant temperature
 const int UNIOUT_COND_RPM = 1;       //RPM
 const int UNIOUT_COND_MAP = 2;       //MAP
 const int UNIOUT_COND_UBAT = 3;      //board voltage
 const int UNIOUT_COND_CARB = 4;      //throttle position limit switch
 const int UNIOUT_COND_VSPD = 5;      //vehicle speed
 const int UNIOUT_COND_AIRFL = 6;     //air flow
 const int UNIOUT_COND_TMR = 7;       //timer, allowed only for 2nd condition
 const int UNIOUT_COND_ITTMR = 8;     //timer, triggered after turning on of ignition
 const int UNIOUT_COND_ESTMR = 9;     //timer, triggered after starting of engine
 const int UNIOUT_COND_CPOS = 10;     //choke position
 const int UNIOUT_COND_AANG = 11;     //advance angle
 const int UNIOUT_COND_KLEV = 12;     //knock signal level
 const int UNIOUT_COND_TPS = 13;      //throttle position sensor
 const int UNIOUT_COND_ATS = 14;      //intake air temperature sensor
 const int UNIOUT_COND_AI1 = 15;      //analog input 1
 const int UNIOUT_COND_AI2 = 16;      //analog input 2
 const int UNIOUT_COND_GASV = 17;     //gas valve input
 const int UNIOUT_COND_IPW = 18;      //injector pulse width
 const int UNIOUT_COND_CE = 19;       //CE state
 const int UNIOUT_COND_OFTMR = 20;    //On/Off delay timer

 //define allowed fuel injection configurations
 const int INJCFG_TROTTLEBODY  = 0;   //single injector for N cylinders
 const int INJCFG_SIMULTANEOUS = 1;   //N injectors, all injectors work simultaneously
 const int INJCFG_SEMISEQUENTIAL = 2; //N injectors, injectors work in pairs
 const int INGCFG_FULLSEQUENTIAL = 3; //N injectors, each injector works 1 time per cycle

 struct InjctrPar
 {
  unsigned char inj_flags;            //todo
  int inj_config;                     //Injection configuration (throttle-body, simultaneous, semi-sequential, fullsequential)
  int inj_squirt_num;                 //Number of squirts per cycle
  float inj_flow_rate;                //Injector flow rate
  float inj_cyl_disp;                 //The displacement of one cylinder in liters
  float inj_sd_igl_const;             //precalculated constant
  int cyl_num;                        //read-only parameter: number of cylinders
 };

 struct LambdaPar
 {
  int lam_str_per_stp;                //Number of strokes per step for lambda control
  float lam_step_size;                //Size of step 
  float lam_corr_limit;               //correction limit
  float lam_swt_point;                //switch point in Volts
  float lam_temp_thrd;                //Temperature turn on threshold
  int lam_rpm_thrd;                   //RPM turn on threshold
  int lam_activ_delay;                //Afterstart activation delay
 };

 struct AccelPar
 {
  float ae_tpsdot_thrd;               //TPS %/sec threshold
  float ae_coldacc_mult;              //Cold acceleration multiplier
 };

 //������� ������������� ���� ������� �� � �������
 const int GAIN_FREQUENCES_SIZE = 64;
 static float hip9011_gain_frequences[GAIN_FREQUENCES_SIZE] =
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

 //������� ������������� ���� �����. �������� ����������� � �����. ��������
 const int ATTENUATOR_LEVELS_SIZE = 64;
 static float hip9011_attenuator_gains[ATTENUATOR_LEVELS_SIZE] =
 {
  2.000f, 1.882f, 1.778f, 1.684f, 1.600f, 1.523f, 1.455f, 1.391f,
  1.333f, 1.280f, 1.231f, 1.185f, 1.143f, 1.063f, 1.000f, 0.944f,
  0.895f, 0.850f, 0.810f, 0.773f, 0.739f, 0.708f, 0.680f, 0.654f,
  0.630f, 0.607f, 0.586f, 0.567f, 0.548f, 0.500f, 0.471f, 0.444f,
  0.421f, 0.400f, 0.381f, 0.364f, 0.348f, 0.333f, 0.320f, 0.308f,
  0.296f, 0.286f, 0.276f, 0.267f, 0.258f, 0.250f, 0.236f, 0.222f,
  0.211f, 0.200f, 0.190f, 0.182f, 0.174f, 0.167f, 0.160f, 0.154f,
  0.148f, 0.143f, 0.138f, 0.133f, 0.129f, 0.125f, 0.118f, 0.111f
 };

 //������� ������������� ���� ���������� ������� �������������� � ���������� �������
 //��������������
 const int INTEGRATOR_LEVELS_SIZE = 32;
 static float hip9011_integrator_const[INTEGRATOR_LEVELS_SIZE] =
 {
  40,  45,  50,  55,  60,  65,  70,   75,
  80,  90,  100, 110, 120, 130, 140, 150,
  160, 180, 200, 220, 240, 260, 280, 300,
  320, 360, 400, 440, 480, 520, 560, 600
 };

 //��� ��������� ���� ������ �������� �������� UART-a SECU-3 � ��������������� �� ��������.
 const int SECU3_ALLOWABLE_UART_DIVISORS_COUNT = 8;
 //<spead, divisor>
#define _SD std::pair<int, int>
 static std::pair<int, int> secu3_allowable_uart_divisors[SECU3_ALLOWABLE_UART_DIVISORS_COUNT] =
 {
  _SD(2400,   /*0x01A0*/0x340),
  _SD(4800,   /*0x00CF*/0x1A0),
  _SD(9600,   /*0x0067*/0xCF),
  _SD(14400,  /*0x0044*/0x8A),
  _SD(19200,  /*0x0033*/0x67),
  _SD(28800,  /*0x0022*/0x44),
  _SD(38400,  /*0x0019*/0x33),
  _SD(57600,  /*0x0010*/0x22)
 };
#undef _SD

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
  SECU3IO::CEErrors     m_CEErrors;
  SECU3IO::FWInfoDat    m_FWInfoDat;
  SECU3IO::MiscelPar    m_MiscelPar;
  SECU3IO::EditTabPar   m_EditTabPar;
  SECU3IO::SepTabPar    m_SepTabPar;
  SECU3IO::DbgvarDat    m_DbgvarDat;
  SECU3IO::DiagInpDat   m_DiagInpDat;
  SECU3IO::DiagOutDat   m_DiagOutDat;
  SECU3IO::ChokePar     m_ChokePar;
  SECU3IO::SecurPar     m_SecurPar;
  SECU3IO::UniOutPar    m_UniOutPar;
  SECU3IO::InjctrPar    m_InjctrPar;
  SECU3IO::LambdaPar    m_LambdaPar;
  SECU3IO::AccelPar     m_AccelPar;
 };

 const float start_map_rpm_slots[16] = {200,240,280,320,360,400,440,480,520,560,600,640,680,720,760,800};
 const float work_map_rpm_slots[16]  = {600,720,840,990,1170,1380,1650,1950,2310,2730,3210,3840,4530,5370,6360,7500};
 const float temp_map_tmp_slots[16]  = {-30,-20,-10,0,10,20,30,40,50,60,70,80,90,100,110,120};
 const float dwellcntrl_map_slots[32] = { 5.4f, 5.8f, 6.2f, 6.6f, 7.0f, 7.4f, 7.8f, 8.2f, 8.6f, 9.0f, 9.4f, 9.8f,10.2f,10.6f,11.0f,11.4f,
                                        11.8f,12.2f,12.6f,13.0f,13.4f,13.8f,14.2f,14.6f,15.0f,15.4f,15.8f,16.2f,16.6f,17.0f,17.4f,17.8f};
 const float choke_op_map_slots[16]  = {-5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70};

 const int SECU3_COMPILE_OPTIONS_BITS_COUNT = 29;
 //<bitnumber, name>
#define _SD std::pair<int, TCHAR*>
 static std::pair<int, TCHAR*> secu3_compile_options_bits[SECU3_COMPILE_OPTIONS_BITS_COUNT] =
 {
  _SD(0,  _T("COPT_ATMEGA16")),
  _SD(1,  _T("COPT_ATMEGA32")),
  _SD(2,  _T("COPT_ATMEGA64")),
  _SD(3,  _T("COPT_ATMEGA128")),
  _SD(4,  _T("COPT_VPSEM")),
  _SD(5,  _T("COPT_WHEEL_36_1")),          /*Obsolete! Left for compatibility reasons*/
  _SD(6,  _T("COPT_INVERSE_IGN_OUTPUTS")), /*Obsolete! Left for compatibility reasons*/
  _SD(7,  _T("COPT_DWELL_CONTROL")),
  _SD(8,  _T("COPT_COOLINGFAN_PWM")),
  _SD(9,  _T("COPT_REALTIME_TABLES")),
  _SD(10, _T("COPT_ICCAVR_COMPILER")),
  _SD(11, _T("COPT_AVRGCC_COMPILER")),
  _SD(12, _T("COPT_DEBUG_VARIABLES")),
  _SD(13, _T("COPT_PHASE_SENSOR")),
  _SD(14, _T("COPT_PHASED_IGNITION")),
  _SD(15, _T("COPT_FUEL_PUMP")),
  _SD(16, _T("COPT_THERMISTOR_CS")),
  _SD(17, _T("COPT_SECU3T")),
  _SD(18, _T("COPT_DIAGNOSTICS")),
  _SD(19, _T("COPT_HALL_OUTPUT")),
  _SD(20, _T("COPT_REV9_BOARD")),
  _SD(21, _T("COPT_STROBOSCOPE")),
  _SD(22, _T("COPT_SM_CONTROL")),
  _SD(23, _T("COPT_VREF_5V")),
  _SD(24, _T("COPT_HALL_SYNC")),
  _SD(25, _T("COPT_UART_BINARY")),
  _SD(26, _T("COPT_CKPS_2CHIGN")),
  _SD(27, _T("COPT_ATMEGA644")),
  _SD(28, _T("COPT_FUEL_INJECT"))
 };
#undef _SD

 const int COPT_ATMEGA16 = 0;
 const int COPT_ATMEGA32 = 1;
 const int COPT_ATMEGA64 = 2;
 const int COPT_ATMEGA128 = 3;
 const int COPT_VPSEM = 4;
 const int COPT_WHEEL_36_1 = 5;          /*Obsolete! Left for compatibility reasons*/
 const int COPT_INVERSE_IGN_OUTPUTS = 6; /*Obsolete! Left for compatibility reasons*/
 const int COPT_DWELL_CONTROL = 7;
 const int COPT_COOLINGFAN_PWM = 8;
 const int COPT_REALTIME_TABLES = 9;
 const int COPT_ICCAVR_COMPILER = 10;
 const int COPT_AVRGCC_COMPILER = 11;
 const int COPT_DEBUG_VARIABLES = 12;
 const int COPT_PHASE_SENSOR = 13;
 const int COPT_PHASED_IGNITION = 14;
 const int COPT_FUEL_PUMP = 15;
 const int COPT_THERMISTOR_CS = 16;
 const int COPT_SECU3T = 17;
 const int COPT_DIAGNOSTICS = 18;
 const int COPT_HALL_OUTPUT = 19;
 const int COPT_REV9_BOARD = 20;
 const int COPT_STROBOSCOPE = 21;
 const int COPT_SM_CONTROL = 22;
 const int COPT_VREF_5V = 23;
 const int COPT_HALL_SYNC = 24;
 const int COPT_UART_BINARY = 25;
 const int COPT_CKPS_2CHIGN = 26;
 const int COPT_ATMEGA644 = 27;
 const int COPT_FUEL_INJECT = 28;

 //scale factor for adv. angle maps (they are stored in integer format)
 const float AA_MAPS_M_FACTOR = 2.0f;
 const float VE_MAPS_M_FACTOR = 128.0f;
 const float AFR_MAPS_M_FACTOR = 2048.0f;
 const float WRMP_MAPS_M_FACTOR = (128.0f * 0.01f);
 const float IACPOS_MAPS_M_FACTOR = 2.0f;
 const float AETPSB_MAPS_M_FACTOR = 0.1f;
 const float AETPSV_MAPS_ADDER = 55.0f;
 const float AERPMB_MAPS_M_FACTOR = 0.01f;
 const float AERPMV_MAPS_M_FACTOR = (128.0f * 0.01f);
 const float AFTSTR_MAPS_M_FACTOR = (128.0f * 0.01f);
};
