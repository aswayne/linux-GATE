/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : tmbaselib.h
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
#ifndef __TMBASELIB_H__
#define __TMBASELIB_H__


/*-------------------------------ϵͳ����ͷ�ļ�------------------------------*/

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <termios.h>


/*----------------------baselib�Զ���ͷ�ļ�(���Ⱥ�˳��)----------------------*/

#include "print_color.h"
#include "zlog.h"
#include "lib_config.h"
#include "log_mng.h"
#include "debug.h"
#include "tm_socket.h"
#include "common.h"
#include "crc_public.h"
#include "circular_queue.h"
#include "decoupler.h"
#include "gb2unicode_ucs2.h"
#include "client.h"
#include "client_mng.h"
#include "cmd_proc.h"
#include "serial.h"
#include "tm_can_pack_info_mng.h"
#include "tm_can_data_send.h"
#include "tm_can_data_recv.h"
#include "timer.h"
#include "can_data.h"
#include "can_data_mng.h"
#include "can_data_recv.h"
#include "resend_data.h"
#include "resend_data_mng.h"


#endif /* __TMBASELIB_H__ */




