#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <wiringPi.h>

int main ()
{
	int fd;
	char sbuf;

	// start the wiringPi library
	if (wiringPiSetup () == -1)
	{
		fprintf (stdout, "Unable to start wiringPi.\n") ;
		return -1;
	}

	// setup output mode for BCM17 (WiringPi 0) used to control communication direction
	// BCM17 (WiringPi 0) = HIGH: RS-485 transmit mode
	// BCM17 (WiringPi 0) = LOW: RS-485 receive mode
	pinMode(0, OUTPUT);
	  
	// set it initially to be in receive mode
	digitalWrite(0, LOW);

	// TODO: open and configure serial port with 9600 baudrate
	
	// loop forever
	while (1)
	{
		// TODO: read the received byte and send it back via RS-485 network
	}

	close(fd);	// close the serial connection
	return 0;
}
