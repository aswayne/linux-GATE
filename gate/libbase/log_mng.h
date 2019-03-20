/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : logmng.h
  �� �� ��   : ����
  ��    ��   : tm8731_xgui
  ��������   : 2015��7��11��
  ����޸�   :
  ��������   : logmng.cc ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��7��11��
    ��    ��   : tm8731_xgui
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __LOGMNG_H__
#define __LOGMNG_H__

#define LOG_CONFIG "/mnt/flash/gate_log.conf"
#define LOG_PATH   "/mnt/nand/log/"
#define LOG_CAT    "gate_cat"


#define LOG_CONFIG_TEXT   \
"[global]\n"\
"strict init = true\n"\
"buffer min = 1024\n"\
"buffer max = 2048\n"\
"#��־����Ȩ��,600 ֻ����ǰ�û�����\n"\
"file perms = 600\n"\
"[formats]\n"\
"simple	= \"%d.%-8.8us %-5V [%-8.8p.%-8.8t %F %L] %m%n\"\n"\
"simple1 = \"[%F %L] %m%n\"\n"\
"[rules]\n"\
"gate_cat.warn		>stderr;simple1\n"\
"gate_cat.ERROR	\"/mnt/nand/log/gate.txt\",100kb * 3 ~ \"/mnt/nand/log/gate.txt.#r\";simple\n"


typedef zlog_category_t     TLog;

extern TLog *g_ptLogHdl;

// �ּ���ӡ,��ͬ������ɫ��ͬ
#define LOG_DEBUG(format, args...)        zlog_debug(g_ptLogHdl, LIGHT_GREEN format NONE, ##args) // ǳ��
#define LOG_INFO(format, args...)         zlog_info(g_ptLogHdl, BLUE format NONE, ##args)         // ��
#define LOG_NOTICE(format, args...)       zlog_notice(g_ptLogHdl, YELLOW format NONE, ##args)     // ��
#define LOG_WARN(format, args...)         zlog_warn(g_ptLogHdl,  CYAN format NONE, ##args)        // ��
#define LOG_ERROR(format, args...)        zlog_error(g_ptLogHdl, PURPLE format NONE, ##args)      // ��
#define LOG_FATAL(format, args...)        zlog_fatal(g_ptLogHdl,  RED format NONE, ##args)        // ��


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

int  InitLogMng(); // ��ʼ����־ģ��
int  CheckLogCfgFile(const char *pchFile);   // �����־�����ļ�
int  CheckLogSaveDir(const char *pchLogDir); // �����־�ļ�Ŀ¼
TLog *GetLogHandler();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __LOGMNG_H__ */




