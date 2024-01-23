#pragma once
#include "Arduino.h"
#include "../MachineEnum/MachineEnum.h"

typedef struct EEPROM_Data
{
    int spCurrentSensor[CalPoint_Total];
    int spCurrentSensorAdc[CalPoint_Total];
    int spVoltageSensor[CalPoint_Total];
    int spVoltageSensorAdc[CalPoint_Total];
    uint8_t checksum;
} ee_data;

class MachineData
{
public:
    // Constructor
    MachineData(void);
    ~MachineData();

    // Member function
    void updateEeprom();
    bool readEeprom();
    void initEeprom();
    uint8_t calculateChecksum(const ee_data &data);

    // Member function to get data
    uint8_t getMachineState() const;
    int getSpCurrentSensor(uint8_t point);
    int getSpCurrentSensorAdc(uint8_t point);
    int getSpVoltageSensor(uint8_t point);
    int getSpVoltageSensorAdc(uint8_t point);

    // Member function to set data
    void setMachineData(uint8_t newValue);
    void setSpCurrentSensor(uint8_t point, int newValue);
    void setSpCurrentSensorAdc(uint8_t point, int newValue);
    void setSpVoltageSensor(uint8_t point, int newValue);
    void setSpVoltageSensorAdc(uint8_t point, int newValue);

private:
    ee_data m_data;

    uint8_t m_machineState; // Data member of the class
};