
#pragma once

#include "resource.h"
#include <vector>

#include "ui-core/TabController.h"
#include "ui-core/UpdatableDialog.h"
#include "FunSetPageDlg.h"
#include "IParamDeskView.h"
#include "ui-core/ITabControllerEvent.h"
#include "common/unicodesupport.h"



/////////////////////////////////////////////////////////////////////////////
// CParamDeskDlg dialog

class AFX_EXT_CLASS CParamDeskDlg : public CUpdatableDialog, public IParamDeskView, private ITabControllerEvent
{
// Construction
public:
	CParamDeskDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParamDeskDlg();

	void SetPosition(int x_pos, int y_pos);
	void SetTitle(const CString& i_str);
	void GetTitle(CString& o_str);

	//----------interface implementation---------------------------
	virtual bool IsEnabled(void) { return m_enabled ? true : false;};
    virtual void Enable(bool enable);
    virtual void Show(bool show);
    virtual void ShowSaveButton(bool i_show);

	virtual bool SetValues(BYTE i_descriptor, const void* i_values);
	virtual bool GetValues(BYTE i_descriptor, void* o_values);

	virtual void SetFunctionsNames(const std::vector<_TSTRING>& i_names);
	virtual const std::vector<_TSTRING>& GetFunctionsNames(void) { return m_pFunSetPageDlg->AccessFunNames();}

    virtual BYTE GetCurrentDescriptor(void); 

	virtual void SetOnTabActivate(EventHandler OnTabActivate) {m_OnTabActivate = OnTabActivate;}
	virtual void SetOnChangeInTab(EventHandler OnChangeInTab) {m_OnChangeInTab = OnChangeInTab;}
    virtual void SetOnSaveButton(EventHandler OnSaveButton)   {m_OnSaveButton  = OnSaveButton;}

	//!!!
	virtual bool SetCurSel(int sel) { return m_tab_control.SetCurSel(sel);};
	virtual int GetCurSel(void) {return m_tab_control.GetCurSel();};

    //-------------------------------------------------------------

// Dialog Data
	//{{AFX_DATA(CParamDeskDlg)
	enum { IDD = IDD_PARAMETERS_DESK };
	CStatic	m_pd_title;
	CTabController	m_tab_control;
	CButton m_save_button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamDeskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	CImageList* m_pImgList;

    EventHandler m_OnTabActivate;
    EventHandler m_OnChangeInTab;
	EventHandler m_OnSaveButton;

// Implementation
protected:
	BOOL m_enabled;

    class CStarterPageDlg*   m_pStarterPageDlg;		 
    class CAnglesPageDlg*    m_pAnglesPageDlg;		 
    class CIdlRegPageDlg*    m_pIdlRegPageDlg; 
    class CFunSetPageDlg*    m_pFunSetPageDlg;		 
    class CTemperPageDlg*    m_pTemperPageDlg;		  
    class CCarburPageDlg*    m_pCarburPageDlg;
	class CADCCompenPageDlg* m_pADCCompenPageDlg;
	class CCKPSPageDlg*      m_pCKPSPageDlg;

	// Generated message map functions
	//{{AFX_MSG(CParamDeskDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnUpdateControls(CCmdUI* pCmdUI);
	afx_msg void OnSaveButton();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	virtual void OnSelchangeTabctl(void) { if (m_OnTabActivate) m_OnTabActivate();}; 

	virtual void OnSelchangingTabctl(void) {};

	void OnChangeInTab(void) { if (m_OnChangeInTab) m_OnChangeInTab(); }

};

/////////////////////////////////////////////////////////////////////////////
