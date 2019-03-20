/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : circularqueue.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��6��17��
  ����޸�   :
  ��������   : circularqueue.cc ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��6��17��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __CIRCULARQUEUE_H__
#define __CIRCULARQUEUE_H__

class CCirQue
{
public:
    CCirQue(int iBuffSize);
    ~CCirQue();
    int  PutData(const char *pchBuff, int iCount);   // �����л����з�����
    int  GetData(char *pchBuff, int iCount);         // �Ӷ�����ȡ����
    int  GetDataTemp(char *pchBuff, int iCount);     // �Ӷ��л�������ʱȡ����
    bool IsEmpty();          // �ж϶����Ƿ�Ϊ��
    bool IsFull();           // �ж϶����Ƿ�����
    int  GetUsedSpace();     // ��ȡ���л�����ʹ�õĿռ�
    int  GetFreeSpace();     // ��ȡ���л���ʣ��ռ�
    int  SetFront();         // ���ö�ͷָ��
    int  Clear();            // ��ն���
    int  PrintCirQueBuff();  // ��ӡ��������

private:
    char *m_pchBuff;     // ѭ�����л�����
    int  m_iBuffSize;    // ��������С
    int  m_iFront;       // ѭ�����ж�ͷ
    int  m_iRear;        // ѭ�����ж�β
    int  m_iTempCnt;     // ��ʱȡ��������
};

#endif /* __CIRCULARQUEUE_H__ */




