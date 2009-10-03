
#pragma once

#include <vector>
#include "ui-core/ITabControllerEvent.h"

class CTabController;
class ITabController;

class CMainTabController  : public ITabControllerEvent
{
 public:
  CMainTabController();
  virtual ~CMainTabController();

  //��������� ����������
  void AddTabController(ITabController* i_pCntr);
   
  //���������� ������ ������������
  std::vector<ITabController*>& GetControllersList(void);

  //�������� � Tab-��������
  void SetTabController(CTabController* i_pTabController);
  
  //���������� ���������� �������� �������
  virtual ITabController* GetActiveController() const;
   
 private:
  //ITabControllerEvent imp.
  virtual void OnSelchangeTabctl(void); 
  virtual void OnSelchangingTabctl(void); 
    
 private:
  std::vector<ITabController*> m_controllers_list;
  CTabController* m_pTabController;
};
