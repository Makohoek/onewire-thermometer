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
  OneWireWriteByte(functionCommand);
}

static void writeROMCommand(ROMCommand romCommand)
{
  OneWireWriteByte(romCommand);
}
