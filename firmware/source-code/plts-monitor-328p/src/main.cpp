/*
 */
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <DS3232RTC.h>
#include "MachineEnum/MachineEnum.h"
#include "MachineData/MachineData.h"
#include "Sensor/Sensor.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

MachineData *mData;
Sensor current;
Sensor voltage;
DS3232RTC rtc;

void sensorInit();
void rtcInit();
void sdInit();

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Machine Setup...");
  //
  Serial.begin(115200);
  Serial.println("Init");
  //
  mData = new MachineData;
  mData->setMachineData(MachineState_Setup);
  sensorInit();
  //
  rtcInit();
  //
  sdInit();
  //
  setSyncProvider(rtc.get); // the function to get the time from the RTC
  if (timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");
  //

  lcd.setCursor(0, 1);
  lcd.print("      DONE      ");
  delay(1000);
  lcd.clear();

  mData->setMachineData(MachineState_Loop);
}

void loop()
{
  unsigned long prevMillisSensor = 0;
  unsigned long prevMillisLog = 0;
  const long intervalSensor = 100; // ms
  const long intervalLog = 1000; // ms
  char logText[32];              // yyyy-MM-dd;hh:mm:ss;AA.AA;VV.VV

  while (1)
  {
    unsigned long currentMillis = millis();

    if (currentMillis - prevMillisSensor >= intervalSensor)
    {
      prevMillisSensor = currentMillis;
      // Sensor Reading
      current.setSensorValueAdc(analogRead(A0));
      current.routineTask();
      voltage.setSensorValueAdc(analogRead(A1));
      voltage.routineTask();
      //
    }

    if (currentMillis - prevMillisLog >= intervalLog)
    {
      prevMillisLog = currentMillis;

      // Data Logging
      char dateStr[11];
      char timeStr[9];
      char ampsStr[6];
      char voltsStr[6];

      double amps = double(current.getSensorValue()) / 100.0;
      double volts = double(voltage.getSensorValue()) / 100.0;

      snprintf(dateStr, sizeof(dateStr), "%04d-%02d-%02d", year(), month(), day());
      snprintf(timeStr, sizeof(dateStr), "%02d:%02d:%02d", hour(), minute(), second());
      dtostrf(amps, 4, 2, ampsStr);
      dtostrf(volts, 4, 2, voltsStr);

      sprintf(logText, "%s;%s;%s;%s", dateStr, timeStr, ampsStr, voltsStr);

      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      File dataFile = SD.open("datalog.csv", FILE_WRITE);
      // if the file is available, write to it:
      if (dataFile)
      {
        dataFile.println(logText);
        dataFile.close();
        // print to the serial port too:
        Serial.println(logText);
      }
      // if the file isn't open, pop up an error:
      else
      {
        Serial.println("error opening datalog.csv");
      }
      //

      // Display on LCD
      char row1Str[16], row2Str[16];
      timeStr[5] = '\0';
      sprintf(row1Str, "%s %s", dateStr, timeStr);
      sprintf(row2Str, "A:%s  V:%s", ampsStr, voltsStr);
      lcd.clear();
      lcd.print(row1Str);
      lcd.setCursor(0, 1);
      lcd.print(row2Str);
      //
    }
  }
}

/// @brief Sensor initialization (must init the MachineData instance first)
void sensorInit()
{
  if (mData == NULL)
  {
    Serial.println("mData is not defined!");
    return;
  }

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

void rtcInit()
{

  rtc.begin();
  setSyncProvider(rtc.get);
  if (timeStatus() == timeSet)
  {
    if (year() < 2024)
    {
      time_t t;
      tmElements_t tm;
      tm.Year = CalendarYrToTm(2024);
      tm.Month = 1;
      tm.Day = 25;
      tm.Hour = 23;
      tm.Minute = 59;
      tm.Second = 59;
      t = makeTime(tm);
      rtc.set(t);
      setTime(t);
    }
  }
}

void sdInit()
{
  const int chipSelect = 10;

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial.println("card initialized.");

  // One time execution only
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile)
  {
    dataFile.println("Date;Time;Current (A);Voltage(V)");
    dataFile.close();
    // print to the serial port too:
    Serial.println("Date;Time;Current (A);Voltage(V)");
  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening datalog.csv");
  }
  //
}