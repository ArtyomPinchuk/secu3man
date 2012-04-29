
#pragma once

/////////////////////////////////////////////////////////////////////////////

#include <vector>

class CTabDialog;
class ITabControllerEvent;

// ������������ ����� TCS_OWNERDRAWFIXED!

class AFX_EXT_CLASS CTabController : public CTabCtrl
{
  DECLARE_DYNAMIC(CTabController)

 public:
  CTabController();
  virtual ~CTabController();

  //������������� ������, ������� �������� ������� ����������� ��� ������ ���������.
  void SetResourceModule(HMODULE hModule);

  //���� reflect - true, �� ��������� ����� �������������� ������������� ����,
  //����� - ���.
  void SetMsgReflection(bool reflect);

  //���������� �������
  int  AddPage(CString name, CTabDialog* pPageDlg);
  int  AddPage(CString name, CTabDialog* pPageDlg, const int nImage);

  // set tab (fail if disabled)
  bool SetCurSel(UINT iNewTab);

  //����������/���������� ��������� �������
  void EnableItem(int ITab, bool enable);

  //�������������. ����������� ����� ������� ����� ������� ������!
  void Init(void);

  //��������� ����������� �������. �������� �������� ITabControllerEvent.
  void SetEventListener(ITabControllerEvent* i_listener);

  CTabDialog* GetCurrentPage(void) const;

 protected:
  // Generated message map functions
  afx_msg BOOL OnSelchangeTabctl(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg BOOL OnSelchangingTabctl(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnDestroy();
  DECLARE_MESSAGE_MAP()

  virtual BOOL IsTabEnabled(int iTab) const;
  int NextEnabledTab(int iTab, BOOL bWrap);  // get next enabled tab
  int PrevEnabledTab(int iTab, BOOL bWrap);  // get prev enabled tab

  // MFC overrides
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

 private:
  class TabPageData;
  TabPageData* GetItemData(int item) const;
  void CreateTabPage(void);
  void DestroyTabPage(void);
  void CalculatePageRect(int nItem, CRect& o_rect);
  int GetTCOrientation(void);

  CTabDialog*  mp_CurDlg;
  HMODULE m_hResourceModule;
  ITabControllerEvent* m_pEventHandler;

  int   m_tab_item_index;
  bool  m_msg_reflect;
  int   m_tcmn;
};

/////////////////////////////////////////////////////////////////////////////
