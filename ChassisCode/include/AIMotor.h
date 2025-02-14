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
        void steerdistance(int distance);
        void steerspeed(int speed);
        void movelocation();
    private:    
        uint8_t _slaveID;
        HardwareSerial* _serialPort;
        ModbusMaster _node;
        long buma(int number);
};


#endif