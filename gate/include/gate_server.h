/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : Server.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��6��16��
  ����޸�   :
  ��������   : gateserver.cc ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��16��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __GATESERVER_H__
#define __GATESERVER_H__


using namespace std;

/*------------------------------��������-------------------------------------*/
#define MOUDLE_NUM_MAX             (0x0A)  // GateSvr�������������

// GateSvr״̬����
#define GATESVR_STATUS_START       (0x01)  // GateSvr������
#define GATESVR_STATUS_STOP        (0x02)  // GateSvr��ֹͣ

#define GATE_MODULE_NAME_LEN_MAX   (64)    // ģ�����ֳ������ֵ

/*------------------------------�ṹ����-------------------------------------*/
#pragma pack(1)

typedef struct tagTGateClientInfo
{
    int iModuleType;  // ģ������
    int iHrtBtTime;   // ���յ�������ʱ��
    CDecoupler *pcDecoupler;  // ģ���Ӧ�ķְ���
} TGateClientInfo, *PTGateClientInfo;

typedef struct tagTMdlInfo
{
    char achKillMdlCmd[GATE_MODULE_NAME_LEN_MAX];
} TMdlInfo, *PTMdlInfo;

#pragma pack()


/*----------------------------- �ඨ�� --------------------------------------*/
class CGateSvr
{
public:
    CGateSvr();
    CGateSvr(TSockAddrUn *ptGateSvrAddr);
    virtual ~CGateSvr();
    virtual int StartGateSvr();    // ����������
    virtual int StopGateSvr();     // ֹͣ������
    virtual int AcceptConnect(TSockAddr *pstAddr, SocketLen &iAddrLen);    // ���ܿͻ�������
    virtual int RecvData(int iFd, char *pchBuff, int iLen, int &iRecvLen); // ��������
    virtual int SendData(int iFd, char *pchBuff, int iLen);   // ��������
    virtual int AddGateClient(int iFd);  // ����ģ��
    virtual int UpdateGateClientType(int iFd, int iMdlType);  // ����ģ������
    virtual int RemoveClient(int iFd);  // �Ƴ�ģ��
    virtual int RemoveAllClient();      // �Ƴ�����ģ��
    virtual int GetGateSvrFd(int &iFd); // ��ȡ������Fd
    virtual int GetAllMdlFd(vector<int> &vtFd); // ��ȡ��������ģ��Fd
    virtual int GetFdByMdlType(int iMdlType, int &iFd); // ͨ��ģ�����ͻ�ȡģ��Fd
    virtual int GetGateSvrStatus(int &iStatus);  // ��ȡGateSvr״̬
    virtual int GetModuleInfo(map<int, PTGateClientInfo> &mapModule);
    virtual CDecoupler* GetFdDecoupler(int iFd);
    virtual int UpdateHrtBtTime(int iFd, int iSrcMdl);  // ����ģ��������ʱ��
    virtual int CheckMdlHrtBtTimeOut();

private:
    TSockAddrUn *m_ptGateSvrAddr;     // ͨѶ�ļ�·��
    int         m_iSvrFd;             // ������Fd
    int         m_iStatus;            // ������״̬
    ThreadMutex m_tMutex;             // �������ݺ�ȡ���ݻ���ʹ��
    map<int, PTGateClientInfo> m_mapMdlContainer; // ��GateSvr���ӵ�ģ������<ģ��Fd, ģ����Ϣ>
    map<int, PTMdlInfo> m_mapMdlInfo; // ģ����Ϣӳ�䣬������ʱkillģ��ʹ��
};



#endif /* __SERVER_H__ */




