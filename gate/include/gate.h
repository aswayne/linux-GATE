/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : gate.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��6��17��
  ����޸�   :
  ��������   : gate.cc ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��17��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __GTAE_H__
#define __GTAE_H__


/*------------------------------��������------------------------------------*/

#define REG_ACK_PACK_LEN             (0x0C)       // ģ��ע��Ӧ�������
#define REG_ACK_PACK_PREFIX_LEN      (0x04)       // Ӧ���ǰ׺����

#define GATE_INFO_GATESVR            (0x01)       // GateSvr��������Ϣ
#define GATE_INFO_MODULE             (0x02)       // ����GateSvr��ģ��������Ϣ
#define GATE_INFO_ALL                (0x03)       // GateSvr��module����Ϣ

#define GATE_DEBUG_FIFO      "/tmp/gate_debug"    // ����ʹ�ùܵ��ļ�

#define GATE_HEART_BEAT_TIME_INVALID       (-1)   // ����������ʱ����Чֵ
#define GATE_HEART_BEAT_TIME_OUT_LEN       (60)   // ģ��������ʱʱ��
#define GATE_HEEAT_BEAT_TIME_OUT_FILTER    (600)  // ����ϵͳ����û��Уʱ���´�ɱģ������


/*-------------------------------�ඨ��-------------------------------------*/

class CGate
{
public:
    CGate();
    CGate(TSockAddrUn *ptGateSvrAddr);
    ~CGate();
    int Init();         // ��ʼ��
    int ProcessData();  // Gate����ת������
    static void* ProcessDebugCmd(void *pArg);   // �����������
    static void* CheckMdlHrtBtTime(void *pArg); // ���ģ��������ʱ
    int CreatHrtBtThread();     // �������������߳�
    int CreatDebugCmdThread();  // ��������������߳�
    int ProcessGateData(int iFd, PTPackInfo ptPackInfo, char *pchPackData, int iDataLen); // ������3GGate�����ݰ�
    int ProcessBroadcast(int iFd, char *pchPackData, int iDataLen);
    int SendRegAckPacket(int iFd, PTPackInfo ptPackInfo);
    int CheckGateSvrData(int &iGateSvrFd, FdSet &fdReadSet);      // �����������Ƿ�ɶ�
    int CheckClientData(vector<int> &vtMdlFd, FdSet &fdReadSet);  // ���ͻ����Ƿ�ɶ�
    int ShowGateRunInfo(int iShowType);   // ��ӡGate��������Ϣ
    int ShowGateSvrInfo();  // ��ӡGateSvr����Ϣ
    int ShowModuleInfo();   // ��ӡ����ģ�����Ϣ
    int CheckHrtBtTimeOut();

private:
    CGateSvr   m_cGateSvr;  // 3GGate������
    TLog       *m_ptLogHdl;  // ��־�ļ�
};

#endif /* __GTAE_H__ */




