/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : common.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��6��16��
  ����޸�   :
  ��������   : ���ó������ṹ����ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��16��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__


/*-------------------------------��������------------------------------------*/

#define RTN_SUCCESS            (0x00)       // �ɹ� ����
#define RTN_FAIL               (0x01)       // ʧ�� �쳣

#define ABS(val)   (((val) > 0) ? (val) : (-(val)))    // �������ֵ

#define GATE_RCVE_TIME_OUT      (0x00)      // ���ճ�ʱ

#define PATH_LEN_MAX            (128)       // ·���������ֵ

// ģ�����Ͷ���
#define MODULE_TYPE_INVALID     (0x00)      // ��Ч��ģ������
#define MODULE_TYPE_MIN         (0x2F)      // ģ��������Сֵ(У��ʹ��)
#define MODULE_TYPE_BROADCAST   (0x30)      // �㲥��ַ,��������ģ��
#define MODULE_TYPE_GATE        (0x31)      // 3GGATEģ��
#define MODULE_TYPE_AV          (0x32)      // AVģ��
#define MODULE_TYPE_GUI         (0x33)      // UIģ��
#define MODULE_TYPE_BZQ         (0x34)      // ��վ��ģ��
#define MODULE_TYPE_PHONE       (0x35)      // PHONEģ��
#define MODULE_TYPE_ACCESS      (0x36)      // ACCESSģ��(tm8726��ʹ��)
#define MODULE_TYPE_BASIC       (0x36)      // BASICģ��(tm8721��ʹ��)
#define MODULE_TYPE_MONITOR     (0x37)      // Monitorģ��
#define MODULE_TYPE_PARA        (0x38)      // ����ģ��
#define MODULE_TYPE_MCU         (0x39)      // ���ػ�ģ��
#define MODULE_TYPE_SCHDL       (0x3A)      // ����ģ��
#define MODULE_TYPE_DATA_PROC   (0x3B)      // ���ݴ���ģ��
#define MODULE_TYPE_NET_UPDATE  (0x3C)      // ����ģ��
#define MODULE_TYPE_WIFI        (0x3D)      // WIFIģ��
#define MODULE_TYPE_SVR         (0x40)      // ������ģ��
#define MODULE_TYPE_MAX         (0x41)      // ģ���������ֵ(У��ʹ��)

#define CUR_MODULE              MODULE_TYPE_INVALID

#define FD_INVALID              (-1)        // Fd����Чֵ

#define CONNECT_STATUS_OK       (0x00)      // ����״̬ OK
#define CONNECT_STATUS_BAD      (0x01)      // ����״̬ �Ͽ�

#define SOCK_ADDR_INET          (0x01)      // INET��ַ
#define SOCK_ADDR_UNIX          (0x02)      // UNIX����

#define PACK_LEN_MAX            (2048)      // ������󳤶�

// �����ֶ���
#define GATE_MOUDLE_REG_CMD     (0x01)      // ģ����Gate����ע���������
#define GATE_HEART_BEAT_CMD     (0x02)      // ģ����Gate����������

#define GATE_LOCAL_PATH   "/tmp/tm_sock"    // GATE����Sock file


// �����ģ�鷢��Э�����ݰ�
#define SendDataToSchdl(pcGate, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_SCHDL; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToGate(pcGate, &tPackInfo, pchData, iDataLen); \
}

// ��վ��ģ�鷢��Э�����ݰ�
#define SendDataToBzq(pcGate, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_BZQ; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToGate(pcGate, &tPackInfo, pchData, iDataLen); \
}

// ��Accessģ�鷢��Э�����ݰ�
#define SendDataToAccess(pcGate, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_ACCESS; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToGate(pcGate, &tPackInfo, pchData, iDataLen); \
}

// ��Svrģ�鷢��Э�����ݰ�
#define SendDataToSvr(pcGate, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_SVR; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToGate(pcGate, &tPackInfo, pchData, iDataLen); \
}

// ��DataProcģ�鷢��Э�����ݰ�
#define SendDataToDataProc(pcGate, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_DATA_PROC; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToGate(pcGate, &tPackInfo, pchData, iDataLen); \
}

