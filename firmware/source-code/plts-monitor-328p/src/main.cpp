/*
 */
#include "MachineEnum/MachineEnum.h"
#include "MachineData/MachineData.h"

void setup()
{
  Serial.begin(9600);
  Serial.println("Init");
  MachineData *mData;
  mData = new MachineData;//(MachineData *)malloc(sizeof(MachineData));

  if (mData == NULL)
  {
    // Handle allocation failure
    Serial.println("Handle allocation failure");
    while (1)
    {
      // Loop indefinitely or take appropriate action
    }
  }

  mData->setMachineData(MachineState_Setup);
  // free(mData);
  delete(mData);
}
void loop()
{
  MachineData *mData;
  mData = new MachineData;//(MachineData *)malloc(sizeof(MachineData));

  mData->setMachineData(MachineState_Loop);
  while (1)
  {
  }
}