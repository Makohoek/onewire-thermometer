/**
 * This handles all the high level logic based on the operations we want to do
 */
#include "SensorOperations.h"

unsigned int discoverEachSensorID(LinkedList* sensorsList)
{
  Sensor* currentSensor;
  SensorID discoveredID;
  unsigned int numberOfSensors = 0;
  while (!isEverySensorDiscovered())
  {
    logk((KERN_INFO "Sending an initialization sequence...\n"));
    sendInitializationSequence();
    writeROMCommand(SEARCH_ROM);
    performDiscovery(discoveredID);

    currentSensor = kmalloc(GFP_KERNEL, sizeof(Sensor));
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
  logk((KERN_INFO "Send match rom to this ID:"));
  printSensorID(sensor.id);

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

void setNewResolution(Sensor sensor)
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

void blinkGpioLed(void)
{
  int i;
  initializeLed();
  logk((KERN_INFO "Blinking led 3 times\n"));
  for ( i = 0; i < 3; i++ )
  {
    turnLedOn();
    msleep(1000);
    turnLedOff();
    msleep(1000);
  }
  freeLed();
}

void initializeOneWire(int gpioPin)
{
  /* displays GPIO port */
  logk((KERN_INFO "gpioPin=%d\n", gpioPin));
  if (initializeBus(gpioPin))
  {
    logk((KERN_INFO "Gpio initialized"));
  }
  else
  {
    logk((KERN_ALERT "ERROR while calling initializeGPIO"));
  }
}
