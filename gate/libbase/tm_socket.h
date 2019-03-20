/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : tmsocket.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��7��10��
  ����޸�   :
  ��������   : tmsocket.cc ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��7��10��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TMSOCKET_H__
#define __TMSOCKET_H__

/*--------------------------------��������-----------------------------------*/

#define BIND_ERROR             (-1)         // bind���÷��ش���
#define ACCEPT_ERROR           (-1)         // accept���÷��ش���


/*--------------------------------���Ͷ���-----------------------------------*/

typedef struct sockaddr    TSockAddr;
typedef struct sockaddr_un TSockAddrUn;
typedef struct sockaddr_in TSockAddrIn;
typedef struct timeval     TTimeVal;
typedef struct linger      TSoLinger;


/*--------------------------------�ӿڶ���-----------------------------------*/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

int WriteN(int iFd, char *pchBuff, int iWriteLen);                    // дn��byte��������
int ReadN(int iFd, char *pchBuff, int iBuffLen, int &iReadLen);       // ����������n��byte
int CnnctToServerUn(TSockAddrUn tSvrAddr, int *piSvrFd);              // ���ӵ�Unix���ط����
int CnnctToServerIn(TSockAddrIn tSvrAddr, int *piSvrFd);              // ���ӵ�IP���������
int StartServerUn(TSockAddrUn tSvrAddr, int *piSvrFd, int iLstnNum);  // ����Unix�����

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __TMSOCKET_H__ */




