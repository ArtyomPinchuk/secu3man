 /****************************************************************
 *
 *  Created by Alexey A. Shabelnikov. Ukraine, Gorlovka 2008. 
 *   ICQ: 405-791-931. e-mail: shabelnikov-stc@mail.ru
 *  Microprocessors systems - design & programming.
 *
 *****************************************************************/

#pragma once

#include <map>


//��� ��������� ���������� ���������� ��� �������� � ����������� ����������� this, ����� ���������
//������������ �� �������������� �������, ������ ������� �������� ��������� �� CObjectTimer 
static std::map<int,void*> g_object_timer_instance_map;


template<class T> class CObjectTimer  
{
  public:
    typedef void (T::*msgHandlerType) (void);

	CObjectTimer(T* ip) : m_pDispatcher(ip), msgHandler(NULL) {};
	CObjectTimer()      : m_pDispatcher(NULL), msgHandler(NULL) {};

	//��� �������� ���������� �������-������� ���� ���������� ������ �� ���� �������
	virtual ~CObjectTimer() 
	{
	 KillTimer();	
	};

	//������������� ��� ����������� ��������� � ��������� ������ 
    void SetTimer(T* object, msgHandlerType i_function, int interval)
	{
      SetMsgHandler(i_function);
      m_pDispatcher = object;
      SetTimer(interval);
	};

    //��������� ������ ���� �������� ����� ������ ���� �������������� � ����������� ��� ������� 
	//��������� �� ������ � ���� ������� ������� SetMsgHandler() ��� ��������� �����������
    void SetTimer(int interval)
	{
	  m_timer_id = ::SetTimer(NULL,0,interval,(TIMERPROC)&TimerProc);
      g_object_timer_instance_map[m_timer_id] = this;
	}

    //������� ������ ���� �������� 
    void KillTimer(void)
	{
	  ::KillTimer(NULL,m_timer_id);
	  if (!g_object_timer_instance_map.empty())
	    if (g_object_timer_instance_map.find(m_timer_id)!=g_object_timer_instance_map.end())	  
          g_object_timer_instance_map.erase(m_timer_id);  	 
	}

	//��� ������� ��������� �� ����� ����� �������-�����: void T::Function(void)
    inline void SetMsgHandler(msgHandlerType i_function)
	{
      msgHandler = i_function;
	}

    inline void SetMsgHandler(T* ip_this, msgHandlerType i_function)
	{
      msgHandler = i_function;
      m_pDispatcher = ip_this;
	}

  private:
    msgHandlerType msgHandler;
    T*   m_pDispatcher; 
    UINT_PTR m_timer_id;
    int  m_interval_ms;

  private:
    
    static inline VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
	{
     //welcome to hell :-D ... � ��� ������ ��� � �++ ���� ��������!?
      CObjectTimer* p_this = reinterpret_cast<CObjectTimer*>(g_object_timer_instance_map[idEvent]); 
      ASSERT(p_this);
      if (!p_this)  return;

      T *x = p_this->m_pDispatcher;
      ASSERT(x);
	  if (!x) return;
	  	  
      //���������� ���������� �������� ���������� ������ ������ T, ������ ���� ������� ������������������
	  if (p_this->msgHandler!=NULL)
	  {
	    (x->*(p_this->msgHandler))(); 	  
	  }
	};
};


