/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Gorlovka

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   contacts:
              http://secu-3.narod.ru
              email: secu-3@yandex.ru
*/

#include <windows.h>
#include "types.h"

#ifndef _BOOTLOADER_
#define _BOOTLOADER_

//��� ��������� ������� ��� ������ � ����������� ���������� ������������� �� ����� ������
class AFX_EXT_CLASS IBLDEventHandler
{
 public:
  struct poolUpdateUI
  {
   void Set(int i_opcode,int i_total,int i_current)
   {
    opcode  = i_opcode;
    total   = i_total;
    current = i_current;
   }

   void operator=(poolUpdateUI& i_other)
   {
    opcode  = i_other.opcode;
    total   = i_other.total;
    current = i_other.current;
   }

   int opcode;
   int total;
   int current;
  };

  //����������� ������� - ������������ ����� ������ ������ ����������� ����������� ��� �������!
  virtual void OnUpdateUI(IBLDEventHandler::poolUpdateUI* ip_data) = 0;
  virtual void OnBegin(const int opcode, const int status) = 0;
  virtual void OnEnd(const int opcode, const int status) = 0;

};


class CComPort;
class AFX_EXT_CLASS CBootLoader
{
 public:
  typedef CRITICAL_SECTION CSECTION;

  CBootLoader();
  virtual ~CBootLoader();

  bool Initialize(CComPort* p_port, const DWORD uart_seed);
  void SetEventHandler(IBLDEventHandler* i_pEventHandler) 
  { 
   m_pEventHandler = i_pEventHandler;
  }

  void SwitchOn(bool state, bool i_force_reinit = false);
  bool GetWorkState(void) {return m_work_state;};

  bool Terminate(void);

  //starts the specified operation
  //!!! i_addr ������������ ������ ��� ������ FLASH
  bool StartOperation(const int opcode,BYTE* io_data,int i_size,int i_addr = 0);
 
  inline bool IsIdle(void) const {return (m_ThreadBusy)?false:true;};

  inline HANDLE    GetThreadHandle(void) const {return m_hThread;}
  inline DWORD     GetThreadId(void) const     {return m_ThreadId;}
  inline CComPort* GetPortHandle(void) const   {return m_p_port;}
  inline int       GetLastError() const        {return m_ErrorCode;}

  inline CSECTION* GetSyncObject(void) const;
  inline void EnterCriticalSection(void) const;
  inline void LeaveCriticalSection(void) const;

  //���������� true ���� ��� ��
  inline int Status(void) const {return ((m_ErrorCode==0)?true:false);}     

  static DWORD WINAPI BackgroundProcess(LPVOID lpParameter);

  enum {BL_SIGNATURE_STR_LEN = 24};
  enum {EEPROM_SIZE = 512};         //size of EEPROM
  enum {EEPROM_WR_DELAY_MULTIPLIER = 3};
  enum {FLASH_PG_ERASE_DELAY = 30}; 

  enum 
  {
   FLASH_PAGE_SIZE = 128,         //������ �������� ������ �������� � ������
   FLASH_TOTAL_SIZE = 16384,      //����� ������ ������ �������� ����������������
   FLASH_BL_SECTION_SIZE = 512,   //���-�� ���� ���������� ��� ���������� (�� ������ ���������� ���� ���� ����� ������)
   FLASH_APP_SECTION_SIZE = FLASH_TOTAL_SIZE - FLASH_BL_SECTION_SIZE, //����� �������� ����� ����������
   FLASH_ONLY_CODE_SIZE = 0x32F0,  //������ ���� ��� ������, ������� � �����

   //����� �������� ��������������� ����� ���� ��������� � ����� �����������. ���� ������ ������ ������������, ������ 
   //� ����������� ����� ��������.
   FLASH_ONLY_OVERHEAD_SIZE = FLASH_APP_SECTION_SIZE - FLASH_ONLY_CODE_SIZE
  };

  enum //������ �������������� ������
  {
   BL_OP_READ_FLASH     = 1,
   BL_OP_WRITE_FLASH    = 2,
   BL_OP_READ_EEPROM    = 3,
   BL_OP_WRITE_EEPROM   = 4,
   BL_OP_READ_SIGNATURE = 5,
   BL_OP_EXIT = 6
  };

  enum
  {
   BL_ERROR_NOANSWER   = 1,
   BL_ERROR_CHKSUM     = 2,
   BL_ERROR_WRONG_DATA = 3
  };

  class xThread {};

 private:

  CSECTION* mp_csection;
  CComPort* m_p_port;
  HANDLE    m_hThread;
  HANDLE    m_hAwakeEvent;
  DWORD     m_ThreadId;
  bool      m_ThreadBusy;
  int       m_ErrorCode;

  volatile bool m_is_thread_must_exit;
  bool      m_work_state;
  DWORD     m_uart_speed;

  enum {PENDING_DATA_QUEUE_SIZE = 256};
  struct PendingData
  {
   IBLDEventHandler::poolUpdateUI m_update_ui;
  }m_pending_data[PENDING_DATA_QUEUE_SIZE];

  int m_current_pending_data_index;

  IBLDEventHandler* m_pEventHandler; //��������� �� �����-���������� ������� (����������� ��������� IBLDEventHandler)

  struct  //��������� ������ ����������� ��� ���������� ��������
  {
   int   opcode;
   BYTE* data;
   int   addr;
   int   size;
  }m_opdata;

  bool IsOpcodeValid(const int opcode);
  bool FLASH_ReadOnePage(int n_page,BYTE* o_buf,int total_size,int* current);

  void EventHandler_OnUpdateUI(const int i_opcode,const int i_total,const int i_current);
  void EventHandler_OnBegin(const int i_opcode, const int i_status);
  void EventHandler_OnEnd(const int i_opcode, const int i_status);
};

#endif //_BOOTLOADER_
