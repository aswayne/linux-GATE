/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : can_data_recv.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��11��09��
  ����޸�   :
  ��������   : CAN���ݽ��մ�������ض���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��11��09��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __CAN_DATA_RECV_H__
#define __CAN_DATA_RECV_H__


class CCanDataRecv
{
public:
    CCanDataRecv();
    CCanDataRecv(CCanDataMng *pcCanDataMng);
    ~CCanDataRecv();
    int Init();  // ��ʼ��
    int ProcRecvCanData(int iCanId, char *pchCanData, int iDataLen);  // ������յ���CAN����

private:
    CCanDataMng *m_pcCanDataMng;  // CAN���ݴ������

};


#endif /*__CAN_DATA_RECV_H__*/




