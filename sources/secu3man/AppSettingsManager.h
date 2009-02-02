
#pragma once

class CAppSettingsModel;
class CAppSettingsDlg;
class CAppSettingsController;


class CAppSettingsManager  
{
public:

    CAppSettingsModel* m_pModel;
	CAppSettingsController* m_pController;
	CAppSettingsDlg* m_pDialog;

	CAppSettingsManager();
	virtual ~CAppSettingsManager();

	//��������� ��������� � �����
    bool ReadSettings(void) const;

	//��������� ��������� �� ����
	bool WriteSettings(void) const;

	//�������� ������ ����������
	int ShowDialog(void) const;
};

