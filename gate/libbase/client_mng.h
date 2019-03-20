/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : client_mng.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��9��23��
  ����޸�   :
  ��������   : �ͻ��˹�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��9��23��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __CLIENT_MNG_H__
#define __CLIENT_MNG_H__

/*---------------------------------�ṹ����----------------------------------*/

#pragma pack(1)

// �ͻ���ʵ����Ϣ
typedef struct tagTClientInfo
{
    int iSvrType;      // ����������
    CClient *pcClient; // �ͻ���ʵ��ָ��
} TClientInfo, *PTClientInfo;

// �ͻ���ʵ���б�
typedef struct tagTClientList
{
    int  iClientNum;        // �ͻ�������
    char achClientInfo[0];  // �ͻ���ʵ����Ϣ��
} TClientList, *PTClientList;

#pragma pack()


/*----------------------------------�ඨ��-----------------------------------*/

class CClientMng
{
public:
    CClientMng();
    CClientMng(PTClientList ptClientList);
    ~CClientMng();
    int  ConnectToAllSvr();  // �������з�����
    int  DisCnctToAllSvr();  // �Ͽ������з�����������
    int  AddClient(int iSvrType, CClient *pcClient);  // ���ӿͻ���
    int  DelCilent(int iSvrType);  // ɾ���ͻ���
    int  ModifyClient(int iSvrType, PTAddr ptSvrAddr);  // �޸Ŀͻ���
    bool GetAllConnectStatus();  // ��ȡ���з���������״̬
    int  GetCnctFdTypeMap(map<int, int> &mpFdType); // ��ȡ�����ӿͻ���Fd�����ӷ��������͵�ӳ��
    int  RecvData(int iSvrType, char *pchRecvBuff, int iBuffLen, int &iRecvLen);  // ��ָ�����ͷ�������������
    int  SendData(int iSvrType, char *pchSendBuff, int iBuffSize);  // ��ָ�����ͷ�������������
    int  ProcSvrData(int iSvrType, char *pchData, int iDataLen);    // ����ָ�����������͵�����
    int  GetConnectStatus(int iSvrType, int &iStatus);  // ��ȡ����������״̬
    int  IncRecvTimeOut(int iSvrType);    // ��ָ�����������ճ�ʱ������1
    bool CheckSvrExist(int iSvrType);     // �ж�ָ�����ͷ������������
    CClient* GetSvrClient(int iSvrType);  // ��ȡָ�����ͷ������Ŀͻ���
    int  DisCnctSvr(int iSvrType);  // �Ͽ���ָ��������������

private:
    bool m_bAllSvrCnct;  // ���з���������״̬ true :���з�������������
                                            // false:�������з�������������
    map<int, CClient*> m_mpTypeClient;      // <����������, ��Ӧ�ͻ���>

};


#endif /* __CLIENT_MNG_H__ */



