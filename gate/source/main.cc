/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : main.cc
  �� �� ��   :
  ��    ��   : wanggaofeng
  ��������   :
  ����޸�   :
  ��������   :
  �����б�   :
  �޸���ʷ   :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
******************************************************************************/


#include "gate_module.h"


/*****************************************************************************
 �� �� ��  : SignalHander
 ��������  : �źŴ�����
 �������  : int signal
 �������  : ��
 �� �� ֵ  :
 �޸���ʷ      :
  1.��    ��   : 2013��11��8�� ������
    ��    ��   : wuquanwei
    �޸�����   : �����ɺ���

*****************************************************************************/
void SignalHander(int signal)
{
    printf("Gate get signal ( signal : %d )( pid: %d )\n", signal, (int)(getpid()));

    if ( signal != 6 )  //28: SIGWINCH 6: SIGABRT
    {
        exit(-1);
    }
}


/*****************************************************************************
 �� �� ��  : AvCaptureAllSignal
 ��������  : ��׽���е��ź�
 �������  : ��
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2012��12��1�� ������
    ��    ��   : wuquanwei
    �޸�����   : �����ɺ���

*****************************************************************************/
void CaptureAllSignal()
{
    int iLoop;
    for(iLoop = 0; iLoop < 32; iLoop++)
    {
        if((iLoop == SIGCHLD) || (iLoop == SIGPIPE) || (iLoop == SIGWINCH))
        {
            continue;
        }

        signal(iLoop, SignalHander);
    }
}


/*****************************************************************************
 �� �� ��  : main
 ��������  : main����
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   :
*****************************************************************************/
int main(int argc, char* argv[])
{
    int         iResult;
    TSockAddrUn tGateSvrAddr;

    CaptureAllSignal();
    signal(SIGPIPE, SIG_IGN);

    tGateSvrAddr.sun_family = AF_UNIX;
    strcpy(tGateSvrAddr.sun_path, GATE_LOCAL_PATH);

    // ���ô��ι��캯��
    CGate cGate(&tGateSvrAddr);
    iResult = cGate.Init();
    if(RTN_SUCCESS != iResult)
    {
        ErrorPrint("Gate init failed. ");
        exit(-1);
    }

    cGate.ProcessData();

    return RTN_SUCCESS;
}




