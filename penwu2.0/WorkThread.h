#pragma once
#define WM_DISPLAY WM_USER+2

#define ONESTOPBIT 0   //通信协议里面的定义
 #define _AFXDLL
// CWorkThread

class CWorkThread : public CWinThread
{
	DECLARE_DYNCREATE(CWorkThread)

protected:
	CWorkThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CWorkThread();
	
public:
	virtual BOOL InitInstance();  //初始化事例的类型
	virtual int ExitInstance();
	afx_msg void OnWork(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
};


