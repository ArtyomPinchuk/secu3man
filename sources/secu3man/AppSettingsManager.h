
#pragma once

#include "AppSettingsModel.h"
#include "AppSettingsController.h"
#include "AppSettingsDlg.h"


class CAppSettingsManager  
{
public:

    CAppSettingsModel* m_pModel;
	CAppSettingsController* m_pController;
	CAppSettingsDlg* m_pDialog;

	CAppSettingsManager();
	virtual ~CAppSettingsManager();

	//��������� ��������� � �����
    bool ReadSettings(void) 
	{
      ASSERT(m_pModel);
      return m_pModel->ReadSettings();
	}

	//��������� ��������� �� ����
	bool WriteSettings(void) 
	{
      ASSERT(m_pModel);
      return m_pModel->WriteSettings();
	}

	//�������� ������ ����������
	int ShowDialog(void) 
	{ //���������� ����� �����������...
      ASSERT(m_pController);
	  return m_pController->ShowDialog(); 
	}
};

