
#pragma once

class CMainFrame;
class CChildViewManager;
class MainFrameController;
class CStatusBarManager;

class CMainFrameManager  
{
 public:
  CMainFrameManager();
  virtual ~CMainFrameManager();
  
  //������� ������ ������� ���� � ���������� ��������� �� ����
  CMainFrame* GreateMainWindow(void);
  
  //�������������� ���������� ���� � �������� �����������
  bool Init(CWnd* &o_pMainWnd);

  //���������� �������� ��������� ������ (� ������ ����� �������� ����)
  CStatusBarManager* GetStatusBarManager(void) const;  

 private:
  MainFrameController* m_pMainFrameController; //controller
  CStatusBarManager* m_pStatusBarManager;
  CChildViewManager* m_pChildViewManager;
  CMainFrame* m_pMainFrame;	//view 
};
