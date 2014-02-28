/**
 * Klogger
 *
 * conditional kernel logging for more
 * efficient debugging
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 * Alexandre Montilla
 * <alexandre.montilla@gmail.com>
 */
#ifndef __INC_DMESGLOGGING__
#define __INC_DMESGLOGGING__

#include <linux/kernel.h>

#define LOGGING 0
#define logk(x); if (LOGGING) { printk x; }

#endif /* end of include guard: __INC_DMESGLOGGING__ */
