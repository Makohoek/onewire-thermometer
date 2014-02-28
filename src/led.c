#include "led.h"

static const unsigned int mLedPin = 12;
static unsigned char isInitialized = 0;

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

void turnLedOn( void )
{
  int errorOccured;
  if (! isInitialized)
  {
    logk((KERN_ALERT "[Led]: ERROR led subsystem is not initialized"));
    return;
  }
  errorOccured = gpio_direction_output(mLedPin, 1);
  if (errorOccured)
  {
    logk((KERN_ALERT "[Led]: ERROR occured while setting led to output"));
    return;
  }
  gpio_set_value(mLedPin, 1);
}

void turnLedOff( void )
{
  int errorOccured;
  if (! isInitialized)
  {
    logk((KERN_ALERT "[Led]: ERROR led subsystem is not initialized"));
    return;
  }
  errorOccured = gpio_direction_input(mLedPin);
  if (errorOccured)
  {
    logk((KERN_ALERT "[Led]: ERROR occured while setting led to input"));
  }
}

int initializeLed( void )
{
  int errorOccured;
  if (!gpio_is_valid(mLedPin))
  {
    logk((KERN_ALERT "[Led]: ERROR %d pin is not valid\n", mLedPin));
  }
  errorOccured = gpio_request(mLedPin, "Green led");
  if (errorOccured)
  {
    logk((KERN_ALERT "[Led]: ERROR occured requesting the pin(%d)\n", mLedPin));
  }
  isInitialized = 1;
  return 0;
}

void freeLed( void )
{
  if (isInitialized)
  {
    gpio_free(mLedPin);
  }
}
