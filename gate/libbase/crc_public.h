/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : crcpublic.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��6��17��
  ����޸�   :
  ��������   : crcpublic.cc ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��17��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __CRCPUBLIC_H__
#define __CRCPUBLIC_H__

class CCrcPublic
{
 public:
    CCrcPublic();
    virtual ~CCrcPublic();
    virtual unsigned short Crc16(unsigned char *pucDt, unsigned short usLen);     // CRC16����
    virtual unsigned short Crc16Big(unsigned char *pucDt, unsigned short usLen);  // CRC16����(���Ϊ���ģʽ)
    virtual unsigned char  Crc8(unsigned char *pucDt, unsigned short usLen);      // CRC8����
 private:
    static const unsigned short m_asCrc16Tab[256];  // CRC16���
    static const unsigned char  m_aucCrc8Tab[256];  // CRC8���
};

#endif




