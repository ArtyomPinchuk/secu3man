
#pragma once

class CAppSettingsModel;
class CAppSettingsDlg;
class CAppSettingsController;
class ISettingsData;

class CAppSettingsManager  
{
public:

 CAppSettingsManager();
 virtual ~CAppSettingsManager();

 //��������� ��������� � �����
 bool ReadSettings(void) const;

 //��������� ��������� �� ����
 bool WriteSettings(void) const;

 //�������� ������ ����������
 int ShowDialog(void) const;

 //���������� ��������� � ������ �������� ��������� ������ ��� ������
 ISettingsData* GetSettings(void) const;
   
 //���������� ��������� �� ������ � ������� �������� ���������
 CAppSettingsModel* GetModel(void) const;

private:
 CAppSettingsModel* m_pModel;
 CAppSettingsController* m_pController;
 CAppSettingsDlg* m_pDialog;
};
