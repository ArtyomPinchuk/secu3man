
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

    ////////////////////////////////////////////////////
    CString GetINIFileFullName(void);

    bool ReadSettings(void);
	bool WriteSettings(void);

	//��� ������ � �������� �����
	const CString m_Name_Options_Section;
	const CString m_Name_PortName;
	const CString m_Name_BaudRateApplication;
	const CString m_Name_BaudRateBootloader;
	const CString m_Name_MAPCurveSlope;
	const CString m_Name_MAPCurveOffset;

    //������ ������� �������� � INI-�����
	_TSTRING m_optPortName;      
	DWORD m_optBaudRateApplication;
	DWORD m_optBaudRateBootloader;
	float m_optMAPCurveSlope;  //kPa/V
	float m_optMAPCurveOffset; //Volts
    ////////////////////////////////////////////////////

	CAppSettingsModel();
	virtual ~CAppSettingsModel();

  private:
    bool CheckAllowableBaudRate(DWORD baud); 
};

