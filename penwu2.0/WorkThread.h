#pragma once
#define WM_DISPLAY WM_USER+2

#define ONESTOPBIT 0   //ͨ��Э������Ķ���
 #define _AFXDLL
// CWorkThread

class CWorkThread : public CWinThread
{
	DECLARE_DYNCREATE(CWorkThread)

protected:
	CWorkThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CWorkThread();
	
public:
	virtual BOOL InitInstance();  //��ʼ������������
	virtual int ExitInstance();
	afx_msg void OnWork(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
};


