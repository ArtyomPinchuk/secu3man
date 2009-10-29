
#pragma once

//����� ���� ��������� ������� ���������� ��������� ������������� ������� (Tab control) 
class ITabController  
{
 public:
  virtual void OnActivate(void) = 0;
  virtual void OnDeactivate(void) = 0;
  virtual bool OnClose(void) = 0;
  virtual bool OnAskFullScreen(void) = 0;
  virtual void OnFullScreen(bool i_what, const CRect& i_rect) = 0;
};