#pragma once
#include "Arduino.h"
#include "../MachineEnum/MachineEnum.h"

class Sensor
{
public:
    // Constructor
    Sensor(void);
    ~Sensor();

    // Member function
    void routineTask();
    void initScope();

    // Member function to get data
    int getSpSensorValue(uint8_t point) const;
    int getSpSensorValueAdc(uint8_t point) const;
    int getSensorValue() const;
    int getSensorValueAdc() const;

    // Member function to set data
    void setSpSensorValue(uint8_t point, int newValue);
    void setSpSensorValueAdc(uint8_t point, int newValue);
    void setSensorValue(int newValue);
    void setSensorValueAdc(int newValue);

private:
    // Data member of the class
    int m_sensorValue;
    int m_sensorValueAdc;
    int m_spSensorValue[CalPoint_Total];
    int m_spSensorValueAdc[CalPoint_Total];

    // scope
    double m_m[CalPoint_Total - 1];
    double m_b[CalPoint_Total - 1];
};