/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Kiev

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

/** \file ccomport.cpp
 * \author Alexey A. Shabelnikov
 */

#include "stdafx.h"
#include "ccomport.h"
#include <stdio.h>
#include <string.h>
#include <string>

#pragma warning (disable: 4800)  //'type' : forcing value to bool 'true' or 'false' (performance warning)

CComPort::xRuntimeError::xRuntimeError(const _TSTRING& str) 
: m_detail_str(str)
{
 //empty
};

LPCTSTR CComPort::xRuntimeError::GetDetailStr(void) const
{
 return m_detail_str.c_str(); //save details
}

void CComPort::_LoadDefaultTimeouts(void)
{
 //default timeouts values
 m_CommTimeouts.ReadIntervalTimeout         = 200;
 m_CommTimeouts.ReadTotalTimeoutConstant    = 200;
 m_CommTimeouts.ReadTotalTimeoutMultiplier  = 200;
 m_CommTimeouts.WriteTotalTimeoutConstant   = 200;
 m_CommTimeouts.WriteTotalTimeoutMultiplier = 200;
}

//�����������
CComPort::CComPort(const _TSTRING& sComPort, UINT dwInQueuep, UINT dwOutQueuep)
: m_sComPort(sComPort)
, m_bPortReady(FALSE)
, m_snd_err_num(0)  //��� ������ ������ � ����
, m_rcv_err_num(0)  //��� ������ ������ �� �����
, dwInQueue(dwInQueuep)
, dwOutQueue(dwOutQueuep)
, m_hCom(INVALID_HANDLE_VALUE)
{
 _LoadDefaultTimeouts();
}

//����������� (�������� ����� ����� ����� (1,2,3,4))
CComPort::CComPort(int nComPort, UINT dwInQueuep, UINT dwOutQueuep)
: m_bPortReady(FALSE)
, m_snd_err_num(0)  //no port writing errors
, m_rcv_err_num(0)  //no port reading errors
, dwInQueue(dwInQueuep)
, dwOutQueue(dwOutQueuep)
{
 TCHAR buff[32];
 wsprintf(buff,_T("COM%d"),nComPort);
 m_sComPort = buff;

 _LoadDefaultTimeouts();
}

//����������
CComPort::~CComPort()
{
 //empty
}

//
// ������������� COM ����� (�����������)
//
//exceptions: xInitialize
bool CComPort::Initialize(DWORD baud, BYTE parity, BYTE stopbit, int Dtr, int Rts)
{
 DWORD dwRC;
 DWORD dwError;
 TCHAR sMsg[1024];
 LPVOID  lpMsgBuf = NULL;

 m_bPortReady = TRUE; // All is OK

 //���� � ������ ������� ���� ������� �� ������� �� ������� ������ � �������, ��
 //������ ����� ������� ����� ������ ���������� �������.
 for(int i = 0; i < 5; i++)
 {
  m_hCom = CreateFile(m_sComPort.c_str(),GENERIC_READ | GENERIC_WRITE,
                      0,             // ���������� �������� ����
                      NULL,          // no security
                      OPEN_EXISTING, // port is existing file
                      0,             // no overlapped I/O
                      NULL);         // null template
  if (m_hCom!=INVALID_HANDLE_VALUE)
   break;
  //�������� ������� ��������� ������ ���������
  m_hCom = CreateFile((_T("\\\\.\\") + m_sComPort).c_str(),GENERIC_READ | GENERIC_WRITE,
                       0,             // ���������� �������� ����
                       NULL,          // no security
                       OPEN_EXISTING, // port is existing file
                       0,             // no overlapped I/O
                       NULL);         // null template
  if (m_hCom!=INVALID_HANDLE_VALUE)
   break;

  if (GetLastError()!=ERROR_ACCESS_DENIED)
   break; //��� ������ ������� �� ������ ����� �������

  Sleep(250);
 }

 if (m_hCom == INVALID_HANDLE_VALUE)
 {
  m_bPortReady = FALSE;
  dwError = GetLastError();

  // ��������� ������
  dwRC = FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,
         dwError, //  �� GetLastError(),
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
         (LPTSTR) &lpMsgBuf,
         0,
         NULL);

  if (dwRC && lpMsgBuf)
  {
   wsprintf(sMsg, _T("COM open failed: Port=%s Error=%d - %s"),m_sComPort.c_str(), dwError, lpMsgBuf);
   throw xInitialize(sMsg);
  }
  else
  {
   wsprintf(sMsg, _T("COM open failed: Port=%s Error=%d "),m_sComPort.c_str(), dwError);
   throw xInitialize(sMsg);
  } // end if
  if (dwRC && lpMsgBuf)
  {
   LocalFree( lpMsgBuf );
  } // end if
 } // end if

 if (m_bPortReady)
 {
  m_bPortReady = SetupComm(m_hCom, dwInQueue,dwOutQueue); // Size of buffer
  if (!m_bPortReady)
  {
   dwError = GetLastError();
   wsprintf(sMsg, _T("SetupComm failed: Port=%s Error=%d"),m_sComPort.c_str(), dwError);
   throw xInitialize(sMsg);
  } // end if
 } // end if

 if (m_bPortReady)
 {
  m_bPortReady = GetCommState(m_hCom, &m_dcb);
  if (!m_bPortReady)
  {
   dwError = GetLastError();
   wsprintf(sMsg, _T("GetCommState failed: Port=%s Error=%d"),m_sComPort.c_str(), dwError);
   throw xInitialize(sMsg);
  } // end if
 } // end if

 if (m_bPortReady)
 {
  m_dcb.BaudRate      = baud;
  m_dcb.ByteSize      = 8;
  m_dcb.Parity        = parity;
  m_dcb.StopBits      = stopbit;
  m_dcb.fAbortOnError = FALSE;

  //disable hardware and XON/XOFF flow control
  m_dcb.fOutxCtsFlow = FALSE;
  m_dcb.fOutxDsrFlow = FALSE;
  m_dcb.fDsrSensitivity = FALSE;
  m_dcb.fOutX = FALSE;
  m_dcb.fInX = FALSE;

  //���������� ���� ���� ���. ��������� ����� ����������
  if (Dtr==0)
   m_dcb.fDtrControl   = DTR_CONTROL_DISABLE;
  else if (Dtr==1)
   m_dcb.fDtrControl   = DTR_CONTROL_ENABLE;
  if (Rts==0)
   m_dcb.fRtsControl   = RTS_CONTROL_DISABLE;
  else if (Rts==1)
   m_dcb.fRtsControl   = RTS_CONTROL_ENABLE;


  m_bPortReady = SetCommState(m_hCom, &m_dcb);
  if (!m_bPortReady)
  {
   dwError = GetLastError();
   wsprintf(sMsg, _T("SetCommState failed: Port=%s Error = %d"),m_sComPort.c_str(), dwError);
   throw xInitialize(sMsg);
  }
 } // end if

 if (m_bPortReady)
 {
  m_bPortReady = SetCommTimeouts (m_hCom, &m_CommTimeouts);
  if (!m_bPortReady)
  {
   dwError = GetLastError();
   wsprintf(sMsg, _T("SetCommTimeouts failed: Port=%s Error = %d"),m_sComPort.c_str(), dwError);
   throw xInitialize(sMsg);
  } // end if
 } // end if

 return m_bPortReady;
}

