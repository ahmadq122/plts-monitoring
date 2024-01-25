#pragma once
#include "Arduino.h"
#include "../MachineEnum/MachineEnum.h"

// #define MOVINGAVERAGE 20

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
    // bool getMovingAverageEnabled() const;

    // Member function to set data
    void setSpSensorValue(uint8_t point, int newValue);
    void setSpSensorValueAdc(uint8_t point, int newValue);
    void setSensorValue(int newValue);
    void setSensorValueAdc(int newValue);
    // void setMovingAverageEnabled(bool enable);

private:
    // Moving Average Filter
    // void _filterMovingAverageAdc();
    // uint32_t m_sensorValueAdcTotal;
    // bool m_movingAverageEnabled;

    // Data member of the class
    int m_sensorValue;
    int m_sensorValueAdc;
    // int m_sensorValueAdcFiltered;
    int m_spSensorValue[CalPoint_Total];
    int m_spSensorValueAdc[CalPoint_Total];

    // scope
    double m_m[CalPoint_Total - 1];
    double m_b[CalPoint_Total - 1];

    // Declare a vector of int
    // int m_sensorValueArray[MOVINGAVERAGE];
};