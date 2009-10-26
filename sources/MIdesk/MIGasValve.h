
#pragma once

#include "IMeasInstrument.h"
#include "ui-core\LEDIndicator.h"

class AFX_EXT_CLASS CMIGasValve : public IMeasInstrument  
{
 public:
  CMIGasValve();
  virtual ~CMIGasValve();

  void DDX_Controls(CDataExchange* pDX, int nIDC_led, int nIDC_caption);

  //-------interface-----------------------
  virtual void Create(void);
  virtual void Scale(float i_x_factor, float i_y_factor);
  virtual void SetValue(float value);
  virtual float GetValue(void);
  virtual void Show(bool show);
  virtual void Enable(bool enable);
  virtual bool IsVisible(void);
  virtual bool IsEnabled(void);	
  virtual void SetLimits(float loLimit, float upLimit);
  virtual void SetTicks(int number);
  //---------------------------------------

 private:
  CLEDIndicator m_led;
  CStatic m_caption;
  bool m_prev_enable;
};
