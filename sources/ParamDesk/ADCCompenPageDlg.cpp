 /****************************************************************
 *       SECU-3  - An open source, free engine control unit
 *    Designed by Alexey A. Shabelnikov. Ukraine, Gorlovka 2008.
 *       Microprocessors systems - design & programming.
 *    contacts:
 *              http://secu-3.narod.ru
 *              ICQ: 405-791-931
 ****************************************************************/

#include "stdafx.h"
#include "ADCCompenPageDlg.h"
#include "ui-core/ddx_helpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CADCCompenPageDlg::CADCCompenPageDlg(CWnd* pParent /*=NULL*/)
: CTabDialog(CADCCompenPageDlg::IDD, pParent)
, m_enabled(FALSE)
{
  m_params.map_adc_factor = 1.0f;
  m_params.map_adc_correction = 0.0f;
  m_params.ubat_adc_factor = 1.0f;
  m_params.ubat_adc_correction = 0.0f;
  m_params.temp_adc_factor = 1.0f;
  m_params.temp_adc_correction = 0.0f;
 
  //{{AFX_DATA_INIT(CADCCompenPageDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

LPCTSTR CADCCompenPageDlg::GetDialogID(void) const
{
	return (LPCTSTR)IDD; 
}

void CADCCompenPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADCCompenPageDlg)
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_MAP_FACTOR_SPIN, m_map_factor_spin);
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_MAP_KORRECTION_SPIN, m_map_correction_spin);
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_MAP_FACTOR_EDIT, m_map_factor_edit);
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_MAP_KORRECTION_EDIT, m_map_correction_edit);

	DDX_Control(pDX, IDC_PD_ADCCOMPEN_UBAT_FACTOR_SPIN, m_ubat_factor_spin);
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_UBAT_KORRECTION_SPIN, m_ubat_correction_spin);
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_UBAT_FACTOR_EDIT, m_ubat_factor_edit);
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_UBAT_KORRECTION_EDIT, m_ubat_correction_edit);

	DDX_Control(pDX, IDC_PD_ADCCOMPEN_TEMP_FACTOR_SPIN, m_temp_factor_spin);
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_TEMP_KORRECTION_SPIN, m_temp_correction_spin);
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_TEMP_FACTOR_EDIT, m_temp_factor_edit);
	DDX_Control(pDX, IDC_PD_ADCCOMPEN_TEMP_KORRECTION_EDIT, m_temp_correction_edit);
	//}}AFX_DATA_MAP

  m_map_factor_edit.DDX_Value(pDX, IDC_PD_ADCCOMPEN_MAP_FACTOR_EDIT, m_params.map_adc_factor);
  m_map_correction_edit.DDX_Value(pDX, IDC_PD_ADCCOMPEN_MAP_KORRECTION_EDIT, m_params.map_adc_correction);

  m_ubat_factor_edit.DDX_Value(pDX, IDC_PD_ADCCOMPEN_UBAT_FACTOR_EDIT, m_params.ubat_adc_factor);
  m_ubat_correction_edit.DDX_Value(pDX, IDC_PD_ADCCOMPEN_UBAT_KORRECTION_EDIT, m_params.ubat_adc_correction);

  m_temp_factor_edit.DDX_Value(pDX, IDC_PD_ADCCOMPEN_TEMP_FACTOR_EDIT, m_params.temp_adc_factor);
  m_temp_correction_edit.DDX_Value(pDX, IDC_PD_ADCCOMPEN_TEMP_KORRECTION_EDIT, m_params.temp_adc_correction);
 }


BEGIN_MESSAGE_MAP(CADCCompenPageDlg, CDialog)
	//{{AFX_MSG_MAP(CADCCompenPageDlg)
	ON_EN_CHANGE(IDC_PD_ADCCOMPEN_MAP_FACTOR_EDIT, OnChangePdMAPFactorEdit)
	ON_EN_CHANGE(IDC_PD_ADCCOMPEN_MAP_KORRECTION_EDIT, OnChangePdMAPCorrectionEdit)

	ON_EN_CHANGE(IDC_PD_ADCCOMPEN_UBAT_FACTOR_EDIT, OnChangePdUBATFactorEdit)
	ON_EN_CHANGE(IDC_PD_ADCCOMPEN_UBAT_KORRECTION_EDIT, OnChangePdUBATCorrectionEdit)
	
	ON_EN_CHANGE(IDC_PD_ADCCOMPEN_TEMP_FACTOR_EDIT, OnChangePdTEMPFactorEdit)
	ON_EN_CHANGE(IDC_PD_ADCCOMPEN_TEMP_KORRECTION_EDIT, OnChangePdTEMPCorrectionEdit)

	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_MAP_FACTOR_EDIT, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_MAP_KORRECTION_EDIT, OnUpdateControls)
	
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_UBAT_FACTOR_EDIT, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_UBAT_KORRECTION_EDIT, OnUpdateControls)
	
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_TEMP_FACTOR_EDIT, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_TEMP_KORRECTION_EDIT, OnUpdateControls)

	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_MAP_CAPTION, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_UBAT_CAPTION, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_TEMP_CAPTION, OnUpdateControls)


	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_MAP_FACTOR_UNIT, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_MAP_KORRECTION_UNIT, OnUpdateControls)
	
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_UBAT_FACTOR_UNIT, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_UBAT_KORRECTION_UNIT, OnUpdateControls)
	
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_TEMP_FACTOR_UNIT, OnUpdateControls)
	ON_UPDATE_COMMAND_UI(IDC_PD_ADCCOMPEN_TEMP_KORRECTION_UNIT, OnUpdateControls)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//���� ���� ��������� ��������� ���������, �� ���� ����� ������� �������
