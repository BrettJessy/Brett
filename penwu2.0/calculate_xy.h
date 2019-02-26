#ifndef CAPTURE_XY_H
#define CAPTURE_XY_H


//#define CENTER  0.5 //树冠中心到激光扫描器之间距离
#define NOZZLE_NUM 4//喷嘴的个数

void compute_FIFO();  
void send_pwm();
void data_process();
extern int capture_start(int argc, char *argv[]);

#endif