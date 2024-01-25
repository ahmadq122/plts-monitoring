#include "Sensor.h"

#define MOVINGAVERAGE 20

Sensor::Sensor(void)
{
    Serial.println("Sensor constructor!");
    m_sensorValue = 0;
    m_sensorValueAdc = 0;
    m_sensorValueAdcFiltered = 0;
    m_sensorValueAdcTotal = 0;

    for (uint8_t i = 0; i < CalPoint_Total; i++)
    {
        m_spSensorValue[i] = 0;
        m_spSensorValueAdc[i] = 0;
    }
    for (uint8_t i = 0; i < CalPoint_Total - 1; i++)
    {
        m_m[i] = 0;
        m_b[i] = 0;
    }

    m_movingAverageEnabled = true;
    // Allocate memory for the dynamic array
    m_sensorValueArray = new int[MOVINGAVERAGE];
    for (uint8_t i = 0; i < MOVINGAVERAGE; i++)
    {
        m_sensorValueArray[i] = 0;
    }
}

Sensor::~Sensor()
{
    // Deallocate memory when done
    delete[] m_sensorValueArray;
    Serial.println("Sensor destructor!");
}

void Sensor::routineTask()
{
    _filterMovingAverageAdc();

    int sensorAdc = m_sensorValueAdcFiltered;
    double sensorValue = 0;

    if (sensorAdc < m_spSensorValueAdc[CalPoint_1])
    {
        sensorValue = 0;
    }
    else if (sensorAdc < m_spSensorValueAdc[CalPoint_2])
    {
        if (m_m[CalPoint_2 - 1] != 0)
            sensorValue = ((double)sensorAdc - m_b[CalPoint_2 - 1]) / m_m[CalPoint_2 - 1];
    }
    else
    {
        if (m_m[CalPoint_3 - 1] != 0)
            sensorValue = ((double)sensorAdc - m_b[CalPoint_3 - 1]) / m_m[CalPoint_3 - 1];
    }
    m_sensorValue = round(sensorValue);
    // Serial.print("sensorValue ");
    // Serial.print(sensorValue);
    // Serial.print(" m_sensorValue ");
    // Serial.println(m_sensorValue);
}

void Sensor::initScope()
{
    double temp = (m_spSensorValue[CalPoint_2] - m_spSensorValue[CalPoint_1]);
    if (temp != 0)
    {
        m_m[CalPoint_1] = (static_cast<double>(m_spSensorValueAdc[CalPoint_2] - m_spSensorValueAdc[CalPoint_1])) / temp;
        m_b[CalPoint_1] = (static_cast<double>(m_spSensorValueAdc[CalPoint_1])) - (m_m[CalPoint_1] * m_spSensorValue[CalPoint_1]);
    }

    temp = (m_spSensorValue[CalPoint_3] - m_spSensorValue[CalPoint_2]);
    if (temp != 0)
    {
        m_m[CalPoint_2] = (static_cast<double>(m_spSensorValueAdc[CalPoint_3] - m_spSensorValueAdc[CalPoint_2])) / temp;
        m_b[CalPoint_2] = (static_cast<double>(m_spSensorValueAdc[CalPoint_2])) - (m_m[CalPoint_2] * m_spSensorValue[CalPoint_2]);
    }
}

int Sensor::getSpSensorValue(uint8_t point) const
{
    point = constrain(point, CalPoint_1, CalPoint_3);
    return m_spSensorValue[point];
}

int Sensor::getSpSensorValueAdc(uint8_t point) const
{
    point = constrain(point, CalPoint_1, CalPoint_3);
    return m_spSensorValueAdc[point];
}

int Sensor::getSensorValue() const
{
    return m_sensorValue;
}

int Sensor::getSensorValueAdc() const
{
    return m_sensorValueAdc;
}

bool Sensor::getMovingAverageEnabled() const
{
    return m_movingAverageEnabled;
}

void Sensor::setSpSensorValue(uint8_t point, int newValue)
{
    point = constrain(point, CalPoint_1, CalPoint_3);
    if (m_spSensorValue[point] == newValue)
        return;
    m_spSensorValue[point] = newValue;
}

void Sensor::setSpSensorValueAdc(uint8_t point, int newValue)
{
    point = constrain(point, CalPoint_1, CalPoint_3);
    if (m_spSensorValueAdc[point] == newValue)
        return;
    m_spSensorValueAdc[point] = newValue;
}

void Sensor::setSensorValue(int newValue)
{
    if (m_sensorValue == newValue)
        return;
    m_sensorValue = newValue;
}

void Sensor::setSensorValueAdc(int newValue)
{
    if (m_sensorValueAdc == newValue)
        return;
    m_sensorValueAdc = newValue;
}

void Sensor::setMovingAverageEnabled(bool enable)
{
    if (m_movingAverageEnabled == enable)
        return;
    m_movingAverageEnabled = enable;
}

void Sensor::_filterMovingAverageAdc()
{
    const int adc = m_sensorValueAdc;

    if (m_movingAverageEnabled)
    {
        uint32_t adcTotal = 0;

        // Push back the sensor value
        for (int8_t i = sizeof(m_sensorValueArray) - 1; i >= 0; i--)
        {
            if (i > 0)
            {
                m_sensorValueArray[i] = m_sensorValueArray[i - 1];
                adcTotal += m_sensorValueArray[i];
            }
            else
                break;
        }
        m_sensorValueArray[0] = adc;
        adcTotal += m_sensorValueArray[0];
        m_sensorValueAdcFiltered = round((double)adcTotal / (double)sizeof(m_sensorValueArray));
    }
    else
    {
        m_sensorValueAdcFiltered = adc;
    }

    Serial.print("Adc: ");
    Serial.print(m_sensorValueAdc);
    Serial.print(" Fil: ");
    Serial.println(m_sensorValueAdcFiltered);
}
