#include "AIMotor.h"
#include "ModbusMaster.h"
Motorlocation::Motorlocation(uint8_t slaveID, HardwareSerial& serialPort) {

    // 实现构造函数
    _slaveID = slaveID;
    _serialPort = &serialPort;
    // 初始化 ModbusMaster 节点
    _node.begin(slaveID, serialPort);
    ModbusMaster _node;
}//该函数在初始化前调用，用于设置从机ID和串口

void Motorlocation::onpower() {
    _node.setTransmitBuffer(0, 1);
    delay(5);
    _node.writeMultipleRegisters(0x0303, 1);
    delay(5); 
}//该函数用于通过Modbus协议向电机发送使能指令

void Motorlocation::offpower() {
    _node.setTransmitBuffer(0, 0);
    delay(5);
    _node.writeMultipleRegisters(0x0303, 1); 
    delay(5); 
}//该函数用于通过Modbus协议向电机发送使能指令

long Motorlocation::buma(int number){
// 将负数转换为补码表示法
    unsigned int hexValue;
    if (number < 0) {
    hexValue = 0xFFFF + number + 1;
    } else {
    hexValue = number;
    }
    // 将十六进制转换为十进制
    return strtol(String(hexValue, HEX).c_str(), NULL, 16);
}//调取buma（number）
void Motorlocation::steerdistance(int abslocation) {
  if(abslocation>=0){
    word1=0;
   }
   else{
    word1=65535;
   }
      Serial.print("当前电机输入位移");
      Serial.println(abslocation);
      _node.setTransmitBuffer(0,buma(abslocation));
      delay(1);
      _node.setTransmitBuffer(1,word1);
      delay(1);
      _node.writeMultipleRegisters(0x110C, 2); 
      delay(5);
}//该函数用于通过Modbus协议向电机发送转向距离指令

void Motorlocation::steerspeed(int speed, int acceleration){
    _node.setTransmitBuffer(0, speed);
    delay(5);
    _node.writeSingleRegister(0x110E, 1);
    delay(5);
    _node.setTransmitBuffer(0, acceleration);
    delay(5);
    _node.writeSingleRegister(0x110F, 1);
    delay(5);
}

void Motorlocation::movelocation(){
    _node.setTransmitBuffer(0, 0);
    delay(10);
    _node.writeMultipleRegisters(0x0305, 1);
    delay(10);
    _node.setTransmitBuffer(0, 1);
    delay(5);
    _node.writeMultipleRegisters(0x0305, 1);
    delay(5);
}

int Motorlocation::readposition(){
    _node.readHoldingRegisters(0x0B07, 2);
    delay(2);
    return _node.getResponseBuffer(0);
}
Motorspeed::Motorspeed(uint8_t slaveID, HardwareSerial& serialPort) {

    // 实现构造函数
    _slaveID = slaveID;
    _serialPort = &serialPort;
    // 初始化 ModbusMaster 节点
    _node.begin(slaveID, serialPort);
    ModbusMaster _node;
}//该函数在初始化前调用，用于设置从机ID和串口

void Motorspeed::onpower() {
    _node.setTransmitBuffer(0, 1);
    delay(5);
    _node.writeMultipleRegisters(0x0303, 1);
    delay(5); 
}//该函数用于通过Modbus协议向电机发送使能指令

void Motorspeed::offpower() {
    _node.setTransmitBuffer(0, 0);
    delay(5);
    _node.writeMultipleRegisters(0x0303, 1);
    delay(5); 
}//该函数用于通过Modbus协议向电机发送使能指令

void Motorspeed::speedvalue(int16_t value, uint16_t acceleration, uint16_t deceleration){
    _node.setTransmitBuffer(0, buma(value));
    delay(5);
    _node.writeMultipleRegisters(0x0603,1);
    delay(5);
    _node.setTransmitBuffer(0, acceleration);
    delay(5);
    _node.writeMultipleRegisters(0x0605,1);
    delay(5);
    _node.setTransmitBuffer(0, deceleration);
    delay(5);
    _node.writeMultipleRegisters(0x0606,1);
    delay(5);
}

Motortorque::Motortorque(uint8_t slaveID, HardwareSerial& serialPort){
    // 实现构造函数
    _slaveID = slaveID;
    _serialPort = &serialPort;
    // 初始化 ModbusMaster 节点
    _node.begin(slaveID, serialPort);
    ModbusMaster _node;
}
void Motortorque::onpower(){
    _node.setTransmitBuffer(0, 1);
    delay(5);
    _node.writeMultipleRegisters(0x0303, 1);
    delay(5); 
}

void Motortorque::offpower(){
    _node.setTransmitBuffer(0, 0);
    delay(5);
    _node.writeMultipleRegisters(0x0303, 1);
    delay(5); 
}

long Motortorque::buma(int number){
    // 将负数转换为补码表示法
        unsigned int hexValue;
        if (number < 0) {
        hexValue = 0xFFFF + number + 1;
        } else {
        hexValue = number;
        }
        // 将十六进制转换为十进制
        return strtol(String(hexValue, HEX).c_str(), NULL, 16);
    }//调取buma（number）

void Motortorque::settorque(int16_t torque){
    _node.setTransmitBuffer(0, buma(torque));
    delay(5);
    _node.writeMultipleRegisters(0x0703, 1);
    delay(5); 
}

void  Motortorque::speedlimit(int16_t max){
    _node.setTransmitBuffer(0, buma(max));
    delay(5);
    _node.writeMultipleRegisters(0x0719, 1);
    delay(5); 
}