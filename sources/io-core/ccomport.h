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
              http://secu-3.org
              email: shabelnikov@secu-3.org
*/

#include <windows.h>
#include <string>
#include "iocore_api.h"
#include "common/unicodesupport.h"

#ifndef _CCOMPORT_
#define _CCOMPORT_

//����� ��� ������ � COM - ������
class IOCORE_API CComPort
{
 public:   //public functions

  CComPort(const _TSTRING&,UINT,UINT);//�����������
  CComPort(int,UINT,UINT);
  ~CComPort();                        //����������
  void   Set9bitState(BOOL state);

  BOOL   Initialize(DWORD,BYTE,BYTE); //������������� �����
  BOOL   Initialize(DWORD,BYTE,BYTE,char,char);
  BOOL   Initialize(const _TSTRING& i_sComPort,DWORD,BYTE,BYTE,char,char);

  VOID   Terminate();                 //�������� �����
  bool   SendByte(unsigned char);     //�������� ������ �����
  bool   RecvByte(unsigned char*);    //����� ������ �����
  bool   SendBlock(BYTE*,UINT);       //�������� ��������� ���-�� ����
  bool   RecvBlock(BYTE*,UINT);       //����� ��������� ���-�� ����
  bool   SendASCII(const char* str);
  HANDLE GetHandle(void) const;       //���������� ����� �����
  bool   SetDTR(bool);                //���. � ����. ����. ����� DTR
  bool   SetRTS(bool);                //���. � ����. ����. ����� RTS
  bool   SetTXD(bool);                //���. � ����. ����. ����� TXD
  UINT   GetRecvErrNum(void) const;
  UINT   GetSendErrNum(void) const;
  void   ResetPortErrors(void);
  BOOL   Purge(const DWORD dwFlags = PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR) const;
  BOOL   SetTimeouts(COMMTIMEOUTS* i_cto);
  BOOL   GetTimeouts(COMMTIMEOUTS* o_cto);
  bool   GetState(DCB* o_dcb);
  bool   SetState(DCB* i_dcb);
  bool   RecvBlock(BYTE* data,UINT datasize,DWORD* real_readed);

  //��, �� - ������ float �.� ����� ����� ��������� ������� � ������ ������ ����� ������� ����� �������
  //�������� ������ � ������� ������� ������������ ��� N ����.
  inline static float ms_need_for_one_byte_8N1(DWORD baud_rate)
  {
   return ((1.0f / (((float)baud_rate) / 10.0f)) * 1000.0f);
  }

  inline DCB* AccessDCB(void)
  {
   return &m_dcb;
  }

  inline BOOL SetState()
  {
   return SetCommState(m_hCom,&m_dcb);
  }

  inline bool IsInitialized(void)
  {
   return ((m_hCom==INVALID_HANDLE_VALUE) ? false : true);
  }

  enum { MAX_PORTS_ALLOWED = 32};

  class xInitialize
  {
   _TSTRING m_detail_str;       //string that contain details about exception
   public:
    xInitialize(const _TSTRING& str) : m_detail_str(str){};
    LPCTSTR GetDetailStr(void) const
    {
     return m_detail_str.c_str(); //save details
    }
  };

  typedef xInitialize xSetTimeout;

 private:
  void _LoadDefaultTimeouts(void);

  bool            m_bPortReady;
  HANDLE          m_hCom;         //����� ����� ��� �����
  _TSTRING        m_sComPort;     //��� ����� - �����
  DCB             m_dcb;          //describes a COM port
  COMMTIMEOUTS    m_CommTimeouts;
  UINT            dwInQueue;      //������ ������ ���������
  UINT            dwOutQueue;     //������ ������ �����������
  UINT            m_snd_err_num;  //������ ���-�� ������ ������ � ����
  UINT            m_rcv_err_num;  //������ ���-�� ������ ������ �� �����
};

#endif  //_CCOMPORT_
