#include "ChassisLED.h"
#include "Arduino.h"
#include "AIMotor.h"


Motorlocation FL1(1, Serial1);
Ledwrite myLed1;          //定义Led类对象myLed1
Ledread myLed2;          //定义Led类对象myLed2

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  myLed1.setLedPin(40); //设置LED灯的引脚为数字口40
  myLed2.setLedPin(A15); //设置LED灯的引脚为数字口41

}

void loop() {
  int ledStatus = myLed2.read();  // 调用 read 函数并获取返回值
  Serial.print("LED 状态返回值：");
  Serial.println(ledStatus);  // 打印返回的 LED 状态

  FL1.onpower();
  delay(1000);
  FL1.offpower();
  delay(1000);
}
