 /****************************************************************
 *       SECU-3  - An open source, free engine control unit
 *    Designed by Alexey A. Shabelnikov. Ukraine, Gorlovka 2007.
 *       Microprocessors systems - design & programming.
 *    contacts:
 *              http://secu-3.narod.ru
 *              ICQ: 405-791-931
 ****************************************************************/

#include "stdafx.h"
#include "secu3man.h"
#include "AppSettingsManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppSettingsManager::CAppSettingsManager()
: m_pModel(NULL)
, m_pController(NULL)
, m_pDialog(NULL)
{

 m_pModel = new CAppSettingsModel();
 m_pDialog = new CAppSettingsDlg(); //View
 m_pController = new CAppSettingsController(m_pDialog,m_pModel); 

}

CAppSettingsManager::~CAppSettingsManager()
{
 delete m_pModel;
 delete m_pController;
 delete m_pDialog;
}