//
// ������������� COM �����
//
bool CComPort::Initialize(DWORD baud, BYTE parity, BYTE stopbit)
{                    //�� �������� ����. �����
 return Initialize(baud, parity, stopbit, -1, -1);
}

//
// ������������� COM �����
//
bool CComPort::Initialize(const _TSTRING& i_sComPort, DWORD baud, BYTE parity, BYTE stopbit, int Dtr, int Rts)
{
 m_sComPort = i_sComPort;
 return Initialize(baud,parity,stopbit,Dtr,Rts);
}

//
// ����������� COM ����
//
void CComPort::Terminate()
{
 CloseHandle(m_hCom);
 m_hCom = INVALID_HANDLE_VALUE;
}

//
// �������� ��������� ���� ����� COM - ����
// Return: TRUE - OK, FALSE - error
//
bool CComPort::SendByte(unsigned char byte)
{
 DWORD  numwr;
 //���������� � ���� 1 ����
 WriteFile(m_hCom,&byte,1,&numwr,NULL);
 if (numwr != 1)
 {
  m_snd_err_num++;
  return false;    //error
 }//if
 else
 {
  return true;     //Ok
 }//else
}

//
// ������  ���� �� COM - �����
// Return: TRUE - OK, FALSE - error
//
bool CComPort::RecvByte(unsigned char* byte)
{
 DWORD numrd;
 if (!byte) return false;   //bad pointer

 //������ �� ����� 1 ����
 ReadFile(m_hCom,byte,1,&numrd,NULL);
 if (numrd!=1)
 {
  m_rcv_err_num++;
  return false;   //error
 } //if
 else
 {
  return true;   //Ok
 }//else
}

//
// �������� ���� ������ ����� COM - ����
// data     - ����� ������ � ������� ��� ��������
// datasize - ������ ������ � ������ (� ������)
// Return: TRUE - OK, FALSE - error
bool CComPort::SendBlock(BYTE* data, UINT datasize)
{
 DWORD  numwr;
 if (!data) return false;   //bad pointer

 //���������� � ���� 1 ����
 WriteFile(m_hCom,data,datasize,&numwr,NULL);
 if (numwr != datasize)
 {
  m_snd_err_num++;
  return false;    //error
 }//if
 else
 {
  return true;     //Ok
 }//else
}

//
// �������� ���� ������ ����� COM - ����
// data     - ����� ������ ��� ������ ������
// datasize - ������ ������ ��� ������ (� ������)
// Return: TRUE - OK, FALSE - error
bool CComPort::RecvBlock(BYTE* data,UINT datasize)
{
 DWORD numrd;
 if (!data) return false;   //bad pointer

 //������ �� ����� ��������� ���-�� ����
 ReadFile(m_hCom,data,datasize,&numrd,NULL);
 if (numrd!=datasize)
 {
  m_rcv_err_num++;
  return false;   //error
 } //if
 else
 {
  return true;   //Ok
 }//else
}

