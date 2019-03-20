/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ֣�������Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : cmd_proc.h
  �� �� ��   : ����
  ��    ��   : WangGaofeng
  ��������   : 2016��3��4��
  ����޸�   :
  ��������   : ָ�����ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��3��4��
    ��    ��   : WangGaofeng
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __CMD_PROC_H__
#define __CMD_PROC_H__


/*----------------------------------�ඨ��-----------------------------------*/

template <typename TypeCmd, typename TypeFunc, typename TypeFnData, typename TypeFnDataLen>
class CCmdProc
{
public:
    CCmdProc() {}
    ~CCmdProc() {}
    int RegCmdProcFunc(TypeCmd tpCmd, TypeFunc tpCmdProcFunc);   // ע�ᴦ����
    int ProcCmdData(TypeCmd tpCmd, TypeFnData tpData, TypeFnDataLen tpDataLen);  // ����ָ������
protected:
    map<TypeCmd, TypeFunc> m_mpCmdProcMap;   // �������봦����ӳ���
};


/*----------------------------------��ʵ��-----------------------------------*/

template <typename TypeCmd, typename TypeFunc, typename TypeFnData, typename TypeFnDataLen>
int CCmdProc<TypeCmd, TypeFunc, TypeFnData, TypeFnDataLen>::RegCmdProcFunc(TypeCmd tpCmd, TypeFunc tpCmdProcFunc)
{
    typename map<TypeCmd, TypeFunc>::iterator iter = m_mpCmdProcMap.find(tpCmd);
    if(iter != m_mpCmdProcMap.end())
    {
        cout<<"Cmd "<<tpCmd<<" has been registered. "<<endl;
        return RTN_FAIL;
    }
    else
    {
        m_mpCmdProcMap[tpCmd] = tpCmdProcFunc;
    }

    return RTN_SUCCESS;
}


template <typename TypeCmd, typename TypeFunc, typename TypeFnData, typename TypeFnDataLen>
int CCmdProc<TypeCmd, TypeFunc, TypeFnData, TypeFnDataLen>::ProcCmdData(TypeCmd tpCmd, TypeFnData tpData, TypeFnDataLen tpDataLen)
{
    typename map<TypeCmd, TypeFunc>::iterator iter = m_mpCmdProcMap.find(tpCmd);
    if(iter != m_mpCmdProcMap.end())
    {
        iter->second(tpCmd, tpData, tpDataLen);
    }
    else
    {
        cout<<"Can't find cmd "<<tpCmd<<" process function. "<<endl;
        return RTN_FAIL;
    }

    return RTN_SUCCESS;
}


#endif /* __CMD_PROC_H__ */




