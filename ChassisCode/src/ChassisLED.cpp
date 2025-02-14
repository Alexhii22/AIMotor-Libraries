//源文件主要写实现头文件中已经声明的那些函数的具体代码。需要注意的是，开头必须#include一下实现的头文件，以及要用到的头文件。那么当你需要用到自己写的头文件中的类时，只需要#include进来就行了。

#include "ChassisLED.h"

Ledwrite::Ledwrite() {
  Serial.println("Ledwrite Object Created.");
  pinMode(userLedPin1, OUTPUT);
}

void Ledwrite::setLedPin(int userLedPin1) {
  pinMode(userLedPin1, OUTPUT);      //设置为OUTPUT
}

void Ledwrite::on() {                //对ledPin输出高电平
  digitalWrite(userLedPin1, HIGH);
}

void Ledwrite::off() {               //对ledPin输出低电平
  digitalWrite(userLedPin1, LOW);
}


/// @brief 
Ledread::Ledread() {
  Serial.println("Ledread Object Created.");
  pinMode(userLedPin2, INPUT);
}

void Ledread::setLedPin(int userLedPin2) {
  pinMode(userLedPin2, INPUT);      //设置
}
int Ledread::read() {
  int value = digitalRead(userLedPin2);
  Serial.print("LED状态：");
  Serial.println(value);
  return value;  // 返回 value
}
