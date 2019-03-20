/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : resend_data_mng.h
  �� �� ��   : ����
  ��    ��   : WangGaofeng
  ��������   : 2016��3��28��
  ����޸�   :
  ��������   : �ط����ݹ�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��3��28��
    ��    ��   : WangGaofeng
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __RESEND_DATA_MNG_H__
#define __RESEND_DATA_MNG_H__


/*----------------------------------�ඨ��-----------------------------------*/

class CReSendDataMng
{
public:
    CReSendDataMng();
    ~CReSendDataMng();
    int AddReSendData(PTReSndDtInfo ptReSendDataInfo, int iReSndDtInfoLen, CClient *pcGate);  // �����ط�����
    int CheckReSendData(void *pvData, int iDataLen);       // ��������ط�
    int StopReSendData(int iDataId);                       // ֹͣ�ط�ָ������
    static CReSendDataMng* GetInstance();                  // ��ȡʵ��ָ��
    int SetReSendDataTimes(int iDataId, char chTimes);     // ����ָ�����ݷ��ʹ���
    int SetReSendDataInterval(int iDataId, int iInterval); // ����ָ�����ݷ��ͼ��
    int RegReSendDataFunc(int iDataId, PReSndDtFunc pfnReSndDtFunc);      // ����ָ�������ط�����
    int RegReSendFnshFunc(int iDataId, PReSndFnshFunc pfnReSndFnshFunc);  // ����ָ�������ط����κ���

private:
    int m_iTmrId;  // ��ʱ��ID
    int m_iDataId; // �ط�����ID(��1��ʼ)
    static CReSendDataMng* m_pcInstance;   // ʵ��ָ��
    map<int, CReSendData*> m_mpReSndDtMap;  // �ط�����ӳ���
    class CGarbo  // ������
    {
    public:
        ~CGarbo()
        {
            if (NULL != CReSendDataMng::m_pcInstance)
            {
                delete CReSendDataMng::m_pcInstance;
            }
        }
    };
    static CGarbo m_cGarbo;  // ��̬��ԱΪ����CReSendDataMng��ʵ��ʹ��
};


#endif /* __RESEND_DATA_MNG_H__ */




