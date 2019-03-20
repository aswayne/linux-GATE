/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : resend_data.h
  �� �� ��   : ����
  ��    ��   : WangGaofeng
  ��������   : 2016��3��28��
  ����޸�   :
  ��������   : �ط�������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��3��28��
    ��    ��   : WangGaofeng
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __RESEND_DATA_H__
#define __RESEND_DATA_H__


/*----------------------------------�궨��-----------------------------------*/

#define BIND_RSND_FNSH_FUNC(CLASS, FUNCTION)    bind(&CLASS::FUNCTION, this, _1, _2)
#define BIND_RSND_DT_FUNC(CLASS, FUNCTION)      bind(&CLASS::FUNCTION, this, _1, _2, _3, _4)
#define BIND_RSND_DT_FUNC_C(FUNCTION)           bind(&FUNCTION, _1, _2, _3, _4)

#define RSND_TIMES_INFINITE           (0xFF)                // ���޴��ط�


/*---------------------------------���Ͷ���----------------------------------*/

typedef boost::function<int(void*, int)>    PReSndFnshFunc;
typedef boost::function<int(CClient*, PTPackInfo, char*, int)> PReSndDtFunc;


/*---------------------------------�ṹ����----------------------------------*/

#pragma pack(1)

// �ط����ݰ���Ϣ
typedef struct tagTReSndDtInfo
{
    char chDest;      // ���ݰ�����Ŀ�ĵ�ַ
    char chSrc;       // ���ݰ�����Դ��ַ
    char chCmd;       // ���ݰ�������
    int  iDataLen;    // ���ݰ�����
    char achData[0];  // �ط����ݰ�
} TReSndDtInfo, *PTReSndDtInfo;

#pragma pack()


/*----------------------------------�ඨ��-----------------------------------*/

class CReSendData
{
public:
    CReSendData();
    CReSendData(TReSndDtInfo *ptReSndDtInfo, int iReSndInfoLen, CClient *pcGate);
    ~CReSendData();
    int ReSendData(void);                     // �ط�����
    int SetReSendTimes(char chReSendTimes);   // �����ط�����
    int SetReSendInterval(int iInterval);     // �����ط����
    int CheckReSendData(void);                // ����Ƿ��ط�
    int RegReSendFinishFunc(PReSndFnshFunc pfnReSndFnshFunc);  // ע�ᳬ�λص�����
    int RegReSendDataFunc(PReSndDtFunc pfnReSndDtFunc);        // ע���ط����ݻص�����

private:
    PTReSndDtInfo m_ptReSndDtInfo;      // �ط�������Ϣ
    char m_chReSendTimes;               // �ط�����
    int  m_iReSndIntvl;                 // �ط����
    char m_chSentTimes;                 // �ѷ������ݴ���
    int  m_iSecCnt;                     // �����ط�����������
    PReSndFnshFunc m_pfnReSndFnshFunc;  // ���λص�����
    PReSndDtFunc   m_pfnReSndDtFunc;    // �ط����ݻص�����
    CClient       *m_pcGate;            // ����Gate�Ŀͻ���ʵ��ָ��
};


#endif /* __RESEND_DATA_H__ */




