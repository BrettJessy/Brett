// WorkThread.cpp : 实现文件
//

#include "stdafx.h"
#include "penwu2.0.h"

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <winbase.h>
#include<string.h>
#include "WorkThread.h"
#include "penwu2.0Dlg.h"
#include "calculate_xy.h"
//#include "global.h"
#include "urg_sensor.h"

extern char *data_compute;
extern BOOL RUN;             //线程启停
extern BOOL  L;		  //左侧喷雾
extern BOOL  R;		  //右侧喷雾
extern BOOL  AUTO;         //单选按钮状态;自动手动
extern float V;           //速度
extern BOOL NOZZLE[8];           //初始化8个灯的状态
extern urg_t urg;
extern long time_stamp;
extern long *data;
extern char *DC_FIFO;
extern int n_delay;
extern float distance;  //传感器距离喷嘴距离单位为米
extern float t_delay;
extern float *rho;

extern HANDLE hCom;
extern COMMTIMEOUTS TimeOuts;
extern DCB dcb;
extern byte lpOutBuffer[1];
extern OVERLAPPED m_osWrite;
//extern DWORD dwBytesWrite;
extern COMSTAT ComStat;
extern DWORD dwErrorFlags;
extern BOOL bWriteStat;
// CWorkThread

IMPLEMENT_DYNCREATE(CWorkThread, CWinThread)

CWorkThread::CWorkThread()
{
}

CWorkThread::~CWorkThread()
{
}

BOOL CWorkThread::InitInstance()
{
	                        // TODO:    在此执行任意逐线程初始化
	//n_delay = floor((distance / V - t_delay) / 0.025 - 1) + 1;                             //初始化参数n_delay
	capture_start(0,NULL);                       //分配空间FIFO
                               //LiDAR 初始化
	hCom = CreateFile(L"COM8",  //COM4口   
		GENERIC_READ | GENERIC_WRITE, //允许读和写  
		0,  //独占方式   
		NULL,
		OPEN_EXISTING,  //打开而不是创建   
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //重叠方式               读写到重叠方式此段全部跳过
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)                       //直接在这儿跳出，printf("error!");
	{
		printf("error!");
		//			return 0;
	}

	//串口初始化
	SetupComm(hCom, 100, 100); //输入缓冲区和输出缓冲区的大小都是100
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 9600;//波特率为9600
	dcb.ByteSize = 8;//每个字节有8位
	dcb.Parity = EVENPARITY;      //有偶校验位
	dcb.StopBits = ONESTOPBIT;//一个停止位
	PurgeComm(hCom, PURGE_TXCLEAR);   //初始化| PURGE_TXABORT
	SetCommState(hCom, &dcb);

	
     	
	
	return TRUE;
}

int CWorkThread::ExitInstance()
{
	             // TODO:    在此执行任意逐线程清理
	               //释放空间
	if(data!=NULL)
         free(data);
	if (data_compute!= NULL)
	free(data_compute);
	if (rho!= NULL)
	free(rho);
	if (DC_FIFO != NULL)
	free(DC_FIFO);
	PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);
	CloseHandle(hCom);             //结束通信  
	
	return CWinThread::ExitInstance();

	
}


BEGIN_MESSAGE_MAP(CWorkThread, CWinThread)
	ON_THREAD_MESSAGE(WM_WORK, &CWorkThread::OnWork)
END_MESSAGE_MAP()


void CWorkThread::OnWork(WPARAM wParam, LPARAM lParam)
{
	while (1)
	{
		//1.给出喷雾指令NOZZLE

		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (RUN)
		{
			if (AUTO )
			{
				int n = urg_get_distance(&urg, data, &time_stamp);             //自动対靶喷雾程序
				data_process();                                                 //1.capture
				for (int i = 0;i <= 7;i++)
				{
					if(DC_FIFO[i]==0)
					   NOZZLE[i] = FALSE;
					else
					   NOZZLE[i] = TRUE;
				}                                                                  //2.detect
			}
			else
			{  //手动
				for (int i = 0;i <= 7;i++)NOZZLE[i] = TRUE;
			}
			if (!L )            //自动喷雾或手动喷雾依旧要选择左右喷雾状态
				for(int i = 0;i <= 3;i++)NOZZLE[i] = FALSE;
			if (!R )
				for (int i = 4;i <= 7;i++)NOZZLE[i] = FALSE;
			lpOutBuffer[0] = 0x00; // 8位
			for (int i = 0; i <= 7; i++)
			{
				if (NOZZLE[i] == TRUE)
					lpOutBuffer[0] = lpOutBuffer[0] | 0x80;         // lpOutBuffer值多右移了一次,必须多加一次if判断是否右移了6次
				if (i<7)
					lpOutBuffer[0] = lpOutBuffer[0] >> 1;
			}
		
		}
		else
		{for (int i = 0; i <=7; i++)NOZZLE[i] = FALSE;
	     lpOutBuffer[0] = 0x00;
		}
	DWORD dwBytesWrite = 1;
	ClearCommError(hCom, &dwErrorFlags, &ComStat);
	bWriteStat = WriteFile(hCom, lpOutBuffer,                  //bwritestat为0时表示写入未完成，为1表示写入完成，不再写入
		dwBytesWrite, &dwBytesWrite, &m_osWrite);
	if (!bWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osWrite.hEvent, 1000);
		}
	}
	
	

	
		//2.发送指令    
		 Sleep(50);
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_DISPLAY, NULL, NULL);
	}
	
}
// CWorkThread 消息处理程序

