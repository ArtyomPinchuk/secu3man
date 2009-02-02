
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

    //������ ������� �������� � INI-�����
	_TSTRING m_optPortName;      
	DWORD m_optBaudRateApplication;
	DWORD m_optBaudRateBootloader;
	CString m_optLogFilesFolder;
	bool  m_optUseAppFolder;
    ////////////////////////////////////////////////////

  private:
	//��������� ��������� �������� �������� �� ������������ ���������
    bool CheckAllowableBaudRate(DWORD baud);

	//���������� �� ������� ���� �������� ����������
    TCHAR m_current_directory[MAX_PATH+1]; 
};

