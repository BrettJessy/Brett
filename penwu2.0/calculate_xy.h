#ifndef CAPTURE_XY_H
#define CAPTURE_XY_H


//#define CENTER  0.5 //�������ĵ�����ɨ����֮�����
#define NOZZLE_NUM 4//����ĸ���

void compute_FIFO();  
void send_pwm();
void data_process();
extern int capture_start(int argc, char *argv[]);

#endif