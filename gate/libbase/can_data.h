/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : can_data.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��11��09��
  ����޸�   :
  ��������   : CAN����ҵ������ض���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��11��09��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __CAN_DATA_H__
#define __CAN_DATA_H__


/*---------------------------------���Ͷ���----------------------------------*/

typedef boost::function<int(int, char*, int)>  PCanIdFunc;  // Can Id��Ӧ�ص���������


/*----------------------------------�궨��-----------------------------------*/

#define BIND_CAN_ID_FUNC(CLASS, FUNCTION)  bind(&CLASS::FUNCTION, this, _1, _2, _3)


/*---------------------------------�ṹ����----------------------------------*/
#pragma pack(1)

// �豸Can��������
typedef struct tagTCanDataFromDev
{
    unsigned int  uiCanId;       // CAN ID
    unsigned int  uiDataLen;     // CAN���ݳ���
    unsigned char ucCanPort;     // CAN�� : CAN_PORT_1, CAN_PORT_2
    unsigned char aucCanData[0]; // CAN����
} TCanDataFromDev, *PTCanDataFromDev;

#pragma pack()


/*----------------------------------�ඨ��-----------------------------------*/

class CCanData
{
public:
    CCanData();
    virtual ~CCanData();
    virtual int Init();  // ��ʼ��
    virtual int ProcessCanData(int iCanId, char *pcCanData, int iDataLen);  // ����CAN����
    virtual int RegCanIdFunc(int iCanId, PCanIdFunc pfnCanIdFunc);  // ע��CAN ID������

protected:
    map<int, PCanIdFunc> m_mpCanIdFuncMap; // <CanId, CanId��Ӧ�ص�>

};


#endif /* __CAN_DATA_H__ */



