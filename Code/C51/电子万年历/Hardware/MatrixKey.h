#ifndef _MATRIXKEY_H__
#define _MATRIXKEY_H__

unsigned char MatrixKey();  // 矩阵键盘获取键值，返回键值
unsigned char Key_GetState();// 
void MatrixKey_Loop(void); //放入定时器
#endif
