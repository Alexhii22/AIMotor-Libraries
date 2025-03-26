#include "ChassisLED.h"
#include "Arduino.h"
#include "AIMotor.h"
#include "sbus.h"

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial3);//Serial3 copy sbus data
bfs::SbusData data;

//初始化
Motortorque FL1(1, Serial1);
Motorlocation FL2(2, Serial1);
Motortorque FR1(3, Serial1);
Motorlocation FR2(4, Serial1);
Motortorque RL1(5, Serial1);
Motorlocation RL2(6, Serial1);
Motortorque RR1(7, Serial1);
Motorlocation RR2(8, Serial1);

Ledwrite myLed1;          //定义Led类对象myLed1
Ledread myLed2;          //定义Led类对象myLed2

//全局变量
int mappedValues[16]; // 映射数组
bool isFirstTime = true;//单次指令符
float r;//转弯半径
float front_inner_angle;
float front_outer_angle;
float rear_inner_angle;
float rear_outer_angle;
float front_left_angle;
float front_right_angle;
float rear_left_angle;
float rear_right_angle;
float speed_value;
int8_t torque_limit;
int16_t change_value;
int16_t last_value = 0;
int16_t now_value;
int16_t turning_value;

//程序函数

//打印通道值函数
void mapSBUSValues() {
  for (int i = 0; i < 16; ++i) {
    mappedValues[i] = map(data.ch[i], 282, 1722, -100, 100);
  }
}
// 打印SBUS状态
void printSBUSStatus() {
  if (sbus_rx.Read()) {
    data = sbus_rx.data();
    mapSBUSValues();
    // Serial.print("Ch1  ");
    // Serial.println(mappedValues[0]); // 右摇杆左右
    // Serial.print("Ch3  ");
    // Serial.println(mappedValues[2]); // 左摇杆前后
    // Serial.print("Ch5  ");
    // Serial.println(mappedValues[4]); // 左E键摇杆控制往下遥控器，往上上位机
    // Serial.print("Ch6  ");
    // Serial.println(mappedValues[5]); // 右F键摇杆控制往下斜行，中间两轮阿克曼，往上四轮阿克曼
    // Serial.print("Ch7  ");
    // Serial.println(mappedValues[6]); // A键
    // Serial.print("Ch8  ");
    // Serial.println(mappedValues[7]); // B键
    Serial.print("Ch8  ");
    Serial.println(mappedValues[11]); // B键
    
  }
}

void allonpower(){
  //全体电机使能
  FL1.onpower();
  delay(2);
  FL2.onpower();
  delay(2);
  FR1.onpower();
  delay(2);
  FR2.onpower();
  delay(2);
  RL1.onpower();
  delay(2);
  RL2.onpower();
  delay(2);
  RR1.onpower();
  delay(2);
  RR2.onpower();
  delay(2);
}

void alloffpower(){
  //全体电机失能
  FL1.offpower();
  delay(2);
  FL2.offpower();
  delay(2);
  FR1.offpower();
  delay(2);
  FR2.offpower();
  delay(2);
  RL1.offpower();
  delay(2);
  RL2.offpower();
  delay(2);
  RR1.offpower();
  delay(2);
  RR2.offpower();
  delay(2);
}

void FourWS(){
  //右摇杆mappedValues[0]范围-100-100
  int a = mappedValues[0];
  turning_value = -a * 20;
  if(turning_value == 0){
  r = 0;
  }
  else{
  float rad = abs(1000 / tan(a * PI / 180.0));
  if(rad < 301){
  r = 300;
  }
  else{
  r = rad;
  }
  }
  now_value = a;
  change_value = now_value - last_value;
  delay(1);
  last_value = now_value;
  if(a > 0){//右转
    front_inner_angle = -130*atan(1000 / (r - 300)) * 180.0 / PI;//期望内轮转角
    front_outer_angle = -130*atan(1000 / (r + 300)) * 180.0 / PI;//期望外轮转角
    rear_inner_angle = front_inner_angle;
    rear_outer_angle = front_outer_angle;
    front_left_angle = front_outer_angle/2;
    front_right_angle = front_inner_angle/2;
    rear_left_angle = -rear_outer_angle/2;
    rear_right_angle = -rear_inner_angle/2;
  }
  else if(a < 0){//左转
    front_inner_angle = 130*atan(1000 / (r - 300)) * 180.0 / PI;//期望内轮转角
    front_outer_angle = 130*atan(1000 / (r + 300)) * 180.0 / PI;//期望外轮转角
    rear_inner_angle = -front_inner_angle;
    rear_outer_angle = -front_outer_angle;
    front_left_angle = front_inner_angle/2;
    front_right_angle = front_outer_angle/2;
    rear_left_angle = rear_inner_angle/2;
    rear_right_angle = rear_outer_angle/2;
  }
  else{//直行
    front_inner_angle = 0;
    front_outer_angle = 0;
    rear_inner_angle = 0;
    rear_outer_angle = 0;
    // 明确控制轮组回到零位
  FL2.steerdistance(0);
  FR2.steerdistance(0);
  RL2.steerdistance(0);
  RR2.steerdistance(0);
  delay(5);  // 确保控制指令有足够时间执行
  FL2.movelocation();
  FR2.movelocation();
  RL2.movelocation();
  RR2.movelocation();
  }
  ///计算各轮需求转角
  int position = FL2.readposition();
  delay(1);
  // Serial.print("当前电机转向位置");
  // Serial.println(position);
  now_value = turning_value;
  if(abs(now_value) - abs(last_value) > 100){
  //转向执行
  FL2.steerdistance(front_left_angle);
  FR2.steerdistance(front_right_angle);
  RL2.steerdistance(rear_left_angle);
  RR2.steerdistance(rear_right_angle);
  delay(5);
  FL2.movelocation();
  FR2.movelocation();
  RL2.movelocation();
  RR2.movelocation();
  last_value = now_value;
  delay(10);
  }
  FL1.settorque(mappedValues[2]*2);
  FR1.settorque(mappedValues[2]*2);
  RL1.settorque(mappedValues[2]*2);
  RR1.settorque(mappedValues[2]*2);
}

