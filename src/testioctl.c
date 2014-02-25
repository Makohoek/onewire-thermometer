/*
 * testIoctl 
 *
 * Performs the set resolution ioctl
 *
 * Mattijs Korpershoek
 * <mattijs.korpershoek@gmail.com>
 */
#include <stdio.h> // for PRINTF
#include <stdlib.h> // for EXIT_SUCCESS, FAILURE and fprinf
/* required for perror */
#include <assert.h> 
#include <errno.h>
/* required for open */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/* required for close */
#include <unistd.h>
/* required for ioct */
#include <linux/ioctl.h>
#include <sys/ioctl.h>

/* IOCTL commands */
#define IOCTL_THERM_MAGIC 'k'
#define THERM_IOCTL_RESOLUTION _IO(IOCTL_THERM_MAGIC, 0)
#define THERM_IOCTL_MAXNR 0

typedef enum
{
  MAXIMUM=12,
  HIGH=11,
  LOW=10,
  MINIMUM=9
} TemperatureResolution;

static inline void assertOnPosixError(int value);
void changeResolution( const char* filename , TemperatureResolution resolution);

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }
  changeResolution(argv[1], MINIMUM);
  return EXIT_SUCCESS;
}

void changeResolution( const char* filename , TemperatureResolution resolution)
{
  int fd;
  printf("Performing THERM_IOCTL_RESOLUTION on %s\n", filename);
  fd = open(filename, O_RDONLY);
  assertOnPosixError(fd);

  assertOnPosixError(ioctl(fd, THERM_IOCTL_RESOLUTION, resolution));
  assertOnPosixError(close(fd));
}


static inline void assertOnPosixError(int value)
{
  if (-1 == value) 
  {
    perror("ERROR");
  }
}
