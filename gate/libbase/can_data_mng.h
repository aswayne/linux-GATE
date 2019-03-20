/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : can_data_mng.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2015��11��09��
  ����޸�   :
  ��������   : CAN���ݹ�������ض���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��11��09��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __CAN_DATA_MNG_H__
#define __CAN_DATA_MNG_H__


class CCanDataMng
{
public:
    CCanDataMng();
    ~CCanDataMng();
    int Init();  // ��ʼ��
    int ProcessCanData(int iCanId, char *pchCanData, int iDataLen);  // ����CAN����
    int AddCanDataItem(int iItemType, CCanData *pcCanData);  // ����CAN���ݴ�����
    int RemoveCanDataItem(int iItemType);  // ɾ��CAN���ݴ�����

private:
    map<int, CCanData*> m_mpCanDataMng;  // Can���ݹ������� <CAN���ݴ���������, ������ʵ��ָ��>

};


#endif /* __CAN_DATA_MNG_H__ */




