/*
 * testTemperature 
 *
 * Reads the temperature once and displays it
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

static inline void assertOnPosixError(int value);

static inline void performOneRead(const char* fileName);

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }
  performOneRead(argv[1]);
  return EXIT_SUCCESS;
}

static inline void performOneRead(const char* fileName)
{
  char temperatureString[512]={'\0'};
  int fd = open(fileName, O_RDONLY);
  assertOnPosixError(fd);
  assertOnPosixError(read(fd, temperatureString, 512));
  printf("%s\n", temperatureString);
  assertOnPosixError(close(fd));
}

static inline void assertOnPosixError(int value)
{
  if (-1 == value) 
  {
    perror("ERROR");
  }
}
