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

#pragma once

#include <vector>
#include "common/ObjectTimer.h"
#include "PMControllerBase.h"

class CCommunicationManager;
class CStatusBarManager;
class CTablesDeskDlg;
struct SECU3FWMapsItem;
namespace SECU3IO {struct EditTabPar;}

class CPMTablesController : public CPMControllerBase<CTablesDeskDlg>
{
  typedef CPMControllerBase<VIEW> Super;
 public:
  CPMTablesController(VIEW* ip_view, CCommunicationManager* ip_comm, CStatusBarManager* ip_sbar);
  virtual ~CPMTablesController();

  //������ ������ �����������
  void OnActivate(void);

  //����� ������ �����������
  void OnDeactivate(void);

  virtual void Enable(bool state);

  virtual void StartDataCollection(void);

  //������ ���� ����������� ������ ����� �������� (�������� �������)
  //���������� true ����� ��� ������ ���������  
  virtual bool CollectData(const BYTE i_descriptor, const void* i_packet_data);

  void InvalidateCache(void);
  bool IsValidCache(void);

 private:
  //Events from view
  void OnMapChanged(int fuel_type, int i_mapType);
  void OnCloseMapWnd(HWND i_hwnd, int i_mapType);
  void OnOpenMapWnd(HWND i_hwnd, int i_mapType);
  void OnTabActivate(void);
  void OnSaveButton(void);
  void OnChangeTablesSetName(int fuel_type);

 private:
  //helpful methods
  bool _CompareViewMap(int i_mapType, size_t size) const;
  float* _GetMap(int fuel_type, int i_mapType, bool i_original);
  void _MoveMapToChart(int fuel_type, int i_mapType, bool i_original);
  void _MoveMapsToCharts(int fuel_type, bool i_original);
  void _ClearAcquisitionFlags(void);
  void _ResetModification(int fuel_type);

  void _UpdateCache(const SECU3IO::EditTabPar* data); 
  bool _IsCacheUpToDate(void);
  bool _IsModificationMade(void) const;
  void _SynchronizeMap(int fuel_type, int i_mapType);

  //tables's data collected 
  void OnDataCollected(void);
  //from timer
  void OnTableDeskChangesTimer(void);

 private:
  CCommunicationManager* mp_comm;
  CStatusBarManager* mp_sbar;

  //��� ������ 
  std::vector<SECU3FWMapsItem*> m_maps;  //current
  std::vector<SECU3FWMapsItem*> m_omaps; //original
  //����� ����������� ��������� ����� ���������� �� ��������
  std::vector<SECU3FWMapsItem*> m_maps_flags;

  bool m_valid_cache; //������� ��������� ���� (������ � ��������� ������������� ������ � SECU-3)
  CObjectTimer<CPMTablesController> m_td_changes_timer;
};