void AckmanWS(){
//右摇杆mappedValues[0]范围-100-100
int a = mappedValues[0];
turning_value = -a * 20;
if(turning_value == 0){
r = 0;
}
else{
float rad = abs(1000 / tan(a * PI / 180.0));
if(rad < 301){
r = 300;
}
else{
r = rad;
}
}
now_value = a;
change_value = now_value - last_value;
delay(1);
last_value = now_value;
if(a > 0){//右转
  front_inner_angle = -130*atan(1000 / (r - 300)) * 180.0 / PI;//期望内轮转角
  front_outer_angle = -130*atan(1000 / (r + 300)) * 180.0 / PI;//期望外轮转角
  front_left_angle = front_outer_angle/2;
  front_right_angle = front_inner_angle/2;
}
else if(a < 0){//左转
  front_inner_angle = 130*atan(1000 / (r - 300)) * 180.0 / PI;//期望内轮转角
  front_outer_angle = 130*atan(1000 / (r + 300)) * 180.0 / PI;//期望外轮转角
  front_left_angle = front_inner_angle/2;
  front_right_angle = front_outer_angle/2;
}
else{//直行
  // 明确控制轮组回到零位
FL2.steerdistance(0);
FR2.steerdistance(0);
RL2.steerdistance(0);
RR2.steerdistance(0);
delay(5);  // 确保控制指令有足够时间执行
FL2.movelocation();
FR2.movelocation();
RL2.movelocation();
RR2.movelocation();
}
///计算各轮需求转角
int position = FL2.readposition();
delay(1);
// Serial.print("当前电机转向位置");
// Serial.println(position);
now_value = turning_value;
if(abs(now_value) - abs(last_value) > 100){
//转向执行
FL2.steerdistance(front_left_angle);
FR2.steerdistance(front_right_angle);
delay(5);
FL2.movelocation();
FR2.movelocation();
last_value = now_value;
delay(10);
}
torque_limit = mappedValues[11]*8;

FL1.settorque(torque_limit + torque_limit);
FR1.settorque(torque_limit + torque_limit);
RL1.settorque(torque_limit + torque_limit);
RR1.settorque(torque_limit + torque_limit);

}

void CrabWS(){
int a = -mappedValues[0]*105;
FL2.steerdistance(a);
FR2.steerdistance(a);
RL2.steerdistance(a);
RR2.steerdistance(a);
delay(5);
FL2.movelocation();
FR2.movelocation();
RL2.movelocation();
RR2.movelocation();
delay(5);
FL1.settorque(mappedValues[2]*2);
FR1.settorque(mappedValues[2]*2);
RL1.settorque(mappedValues[2]*2);
RR1.settorque(mappedValues[2]*2);
}

void CircleWS(){
  int a = 7500;
  FL2.steerdistance(-a);
  FR2.steerdistance(a);
  RL2.steerdistance(a);
  RR2.steerdistance(-a);
  delay(5);
  FL2.movelocation();
  FR2.movelocation();
  RL2.movelocation();
  RR2.movelocation();
  delay(5);
  FL1.settorque(mappedValues[2]*3);
  FR1.settorque(-mappedValues[2]*3);
  RL1.settorque(-mappedValues[2]*3);
  RR1.settorque(mappedValues[2]*3);
}

void PCws(){

}

void ZeroWS(){

}

void setup() {
  while (!Serial) {}
  sbus_rx.Begin();//串口初始化完成，开始接收数据
  Serial.begin(9600);
  Serial1.begin(57600, SERIAL_8N1);
  myLed1.setLedPin(40); //设置LED灯的引脚为数字口40
  myLed2.setLedPin(A15); //设置LED灯的引脚为数字口41
  speed_value = mappedValues[2] * 4;
}

void loop() {
  printSBUSStatus();
  if(mappedValues[6]>0){
  //A键点亮底盘进入运动模式
  if(isFirstTime){
    allonpower();
    isFirstTime = false;
  }
  switch(mappedValues[4]){
  //左摇杆往下为遥控器，往上为上位机控制
    case -100:
      //原地转向模式
      Serial.println("原地模式");
      CircleWS();
    break;

    case 100:
      //上位机指令
      Serial.println("上位机模式");
      PCws();
    break;

    case 0:
      //遥控器控制
      switch(mappedValues[5]){
  //右摇杆控制转向模式
      case -100:
      Serial.println("斜行模式");
      CrabWS();
      break;

      case 0:
      Serial.println("阿克曼模式");
      AckmanWS();
      break;
      
      case 100:
      Serial.println("四轮转向模式");
      FourWS();
      break;
      }
    break;
  }

  }
  else{
  //A键熄灭底盘进入休眠模式
    if(mappedValues[7]>0){
      Serial.println("四轮校准模式");
      ZeroWS();
    }
    else{
      if(isFirstTime == false){
      alloffpower();
      isFirstTime = true;
      }
      Serial.println("底盘已休眠");

    }
  }
}
