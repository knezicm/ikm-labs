#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int fd;
  char byteReceived;

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
	// get a character from console
	byteReceived = getchar();

	// exit the program if a user typed c
	if (byteReceived == 'c')
	{  	
	  serialClose(fd);
	  return 0;
	}
	// send a byte entered by a user
	else
	{
	  // if the entered character is not ENTER
	  if (byteReceived != '\n')
	  {
	    digitalWrite(0, HIGH);				// enable RS-485 transmit mode
	    serialPutchar(fd, byteReceived);	// send the entered byte
	    delay(1);							// delay for 1ms
	    digitalWrite(0, LOW);				// disable RS-485 transmit mode
	  }
	}

	if (serialDataAvail(fd))
	{
	  byteReceived = serialGetchar(fd);	// read the received byte
	  putchar(byteReceived);			// show it on console
	}
  }

  serialClose(fd);	// close the serial connection
  return 0 ;
}
