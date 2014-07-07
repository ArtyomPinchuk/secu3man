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

#pragma once

#include "io-core/FirmwareMapsDataHolder.h"

class CMapImpExpDlg;
class S3FFileDataIO;

class AFX_EXT_CLASS S3FImportController
{
 public:
  S3FImportController(FWMapsDataHolder* ip_fwd);
 ~S3FImportController();

  int  DoImport(void);

  void OnOkPressed(void);
  void OnCancelPressed(void);
  void OnExchangePressed(void);
  void OnViewActivate(void);
  void OnCurrentListNameChanged(int item, CString text);
  void OnOtherListNameChanged(int item, CString text);
  bool IsExchangeButtonAllowed(void);

 private:
  FWMapsDataHolder* mp_fwd;
  CMapImpExpDlg* mp_view;
  S3FFileDataIO* mp_s3f_io;
  _TSTRING m_s3f_file_name;
};

class AFX_EXT_CLASS S3FExportController
{
 public:
  S3FExportController(FWMapsDataHolder* ip_fwd);
 ~S3FExportController();

  int  DoExport(void);

  void OnOkPressed(void);
  void OnCancelPressed(void);
  void OnExchangePressed(void);
  void OnViewActivate(void);
  void OnCurrentListNameChanged(int item, CString text);
  void OnOtherListNameChanged(int item, CString text);
  bool IsExchangeButtonAllowed(void);

 private:
  FWMapsDataHolder* mp_fwd;
  CMapImpExpDlg* mp_view;
  S3FFileDataIO* mp_s3f_io;
  _TSTRING m_s3f_file_name;
};
