
#pragma once

#include "ControlApp.h"
#include "common/unicodesupport.h"


class AFX_EXT_CLASS LogWriter : public IAPPEventHandler
{
public:
 LogWriter();
 virtual ~LogWriter();

 //���������� �������� ������. ���� ��������� ������ ��������, �� ���������� false.
 //i_folder - ������ ���� � �������� ��� ����� �������� ���-����.
 //o_full_file_name - ���������� ������� ������ ��� ����� � ������� �������� ������
 bool BeginLogging(const _TSTRING& i_folder, _TSTRING* o_full_file_name = NULL);

 //������������ �������� ������
 void EndLogging(void);

 //���������� true, ���� � ������� ������ ���� ������ 
 bool IsLoggingInProcess(void);

public:

  //����� ��� ����������� ������ ��������� � �������� ������
  virtual void OnPacketReceived(const BYTE i_descriptor, SECU3IO::SECU3Packet* ip_packet);
  virtual void OnConnection(const bool i_online);

private:
  FILE* m_out_handle;
  bool  m_is_busy;
};