/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : client.h
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
#ifndef __CLIENT_H__
#define __CLIENT_H__


using namespace std;

class CClient;    // ��ǰ��������


/*---------------------------------���Ͷ���----------------------------------*/

typedef boost::function<int(PTPackInfo, char*, int)> PClientCmdFunc;
typedef boost::function<int(CClient *)>  PCnctSucsFunc;
typedef boost::function<int(CClient *)>  PDisCnctFunc;


/*---------------------------------�ṹ����----------------------------------*/
#pragma pack(1)

// �ͻ����������������״̬
typedef struct tagTSvrConStatus
{
    char chSvrType;  // ����������
    char chStatus;   // ����״̬
} TSvrConStatus, *PTSvrConStatus;

typedef struct tagTRecvTimeOutPara
{
    bool bValid;        // �Ƿ����ý��ճ�ʱ����
    int  iTmIntrvl;     // selectʱ����
    int  iNtcTmLen;     // ��ʱ����ʱ��
    int  iHrtTmLen;     // ��ʱ����������ʱ��
    int  iHrtPackLen;   // ����������
    char achHrtPack[0]; // ������
} TRvTmOutPara, *PTRvTmOutPara;

typedef struct tagTRvTmOutInfo
{
    bool bValid;       // �Ƿ��������ճ�ʱ����
    int  iRvTmOut;     // ���ճ�ʱ����
    int  iNtcTimes;    // ��ӡ��ʱ���ѳ�ʱ����
    int  iHrtTimes;    // ������������ʱ����
    int  iHrtPackLen;  // ����������
    char *pchHrtPack;  // ������
} TRvTmOutInfo, *PTRvTmOutInfo;

#pragma pack()


/*----------------------------------�궨��-----------------------------------*/

#define BIND_CLIENT_FUNC(CLASS, FUNCTION)    bind(&CLASS::FUNCTION, this, _1, _2, _3)
#define BIND_CNCT_SUCS_FUNC(CLASS, FUNCTION) bind(&CLASS::FUNCTION, this, _1)
#define BIND_DIS_CNCT_FUNC(CLASS, FUNCTION)  bind(&CLASS::FUNCTION, this, _1)

#define SVR_TYPE_GATE                (0x00)     // Gate������
#define SVR_TYPE_SCHDL               (0x01)     // ���ȷ�����


/*----------------------------------�ඨ��-----------------------------------*/

class CClient
{
public:
    CClient();
    CClient(PTAddr ptSvrAddr, int iSvrType, CDecoupler *pcDecoupler, PTRvTmOutPara ptRvTmOutPara = NULL);
    ~CClient();
    int ConnectToSvr();   // ���ӵ�������
    int DisConctToSvr();  // �Ͽ��������������
    int SendData(char *pchSendBuff, int iDataLen);  // ���������������
    int RecvData(char *pchRecvBuff, int iBuffSize, int &iRecvLen);  // ���շ���������
    int GetConnectFd(int &iFd);  // ��ȡ�����������Fd
    int GetConnectStatus(int &iStatus);  // ��ȡ�����������״̬
    int GetSvrType(int &iSvrType);  // ��ȡ���ӵķ���������
    int ModifySvrAddr(PTAddr ptSvrAddr);  // �޸����ӷ������ĵ�ַ
    int RegCmdFunc(char chCmd, char chSrcMdl, PClientCmdFunc pfnCmdFunc);  // ע������������ִ�����
    int ProcessSvrData(PTPackInfo ptPackInfo, char *pchPackData, int iPackLen);  // �������������
    int PutDataInDecoupler(char *pchData, int iDataLen);  // �����ݷ���ְ���
    int GetPackInfo(PTPackInfo ptPackInfo, char *pchPackData, int iPackBuffSize, int &iPackLen);  // ��ȡ�ְ�����
    bool IsDcplrEmpty();   // �жϷְ������Ƿ�������
    int IncRecvTimeOut();  // ���ճ�ʱ������1
    int AddCnctSucsFunc(PCnctSucsFunc pfnCnctSucsFunc); // ������ӳɹ�������
    int AddDisCnctFunc(PDisCnctFunc pfnDisCnctFunc);    // ������ӶϿ�������
    int UpdateHeartBeatPacket(char *pchHtBtPkt, int iPkLen);  // ��������������

private:
    TAddr m_tSvrAddr;  // ��������ַ
    int   m_iSvrType;  // ����������
    int   m_iFd;       // �����������Fd
    int   m_iStatus;   // �����������״̬
    TRvTmOutInfo m_tRvTmOutInfo;  // ���ճ�ʱ������Ϣ
    CDecoupler  *m_pcDecoupler;   // �ְ���
    map<int, map<int, vector<PClientCmdFunc> > > m_mpCmdMap;  // <������, <Դģ��, <�ص���������> > >
    PCnctSucsFunc m_pfnCnctSucsFunc;  // ���ӳɹ�������
    PDisCnctFunc  m_pfnDisCnctFunc;   // ����ʧ�ܴ�����

};


#endif /* __CLIENT_H__ */




