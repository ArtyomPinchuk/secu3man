
#pragma once

#include <string>
#include <vector>
#include "common/unicodesupport.h"
#include "ISettingsData.h"

class CAppSettingsController;

//������ � ���������/��������� ������ 
class CAppSettingsModel : public ISettingsData 
{
  friend CAppSettingsController;

 public:
  std::vector<DWORD> m_AllowableBaudRates;
  std::vector<_TSTRING> m_AllowablePorts;

  //<UIstring, SepSymbol>
  std::vector<std::pair<_TSTRING, char> > m_AllowaleCSVSepSymbols;

  CAppSettingsModel();
  virtual ~CAppSettingsModel();

  ////////////////////////////////////////////////////
  //���������� ������ ��� INI-�����. INI-���� ��������� � �������� �� ��������
  //�������� ���������.
  CString GetINIFileFullName(void) const;

  //������ �������� �� INI-�����
  bool ReadSettings(void);
    
  //������ �������� � INI-����
  bool WriteSettings(void);

  //��� ������ � �������� �����
  const CString m_Name_Options_Section;
  const CString m_Name_PortName;
  const CString m_Name_BaudRateApplication;
  const CString m_Name_BaudRateBootloader;
  const CString m_Name_LogFilesFolder;
  const CString m_Name_UseAppFolder;
  const CString m_Name_CSVSepSymbol;
  const CString m_Name_MIDeskUpdatePeriod;


  //ISettingsData
  virtual const _TSTRING& GetPortName(void) const;   
  virtual DWORD GetBaudRateApplication(void) const;
  virtual DWORD GetBaudRateBootloader(void) const;
  virtual const CString& GetLogFilesFolder(void) const;
  virtual bool  GetUseAppFolder(void) const;
  virtual char  GetCSVSepSymbol(void) const;
  virtual int   GetMIDeskUpdatePeriod(void) const;
  //���������� ������ ���� � �������� �� �������� ���� �������� ����������
  //(������� ������� ��� ������� �� ������ ����� ������������ ����� ������)
  virtual CString GetAppDirectory(void) const;
  ////////////////////////////////////////////////////

 private:
  //������ ������� �������� � INI-�����
  _TSTRING m_optPortName;      
  DWORD m_optBaudRateApplication;
  DWORD m_optBaudRateBootloader;
  CString m_optLogFilesFolder;
  bool  m_optUseAppFolder;
  char  m_optCSVSepSymbol;
  int   m_optMIDeskUpdatePeriod;

  //��������� ��������� �������� �������� �� ������������ ���������
  bool CheckAllowableBaudRate(DWORD baud);
  bool CheckAllowableCSVSepSymbol(char i_symbol);

  //���������� �� ������� ���� �������� ����������
  TCHAR m_current_directory[MAX_PATH+1]; 
};