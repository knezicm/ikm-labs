#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int fd;
  char byteSend;

  // start the wiringPi library
  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  // setup output mode for GPIO0 used to control communication direction
  // GPIO0 = HIGH: RS-485 transmit mode
  // GPIO0 = LOW: RS-485 receive mode
  pinMode(0, OUTPUT);
  // set it initially to be in receive mode
  digitalWrite(0, LOW);

  // open the serial port on Rpi (9600bps)
  if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  while (1)
  {
	if (serialDataAvail(fd))
	{
	  byteSend = serialGetchar(fd);	// read received byte
	  digitalWrite(0, HIGH);		// enable RS-485 transmit mode
	  serialPutchar(fd, byteSend);	// send the byte back
	  delay(1);						// delay for 1ms
	  digitalWrite(0, LOW);			// disable RS-485 transmit mode
	}
  }

  serialClose(fd);	// close the serial connection
  return 0 ;
}
