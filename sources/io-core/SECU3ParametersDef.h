/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Gorlovka

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

#ifndef _SECU3_PARAMETERS_DEF_H_
#define _SECU3_PARAMETERS_DEF_H_

#include "SECU3Types.h"

#pragma pack( push, enter_SECU3ParametersDef )
#pragma pack(1) //<--SECU3

namespace SECU3IO {

using namespace SECU3Types;

//��������� ��������� �������
typedef struct
{
 _uint  starter_off;                    // ����� ���������� �������� (���-1)
 _uint  smap_abandon;                   // ������� �������� � �������� ����� �� �������  (���-1)

 _uint  map_lower_pressure;             // ������ ������� ��� �� ��� �������(���)
 _int   map_upper_pressure;             // ������� ������� ��� �� ��� ������� (���)
 _int   map_curve_offset;               // can be negative
 _int   map_curve_gradient;             // can be negative (in case of inverse characteristic curve)!

 _uchar carb_invers;                    // �������� ��������� �� �����������
 _int   tps_curve_offset;               // offset of curve in volts
 _int   tps_curve_gradient;             // gradient of curve in Percentage/V
 _uchar tps_threshold;                  // TPS threshold used to switch work and idle modes

 _uint  ephh_lot;                       // ������ ����� ���� (���-1)
 _uint  ephh_hit;                       // ������� ����� ���� (���-1)
 _uint  ephh_lot_g;                     // ������ ����� ���� (���)
 _uint  ephh_hit_g;                     // ������� ����� ���� (���)
 _int   epm_on_threshold;
 _uchar shutoff_delay;                  // �������� ���������� �������

 _int   angle_dec_spead;
 _int   angle_inc_spead;
 _int   max_angle;                      // ����������� ������������� ���
 _int   min_angle;                      // ����������� ������������ ���
 _int   angle_corr;                     // �����-��������� ���
 _uchar zero_adv_ang;                   // Zero advance angle flag

 _uchar fn_benzin;                      // ����� ������ ������������� ������������ ��� �������
 _uchar fn_gas;                         // ����� ������ ������������� ������������ ��� ����

 _uchar idl_regul;                      // ������������ �������� ������� �� �������������� ���
 _uint  idling_rpm;                     // �������� ������� �� ��� ����������� �������������� ���
 _int   ifac1;                          // ������������ �-���������� �������� ��, ��� ������������� �
 _int   ifac2;                          // ������������� ������ ��������������.
 _int   MINEFR;                         // ���� ������������������ ���������� (�������)
 _int   idlreg_min_angle;
 _int   idlreg_max_angle;
 _int   idlreg_turn_on_temp;            // Idling regulator turn on temperature

 _uchar tmp_use;                        // ������� ������������ ����-��
 _int   vent_on;                        // ����������� ��������� �����������
 _int   vent_off;                       // ����������� ���������� �����������
 _uchar vent_pwm;                       // control ventilator by using pwm
 _uchar cts_use_map;                    // Flag which indicates using of lookup table for coolant temperature sensor

 _int   map_adc_factor;
 _long  map_adc_correction;
 _int   ubat_adc_factor;
 _long  ubat_adc_correction;
 _int   temp_adc_factor;
 _long  temp_adc_correction;
 _int   tps_adc_factor;                 // ADC error compensation factor for TPS
 _long  tps_adc_correction;             // ADC error compensation correction for TPS
 _int   ai1_adc_factor;                 // ADC error compensation factor for ADD_IO1 input
 _long  ai1_adc_correction;             // ADC error compensation correction for ADD_IO1 input
 _int   ai2_adc_factor;                 // ADC error compensation factor for ADD_IO2 input
 _long  ai2_adc_correction;             // ADC error compensation correction for ADD_IO2 input

 _uchar ckps_edge_type;
 _uchar ckps_cogs_btdc;
 _uchar ckps_ignit_cogs;
 _uchar ckps_engine_cyl;                // ���-�� ��������� ���������
 _uchar ckps_cogs_num;                  // number of crank wheel's teeth 
 _uchar ckps_miss_num;                  // number of missing crank wheel's teeth
 _uchar ref_s_edge_type;                // REF_S edge type (��� ������ ���)
 _uchar hall_flags;                     // Hall sensor flags
 _int   hall_wnd_width;

 _uchar ign_cutoff;                     // Cutoff ignition when RPM reaches specified threshold
 _uint  ign_cutoff_thrd;                // Cutoff threshold (RPM)
 _uchar ckps_merge_ign_outs;            // Merge ignition signals to single ouput

 _char  hop_start_cogs;                 // Hall output: start of pulse in teeth relatively to TDC 
 _uchar hop_durat_cogs;                 // Hall output: duration of pulse in teeth

 _uint  uart_divisor;                   // �������� ��� ��������������� �������� UART-a
 _uchar uart_period_t_ms;               // ������ ������� ������� � �������� �����������

 // knock control
 _uchar knock_use_knock_channel;        // ������� ������������� ������ ���������
 _uchar knock_bpf_frequency;            // ����������� ������� ���������� �������
 _int   knock_k_wnd_begin_angle;        // ������ �������������� ���� (�������)
 _int   knock_k_wnd_end_angle;          // ����� �������������� ���� (�������)
 _uchar knock_int_time_const;           // ���������� ������� �������������� (���)
 _int   knock_retard_step;              // ��� �������� ��� ��� ���������
 _int   knock_advance_step;             // ��� �������������� ���
 _int   knock_max_retard;               // ������������ �������� ���
 _uint  knock_threshold;                // ����� ��������� - ����������
 _uchar knock_recovery_delay;           // �������� �������������� ��� � ������� ������ ���������

 _uint  sm_steps;                       // Number of steps of choke stepper motor
 _uint  choke_rpm[2];                   // Values of RPM needed for RPM-based control of choke position
 _uchar choke_startup_corr;             // Startup correction value for choke
 _uint  choke_rpm_if;                   // Integral factor for RPM-based control of choke position (factor * 1024)
 _uint  choke_corr_time;                // Time for startup correction to be applied
 _int   choke_corr_temp;                // Temperature threshold for startup correction

 _uchar bt_flags;                       // Bluetooth and security related flags
 _uchar ibtn_keys[2][6];                // iButton keys for immobilizer

 //��� ����������������� ����� ���������� ��� ���������� �������� �������������
 //����� ������ �������� � ����� ������� ��������. ��� ���������� ����� ������
 //� ���������, ���������� ����������� ��� �����.
 _uchar reserved[96];

 _uint crc;                          //����������� ����� ������ ���� ��������� (��� �������� ������������ ������ ����� ���������� �� EEPROM)
}params_t;

} //SECU3IO

#pragma pack( pop, enter_SECU3ParametersDef )

#endif //_SECU3_PARAMETERS_DEF_H_
