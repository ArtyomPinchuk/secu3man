#pragma once


//���������-����������� ��������� ��� ������� EEPROM.

class AFX_EXT_CLASS EEPROMDataMediator
{
  public:
     EEPROMDataMediator();
    ~EEPROMDataMediator();
    
     static int GetDefParamsStartAddress(void);
};