//Returns port handle
//
HANDLE CComPort::GetHandle(void) const
{
 return m_hCom;
}

//set DTR line into specified state
//
bool CComPort::SetDTR(bool state)
{
 if (state)
  m_dcb.fDtrControl = DTR_CONTROL_ENABLE;
 else
  m_dcb.fDtrControl = DTR_CONTROL_DISABLE;

 //TODO: maybe use of EscapeCommFunction() will be more suitable?
 return SetCommState(m_hCom,&m_dcb);
}

//���. � ����. ����. ����� RTS
//
bool CComPort::SetRTS(bool state)
{
 if (state)
  m_dcb.fRtsControl = RTS_CONTROL_ENABLE;
 else
  m_dcb.fRtsControl = RTS_CONTROL_DISABLE;

 //TODO: maybe use of EscapeCommFunction() will be more suitable?
 return SetCommState(m_hCom,&m_dcb);
}

//Set TXD line into specified state
//
bool CComPort::SetTXD(bool state)
{
 //TODO: maybe use of EscapeCommFunction() will be more suitable?
 if (state)
  return SetCommBreak(m_hCom);
 else
  return ClearCommBreak(m_hCom);
}

void CComPort::Set9bitState(bool state)
{
 if (state)
 {//9-� ��� ����� 1
  m_dcb.Parity =  SPACEPARITY;
  SetCommState(m_hCom,&m_dcb);
 }
 else
 {//9-� ��� ����� 0
  m_dcb.Parity =  MARKPARITY;
  SetCommState(m_hCom,&m_dcb);
 }
}

UINT CComPort::GetRecvErrNum(void) const
{
 return m_rcv_err_num;
}

UINT CComPort::GetSendErrNum(void) const
{
 return m_snd_err_num;
}

void  CComPort::ResetPortErrors(void)
{
 m_rcv_err_num = 0;
 m_snd_err_num = 0;
}

//exceptions: xSetTimeout
bool CComPort::SetTimeouts(COMMTIMEOUTS* i_cto)
{
 TCHAR sMsg[1024];
 DWORD dwError;
 memcpy(&m_CommTimeouts,i_cto,sizeof(COMMTIMEOUTS));
 if (m_bPortReady)
 {
  m_bPortReady = SetCommTimeouts (m_hCom, &m_CommTimeouts);
  if (!m_bPortReady)
  {
   dwError = GetLastError();
   wsprintf(sMsg, _T("SetCommTimeouts failed: Port=%s Error = %d"),m_sComPort.c_str(), dwError);
   throw xSetTimeout(sMsg);
  } // end if
 else
  return true;
 } // end if
 return false;
}


bool CComPort::GetTimeouts(COMMTIMEOUTS* o_cto)
{
 if (o_cto==NULL)
  return FALSE;
 return GetCommTimeouts(m_hCom, o_cto);
}


bool CComPort::Purge(const DWORD dwFlags) const
{
 return PurgeComm(m_hCom,dwFlags);
}

bool CComPort::SendASCII(const char* str)
{
 if (!str) return false;
  return SendBlock((BYTE*)str,strlen(str));
}


bool CComPort::GetState(DCB* o_dcb)
{
 if (o_dcb)
  return false;
 return GetCommState(m_hCom, o_dcb);
}

bool CComPort::SetState(DCB* i_dcb)
{
 if (i_dcb)
  return false;
 memcpy(&m_dcb,i_dcb,sizeof(DCB));
 return SetCommState(m_hCom, i_dcb);
}

//�� ��������� ���-�� ������ ������, � ���������� �� ������ �� real_readed
//��������� ���������� ������� ����������� ����. ����� ����� ����������� ������.
bool CComPort::RecvBlock(BYTE* data, UINT datasize, DWORD* real_read)
{
 if (!data || !real_read) return false;   //������ ���������

 //������ �� ����� ��������� ���-�� ����
 DWORD errors = 0;
 ReadFile(m_hCom,data,datasize,real_read,NULL);
 if (ClearCommError(m_hCom, &errors, NULL) && errors)
 {
  *real_read = 0;
  return false; //������ ������
 }

 if (*real_read!=datasize)
  return false;  //��������� ������ ������ ��� �����������
 else
  return true;  //��������� �������, ������� �����
}

float CComPort::ms_need_for_one_byte_8N1(DWORD baud_rate)
{
 return ((1.0f / (((float)baud_rate) / 10.0f)) * 1000.0f);
}

DCB& CComPort::AccessDCB(void)
{
 return m_dcb;
}

bool CComPort::SetState()
{
 return SetCommState(m_hCom,&m_dcb);
}

bool CComPort::IsInitialized(void)
{
 return ((m_hCom==INVALID_HANDLE_VALUE) ? false : true);
}
