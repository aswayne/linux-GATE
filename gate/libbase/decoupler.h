/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : decoupler.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��6��17��
  ����޸�   :
  ��������   : decoupler.cc ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��17��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __DECOUPLER_H__
#define __DECOUPLER_H__


#define PREFIX_LEN_MAX               (4)        // ǰ׺��󳤶�
#define PACK_OFFSET_COMMON           (6)        // (ǰ׺+������)���ֽ���
// ������ƫ��
#define PACK_DATA_OFFSET             (PACK_OFFSET_COMMON + sizeof(TPackInfo))
#define CRC16_VAL_LEN                (0x02)     // CRC16ֵ����

// �ְ���״̬����
#define PACK_STATUS_INIT             (0x01)     // ��ʼ״̬
#define PACK_STATUS_GET_PREFIX       (0x02)     // �ҵ�ǰ׺
#define PACK_STATUS_GET_LENGTH       (0x03)     // ȡ������
#define PACK_STATUS_DECOUPLE_OK      (0x04)     // �ְ����

#define BUFF_SIZE                    (2048)     // ��������С����

class CDecoupler
{
public:
    CDecoupler();
    CDecoupler(char *pchPrefix, int iPrefixLen, int iQueBufSize);
    virtual ~CDecoupler();
    int  PutData(const char *pchBuff, int iCount);         // ���ְ�����������Ҫ�ְ�������
    int  GetData(char *pchBuff, int iLen, int &iPackLen);  // ��ȡ�ְ���ɵ�����
    int  SetPrefix(char *pchPrefix, int iPrefixLen);       // ���÷ְ���ǰ׺
    int  FindPrefix();                                     // �������в���ǰ׺
    bool IsDcplrEmpty();    // �жϷְ������Ƿ�������
    virtual int GetPackInfo(PTPackInfo ptPackInfo, char *pchPackData, int iPackBuffSize, int &iPackLen);   // ��ȡ��������Ϣ

private:
    int     m_iStatus;     // �ְ���״̬
    char    m_achPrefix[PREFIX_LEN_MAX];  // ��ǰ׺
    int     m_iPrefixLen;  // ǰ׺����
    short   m_sPackLen;    // ������
    CCirQue *m_pcCirQue;   // ѭ������
    int     m_iCirQueLen;  // ѭ�����л��泤��
    ThreadMutex m_tMutex;  // �������ݺ�ȡ���ݻ���ʹ��
};

#endif /* __DECOUPLER_H__ */




