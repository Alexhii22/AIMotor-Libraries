#ifndef _AIMOTOR_H_
#define _AIMOTOR_H_
#include <Arduino.h>
#include <ModbusMaster.h>
//以下控制模式均在电机使用上位机设置完模式后调用
class Motorlocation {
 public:
 Motorlocation(uint8_t slaveID, HardwareSerial& serialPort);
 void onpower();
 void offpower();
 void steerdistance(int abslocation);
 void steerspeed(int speed, int acceleration);
 void movelocation();
 int readposition();
 private: 
 uint8_t _slaveID;
 HardwareSerial* _serialPort;
 ModbusMaster _node;
 int16_t abslocation;
 uint16_t word1;//正负值补码可变值
 long buma(int number);
};

class Motortorque {
 public:
 Motortorque(uint8_t slaveID, HardwareSerial& serialPort);
 void onpower();
 void offpower();
 void settorque(int16_t torque);
 void speedlimit(int16_t max);
 void torquereference(int reference);
 void torqueeffective(int effective);
 void torqueinvalid(int invalid);
 
 private: 
 uint8_t _slaveID;
 HardwareSerial* _serialPort;
 ModbusMaster _node;
 int16_t torque;
 int16_t word1;//正负值补码可变值
 int reference;
 int effective;
 int invalid;
 long buma(int number);
};

class Motorspeed {
 public:
 Motorspeed(uint8_t slaveID, HardwareSerial& serialPort);
 void onpower();
 void offpower();
 void speedvalue(int16_t value, uint16_t acceleration, uint16_t deceleration);
 
 private: 
 uint8_t _slaveID;
 HardwareSerial* _serialPort;
 ModbusMaster _node;
 int16_t value;
 uint16_t acceleration;
 uint16_t deceleration;
 long buma(int number);
};
#endif