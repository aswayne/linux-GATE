/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : debug.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��6��27��
  ����޸�   :
  ��������   : debug.cc ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��27��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DEBUG_H__
#define __DEBUG_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern bool g_bErrorPrintFlag;

#define MODULE_NAME   "[Libbase]"

#if ERROR_PRINT_ON
#define ErrorPrint(format,...) \
{\
    if (g_bErrorPrintFlag) \
    {\
        printf(MODULE_NAME"[%s:%d][%s]:", __FILE__,  __LINE__, __FUNCTION__), \
        printf(format,##__VA_ARGS__), printf("\n");\
    }\
}
#else
#define ErrorPrint(format,...)
#endif

void SetErrorPrintFlag(bool bFlag);
void PrintBuffer(char *pchBuffName, char *pchBuff, int iLen);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __DEBUG_H__ */




