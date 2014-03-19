/**
 * Copyright (c) 2014, Mattijs Korpershoek
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */
/**
 * This handles all the high level logic based on the operations we want to do
 */
#include "SensorOperations.h"
static void writeFunctionCommand(FunctionCommand command);
static void writeROMCommand(ROMCommand romcommand);

unsigned int discoverEachSensorID(LinkedList* sensorsList)
{
  Sensor* currentSensor = NULL;
  SensorID discoveredID;
  unsigned int numberOfSensors = 0;
  unsigned int maxRetries = 10;
  while (!isEverySensorDiscovered() && maxRetries > 0)
  {
    logk((KERN_INFO "Sending an initialization sequence...\n"));
    while (sendInitializationSequence() < 0); // send an initialization sequence until we get a response
    writeROMCommand(SEARCH_ROM);
    if (performDiscovery(discoveredID) >= 0)
    {
      currentSensor = kmalloc(sizeof(Sensor), GFP_KERNEL);
      if (currentSensor == NULL)
      {
        printk(KERN_ALERT "ERROR: failed to allocate memory");
        return 0;
      }
      affectSensorID(currentSensor->id, discoveredID);
      currentSensor->resolution = MAXIMUM;
      sensorsList->writeItem(sensorsList, currentSensor);
      numberOfSensors++;
    }

    maxRetries--;
  }
  return numberOfSensors;
}

int sensorRequestTemperature(Sensor sensor)
{
  Scratchpad scratchpadData;
  long temperature = 0;

  initializeLed();
  turnLedOn();

  /* CONVERT-T */
  logk((KERN_INFO "Sending an initialization sequence...\n"));
  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  writeSensorID(sensor.id);
  writeFunctionCommand(CONVERT_TEMP);
  waitForConversionDone();
  turnLedOff();
  freeLed();

  /* READ_SCRATCHPAD */
  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  writeSensorID(sensor.id);
  logk((KERN_INFO "send READ_SCRATCHPAD"));
  writeFunctionCommand(READ_SCRATCHPAD);
  readScratchpad(scratchpadData);
  temperature = extractTemperatureFromScratchpad(scratchpadData, sensor.resolution);
  logk((KERN_INFO "Readed temperature: %ld", temperature));
  return temperature;
}

void sensorSetNewResolution(Sensor sensor)
{
  Scratchpad scratchpadData;
  buildScratchpadNewResolution(scratchpadData, sensor.resolution);

  /* WRITE_SCRATCHPAD */
  logk((KERN_INFO "Sending an initialization sequence...\n"));
  sendInitializationSequence();
  writeROMCommand(MATCH_ROM);
  writeSensorID(sensor.id);
  writeFunctionCommand(WRITE_SCRATCHPAD);
  writeScratchpad(scratchpadData);
}

static void writeFunctionCommand(FunctionCommand functionCommand)
{
  oneWireWriteByte(functionCommand);
}

static void writeROMCommand(ROMCommand romCommand)
{
  oneWireWriteByte(romCommand);
}
