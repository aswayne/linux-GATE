/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : tm_can_data_send.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��10��17��
  ����޸�   :
  ��������   : ����CANЭ�����ݷ��Ͳ��������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��10��17��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TM_CAN_DATA_SEND_H__
#define __TM_CAN_DATA_SEND_H__

/*----------------------------------�궨��-----------------------------------*/

#define TM_CAN_DATA_LEN_MAX             (0x07)    // ����CAN���ݰ������ݳ������ֵ
#define CAN_DATA_LEN_MAX                (0x08)    // CAN���ݰ��������ֵ
#define TM_CAN_END_PACK_FLAG            (0x00)    // ����CAN��������־
#define TM_CAN_CONTINUE_PACK_FLAG       (0x80)    // ����CAN��������־


/*----------------------------------�ඨ��-----------------------------------*/

class CTmCanDataSend
{
public:
    CTmCanDataSend();
    ~CTmCanDataSend();
    int PutData(char chDestNode, char chSrcNode, char *pchData, int iDataLen);  // ���������ݷ��뻺��
    int GetCanDataPack(unsigned int &uiCanId, char *pchCanDataPack, int &iPackLen, bool &bEndPack);  // ��ȡCAN���ݰ�
    int CalcCanId(unsigned int &uiCanId);  // ����CAN Id

private:
    char m_chCurFrmNo;   // ��ǰ֡���
    char m_chDestNode;   // CAN���ݷ���Ŀ�Ľڵ�
    char m_chSrcNode;    // CAN���ݷ���Դ�ڵ�
    int  m_iRestLen;     // ʣ�����ݳ���
    CCirQue *m_pcSendBuff;  // ���CAN���ݵ�ѭ������
};


#endif /* __TM_CAN_DATA_SEND_H__ */




