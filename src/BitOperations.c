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
 * GPIO management based on http://www.mjmwired.net/kernel/Documentation/gpio.txt
 */
#include "BitOperations.h"

/* members */
static int mGpioPin = -1;

/* methods */
static void assertPinIsValid(void);

static int outputGpio(Bit initialBit);
static int inputGpio(void);
static void writeGpio(Bit bitToWrite);
static Bit readGpio(void);

void pullBusLow(void)
{
  outputGpio(ZERO);
  writeGpio(ZERO);
}

void releaseBus(void)
{
  inputGpio();
}

Bit readBus(void)
{
  return readGpio();
}

static void writeGpio(Bit bitToWrite)
{
  int bit;
  assertPinIsValid();
  bit = BitToInt(bitToWrite);
  gpio_set_value(mGpioPin, bit);
}

static int outputGpio(Bit initialBit) // takes an argument to avoid bit glitching
{
  int bit, errorOccured;
  assertPinIsValid();
  bit = BitToInt(initialBit);
  errorOccured = gpio_direction_output(mGpioPin, initialBit);
  return errorOccured;
}

static int inputGpio(void)
{
  int errorOccured;
  assertPinIsValid();
  errorOccured = gpio_direction_input(mGpioPin);
  if (errorOccured)
  {
    logk((KERN_ALERT "Error when setting %d pin to input\n", mGpioPin));
  }
  return errorOccured;
}

Bit readGpio(void)
{
  int bit;
  assertPinIsValid();
  bit = gpio_get_value(mGpioPin);
  return (intToBit(bit));
}

int BitToInt(Bit myBit)
{
  return (myBit == ZERO) ? 0 : 1;
}

Bit intToBit(int myInt)
{
  return (myInt == 0) ? ZERO : ONE;
}

static void assertPinIsValid(void)
{
  if (mGpioPin == -1 || !gpio_is_valid(mGpioPin))
  {
    logk((KERN_ALERT "ERROR: %d pin is not valid\n", mGpioPin));
  }
}

int initializeBus(int GpioPin)
{
  int errorOccured;
  mGpioPin = GpioPin;
  if (!gpio_is_valid(mGpioPin))
  {
    logk((KERN_ALERT "ERROR: %d pin is not valid\n", mGpioPin));
    return -1;
  }
  // initialize the port we want to work on
  // request the gpio device
  errorOccured = gpio_request_one(mGpioPin, GPIOF_OPEN_DRAIN, "One-Wire Bus"); // used for w1 bus
  if (errorOccured)
  {
    logk((KERN_ALERT "ERROR occured requesting the GPIO pin\n"));
  }
  return (errorOccured);
}

void deleteBus(void)
{
  gpio_free(mGpioPin);
}
