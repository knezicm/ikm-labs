#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int fd ;

  if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  while (1)
  {
    // add here the code for enabling transmission in MAX485 transceiver
    // and for sending the data entered by a user in the terminal using getchar()

    // add here the code for enabling reception in MAX485 transceiver
    // and for printing the received data in the terminal using putchar()

    // exit the program when a user enters c
  }
  return 0 ;
}
