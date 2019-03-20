/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : tm_can_data_recv.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��10��17��
  ����޸�   :
  ��������   : ����CANЭ�����ݽ������������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��10��19��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TM_CAN_DATA_RECV_H__
#define __TM_CAN_DATA_RECV_H__


using namespace std;


/*----------------------------------�궨��-----------------------------------*/

#define CAN_FRAME_NO_INVALID       (-1)      // CAN֡�����Чֵ
#define CAN_DATA_RECV_WORKING      (0x00)    // CAN���ݽ��ս�����
#define CAN_DATA_RECV_FINISH       (0x01)    // CAN���ݽ������
#define CAN_PACK_STATUS_NORMAL     (0x00)    // CAN���״̬����
#define CAN_PACK_STATUS_ERROR      (0x01)    // CAN���״̬�쳣


/*----------------------------------�ඨ��-----------------------------------*/

class CTmCanDataRecv
{
public:
    CTmCanDataRecv();
    ~CTmCanDataRecv();
    int PutCanData(unsigned int uiCanId, char *pchCanData, int iDataLen); // �����յ���CAN���ݴ洢
    int ResolveCanId(unsigned int uiCanId, PTTmCanDataInfo ptCanDataInfo);  // ����CAN ID
    int AddNewNodeData(PTTmCanDataInfo ptCanDataInfo, char *pchCanData, int iDataLen);  // �����ڵ����ݴ洢
    int SaveCanData(PTTmCanDataInfo ptCanDataInfo, CTmCanPackInfoMng *pcCanPackInfo,
                    char *pchCanData, int iDataLen);  // ������յ���CAN���ݴ洢(���д洢�ռ�)
    int GetCanData(char &chSrcNode, char &chDestNode, char *pchData, int &iDataLen);  // ��ȡ�����ɵ�CAN���ݰ�

private:
    map<char, CTmCanPackInfoMng*> m_mpRecvData;   // <���ͽڵ�, �����Ϣ>

};


#endif /* __TM_CAN_DATA_RECV_H__ */




