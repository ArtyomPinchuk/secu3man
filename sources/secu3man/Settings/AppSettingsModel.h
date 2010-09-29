
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

  //<<UIString, INIString>, ID>
  std::vector<std::pair<std::pair<_TSTRING, _TSTRING>, int> > m_AllowableLanguages;

  //<<UIString, INIString>, ID>
  std::vector<std::pair<std::pair<_TSTRING, _TSTRING>, int> > m_AllowablePlatforms;

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
  const CString m_Name_InterfaceLang;
  const CString m_Name_ECUPlatformType;

  //������� ���� ������ ���
  const CString m_Name_WndSettings_Section;
  const CString m_Name_StrtMapWnd_X;
  const CString m_Name_StrtMapWnd_Y;
  const CString m_Name_IdleMapWnd_X;
  const CString m_Name_IdleMapWnd_Y;
  const CString m_Name_WorkMapWnd_X;
  const CString m_Name_WorkMapWnd_Y;
  const CString m_Name_TempMapWnd_X;
  const CString m_Name_TempMapWnd_Y;
  const CString m_Name_AttenMapWnd_X;
  const CString m_Name_AttenMapWnd_Y;
  const CString m_Name_MainFrmWnd_X;
  const CString m_Name_MainFrmWnd_Y;

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
  //������� ����
  virtual void SetWndSettings(const WndSettings& i_wndSettings);
  virtual void GetWndSettings(WndSettings& o_wndSettings) const;
  //���� ���������� � ��� ���������
  virtual EInterLang GetInterfaceLanguage(void) const;
  virtual EECUPlatform GetECUPlatformType(void) const;
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
  EInterLang m_optInterLang;
  EECUPlatform m_optECUPlatformType;

  int m_optStrtMapWnd_X;
  int m_optStrtMapWnd_Y;
  int m_optIdleMapWnd_X;
  int m_optIdleMapWnd_Y;
  int m_optWorkMapWnd_X;
  int m_optWorkMapWnd_Y;
  int m_optTempMapWnd_X;
  int m_optTempMapWnd_Y;
  int m_optAttenMapWnd_X;
  int m_optAttenMapWnd_Y;
  int m_optMainFrmWnd_X;
  int m_optMainFrmWnd_Y;

  //��������� ��������� �������� �������� �� ������������ ���������
  bool CheckAllowableBaudRate(DWORD baud);
  bool CheckAllowableCSVSepSymbol(char i_symbol);
  bool CheckAllowableLanguage(const std::string& i_string, EInterLang& o_language_id);
  bool CheckAllowablePlatform(const std::string& i_string, EECUPlatform& o_platform_id);

  //���������� �� ������� ���� �������� ����������
  TCHAR m_current_directory[MAX_PATH+1]; 
};
