#ifndef __INC_DMESGLOGGING__
#define __INC_DMESGLOGGING__

#define LOGGING 0
#define logk(x); if (LOGGING) { printk x; }

#endif /* end of include guard: __INC_DMESGLOGGING__ */
