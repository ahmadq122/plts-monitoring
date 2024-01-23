#include "MachineData.h"
#include <EEPROM.h>

MachineData::MachineData(void)
{
    Serial.println("MachineData constructor!");
    m_machineState = 0;

    readEeprom();
}

MachineData::~MachineData()
{
    Serial.println("MachineData destructor!");
}

void MachineData::updateEeprom()
{
    m_data.checksum = calculateChecksum(m_data);
    EEPROM.put(0, m_data);
}

/// @brief return true if the data match checksum
bool MachineData::readEeprom()
{
    ee_data dataTemp;
    EEPROM.get(0, dataTemp);

    uint8_t checksum = dataTemp.checksum;
    if (checksum == calculateChecksum(dataTemp))
    {
        m_data = dataTemp;
        Serial.println("Data checksum is matched!");
        return true;
    }
    else
    {
        initEeprom();
        Serial.println("Data checksum doesn't match!");
        return false;
    }
}

void MachineData::initEeprom()
{
    m_data.spCurrentSensor[CalPoint_1] = 0;
    m_data.spCurrentSensor[CalPoint_2] = 500;  // 5.00 A
    m_data.spCurrentSensor[CalPoint_3] = 1000; // 10.00 A
    m_data.spCurrentSensorAdc[CalPoint_1] = 0; // 0~1023
    m_data.spCurrentSensorAdc[CalPoint_2] = 512;
    m_data.spCurrentSensorAdc[CalPoint_3] = 1023;
    m_data.spVoltageSensor[CalPoint_1] = 0;
    m_data.spVoltageSensor[CalPoint_2] = 1200;
    m_data.spVoltageSensor[CalPoint_3] = 2400;
    m_data.spVoltageSensorAdc[CalPoint_1] = 0; // 0~1023
    m_data.spVoltageSensorAdc[CalPoint_2] = 512;
    m_data.spVoltageSensorAdc[CalPoint_3] = 1023;
    updateEeprom();
}

uint8_t MachineData::calculateChecksum(const ee_data &data)
{
    const unsigned char *byteData = reinterpret_cast<const unsigned char *>(&data);
    uint8_t checksum = 0;

    for (size_t i = 0; i < sizeof(data) - sizeof(checksum); ++i)
    {
        checksum += byteData[i];
    }

    return checksum;
}

uint8_t MachineData::getMachineState() const { return m_machineState; }

int MachineData::getSpCurrentSensor(uint8_t point)
{
    point = constrain(point, 0, CalPoint_Total);
    return m_data.spCurrentSensor[point];
}

int MachineData::getSpCurrentSensorAdc(uint8_t point)
{
    point = constrain(point, 0, CalPoint_Total);
    return m_data.spCurrentSensorAdc[point];
}

int MachineData::getSpVoltageSensor(uint8_t point)
{
    point = constrain(point, 0, CalPoint_Total);
    return m_data.spVoltageSensor[point];
}

int MachineData::getSpVoltageSensorAdc(uint8_t point)
{
    point = constrain(point, 0, CalPoint_Total);
    return m_data.spVoltageSensorAdc[point];
}

void MachineData::setMachineData(uint8_t newValue)
{
    if (m_machineState == newValue)
        return;
    m_machineState = newValue;
}

void MachineData::setSpCurrentSensor(uint8_t point, int newValue)
{
    point = constrain(point, 0, CalPoint_Total);
    if (m_data.spCurrentSensor[point] == newValue)
        return;
    m_data.spCurrentSensor[point] = newValue;
}

void MachineData::setSpCurrentSensorAdc(uint8_t point, int newValue)
{
    point = constrain(point, 0, CalPoint_Total);
    if (m_data.spCurrentSensorAdc[point] == newValue)
        return;
    m_data.spCurrentSensorAdc[point] = newValue;
}

void MachineData::setSpVoltageSensor(uint8_t point, int newValue)
{
    point = constrain(point, 0, CalPoint_Total);
    if (m_data.spVoltageSensor[point] == newValue)
        return;
    m_data.spVoltageSensor[point] = newValue;
}

void MachineData::setSpVoltageSensorAdc(uint8_t point, int newValue)
{
    point = constrain(point, 0, CalPoint_Total);
    if (m_data.spVoltageSensorAdc[point] == newValue)
        return;
    m_data.spVoltageSensorAdc[point] = newValue;
}
