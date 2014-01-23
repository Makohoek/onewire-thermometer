/*
 * thermOperations
 * 
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#include "thermOperations.h"
#include "bitOperations.h"

int readTemperature( void )
{
  /* code */
}

static void performTransaction( void )
{
  performInitialization();
  performRomCommand();
  performFunctionCommand();
}

/* check w1-io.c for more information about the timings */
static void performInitialization( void )
{
  /* generate a reset pulse */
  writeBitGpio(ZERO);
  udelay(480);

  writeBitGpio(ONE);
  udelay(70);

  /* read a presence pulse */
  result = readBitGpio();
  msleep(1);
  if (result == ZERO)
  {
    printk(KERN_INFO "Device is present and answered\n");
  }
  else
  {
    printk(KERN_ALERT "ERROR: No device answered to initialization sequence\n");
  }
}

static void readBit(  )
{
    return;
}


