
#pragma once

class IMeasInstrument  
{
 public:
  IMeasInstrument() {};
  virtual ~IMeasInstrument() {};

  //interface for measurement instrument
  virtual void Create(void) = 0;
  virtual void Scale(float i_x_factor, float i_y_factor) = 0;

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
