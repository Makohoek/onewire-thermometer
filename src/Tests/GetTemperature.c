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
/* required for printing time */
#include <sys/time.h>
#include <time.h>

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

static inline void getTimeString()
{
  struct timeval tv; 
  struct tm* ptm; 
  char time_string[40]; 
  long milliseconds; 
  /* Obtain the time of day, and convert it to a tm struct. */ 
  gettimeofday (&tv, NULL); 
  ptm = localtime (&tv.tv_sec); 
  /* Format the date and time, down to a single second. */ 
  strftime (time_string, sizeof (time_string), "%H:%M:%S", ptm); 
  /* Compute milliseconds from microseconds. */ 
  milliseconds = tv.tv_usec / 1000; 
  /* Print the formatted time, in seconds, followed by a decimal point 
     and the milliseconds. */ 
  printf ("%s.%03ld", time_string, milliseconds); 
}

static inline void performOneRead(const char* fileName)
{
  char temperatureString[512]={'\0'};
  int fd = open(fileName, O_RDONLY);
  assertOnPosixError(fd);
  assertOnPosixError(read(fd, temperatureString, 512));
  getTimeString();
  printf("\t%s\n", temperatureString);
  assertOnPosixError(close(fd));
}

static inline void assertOnPosixError(int value)
{
  if (-1 == value) 
  {
    perror("ERROR");
  }
}
