/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : tm_can_pack_info_mng.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��10��17��
  ����޸�   :
  ��������   : ����CANЭ�����ݽ�����Ϣ�����ඨ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��10��19��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TM_CAN_PACK_INFO_MNG_H__
#define __TM_CAN_PACK_INFO_MNG_H__


/*---------------------------------�ṹ����----------------------------------*/
#pragma pack(1)

// ����CANЭ��CAN������Ϣ
typedef struct tagTTmCanDataInfo
{
    char chDestNode;  // ���ݷ���Ŀ�Ľڵ�
    char chSrcNode;   // ����Դ�ڵ�
    char chFrmNo;     // ֡���
    char chFrmFlag;   // CAN���ݰ���־
} TTmCanDataInfo, *PTTmCanDataInfo;

#pragma pack()


/*----------------------------------�ඨ��-----------------------------------*/

class CTmCanPackInfoMng
{
public:
    CTmCanPackInfoMng();
    ~CTmCanPackInfoMng();
    int PutCanData(PTTmCanDataInfo ptCanDataInfo, char *pchCanData, int iDataLen);  // ��CAN���ݴ�����
    int GetCanPackStatus(int &iStatus);              // ��ȡCAN���ݰ�����״̬
    int GetCanPack(char *pchCanPack, int &iPackLen); // ��ȡCAN���ݰ�
    int SetCanPackDestNode(char chDestNode);   // ����CAN����Ŀ�Ľڵ�
    int GetCanPackDestNode(char &chDestNode);  // ��ȡCAN����Ŀ�Ľڵ�

private:
    char m_chDestNode;   // CAN������Ŀ�Ľڵ�
    int  m_iRecvLen;     // �������ݳ���
    int  m_iRecvFinish;  // �������
    int  m_iEndFrmNo;    // ����֡���
    int  m_iEndFrmLen;   // ����֡����
    int  m_iPackStatus;  // ��״̬
    vector<char> m_vtRecvFrmNo;   // ���յ���֡���
    char m_achPack[PACK_LEN_MAX]; // �������
};


#endif /* __TM_CAN_PACK_INFO_MNG_H__ */




