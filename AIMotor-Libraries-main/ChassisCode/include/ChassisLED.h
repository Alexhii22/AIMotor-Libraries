//该文件写类的声明（包括类里面的成员和方法的声明）、函数原型、#define常数等，但一般来说不写出具体的实现、
//对于变量最好只声明而不定义，除非是全局变量。

#ifndef _CHASSISLED_H_
#define _CHASSISLED_H_
 
#include <Arduino.h>
 
class Ledwrite {
  public:
    Ledwrite();                    //析构函数
    Ledwrite(int userLedPin1);      //重载析构函数（带参数）
 
    void on();                //打开LED
    void off();               //关闭LED
    void write();
    void setLedPin(int userLedPin1);
  private:
    int userLedPin1 = 40;//初始化引脚编号为3（要与硬件搭建一致）
};//注意这边要有分号结束

class Ledread {
 public:
   Ledread();                    //析构函数
   Ledread(int userLedPin2);      //重载析构函数（带参数）
   int read();
   void setLedPin(int userLedPin2);
   
  
private:
  int userLedPin2 = A15; //初始化引脚编号为3（要与硬件搭建一致）
};//注意这边要有分号结束

#endif
