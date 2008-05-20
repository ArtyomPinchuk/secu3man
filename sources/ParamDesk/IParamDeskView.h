
#pragma once

#include "common/FastDelegate.h"
#include "common/unicodesupport.h"

//����� ���� ��������� ����� ������� � ������� ���������� (��������������)
//�����������: ����� �� ��������� ������� ���������� ����� ���� ������ ���������� 
//�� ���� - ����� ������� ��� ��� (����� SetCurSel()). ��� ������ � �������������� ��� ������� ������������ ������������

class IParamDeskView  
{
  typedef fastdelegate::FastDelegate0<> EventHandler;

  virtual void Enable(bool enable) = 0;                                 //���������/��������� ������������� 
  virtual void Show(bool show) = 0;                                     //��������/�������� ������� �������������
  virtual bool SetValues(BYTE i_descriptor, const void* i_values) = 0;  //������� ������ � �������������
  virtual bool GetValues(BYTE i_descriptor, void* o_values) = 0;        //������� ������ �� ������������� 

  //���������/��������� ���� �������� ������������� (��� ������� FunSet...)
  //������ �������� ����� ���� �������� ������������� �������� � �������� SECU-3
  virtual void SetFunctionsNames(const std::vector<_TSTRING>& i_names) = 0;
  virtual const std::vector<_TSTRING>& GetFunctionsNames(void) = 0; 

  virtual BYTE GetCurrentDescriptor(void) = 0; //�������� ���������� ��������������� ������� (������������) �������

  virtual void SetOnTabActivate(EventHandler OnTabActivate) = 0; //���������� ����� ���������� ��� ��������� �������
  virtual void SetOnChangeInTab(EventHandler OnChangeInTab) = 0; //���������� ����� ���������� ��� ��������� ������������� ������ �������

  //�������� ������ ��� �������� ������!!!
  virtual bool SetCurSel(int sel) = 0;
  virtual int GetCurSel(void) = 0;
};

