/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : gate.cc
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��6��17��
  ����޸�   :
  ��������   : CGate���ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��17��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/


#include "gate_module.h"


/*****************************************************************************
 �� �� ��  : CGate
 ��������  : CGate��Ĺ��캯��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
CGate::CGate()
{
}


/*****************************************************************************
 �� �� ��  : CGate
 ��������  : CGate��Ĺ��캯��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
CGate::CGate(TSockAddrUn *ptGateSvrAddr) : m_cGateSvr(ptGateSvrAddr)
{
}


/*****************************************************************************
 �� �� ��  : CGate
 ��������  : CGate�����������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
CGate::~CGate()
{
}


/*****************************************************************************
 �� �� ��  : Init
 ��������  : ��ĳ�ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::Init()
{
    int iRet;

    // ����GateSvr
    iRet = m_cGateSvr.StartGateSvr();
    if(RTN_FAIL == iRet)
    {
        ErrorPrint("GateSvr start fail. ");
        exit(-1);
    }

    // ���������߳�
    iRet = CreatDebugCmdThread();
    if(RTN_FAIL == iRet)
    {
        ErrorPrint("Debug thread start fail. ");
        exit(-1);
    }

    iRet = CreatHrtBtThread();
    if(RTN_FAIL == iRet)
    {
        ErrorPrint("Heart beat thread start fail. ");
        exit(-1);
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : ProcessData
 ��������  : Gate����ת������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::ProcessData()
{
    int         iGateSvrFd;
    int         iFdMax;
    int         iResult;
    FdSet       fdReadSet;
    TTimeVal    tTimeOut;
    vector<int> vtMdlFd;
    vector<int>::iterator iter;

    while(1)
    {
        FD_ZERO(&fdReadSet);

        m_cGateSvr.GetAllMdlFd(vtMdlFd);
        m_cGateSvr.GetGateSvrFd(iGateSvrFd);

        iFdMax = iGateSvrFd;

        FD_SET(iGateSvrFd, &fdReadSet);

        for(iter = vtMdlFd.begin(); iter != vtMdlFd.end(); iter++)
        {
            if(*iter > iFdMax)
            {
                iFdMax = *iter;
            }

            FD_SET(*iter, &fdReadSet);
        }

        tTimeOut.tv_sec  = 2;
        tTimeOut.tv_usec = 0;

        iResult= select((iFdMax + 1), &fdReadSet, NULL, NULL, &tTimeOut);

        if(0 == iResult)
        {
            // ErrorPrint("select time out!!!");
            continue;
        }
        else if(iResult < 0)
        {
            ErrorPrint("select error happened errno = %d. ", errno);

            // �Ͽ��������ӿͻ���
            m_cGateSvr.RemoveAllClient();

            continue;
        }
        else
        {
            // ��������Fd�ɶ�
            iResult = CheckGateSvrData(iGateSvrFd, fdReadSet);
            if(RTN_SUCCESS != iResult)
            {
                ErrorPrint("Check gate svr data error. ");
            }

            // ���ͻ���Fd�Ƿ�ɶ�
            iResult = CheckClientData(vtMdlFd, fdReadSet);
            if(RTN_SUCCESS != iResult)
            {
                ErrorPrint("Check client data error. ");
                continue;
            }
        }
    }

    // ֹͣ������
    m_cGateSvr.StopGateSvr();

    return RTN_FAIL;
}


/*****************************************************************************
 �� �� ��  : ProcessCzjData
 ��������  : �����������
 �������  : pArg----�߳����������CGate��ʵ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
void* CGate::ProcessDebugCmd(void *pArg)
{
    int iRet;
    int iShowType;
    int iFd = FD_INVALID;
    int bOpenPipe = true;
    FdSet    fdReadSet;
    TTimeVal tTimeOut;
    CGate *pc3GGate = (CGate*)pArg;

    iRet = CreatFifo((char*)(GATE_DEBUG_FIFO));
    if (iRet != RTN_SUCCESS)
    {
        ErrorPrint("Create pipe file failed, path: %s. ", GATE_DEBUG_FIFO);
    }

    while(1)
    {
        if(bOpenPipe)
        {
            if(iFd > 0)
            {
                close(iFd);
            }

            iFd = open(GATE_DEBUG_FIFO, O_RDONLY | O_NONBLOCK);
            if(iFd < 0)
            {
                ErrorPrint("Open pipe file failed, path: %s", GATE_DEBUG_FIFO);
                sleep(2);     // ���򲻿�����ʱ2s�ٳ���
                continue;
            }
            else
            {
                bOpenPipe = false;
            }
        }

        FD_ZERO(&fdReadSet);
        FD_SET(iFd, &fdReadSet);

        tTimeOut.tv_sec  = 2;
        tTimeOut.tv_usec = 0;

        iRet = select((iFd + 1), &fdReadSet, NULL, NULL, &tTimeOut);

        if(0 == iRet)
        {
            //ErrorPrint("Debug select time out. ");
            continue;
        }
        else if(iRet < 0)
        {
            ErrorPrint("Debug select error. ");
            bOpenPipe = true;
            continue;
        }
        else
        {
            iRet = read(iFd, &iShowType, sizeof(iShowType));
            if(0 == iRet)
            {
                ErrorPrint("Debug read pipe error. ")
                bOpenPipe = true;
                continue;
            }

            iRet = pc3GGate->ShowGateRunInfo(iShowType);
            if(RTN_SUCCESS != iRet)
            {
                ErrorPrint("Debug show gate run info error. ");
                continue;
            }
        }
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : CheckMdlHrtBtTime
 ��������  : ���ģ��������ʱ
 �������  : pArg----�߳����������CGate��ʵ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
void* CGate::CheckMdlHrtBtTime(void *pArg)
{
    CGate *pcGate = (CGate*)pArg;

    while(1)
    {
        sleep(30);

        ErrorPrint("CheckMdlHrtBtTime.");
        pcGate->CheckHrtBtTimeOut();
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : CreatHrtBtThread
 ��������  : �������������߳�
 �������  : ��
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::CreatHrtBtThread()
{
    ThreadId tHrtBtThread;
    int iRet;

    iRet = pthread_create(&tHrtBtThread, NULL, CheckMdlHrtBtTime, this);
    if(iRet < 0)
    {
        ErrorPrint("Creat heart beat thread failed. ");
        return RTN_FAIL;
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : CreatDebugCmdThread
 ��������  : ��������������߳�
 �������  : ��
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::CreatDebugCmdThread()
{
    // �ݲ����������߳�
#if 0
    ThreadId tDebugThread;
    int iRet;

    iRet = pthread_create(&tDebugThread, NULL, ProcessDebugCmd, this);
    if(iRet < 0)
    {
        ErrorPrint("Creat debug cmd thread failed. ");
        return RTN_FAIL;
    }
#endif

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : ProcessGateData
 ��������  : ������Gate�����ݰ�
 �������  : iFd-------------��GateSvr�������ݵ�fd
             ptPackInfo------��������Ϣ
             pchPackData-----������
             iDataLen--------������
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::ProcessGateData(int iFd, PTPackInfo ptPackInfo, char *pchPackData, int iDataLen)
{
    int iResult;

    switch(ptPackInfo->chCmd)
    {
        case GATE_MOUDLE_REG_CMD:  // ģ��ע���
        {
            // ע��ģ��
            iResult = m_cGateSvr.UpdateGateClientType(iFd, ptPackInfo->chSrcMdl);
            if(RTN_SUCCESS != iResult)
            {
                return RTN_FAIL;
            }

            // ����Ӧ���
            SendRegAckPacket(iFd, ptPackInfo);

            break;
        }
        case GATE_HEART_BEAT_CMD:
        {
            ErrorPrint("Recv heart beat packet, module type is %2x. ", ptPackInfo->chSrcMdl);
            iResult = m_cGateSvr.UpdateHrtBtTime(iFd, ptPackInfo->chSrcMdl);
            if(RTN_SUCCESS != iResult)
            {
                return RTN_FAIL;
            }

            break;
        }

        default:
        {
            ErrorPrint("Gate packet cmd = %d, not process. ", ptPackInfo->chCmd);

            return RTN_FAIL;
        }
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : ProcessBroadcast
 ��������  : ����㲥��Ϣ
 �������  : iFd------------���͹㲥����Դ��fd
             pchPackData----�㲥����
             iDataLen-------�㲥���ݳ���
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::ProcessBroadcast(int iFd, char *pchPackData, int iDataLen)
{
    int         iRet;
    vector<int> vtAllMdl;
    vector<int>::iterator iter;

    iRet = m_cGateSvr.GetAllMdlFd(vtAllMdl);
    if(RTN_SUCCESS != iRet)
    {
        ErrorPrint("Get all module fd error. ");
        return RTN_FAIL;
    }

    for(iter = vtAllMdl.begin(); iter != vtAllMdl.end(); iter++)
    {
        if(*iter == iFd)  // �㲥��Ϣ�������Լ�
        {
            continue;
        }
        else
        {
            iRet = m_cGateSvr.SendData(*iter, pchPackData, iDataLen);
            if(RTN_SUCCESS != iRet)
            {
                ErrorPrint("Send broadcase data error, fd = %d. ", *iter);
                continue;
            }
        }
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : SendAckPacket
 ��������  : ����ģ��ע��Ӧ����Ϣ
 �������  : iFd-----------Ӧ���Ŀ��ģ���fd
             ptPackInfo----ģ��ע����İ�������Ϣ
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::SendRegAckPacket(int iFd, PTPackInfo ptPackInfo)
{
    int   iOffset = 0;
    int   iResult;
    short sPackLen;
    short sCrc;
    CCrcPublic cCrc;
    PTPackInfo ptPacketInfo;

    char *pchBuff = new char[REG_ACK_PACK_LEN];

    memset(pchBuff, 0x00, REG_ACK_PACK_LEN);

    strncpy(pchBuff, "####", REG_ACK_PACK_PREFIX_LEN);
    iOffset += REG_ACK_PACK_PREFIX_LEN;

    sPackLen = REG_ACK_PACK_LEN;
    sPackLen = Reverse16(sPackLen);  // ��С��ת��
    memcpy((pchBuff + iOffset), (char*)(&sPackLen), sizeof(sPackLen));
    iOffset += sizeof(sPackLen);

    ptPacketInfo = (PTPackInfo)(pchBuff + iOffset);
    ptPacketInfo->chDestMdl = ptPackInfo->chSrcMdl;
    ptPacketInfo->chSrcMdl  = MODULE_TYPE_GATE;
    ptPacketInfo->chPackId  = ptPackInfo->chPackId;
    ptPacketInfo->chCmd     = ptPackInfo->chCmd;
    iOffset += sizeof(TPackInfo);

    sCrc = cCrc.Crc16((unsigned char*)pchBuff, REG_ACK_PACK_LEN - 2);  // ȥ������crcУ���ֽڳ���
    sCrc = Reverse16(sCrc);
    memcpy((pchBuff + iOffset), (char*)(&sCrc), sizeof(sCrc));

    iResult = m_cGateSvr.SendData(iFd, pchBuff, REG_ACK_PACK_LEN);
    if(RTN_SUCCESS != iResult)
    {
        ErrorPrint("Send regist ack packet error. ");
        delete[] pchBuff;

        return RTN_FAIL;
    }

    delete[] pchBuff;

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : CheckGateSvrData
 ��������  : �����������Ƿ�ɶ�
 �������  : iGateSvrFd----GateSvr��fd
             fdReadSet-----select���õ���������
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::CheckGateSvrData(int &iGateSvrFd, FdSet &fdReadSet)
{
    int        iFd;
    SocketLen  sklAddrLen;
    TSockAddr  tSockAddr;

    memset(&tSockAddr, 0x00, sizeof(TSockAddr));
    sklAddrLen = 0;

    if(FD_ISSET(iGateSvrFd, &fdReadSet))
    {
        iFd = m_cGateSvr.AcceptConnect(&tSockAddr, sklAddrLen);
        if(ACCEPT_ERROR == iFd)
        {
            ErrorPrint("Accept connect error. ");
            exit(-1);
        }
        else
        {
            ErrorPrint("GateSvr client %d have connected, type unknow. ", iFd);

            // ע��ͻ���
            m_cGateSvr.AddGateClient(iFd);
        }
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : CheckClientData
 ��������  : ���ͻ����Ƿ�ɶ�
 �������  : vtMdlFd------GateSvr�ͻ���fd��
             fdReadSet----select���õ���������
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::CheckClientData(vector<int> &vtMdlFd, FdSet &fdReadSet)
{
    int  iResult;
    int  iFd;
    int  iDestFd;
    int  iRecvNum;
    int  iGatePackLen;
    bool bError = false;   // �Ƿ����쳣����
    char achReadBuff[PACK_LEN_MAX];
    char achGateBuff[PACK_LEN_MAX];
    CDecoupler  *pcDecoupler;
    TPackInfo   tPackInfo;
    vector<int>::iterator iter;

    for(iter = vtMdlFd.begin(); iter != vtMdlFd.end(); iter++)
    {
        if(FD_ISSET(*iter, &fdReadSet))
        {
            iFd = *iter;

            iResult= m_cGateSvr.RecvData(iFd, achReadBuff, PACK_LEN_MAX, iRecvNum);
            if(RTN_SUCCESS != iResult)
            {
                iResult = m_cGateSvr.RemoveClient(iFd);
                if(RTN_SUCCESS != iResult)
                {
                    ErrorPrint("Remove client fd = %d error. ", iFd);
                }

                return RTN_FAIL;
            }
            else
            {
                pcDecoupler = m_cGateSvr.GetFdDecoupler(iFd);
                if(NULL == pcDecoupler)
                {
                    ErrorPrint("Cant' find decopuler, then continue loop. ");
                    continue;
                }

                iResult = pcDecoupler->PutData(achReadBuff, iRecvNum);
                if(RTN_SUCCESS != iResult)
                {
                    ErrorPrint("Put date into decoupler error. ");
                    continue;
                }

                // ���������һ�ν���һ�����ݣ�Ϊ�˷�ֹ�ͻ������ݷ��͹���һ���յ�������ݣ�����ѭ��ȡ��
                // ȡ�������������������쳣�����������أ��������ڴ���İ��������´�ȡ����ֱ���������
                while(!(pcDecoupler->IsDcplrEmpty()))
                {
                    //��ȡ��������Ϣ
                    iResult = pcDecoupler->GetPackInfo(&tPackInfo, achGateBuff, PACK_LEN_MAX, iGatePackLen);
                    if(RTN_SUCCESS != iResult)
                    {
                        bError = true;
                        ErrorPrint("Get pack infomation error src module fd = %d. ", iFd);
                        break;   // ��ȡ���쳣����������ֹ������ѭ��
                    }

                    // �Ƿ��͸�3GGate�İ�
                    if(MODULE_TYPE_GATE == tPackInfo.chDestMdl)
                    {
                        iResult = ProcessGateData(iFd, &tPackInfo, achGateBuff, iGatePackLen);
                        if(RTN_SUCCESS != iResult)
                        {
                            bError = true;
                            ErrorPrint("Process gate data error. ");
                            PrintBuffer((char*)("achGateBuff"), achGateBuff, iGatePackLen);
                            continue;
                        }
                    }
                    else if(MODULE_TYPE_BROADCAST == tPackInfo.chDestMdl) // �㲥��Ϣ
                    {
                        iResult = ProcessBroadcast(iFd, achGateBuff, iGatePackLen);
                        if(RTN_SUCCESS != iResult)
                        {
                            bError = true;
                            ErrorPrint("Process broadcast data error. ");
                            continue;
                        }
                    }
                    else
                    {
                        // ��ȡ��Ŀ��ģ���Fd
                        iResult = m_cGateSvr.GetFdByMdlType(tPackInfo.chDestMdl, iDestFd);
                        if(RTN_SUCCESS != iResult)
                        {
                            bError = true;
                            ErrorPrint("Can't find moudle type %2x corresponding module, src module %2x. ",
                                       tPackInfo.chDestMdl, tPackInfo.chSrcMdl);
                            PrintBuffer((char*)("packet"), achGateBuff, iGatePackLen);
                            continue;
                        }

                        // ����ת��Ŀ��ģ��
                        iResult = m_cGateSvr.SendData(iDestFd, achGateBuff, iGatePackLen);
                        if(RTN_SUCCESS != iResult)
                        {
                            bError = true;
                            ErrorPrint("Send data to destination fd = %d error. ", iDestFd);

                            iResult = m_cGateSvr.RemoveClient(iDestFd);
                            if(RTN_SUCCESS != iResult)
                            {
                                ErrorPrint("Remove client fd = %d error. ", iDestFd);
                            }

                            continue;
                        }
                    }
                }
            }
        }
    }

    if(bError)
    {
        return RTN_FAIL;
    }
    else
    {
        return RTN_SUCCESS;
    }
}


/*****************************************************************************
 �� �� ��  : ShowGateRunInfo
 ��������  : ��ӡGate��������Ϣ
 �������  : iShowType----��Ҫ��ӡ��������Ϣ����
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::ShowGateRunInfo(int iShowType)
{
    int iResult;

    switch(iShowType)
    {
        case GATE_INFO_GATESVR:
        {
            iResult = ShowGateSvrInfo();
            if(iResult != RTN_SUCCESS)
            {
                ErrorPrint("Show gate svr info failed. ");
                return RTN_FAIL;
            }

            break;
        }
        case GATE_INFO_MODULE:
        {
            iResult = ShowModuleInfo();
            if(iResult != RTN_SUCCESS)
            {
                ErrorPrint("Show module info failed. ");
                return RTN_FAIL;
            }

            break;
        }
        case GATE_INFO_ALL:
        {
            iResult = ShowGateSvrInfo();
            if(iResult != RTN_SUCCESS)
            {
                ErrorPrint("Show gate svr info failed. ");
                return RTN_FAIL;
            }

            iResult = ShowModuleInfo();
            if(iResult != RTN_SUCCESS)
            {
                ErrorPrint("Show module info failed. ");
                return RTN_FAIL;
            }

            break;
        }
        default:
        {
            break;
        }
    }

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : ShowGateSvrInfo
 ��������  : ��ӡGateSvr����Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::ShowGateSvrInfo()
{
    int iFd;
    int iStatus = 0;
    int iRet;

    iRet = m_cGateSvr.GetGateSvrFd(iFd);
    if(iRet != RTN_SUCCESS)
    {
        ErrorPrint("Get gate svr fd error. ");
        return RTN_FAIL;
    }

    m_cGateSvr.GetGateSvrStatus(iStatus);

    printf(PURPLE"-------------------------------Gate svr info-------------------------------\n");
    printf("                   GateSvr Fd                        Status\n");

    if(GATESVR_STATUS_START == iStatus)
    {
        printf("                      %2d                             Start\n", iFd);
    }
    else
    {
        printf("                      %2d                             Stop\n", iFd);
    }

    printf("---------------------------------------------------------------------------\n"NONE);

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : ShowModuleInfo
 ��������  : ��ӡ����ģ�����Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::ShowModuleInfo()
{
    int  iMdlNum;
    int  iIdx;
    char *apchMdlType[13] = {(char*)("GATE"), (char*)("AV"), (char*)("GUI"), (char*)("BZQ"),
                             (char*)("PHONE"), (char*)("ACCESS/BASIC"),(char*)("MONITOR"), (char*)("PARA"),
                             (char*)("MCU"), (char*)("SCHDL"), (char*)("DATA_PROC"), (char*)("UPDATE"),
                             (char*)("SVR")};
    map<int, PTGateClientInfo> mapModule;
    map<int, PTGateClientInfo>::iterator iter;

    m_cGateSvr.GetModuleInfo(mapModule);

    iMdlNum = mapModule.size();

    printf(YELLOW"--------------------------Gate svr connect module--------------------------\n");
    printf("             Module Fd                                Module Type          \n");

    for(iter = mapModule.begin(); iter != mapModule.end(); iter++)
    {
        if(0x40 == iter->second->iModuleType)
        {
            iIdx = 12;
        }
        else
        {
            iIdx = iter->second->iModuleType - 0x31;   // 0x31ΪGATE
        }

        printf("                %2d                                       %s\n",
               iter->first, apchMdlType[iIdx]);
    }

    printf("---------------------------------------------------------------------------\n");

    printf("Module total num = %d.\n", iMdlNum);

    printf("---------------------------------------------------------------------------\n"NONE);

    return RTN_SUCCESS;
}


/*****************************************************************************
 �� �� ��  : CheckHrtBtTimeOut
 ��������  : ��ӡ����ģ�����Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : RTN_SUCCESS----�ɹ� ����
             RTN_FAIL-------ʧ�� �쳣
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int CGate::CheckHrtBtTimeOut()
{
    int iRet = m_cGateSvr.CheckMdlHrtBtTimeOut();

    return iRet;
}




