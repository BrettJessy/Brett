// WorkThread.cpp : ʵ���ļ�
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
extern BOOL RUN;             //�߳���ͣ
extern BOOL  L;		  //�������
extern BOOL  R;		  //�Ҳ�����
extern BOOL  AUTO;         //��ѡ��ť״̬;�Զ��ֶ�
extern float V;           //�ٶ�
extern BOOL NOZZLE[8];           //��ʼ��8���Ƶ�״̬
extern urg_t urg;
extern long time_stamp;
extern long *data;
extern char *DC_FIFO;
extern int n_delay;
extern float distance;  //����������������뵥λΪ��
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
	                        // TODO:    �ڴ�ִ���������̳߳�ʼ��
	//n_delay = floor((distance / V - t_delay) / 0.025 - 1) + 1;                             //��ʼ������n_delay
	capture_start(0,NULL);                       //����ռ�FIFO
                               //LiDAR ��ʼ��
	hCom = CreateFile(L"COM8",  //COM4��   
		GENERIC_READ | GENERIC_WRITE, //�������д  
		0,  //��ռ��ʽ   
		NULL,
		OPEN_EXISTING,  //�򿪶����Ǵ���   
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�ص���ʽ               ��д���ص���ʽ�˶�ȫ������
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)                       //ֱ�������������printf("error!");
	{
		printf("error!");
		//			return 0;
	}

	//���ڳ�ʼ��
	SetupComm(hCom, 100, 100); //���뻺����������������Ĵ�С����100
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 9600;//������Ϊ9600
	dcb.ByteSize = 8;//ÿ���ֽ���8λ
	dcb.Parity = EVENPARITY;      //��żУ��λ
	dcb.StopBits = ONESTOPBIT;//һ��ֹͣλ
	PurgeComm(hCom, PURGE_TXCLEAR);   //��ʼ��| PURGE_TXABORT
	SetCommState(hCom, &dcb);

	
     	
	
	return TRUE;
}

int CWorkThread::ExitInstance()
{
	             // TODO:    �ڴ�ִ���������߳�����
	               //�ͷſռ�
	if(data!=NULL)
         free(data);
	if (data_compute!= NULL)
	free(data_compute);
	if (rho!= NULL)
	free(rho);
	if (DC_FIFO != NULL)
	free(DC_FIFO);
	PurgeComm(hCom, PURGE_TXABORT | PURGE_TXCLEAR);
	CloseHandle(hCom);             //����ͨ��  
	
	return CWinThread::ExitInstance();

	
}


BEGIN_MESSAGE_MAP(CWorkThread, CWinThread)
	ON_THREAD_MESSAGE(WM_WORK, &CWorkThread::OnWork)
END_MESSAGE_MAP()


void CWorkThread::OnWork(WPARAM wParam, LPARAM lParam)
{
	while (1)
	{
		//1.��������ָ��NOZZLE

		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (RUN)
		{
			if (AUTO )
			{
				int n = urg_get_distance(&urg, data, &time_stamp);             //�Զ������������
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
			{  //�ֶ�
				for (int i = 0;i <= 7;i++)NOZZLE[i] = TRUE;
			}
			if (!L )            //�Զ�������ֶ���������Ҫѡ����������״̬
				for(int i = 0;i <= 3;i++)NOZZLE[i] = FALSE;
			if (!R )
				for (int i = 4;i <= 7;i++)NOZZLE[i] = FALSE;
			lpOutBuffer[0] = 0x00; // 8λ
			for (int i = 0; i <= 7; i++)
			{
				if (NOZZLE[i] == TRUE)
					lpOutBuffer[0] = lpOutBuffer[0] | 0x80;         // lpOutBufferֵ��������һ��,������һ��if�ж��Ƿ�������6��
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
	bWriteStat = WriteFile(hCom, lpOutBuffer,                  //bwritestatΪ0ʱ��ʾд��δ��ɣ�Ϊ1��ʾд����ɣ�����д��
		dwBytesWrite, &dwBytesWrite, &m_osWrite);
	if (!bWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			WaitForSingleObject(m_osWrite.hEvent, 1000);
		}
	}
	
	

	
		//2.����ָ��    
		 Sleep(50);
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()), WM_DISPLAY, NULL, NULL);
	}
	
}
// CWorkThread ��Ϣ�������

