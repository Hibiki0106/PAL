#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>

using namespace std;
/*
struct tm {
int tm_sec;   // ��A���`�d��q 0 �� 59�A�����\�� 61
int tm_min;   // ���A�d��q 0 �� 59
int tm_hour;  // �p�ɡA�d��q 0 �� 23
int tm_mday;  // �@�뤤���ĴX�ѡA�d��q 1 �� 31
int tm_mon;   // ��A�d��q 0 �� 11
int tm_year;  // �� 1900 �~�_���~��
int tm_wday;  // �@�P�����ĴX�ѡA�d��q 0 �� 6�A�q�P�����_
int tm_yday;  // �@�~�����ĴX�ѡA�d��q 0 �� 365�A�q 1 �� 1 ���_
int tm_isdst; // �L�O��
};
*/
int main( )
{
// ����e�t�Ϊ���e���/�ɶ�
time_t now = time(0);
   
   // �� now �ഫ���r�Ŧ�Φ�
char* dt = ctime(&now);
 
   cout << "���a����M�ɶ��G" << dt << endl;
 
   // �� now �ഫ�� tm ���c
tm *gmtm = gmtime(&now);
   dt = asctime(gmtm);
   cout << "UTC ����M�ɶ��G"<< dt << endl;
}


