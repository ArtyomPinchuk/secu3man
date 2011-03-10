
#pragma once

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// Enum Types
enum MeterMemberEnum
{
 meter_title,   //��������� (�������� �������)
 meter_needle,  //�������
 meter_grid,    //������� �����
 meter_value,   //�������� �������� ������������ �����
 meter_labels,  //������� �������� ������� �������
 meter_unit,    //������� ���������
 meter_bground, //���
};

/////////////////////////////////////////////////////////////////////////////
// CAnalogMeter command target
class AFX_EXT_CLASS CAnalogMeter
{
 public:
  CAnalogMeter() ;
  virtual ~CAnalogMeter();

 // Operations
 public:
  void ShowMeter(CDC *pDC, CRect rectBorder);
  virtual void UpdateNeedle(CDC *pDC, double dPos);

  void SetColor(enum MeterMemberEnum meter_member, COLORREF Color);
  void SetState(enum MeterMemberEnum meter_member, bool State);
  void SetRange(double dMin, double dMax);
  void SetFontScale(int nFontScale);
  void SetLabelsDecimals(int nRangeDecimals);
  void SetValueDecimals(int nValueDecimals);
  void SetTitle(CString strTitle);
  void SetUnit(CString strUnit);
  void SetGridLineWidth(int width)
  {
   m_nGridLineWidth = width;  // set pen/brush colors
   ActuateColors();
  }

  void SetMeterSize(double half_angle) {m_dLimitAngleDeg = half_angle;}

  void AddAlertZone(double start,double end, COLORREF color);
  void ResetAlertZones(void);

  void    GetColor(enum MeterMemberEnum meter_member, COLORREF* pColor);
  void    GetState(enum MeterMemberEnum meter_member, bool* pState);
  double  GetMinRange()      {return m_dMinScale; };
  double  GetMaxRange()      {return m_dMaxScale; };
  int     GetFontScale()     {return m_nFontScale; };
  int     GetRangeDecimals() {return m_nLabelsDecimals; };
  int     GetValueDecimals() {return m_nValueDecimals; };
  CString GetTitle()         {return m_strTitle; };
  CString GetUnit()          {return m_strUnit; };
  double  GetNeedlePos(void) {return m_dNeedlePos;};
  void    SetTickNumber(int number) { m_nTickNumber = number;};

 protected:
  bool m_boolUseBitmaps;
  bool m_boolForceRedraw;

 // Implementation
 private:
  double DegToRad(double deg);
  void DrawPie(const CRect& Bounds,double start_value, double end_value, COLORREF color);
  void DrawGrid(const CRect& Bounds);
  void DrawChord(const CRect& Bounds);

  void DrawScale();
  void DrawNeedle();
  void ShowMeterImage(CDC *pDC);
  void ActuateColors();
  void DrawAlertZones(const CRect& Bounds);

  struct AlertZone
  {
   double start;
   double end;
   COLORREF color;
  };

  std::vector<AlertZone*> m_AlertZones;

  bool m_swTitle;
  bool m_swGrid;
  bool m_swValue;
  bool m_swLabels;
  bool m_swUnit;
  bool m_swNeedle;

  COLORREF m_colorTitle;
  COLORREF m_colorNeedle;
  COLORREF m_colorGrid;
  COLORREF m_colorValue;
  COLORREF m_colorLabels;
  COLORREF m_colorBGround;

  int m_nFontScale;
  int m_nLabelsDecimals;
  int m_nValueDecimals;
  int m_nRectWidth;
  int m_nRectHeight;
  int m_nCXPix;
  int m_nCYPix;
  int m_nRadiusPix;
  int m_nLeftLimitXPix;
  int m_nLeftLimitYPix;
  int m_nRightLimitXPix;
  int m_nRightLimitYPix;
  int m_nHalfBaseWidth;
  int m_nTextBaseSpacing;
  int m_nFontHeight;
  int m_nGridLineWidth;
  int m_nTickNumber;

  double m_dPI;
  double m_dLimitAngleDeg;
  double m_dLimitAngleRad;
  double m_dRadiansPerValue;
  double m_dNeedlePos;
  double m_dMinScale;
  double m_dMaxScale;

  CString m_strTitle;
  CString m_strUnit;

  CDC     m_dcGrid;
  CBitmap m_bitmapGrid;
  CBitmap *m_pbitmapOldGrid;

  CDC     m_dcNeedle ;
  CBitmap m_bitmapNeedle;
  CBitmap *m_pbitmapOldNeedle;

  CRect m_rectDraw;
  CRect m_rectGfx;
  CRect m_rectOwner;
  CRect m_rectMinValue;
  CRect m_rectMaxValue;
  CRect m_rectValue;

  CFont m_fontValue;
  CString m_fontType;

  CPen    m_PenG_Grid;
  CBrush  m_BrushG_Grid;
  CPen    m_PenG_BGround;
  CBrush  m_BrushG_BGround;
  CPen    m_PenN_Needle;
  CBrush  m_BrushN_Needle;
  CPen    m_PenN_BGround;
  CBrush  m_BrushN_BGround;
};

/////////////////////////////////////////////////////////////////////////////
