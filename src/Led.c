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
#include "Led.h"

static const unsigned int mLedPin = 12;
static unsigned char isInitialized = 0;

void blinkGpioLed(void)
{
  int i;
  initializeLed();
  logk((KERN_INFO "Blinking led 3 times\n"));
  for ( i = 0; i < 3; ++i )
  {
    turnLedOn();
    msleep(1000);
    turnLedOff();
    msleep(1000);
  }
  freeLed();
}

void turnLedOn(void)
{
  int errorOccured;
  if (!isInitialized)
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

void turnLedOff(void)
{
  int errorOccured;
  if (!isInitialized)
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

int initializeLed(void)
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

void freeLed(void)
{
  if (isInitialized)
  {
    gpio_free(mLedPin);
  }
}
