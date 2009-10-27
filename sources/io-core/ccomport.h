 /****************************************************************
 *       SECU-3  - An open source, free engine control unit
 *    Designed by Alexey A. Shabelnikov. Ukraine, Gorlovka 2007.
 *       Microprocessors systems - design & programming.
 *    contacts:
 *              http://secu-3.narod.ru
 *              ICQ: 405-791-931
 ****************************************************************/

#include <windows.h>
#include <string>
#include "common/unicodesupport.h"

#ifndef _CCOMPORT_
#define _CCOMPORT_

//����� ��� ������ � COM - ������
class AFX_EXT_CLASS CComPort 
{
private:  //local variables
	bool            m_bPortReady;
    HANDLE          m_hCom;         //����� ����� ��� �����
	_TSTRING        m_sComPort;     //��� ����� - �����
    DCB             m_dcb;          //describes a COM port
    COMMTIMEOUTS    m_CommTimeouts;
	UINT            dwInQueue;      //������ ������ ���������
	UINT            dwOutQueue;     //������ ������ �����������
    UINT            m_snd_err_num;  //������ ���-�� ������ ������ � ����
	UINT            m_rcv_err_num;  //������ ���-�� ������ ������ �� �����

	void _LoadDefaultTimeouts(void);

public:   //public functions
    CComPort(const _TSTRING&,UINT,UINT);        //�����������
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
		};
	};

	typedef xInitialize xSetTimeout;
};
 
#endif  //_CCOMPORT_