void CADCCompenPageDlg::OnUpdateControls(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_enabled);
}

/////////////////////////////////////////////////////////////////////////////
// CADCCompenPageDlg message handlers

BOOL CADCCompenPageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	m_map_factor_spin.SetBuddy(&m_map_factor_edit);
	m_map_factor_edit.SetLimitText(6);
    m_map_factor_edit.SetDecimalPlaces(3);
    m_map_factor_edit.SetMode(CEditEx::MODE_FLOAT | CEditEx::MODE_SIGNED);
    m_map_factor_spin.SetRangeAndDelta(-2.0f,2.0f,0.001f);

	m_map_correction_spin.SetBuddy(&m_map_correction_edit);
	m_map_correction_edit.SetLimitText(6);
    m_map_correction_edit.SetDecimalPlaces(4);
    m_map_correction_edit.SetMode(CEditEx::MODE_FLOAT | CEditEx::MODE_SIGNED);
    m_map_correction_spin.SetRangeAndDelta(-2.0f,2.0f,0.0025f);

	m_ubat_factor_spin.SetBuddy(&m_ubat_factor_edit);
	m_ubat_factor_edit.SetLimitText(6);
    m_ubat_factor_edit.SetDecimalPlaces(3);
    m_ubat_factor_edit.SetMode(CEditEx::MODE_FLOAT | CEditEx::MODE_SIGNED);
    m_ubat_factor_spin.SetRangeAndDelta(-2.0f,2.0f,0.001f);

	m_ubat_correction_spin.SetBuddy(&m_ubat_correction_edit);
	m_ubat_correction_edit.SetLimitText(6);
    m_ubat_correction_edit.SetDecimalPlaces(4);
    m_ubat_correction_edit.SetMode(CEditEx::MODE_FLOAT | CEditEx::MODE_SIGNED);
    m_ubat_correction_spin.SetRangeAndDelta(-2.0f,2.0f,0.0025f);

	m_temp_factor_spin.SetBuddy(&m_temp_factor_edit);
	m_temp_factor_edit.SetLimitText(6);
    m_temp_factor_edit.SetDecimalPlaces(3);
    m_temp_factor_edit.SetMode(CEditEx::MODE_FLOAT | CEditEx::MODE_SIGNED);
    m_temp_factor_spin.SetRangeAndDelta(-2.0f,2.0f,0.001f);

	m_temp_correction_spin.SetBuddy(&m_temp_correction_edit);
	m_temp_correction_edit.SetLimitText(6);
    m_temp_correction_edit.SetDecimalPlaces(4);
    m_temp_correction_edit.SetMode(CEditEx::MODE_FLOAT | CEditEx::MODE_SIGNED);
    m_temp_correction_spin.SetRangeAndDelta(-2.0f,2.0f,0.0025f);

	UpdateData(FALSE);
	UpdateDialogControls(this,TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CADCCompenPageDlg::OnChangePdMAPFactorEdit() 
{
  UpdateData();	
  OnChangeNotify(); //notify event receiver about change of view content(see class ParamPageEvents)  		  
}

void CADCCompenPageDlg::OnChangePdMAPCorrectionEdit() 
{
  UpdateData();
  OnChangeNotify();   		    
}


void CADCCompenPageDlg::OnChangePdUBATFactorEdit() 
{
  UpdateData();	
  OnChangeNotify(); //notify event receiver about change of view content(see class ParamPageEvents)  		  
}

void CADCCompenPageDlg::OnChangePdUBATCorrectionEdit() 
{
  UpdateData();
  OnChangeNotify();   		    
}

void CADCCompenPageDlg::OnChangePdTEMPFactorEdit() 
{
  UpdateData();	
  OnChangeNotify(); //notify event receiver about change of view content(see class ParamPageEvents)  		  
}

void CADCCompenPageDlg::OnChangePdTEMPCorrectionEdit() 
{
  UpdateData();
  OnChangeNotify();   		    
}


//����������/���������� ���������� (���� ���������)
void CADCCompenPageDlg::Enable(bool enable)
{
  m_enabled = (enable) ? TRUE : FALSE;  
  if (::IsWindow(m_hWnd))
    UpdateDialogControls(this,TRUE);
}

//��� � �����������?
bool CADCCompenPageDlg::IsEnabled(void)
{
  return (m_enabled) ? true : false;
}

//��� ������� ���������� ������������ ����� ���� �������� ������ �� �������
void CADCCompenPageDlg::GetValues(SECU3IO::ADCCompenPar* o_values)
{
  ASSERT(o_values);
  UpdateData(TRUE); //�������� ������ �� ������� � ����������
  memcpy(o_values,&m_params,sizeof(SECU3IO::ADCCompenPar));
}

//��� ������� ���������� ������������ ����� ���� ������� ������ � ������
void CADCCompenPageDlg::SetValues(const SECU3IO::ADCCompenPar* i_values)
{
  ASSERT(i_values);
  memcpy(&m_params,i_values,sizeof(SECU3IO::ADCCompenPar));
  UpdateData(FALSE); //�������� ������ �� ���������� � ������
}