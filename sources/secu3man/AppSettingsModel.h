
#pragma once

#include <string>
#include <vector>
#include "common/unicodesupport.h"

//������ � ���������/��������� ������ 
class CAppSettingsModel  
{
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

	//���������� ������ ���� � �������� �� �������� ���� �������� ����������
	//(������� ������� ��� ������� �� ������ ����� ������������ ����� ������)
	CString GetAppDirectory(void) const;

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

    //������ ������� �������� � INI-�����
	_TSTRING m_optPortName;      
	DWORD m_optBaudRateApplication;
	DWORD m_optBaudRateBootloader;
	CString m_optLogFilesFolder;
	bool  m_optUseAppFolder;
	char  m_optCSVSepSymbol;
    ////////////////////////////////////////////////////

  private:
	//��������� ��������� �������� �������� �� ������������ ���������
    bool CheckAllowableBaudRate(DWORD baud);

	bool CheckAllowableCSVSepSymbol(char i_symbol);

	//���������� �� ������� ���� �������� ����������
    TCHAR m_current_directory[MAX_PATH+1]; 
};

