//#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include "urg_sensor.h"


urg_t urg;
long time_stamp;
int pixel_num;

long *data;     //数据获取FIFO
char *data_compute;
float *rho;
char *DC_FIFO;

int n_delay;                   //延迟的帧数
int count_compute = 0;           //用于计算各个区域是否存在目标
int count_send = 0;        //延迟发送帧数
float distance = 1.65;  //传感器距离喷嘴距离单位为米
float t_delay = 0.16;

BOOL RUN = FALSE;//线程启停
float V;           //速度
float D;		   //距离
BOOL AUTO=TRUE;//单选按钮状态;自动手动
BOOL L;
BOOL R;
BOOL NOZZLE[8] ;//初始化8个灯的状态

//串口通信部分
HANDLE hCom; //全局变量，串口句柄 
COMMTIMEOUTS TimeOuts;
DCB dcb;
byte lpOutBuffer[1] = { 0x00 };
OVERLAPPED m_osWrite;
COMSTAT ComStat;
DWORD dwErrorFlags;
BOOL bWriteStat;



#endif