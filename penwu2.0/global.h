//#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include "urg_sensor.h"


urg_t urg;
long time_stamp;
int pixel_num;

long *data;     //���ݻ�ȡFIFO
char *data_compute;
float *rho;
char *DC_FIFO;

int n_delay;                   //�ӳٵ�֡��
int count_compute = 0;           //���ڼ�����������Ƿ����Ŀ��
int count_send = 0;        //�ӳٷ���֡��
float distance = 1.65;  //����������������뵥λΪ��
float t_delay = 0.16;

BOOL RUN = FALSE;//�߳���ͣ
float V;           //�ٶ�
float D;		   //����
BOOL AUTO=TRUE;//��ѡ��ť״̬;�Զ��ֶ�
BOOL L;
BOOL R;
BOOL NOZZLE[8] ;//��ʼ��8���Ƶ�״̬

//����ͨ�Ų���
HANDLE hCom; //ȫ�ֱ��������ھ�� 
COMMTIMEOUTS TimeOuts;
DCB dcb;
byte lpOutBuffer[1] = { 0x00 };
OVERLAPPED m_osWrite;
COMSTAT ComStat;
DWORD dwErrorFlags;
BOOL bWriteStat;



#endif