/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : serial.h
  �� �� ��   : ����
  ��    ��   : WangGaofeng
  ��������   : 2016��4��20��
  ����޸�   :
  ��������   : ������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��4��20��
    ��    ��   : WangGaofeng
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __SERIAL_H__
#define __SERIAL_H__


/*----------------------------------�궨��-----------------------------------*/

#define BIND_SRL_PT_FUNC(CLASS, FUNCTION)       bind(&CLASS::FUNCTION, this, _1, _2, _3)
#define BIND_SRL_CNT_SCS_FUNC(CLASS, FUNCTION)  bind(&CLASS::FUNCTION, this, _1)

#define SERIAL_PORT_NAME_LEN_MAX             (0x80)   // �������ֳ������ֵ(128)


/*---------------------------------���Ͷ���----------------------------------*/

class CSerial;

typedef boost::function<int(char, char*, int)>  PSrlPrtFunc;
typedef boost::function<int(CSerial* )>      PSrlCntScsFunc;


/*----------------------------------�ඨ��-----------------------------------*/

class CSerial
{
public:
    CSerial();
    CSerial(char *pchSrlPrtNm, int iBaud, CDecoupler *pcSrlPrtDcplr);
    ~CSerial();
    int InitSerialPort();  // ��ʼ������
    int OpenSerialPort();  // �򿪴���
    int CfgSerialPort(int iBaud);   // ���ô���
    int DisCnctSrlPrt();   // �Ͽ�����
    int SendData(char *pchSendData, int iSendLen);   // �Ӵ��ڷ�������
    int RecvData(char *pchRecvBuff, int iRecvBuffLen, int &iRecvLen);  // �Ӵ��ڽ�������
    int GetConnectStatus(int &iCnctStus);  // ��ȡ��������״̬
    int GetConnectFd(int &iFd);  // ��ȡ�������ӵ�������
    int RegSerialPortCmdFunc(char chCmd, PSrlPrtFunc pfnSrlPrtFunc);  // ע�ᴮ�������ִ�����
    int ProcSerialPortData(char chCmd, char *pchData, int iDataLen);  // ����������
    int PutDataToDcplr(char *pchData, int iDataLen);  // �����ݷ���ְ���
    int GetPackInfo(PTPackInfo ptPackInfo, char *pchPackData, int iPackBufSize, int &iPackLen);  // ��ȡ�ְ�����
    int RegCnctSucsFunc(PSrlCntScsFunc pfnCntScsFunc);  // ע�ᴮ�����ӳɹ�������
    bool IsDecouplerEmpty();  // �ְ����Ƿ�Ϊ��

private:
    CCmdProc<char, PSrlPrtFunc, char*, int> m_cSrlPtProc;    // �����������
    char m_achSrlPtNm[SERIAL_PORT_NAME_LEN_MAX];             // ������
    CDecoupler *m_pcSrlPtDplr;          // �ְ���ʵ��
    PSrlCntScsFunc m_pfnSrlCntScsFunc;  // �������Ӵ�����
    int m_iSrlPrtFd;     // ��������Fd
    int m_iSrlCnctStus;  // ��������״̬
    int m_iBaud;  // ���ڲ�����
};


#endif /* __SERIAL_H__ */




