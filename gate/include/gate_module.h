/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : tmgate.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��7��10��
  ����޸�   :
  ��������   : ͷ�ļ���������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��7��10��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __TMGATE_H__
#define __TMGATE_H__


/*-------------------------------ϵͳ����ͷ�ļ�------------------------------*/

#include <stdio.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <memory.h>
#include <termios.h>
#include <map>
#include <vector>


/*----------------------gate�Զ���ͷ�ļ�(���Ⱥ�˳��)-------------------------*/

#include "tm_libbase.h"
#include "gate_server.h"
#include "gate.h"


/*----------------------------------�궨��-----------------------------------*/

// ģ��������
#ifdef  MODULE_NAME
#undef  MODULE_NAME
#endif

#define MODULE_NAME          "[Gate]"

// ��ǰģ�鶨��
#ifdef CUR_MODULE
#undef CUR_MODULE
#endif

#define CUR_MODULE   MODULE_TYPE_GATE


#endif /* __TMGATE_H__ */




