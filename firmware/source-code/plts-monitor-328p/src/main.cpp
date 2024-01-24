/*
 */
#include "MachineEnum/MachineEnum.h"
#include "MachineData/MachineData.h"
#include "Sensor/Sensor.h"

MachineData *mData;
Sensor current;
Sensor voltage;

void setup()
{
  Serial.begin(9600);
  Serial.println("Init");

  mData = new MachineData;
  if (mData == NULL)
  {
    // Handle allocation failure
    Serial.println("Handle allocation failure");
    while (1)
    {
      // Loop indefinitely or take appropriate action
    }
  }
  else
  {
    mData->setMachineData(MachineState_Setup);
  }

  // Analog Input Init For Sensors

  // Current Sensor Init
  current.setSpSensorValue(CalPoint_1, mData->getSpCurrentSensor(CalPoint_1));
  current.setSpSensorValue(CalPoint_2, mData->getSpCurrentSensor(CalPoint_2));
  current.setSpSensorValue(CalPoint_3, mData->getSpCurrentSensor(CalPoint_3));
  current.setSpSensorValueAdc(CalPoint_1, mData->getSpCurrentSensorAdc(CalPoint_1));
  current.setSpSensorValueAdc(CalPoint_2, mData->getSpCurrentSensorAdc(CalPoint_2));
  current.setSpSensorValueAdc(CalPoint_3, mData->getSpCurrentSensorAdc(CalPoint_3));
  current.initScope();

  // Voltage Sensor Init
  voltage.setSpSensorValue(CalPoint_1, mData->getSpVoltageSensor(CalPoint_1));
  voltage.setSpSensorValue(CalPoint_2, mData->getSpVoltageSensor(CalPoint_2));
  voltage.setSpSensorValue(CalPoint_3, mData->getSpVoltageSensor(CalPoint_3));
  voltage.setSpSensorValueAdc(CalPoint_1, mData->getSpVoltageSensorAdc(CalPoint_1));
  voltage.setSpSensorValueAdc(CalPoint_2, mData->getSpVoltageSensorAdc(CalPoint_2));
  voltage.setSpSensorValueAdc(CalPoint_3, mData->getSpVoltageSensorAdc(CalPoint_3));
  voltage.initScope();
}

void loop()
{
  mData->setMachineData(MachineState_Loop);
  while (1)
  {
    current.setSensorValueAdc(analogRead(A0));
    current.routineTask();
    voltage.setSensorValueAdc(analogRead(A1));
    voltage.routineTask();
    Serial.print("Current: ");
    Serial.print(double(current.getSensorValue() / 100.0), 2);
    Serial.print("Amps Voltage: ");
    Serial.print(double(voltage.getSensorValue() / 100.0), 2);
    Serial.print("Volts");
    Serial.println();
    delay(1000);
  }
}