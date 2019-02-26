/*!
*/
#include "stdafx.h"
#include "calculate_xy.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "urg_sensor.h"
#include "open_urg_sensor.h"
#include "urg_utils.h"

extern urg_t urg;
extern long time_stamp;
extern int pixel_num;
extern long *data;
extern char *data_compute;
extern float *rho;
extern float D;
extern char *DC_FIFO;
extern int n_delay;
extern int count_send;
extern int count_compute;

// ��������ռ�
void urg_init()
{

	pixel_num = urg_max_data_size(&urg);                     //���⴫����ɨ���1081������1080����Ŀ
	data = (long*)malloc(pixel_num*sizeof(long));     //��ȡ�ļ������ݴ洢��data_FIFO��
	data_compute = (char*)malloc(NOZZLE_NUM * 2 * sizeof(char));  //�������������Ŀ������
	rho = (float*)malloc(pixel_num*sizeof(float));                 //����data_FIFO��ֵ
	//DC_FIFO = (char*)malloc(NOZZLE_NUM * 2 * sizeof(char)*(1 + n_delay));  //�ӳ�PWM��������������ռ�
	DC_FIFO = (char*)malloc(NOZZLE_NUM * 2 * sizeof(char));
}



int capture_start(int argc, char *argv[])              //capture_start��ʲô����������
{
	int n;
	if (open_urg_sensor(&urg, argc, argv) < 0)
	{
		printf("û�д򿪼��⴫������");
		return 1;
	}
	urg_init();
	memset(DC_FIFO, 0, NOZZLE_NUM * 2 * (1 + n_delay));
	// ��ʼ�ɼ���0����ͣ�ɼ�
	urg_start_measurement(&urg, URG_DISTANCE, 0, 1);
#if defined(URG_MSC)
	getchar();
#endif
	return 0;
}

void compute_FIFO()         
{
	memset(data_compute, 0, NOZZLE_NUM * 2);  //ѭ������ʱ�����nΪ����   �������㣬�����������������Ҫʹ��ѭ������memset����ֻ����int��char������ʹ��
//	
	for (int n = 0;n < NOZZLE_NUM;n++)
	{
		for (int i = 130+n*100; i < pixel_num / 2 - 10; i++)
		{
			rho[i] = data[i] / 1000.0;
			if (i < 130+(n+1)*100 && i >130+n*100)
			{
				if (rho[i] < D)
					count_compute++;}
		}
		if (count_compute > 30)
		{data_compute[n] = 1;}
		else
		{ data_compute[n] = 0; }
		count_compute = 0;}	
	for (int n = 0;n < NOZZLE_NUM;n++)
	{
		for (int i =550 + n * 100; i < pixel_num-130; i++)
		{
			rho[i] = data[i] / 1000.0;
			if (i < 550 + (n + 1) * 100 && i >550 + n * 100)
			{
				if (rho[i] < D)
					count_compute++;
			}
		}
		if (count_compute > 30)
		{
			data_compute[n+4] = 1;
		}
		else
		{
			data_compute[n+4] = 0;
		}
		count_compute = 0;
	}
	for (int n = 0;n < NOZZLE_NUM * 2;n++)
	{
		//DC_FIFO[NOZZLE_NUM * 2 * n_delay+n] = data_compute[n];
		DC_FIFO[n] = data_compute[n];
	}
}

/*
void send_pwm()          //���д��ڷ���
{
	int _tmain(int argc, _TCHAR* argv[]);
}

*/

void data_process()
{ 
	count_send++;
	//memcpy(DC_FIFO,&DC_FIFO[NOZZLE_NUM * 2], NOZZLE_NUM * 2 * sizeof(char)* n_delay);
	compute_FIFO();                                                      // 1.�����ݣ���FIFO   
	if(count_send==n_delay)
	{ 
		count_send = 0;
        //		send_pwm();   
	}
	
	                                                             // 2.�жϰб����  void compute_FIFO
	                                                             // 3.ȷ����ʱʱ��   void update_pwm()
	                                                             // 4.����pwm������ͨ��  void send_pwm()
	
	}

