/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : timer.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��6��19��
  ����޸�   :
  ��������   : ��ʱ��������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��19��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

/*---------------------------------���Ͷ���----------------------------------*/

typedef boost::function<int(void*, int)>  PTimerFunc;   // ��ʱ���ص���������

typedef struct timezone   TTimeZone;


/*----------------------------------�궨��-----------------------------------*/

#define BIND_TIMER_FUNC(CLASS, FUNCTION)   bind(&CLASS::FUNCTION, this, _1, _2)
#define BIND_TIMER_FUNC_INST(CLASS, FUNCTION, INSTANCE) bind(&CLASS::FUNCTION, INSTANCE, _1, _2)

#define TIMER_PRIORITY_HIGH           (0x01)   // ��ʱ�����ȼ���
#define TIMER_PRIORITY_NORMAL         (0x02)   // ��ʱ�����ȼ�����
#define TIMER_PRIORITY_LOW            (0x03)   // ��ʱ�����ȼ���

#define TIMER_ID_INVALID              (-1)     // ��ʱ��ID��Чֵ


/*---------------------------------�ṹ����----------------------------------*/
#pragma pack(1)

typedef struct tagTTimerInfo
{
    int        iTimeOut;       // ��ʱ����ʱʱ��(��λms)
    char       chPriority;     // ��ʱ���������ȼ�(���ڶ����ʱ��ͬʱ��ʱ�ص���������ʱ�����)
    TTimeVal   tTmVal;         // ��ʱ���ϴγ�ʱʱ��
    PTimerFunc pfnTmrFunc;     // ��ʱ���ص�����
    char       achTmrName[0];  // ��ʱ������
} TTimerInfo, *PTTimerInfo;

#pragma pack()


/*----------------------------------�ඨ��-----------------------------------*/

class CTimer
{
public:
    CTimer();
    ~CTimer();
    int AddTimer(char *pchTmrName, int iTmrLen, char chPriority, PTimerFunc pfnTmrFunc, int &iTmrId); // ���Ӷ�ʱ��
    int DelTimer(int iTimerId);      // ͨ����ʱ��IDɾ����ʱ��
    int DelTimer(char *pchTmrName);  // ͨ����ʱ������ɾ����ʱ��
    static CTimer *GetInstance();    // ��ȡ��ʱ��ʵ��(��ʵ��ģʽ)
    int CheckTimeOut();  // ��鶨ʱ����ʱ
private:
    int AddTimer(PTTimerInfo ptTmrInfo, int &iTimerId);  // ���Ӷ�ʱ��

private:
    static CTimer *m_pcTmrInst;       // ʵ��ָ��
    int m_iTmrIdMax;                  // ��ʱ��Id���ֵ,���ڶ�ʱ��Id����
    map<int, PTTimerInfo> m_mpTimer;  // <��ʱ��Id, ��ʱ����Ϣ>
    class CGarbo  // ������
    {
    public:
        ~CGarbo()
        {
            if (NULL != CTimer::m_pcTmrInst)
            {
                delete CTimer::m_pcTmrInst;
            }
        }
    };
    static CGarbo m_cGarbo;  // ��̬��ԱΪ����CTimer��ʵ��ʹ��
};


#endif /* __TIMER_H__ */




