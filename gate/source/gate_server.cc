/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : gateserver.cc
  �� �� ��   :
  ��    ��   : wanggaofeng
  ��������   :
  ����޸�   :
  ��������   : 3GGate��������ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   :
    ��    ��   :
    �޸�����   :

******************************************************************************/


#include "gate_module.h"


/*****************************************************************************
 �� �� ��  : CGateSvr
 ��������  : CGateSvr�Ĺ��캯��
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
CGateSvr::CGateSvr()
{
}


/*****************************************************************************
 �� �� ��  : CGateSvr
 ��������  : CGateSvr�Ĺ��캯��
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
CGateSvr::CGateSvr(TSockAddrUn *ptGateSvrAddr)
{
    m_ptGateSvrAddr = ptGateSvrAddr;

    m_iStatus = GATESVR_STATUS_STOP;  // ��ʼ��״̬Ϊֹͣ
    m_iSvrFd  = FD_INVALID;

    pthread_mutex_init (&m_tMutex, NULL);

    PTMdlInfo ptMdlInfo = PTMdlInfo(new char[sizeof(TMdlInfo)]);
    memset(ptMdlInfo, 0x00, sizeof(TMdlInfo));
    strcpy(ptMdlInfo->achKillMdlCmd, "killall -9 para_mgr");
    m_mapMdlInfo[MODULE_TYPE_PARA] = ptMdlInfo;

    ptMdlInfo = PTMdlInfo(new char[sizeof(TMdlInfo)]);
    memset(ptMdlInfo, 0x00, sizeof(TMdlInfo));
    strcpy(ptMdlInfo->achKillMdlCmd, "killall -9 svr");
    m_mapMdlInfo[MODULE_TYPE_SVR] = ptMdlInfo;

    ptMdlInfo = PTMdlInfo(new char[sizeof(TMdlInfo)]);
    memset(ptMdlInfo, 0x00, sizeof(TMdlInfo));
    strcpy(ptMdlInfo->achKillMdlCmd, "killall -9 schedule");
    m_mapMdlInfo[MODULE_TYPE_SCHDL] = ptMdlInfo;

    ptMdlInfo = PTMdlInfo(new char[sizeof(TMdlInfo)]);
    memset(ptMdlInfo, 0x00, sizeof(TMdlInfo));
    strcpy(ptMdlInfo->achKillMdlCmd, "killall -9 bzq");
    m_mapMdlInfo[MODULE_TYPE_BZQ] = ptMdlInfo;

    ptMdlInfo = PTMdlInfo(new char[sizeof(TMdlInfo)]);
    memset(ptMdlInfo, 0x00, sizeof(TMdlInfo));
    strcpy(ptMdlInfo->achKillMdlCmd, "killall -9 netupdate");
    m_mapMdlInfo[MODULE_TYPE_NET_UPDATE] = ptMdlInfo;

    ptMdlInfo = PTMdlInfo(new char[sizeof(TMdlInfo)]);
    memset(ptMdlInfo, 0x00, sizeof(TMdlInfo));
    strcpy(ptMdlInfo->achKillMdlCmd, "killall -9 data_proc");
    m_mapMdlInfo[MODULE_TYPE_DATA_PROC] = ptMdlInfo;

    ptMdlInfo = PTMdlInfo(new char[sizeof(TMdlInfo)]);
    memset(ptMdlInfo, 0x00, sizeof(TMdlInfo));
    strcpy(ptMdlInfo->achKillMdlCmd, "killall -9 wireless");
    m_mapMdlInfo[MODULE_TYPE_WIFI] = ptMdlInfo;
}


/*****************************************************************************
 �� �� ��  : ~CGateSvr
 ��������  : CGateSvr����������
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
CGateSvr::~CGateSvr()
{
    map<int, PTMdlInfo>::iterator iter = m_mapMdlInfo.begin();
    for(; iter != m_mapMdlInfo.end(); iter++)
    {
        delete [] ((char*)iter->second);
    }

    m_mapMdlInfo.clear();

    int iRet = RemoveAllClient();
    if(RTN_SUCCESS != iRet)
    {
        ErrorPrint("Remove all client faild. ");
    }
}


/*****************************************************************************
 �� �� ��  : StartGateSvr
 ��������  : ����GateSvr
 �������  : ��
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::StartGateSvr()
{
    int iFlag = 1;
    int iRet;
    int iAddrLen;

    //signal(SIGPIPE, SIG_IGN);
    m_iSvrFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(m_iSvrFd < 0)
    {
        ErrorPrint("Socket error fd m_iSvrFd < 0 m_iSvrFd = %d. ", m_iSvrFd);
        return RTN_FAIL;
    }

    setsockopt(m_iSvrFd, SOL_SOCKET, SO_REUSEADDR, (char *)&iFlag, sizeof(iFlag));

    unlink(m_ptGateSvrAddr->sun_path);

    iAddrLen = sizeof(m_ptGateSvrAddr->sun_family) + strlen(m_ptGateSvrAddr->sun_path);

    iRet = bind(m_iSvrFd, (TSockAddr*)(m_ptGateSvrAddr), iAddrLen);
    if(BIND_ERROR == iRet)
    {
        ErrorPrint("Bind error m_iSvrFd = %d. ", m_iSvrFd);
        return RTN_FAIL;
    }

    iRet = listen(m_iSvrFd, MOUDLE_NUM_MAX);
    if(iRet < 0)
    {
        ErrorPrint("Listen error m_iSvrFd = %d. ", m_iSvrFd);
        return RTN_FAIL;
    }

    m_iStatus = GATESVR_STATUS_START;

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : StopGateSvr
 ��������  : ֹͣGateSvr
 �������  : ��
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::StopGateSvr()
{
    // ����ģ������GateSvr��ر�����
    if(0 != m_mapMdlContainer.size())
    {
        int iRet = RemoveAllClient();
        if(RTN_SUCCESS != iRet)
        {
            ErrorPrint("Remove all client failed. ");
        }
    }

    // �ر�GateSvr
    shutdown(m_iSvrFd, SHUT_RDWR);
    close(m_iSvrFd);
    m_iStatus = GATESVR_STATUS_STOP;

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : AcceptConnect
 ��������  : ���ܿͻ�������
 �������  : ��
 �������  : ptAddr-----�ͻ��˵�ַ
             iAddrLen---�ͻ��˵�ַ����
 �� �� ֵ  : �ͻ�������Fd(ʧ�ܷ���ACCEPT_ERROR -1)
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::AcceptConnect(TSockAddr *ptAddr, SocketLen &iAddrLen)
{
    int       iFd;
    int       iFlag = 1;
    TTimeVal  tTimeVal;
    TSoLinger tSoLinger;


    iFd = accept(m_iSvrFd, ptAddr, &iAddrLen);

    if(ACCEPT_ERROR == iFd)
    {
        ErrorPrint("accept client connect error iFd = %d, GateSvr fd %d. ", iFd, m_iSvrFd);
        perror("Accept error");
    }
    else
    {
        // ������������
        setsockopt(iFd, IPPROTO_TCP, TCP_NODELAY, (char*)(&iFlag), sizeof(iFlag));

        tTimeVal.tv_sec  = 2;
        tTimeVal.tv_usec = 0;
        setsockopt(iFd, SOL_SOCKET, SO_SNDTIMEO, (char*)(&tTimeVal), sizeof(tTimeVal));

        tTimeVal.tv_sec  = 2;
        tTimeVal.tv_usec = 0;
        setsockopt(iFd, SOL_SOCKET, SO_RCVTIMEO, (char*)(&tTimeVal), sizeof(tTimeVal));

        tSoLinger.l_onoff = 1;
        tSoLinger.l_linger = 0;
        setsockopt(iFd, SOL_SOCKET, SO_LINGER, (char*)(&tSoLinger), sizeof(tSoLinger));
    }

    return iFd;
}


/*****************************************************************************
 �� �� ��  : RecvData
 ��������  : ���տͻ�������
 �������  : iFd------�ͻ�������Fd
 �������  : pchBuff--���յ�������
             iLen-----���յ������ݳ���
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::RecvData(int iFd, char *pchBuff, int iLen, int &iRecvLen)
{
    int iFlag = 0;

    iRecvLen = recv(iFd, pchBuff, iLen, iFlag);

    if(0 == iRecvLen)
    {
        ErrorPrint("CGateSvr::RecvData Error Fd: %d, iRecvLen: %d", iFd, iRecvLen);
        return RTN_FAIL;
    }
    else if(iRecvLen < 0)
    {
        if((EINTR == errno) || (EAGAIN == errno))
        {
            ErrorPrint("CGateSvr::RecvData time out. ");
            return GATE_RCVE_TIME_OUT;
        }

        ErrorPrint("CGateSvr::RecvData Error Fd: %d, iRecvLen: %d, errno: %d. ", iFd, iRecvLen, errno);
        return RTN_FAIL;
    }
    else
    {
        return RTN_SUCCESS;
    }
}


/*****************************************************************************
 �� �� ��  : SendData
 ��������  : ��ͻ��˷�������
 �������  : iFd--------�ͻ�������Fd
             pchBuff----��ŷ������ݻ���
             iLen-------�������ݳ���
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::SendData(int iFd, char *pchBuff, int iLen)
{
    int iFlag = 0;
    int iResult;

    iResult = send(iFd, pchBuff, iLen, iFlag);

    if(iResult < 0)
    {
        if((errno != EINTR) && (errno != EAGAIN) && (errno != ETIMEDOUT))
        {
            ErrorPrint("SendData Error iResult = %d, errno = %d", iResult, iResult);
            return RTN_FAIL;
        }
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : AddGateClient
 ��������  : ��ӿͻ���
 �������  : iFd---------����3GGate�Ŀͻ���Fd
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::AddGateClient(int iFd)
{
    if(FD_INVALID == iFd)
    {
        ErrorPrint("CGateSvr::AddClient iFd = %d invalid. ", iFd);
        return RTN_FAIL;
    }

    pthread_mutex_lock(&m_tMutex);

    map<int, PTGateClientInfo>::iterator iter = m_mapMdlContainer.find(iFd);
    if(iter != m_mapMdlContainer.end())
    {
        // ������,�ϱ��쳣������
        ErrorPrint("Module fd %d exist, insert to container failed. ", iFd);

        shutdown(iFd, SHUT_RDWR);
        close(iFd);

        pthread_mutex_unlock(&m_tMutex);

        return RTN_FAIL;
    }
    else
    {
        CDecoupler *pcDecoupler = new CDecoupler((char*)("####"), 4, (PACK_LEN_MAX * 2));
        if(NULL == pcDecoupler)
        {
            ErrorPrint("Get fd %d CDecoupler memory failed. ", iFd);
            return RTN_FAIL;
        }

        PTGateClientInfo ptMdlInfo = PTGateClientInfo(new char[sizeof(TGateClientInfo)]);
        if(NULL == ptMdlInfo)
        {
            ErrorPrint("Get fd %d TGateClientInfo memory failed. ", iFd);
            return RTN_FAIL;
        }

        ptMdlInfo->iModuleType = MODULE_TYPE_INVALID;
        ptMdlInfo->iHrtBtTime  = 0;
        ptMdlInfo->pcDecoupler = pcDecoupler;

        // ��������,���������
        m_mapMdlContainer[iFd] = ptMdlInfo;
    }

    pthread_mutex_unlock(&m_tMutex);

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : UpdateGateClient
 ��������  : �������ע��ͻ���
 �������  : iFd---------����3GGate�Ŀͻ���Fd
             iMdlType----�ͻ���ģ������
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::UpdateGateClientType(int iFd, int iMdlType)
{
    map<int, PTGateClientInfo>::iterator iter;

    if(FD_INVALID == iFd)
    {
        ErrorPrint("CGateSvr::UpdateClient iFd = %d invalid. ", iFd);
        return RTN_FAIL;
    }

    if(!CheckModuleType(iMdlType))
    {
        ErrorPrint("Regist module type denied, module type %2x invalid, fd = %d. ", iMdlType, iFd);
        return RTN_FAIL;
    }

    pthread_mutex_lock(&m_tMutex);
    // �����������
    iter = m_mapMdlContainer.find(iFd);
    if(iter != m_mapMdlContainer.end())
    {
        ErrorPrint("Update module type fd = %d , module type is %2x. ", iFd, iMdlType);
        iter->second->iModuleType = iMdlType;
        iter->second->iHrtBtTime = time(NULL);
    }
    else
    {
        ErrorPrint("Can't find fd = %d item, update module type failed.  ", iFd);
        pthread_mutex_unlock(&m_tMutex);
        return RTN_FAIL;
    }

    pthread_mutex_unlock(&m_tMutex);

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : RrmoveClient
 ��������  : ���ͻ����Ƴ�
 �������  : iFd---------����3GGate�Ŀͻ���Fd
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::RemoveClient(int iFd)
{
    pthread_mutex_lock(&m_tMutex);

    map<int, PTGateClientInfo>::iterator iter = m_mapMdlContainer.find(iFd);
    if(iter != m_mapMdlContainer.end())
    {
        ErrorPrint("Remove cilent fd = %d, module type = %2x. ", iFd, iter->second->iModuleType);
        delete iter->second->pcDecoupler;  // �ͷŷְ���
        delete [] ((char*)iter->second);   // ��ģ����Ϣռ�ö�̬�ڴ�
        m_mapMdlContainer.erase(iter);

        shutdown(iFd, SHUT_RDWR);
        close(iFd);
    }
    else
    {
        ErrorPrint("RemoveClient can't find Fd = %d client. ", iFd);
        pthread_mutex_unlock(&m_tMutex);

        return RTN_FAIL;
    }

    pthread_mutex_unlock(&m_tMutex);

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : RemoveAllClient
 ��������  : �Ƴ����пͻ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::RemoveAllClient()
{
    pthread_mutex_lock(&m_tMutex);

    map<int, PTGateClientInfo>::iterator iter;

    for(iter = m_mapMdlContainer.begin(); iter != m_mapMdlContainer.end(); iter++)
    {
        delete iter->second->pcDecoupler;  // �ͷŷְ���
        delete [] ((char*)iter->second);   // ��ģ����Ϣռ�ö�̬�ڴ�
        m_mapMdlContainer.erase(iter);

        shutdown(iter->first, SHUT_RDWR);
        close(iter->first);
    }

    pthread_mutex_unlock(&m_tMutex);

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : GetGateSvrFd
 ��������  : ��ȡGateSvr��Fd
 �������  : iFd---------����3GGate�Ŀͻ���Fd
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::GetGateSvrFd(int &iFd)
{
    if(GATESVR_STATUS_START == m_iStatus)
    {
        iFd = m_iSvrFd;
        return RTN_SUCCESS;
    }
    else
    {
        ErrorPrint("GateSvr status is not OK. ");
        iFd = FD_INVALID;
        return RTN_FAIL;
    }
}


/*****************************************************************************
 �� �� ��  : GetAllMdlFd
 ��������  : ��ȡGateSvr���ӵ�����ģ��Fd
 �������  : ��
 �������  : vtFd----�洢����ģ��fd��vector
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::GetAllMdlFd(vector<int> &vtFd)
{
    map<int, PTGateClientInfo>::iterator iter;

    vtFd.clear();  // �Ƚ�vector���

    for(iter = m_mapMdlContainer.begin(); iter != m_mapMdlContainer.end(); iter++)
    {
        vtFd.push_back(iter->first);
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : GetGateSvrFd
 ��������  : ͨ��ģ�����ͻ�ȡģ������Fd
 �������  : iMdlType---------ģ������
 �������  : iFd--------------ģ������Fd
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::GetFdByMdlType(int iMdlType, int &iFd)
{
    bool bFind = false;
    map<int, PTGateClientInfo>::iterator iter;

    if(!CheckModuleType(iMdlType))
    {
        ErrorPrint("Invalid module type %2x, can't find corresponding module. ", iMdlType);
        return RTN_FAIL;
    }

    for(iter = m_mapMdlContainer.begin(); iter != m_mapMdlContainer.end(); iter++)
    {
        if(iter->second->iModuleType == iMdlType)
        {
            bFind = true;
            iFd = iter->first;

            break;
        }
    }

    if(!bFind)
    {
        ErrorPrint("Module is not exist, module type = %2x", iMdlType);
        return RTN_FAIL;

    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : GetGateSvrFd
 ��������  : ��ȡGateSvr״̬
 �������  : ��
 �������  : ��
 �� �� ֵ  : GateSvr��״̬
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::GetGateSvrStatus(int &iStatus)
{
    iStatus = m_iStatus;

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : GetModuleInfo
 ��������  : ��ȡGateSvr״̬
 �������  : ��
 �������  : ��
 �� �� ֵ  : GateSvr��״̬
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::GetModuleInfo(map<int, PTGateClientInfo> &mapModule)
{
    mapModule = m_mapMdlContainer;

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : GetFdDecoupler
 ��������  : ��ȡģ���Ӧ�ķְ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : GateSvr��״̬
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
CDecoupler* CGateSvr::GetFdDecoupler(int iFd)
{
    CDecoupler *pcDecopuler;

    map<int, PTGateClientInfo>::iterator iter = m_mapMdlContainer.find(iFd);
    if(iter != m_mapMdlContainer.end())
    {
        if(NULL == iter->second->pcDecoupler)
        {
            ErrorPrint("Fd %d decoupler is null pointer. ", iFd);
            return NULL;
        }
        else
        {
            pcDecopuler= iter->second->pcDecoupler;
        }
    }
    else
    {
        ErrorPrint("Can't find fd %d decopuler. ", iFd);
        return NULL;
    }

    return pcDecopuler;
}


/*****************************************************************************
 �� �� ��  : UpdateHrtBtTime
 ��������  : ����ģ��������ʱ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : GateSvr��״̬
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::UpdateHrtBtTime(int iFd, int iSrcMdl)
{
    pthread_mutex_lock(&m_tMutex);

    map<int, PTGateClientInfo>::iterator iter = m_mapMdlContainer.find(iFd);
    if(iter != m_mapMdlContainer.end())
    {
        if(iSrcMdl != iter->second->iModuleType)
        {
            ErrorPrint("Heart beat packet data error, fd %d, src module %2x. ", iFd, iSrcMdl);
            pthread_mutex_unlock(&m_tMutex);
            return RTN_FAIL;
        }

        iter->second->iHrtBtTime = time(NULL);
    }
    else
    {
        ErrorPrint("Update heart beat time failed, fd %d, src module %2x.", iFd, iSrcMdl);
    }

    pthread_mutex_unlock(&m_tMutex);

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : CheckMdlHrtBtTimeOut
 ��������  : ����ģ��������ʱ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : GateSvr��״̬
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGateSvr::CheckMdlHrtBtTimeOut()
{
    int iCurTime = time(NULL);

    pthread_mutex_lock(&m_tMutex);

    if(m_mapMdlContainer.size() > 0)
    {
        map<int, PTGateClientInfo>::iterator iter = m_mapMdlContainer.begin();
        for(; iter != m_mapMdlContainer.end(); iter++)
        {
            if((iter->second->iHrtBtTime != GATE_HEART_BEAT_TIME_INVALID) \
            && (MODULE_TYPE_GUI    != iter->second->iModuleType) \
            && (MODULE_TYPE_ACCESS != iter->second->iModuleType))
            {
                int iTimeLen = ABS((iCurTime - iter->second->iHrtBtTime));
                if(iTimeLen > GATE_HEART_BEAT_TIME_OUT_LEN)
                {
                    if(iTimeLen < GATE_HEEAT_BEAT_TIME_OUT_FILTER)  // ����ϵͳ����δУʱ����ɱģ��
                    {
                        map<int, PTMdlInfo>::iterator itMdl = m_mapMdlInfo.find(iter->second->iModuleType);
                        if(itMdl != m_mapMdlInfo.end())
                        {
                            ErrorPrint("Gate %s. ", itMdl->second->achKillMdlCmd);
                            system(itMdl->second->achKillMdlCmd);
                        }
                        else
                        {
                            ErrorPrint("Can't find mdl type %2x. ", iter->second->iModuleType);
                        }
                    }
                    else
                    {
                        iter->second->iHrtBtTime = iCurTime;  // ��ֹδУʱ,��Զ�޷�������ʱ�Ĵ���
                    }
                }
            }
        }
    }

    pthread_mutex_unlock(&m_tMutex);

    return RTN_SUCCESS;
}




