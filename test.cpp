#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>

using namespace std;
/*
struct tm {
int tm_sec;   // 秒，正常範圍從 0 到 59，但允許至 61
int tm_min;   // 分，範圍從 0 到 59
int tm_hour;  // 小時，範圍從 0 到 23
int tm_mday;  // 一月中的第幾天，範圍從 1 到 31
int tm_mon;   // 月，範圍從 0 到 11
int tm_year;  // 自 1900 年起的年數
int tm_wday;  // 一周中的第幾天，範圍從 0 到 6，從星期日算起
int tm_yday;  // 一年中的第幾天，範圍從 0 到 365，從 1 月 1 日算起
int tm_isdst; // 夏令時
};
*/
int main( )
{
// 基於當前系統的當前日期/時間
time_t now = time(0);
   
   // 把 now 轉換為字符串形式
char* dt = ctime(&now);
 
   cout << "本地日期和時間：" << dt << endl;
 
   // 把 now 轉換為 tm 結構
tm *gmtm = gmtime(&now);
   dt = asctime(gmtm);
   cout << "UTC 日期和時間："<< dt << endl;
}


