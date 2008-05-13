
#pragma once

class CMeasInstrument  
{
public:
	CMeasInstrument();
	virtual ~CMeasInstrument();

//interface for measurement instrument

    virtual void SetValue(float value) = 0;
    virtual float GetValue(void) = 0;

	//�������/����������� �������
	virtual void Show(bool show) = 0;

	//����������/���������� �������
	virtual void Enable(bool enable) = 0;

	//������ ����� ��� ����� ?
	virtual bool IsVisible(void) = 0;

	//������ �������� ��� �������� ?
	virtual bool IsEnabled(void) = 0;	

	//��������� �������� ���������
	virtual void SetLimits(float loLimit, float upLimit) = 0;

	//��������� ���������� �������
	virtual void SetTicks(int number) = 0;
};