// �����ģ�鷢��Э�����ݰ�
#define SendDataToPara(pcGate, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_PARA; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToGate(pcGate, &tPackInfo, pchData, iDataLen); \
}

// ��GUIģ�鷢��Э�����ݰ�
#define SendDataToGui(pcGate, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_GUI; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToGate(pcGate, &tPackInfo, pchData, iDataLen); \
}

// ������ģ�鷢��Э�����ݰ�
#define SendDataToNetUpdate(pcGate, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_NET_UPDATE; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToGate(pcGate, &tPackInfo, pchData, iDataLen); \
}

// ��Gateģ�鷢��Э�����ݰ�
#define SendDataToGate(pcGate, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_GATE; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToGate(pcGate, &tPackInfo, pchData, iDataLen); \
}

// ��Ƭ��ģ�鷢��Э�����ݰ�
#define SendDataToMcu(pcSerial, chPackCmd, pchData, iDataLen)  \
{ \
    TPackInfo tPackInfo; \
\
    tPackInfo.chDestMdl = MODULE_TYPE_MCU; \
    tPackInfo.chSrcMdl  = CUR_MODULE; \
    tPackInfo.chPackId  = 0x00;  \
    tPackInfo.chCmd     = chPackCmd; \
\
    SendPackToSrlPrt(pcSerial, &tPackInfo, pchData, iDataLen); \
}


/*-------------------------------���Ͷ���----------------------------------*/

typedef pthread_t          ThreadId;
typedef fd_set             FdSet;
typedef socklen_t          SocketLen;
typedef pthread_mutex_t    ThreadMutex;


/*-------------------------------�ṹ����----------------------------------*/
#pragma pack(1)

// ��������Ϣ
typedef struct tagTPackInfo
{
    char chDestMdl;  // Ŀ��ģ��
    char chSrcMdl;   // Դģ��
    char chPackId;   // ��Id
    char chCmd;      // ������
} TPackInfo, *PTPackInfo;

// ��Ҫ�������������
typedef struct tagTPackData
{
    char *pchPrefix;   // ��ǰ׺
    int  iPrefixLen;   // ǰ׺����
    char *pchData;     // �������
    int  iDataLen;     // ������ݳ���
} TPackData, *PTPackData;

// ��ַ���Ͷ���
typedef struct tagTAddr
{
    int iAddrType;  // ��ַ����(SOCK_ADDR_INET��SOCK_ADDR_UNIX)
    TSockAddrIn tSockAddr;    // INET�׽��ֵ�ַ
    TSockAddrUn tSockAddrUn;  // UNIX�����׽��ֵ�ַ
} TAddr, *PTAddr;

// ʱ������
typedef struct tagTTimeData
{
    char chYear;   // ��
    char chMonth;  // ��
    char chDay;    // ��
    char chHour;   // ʱ
    char chMin;    // ��
    char chSec;    // ��
} TTimeData, *PTTimeData;

#pragma pack()

class CClient;
class CSerial;

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

bool  CheckModuleType(int iMdlType);    // ���ģ�����͵���Ч��
int   CreatFifo(char *pchPath);         // �����ܵ��ļ�
short Reverse16(short sVal);            // short�ͱ����Ĵ�С��ת��
int   Reverse32(int iVal);              // int�ͱ����Ĵ�С��ת��
int   GetCompressTime(unsigned int &uiCmprsTime);  // ��ȡѹ����ʽ��ʱ��
int   GetCurTime(PTTimeData ptCurTime);            // ��ȡ��ǰʱ��(��ʽNNYYDDHHMMSS)
void  EncryptData(unsigned char *pucEyptData);     // ����Ͷ�һ�����

// Gateת�����ݴ��
int PackGateData(PTPackData ptPackData, PTPackInfo ptPackInfo, char *pchData, short &sDataLen);
// ��Gate����Э�����ݰ�
int SendPackToGate(CClient *pcGate, PTPackInfo ptPackInfo, char *pchData, int iDataLen);
int SendPackToSrlPrt(CSerial *pcSerial, PTPackInfo ptPackInfo, char *pchData, int iDataLen);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __COMMON_H__ */




