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

	// setup output mode for BCM22 (WiringPi 3) used to control communication direction
	// BCM22 (WiringPi 3) = HIGH: RS-485 transmit mode
	// BCM22 (WiringPi 3) = LOW: RS-485 receive mode
	pinMode(3, OUTPUT);
	  
	// set it initially to be in receive mode
	digitalWrite(3, LOW);

	// TODO: open and configure serial port with 9600 baudrate

	while (1)
	{
		// get a character from console
		sbuf = getchar();

		// exit the program if a user typed q(uit)
		if (sbuf == 'q')
		{ 
			// close the serial connection
			close(fd);
			return 0;
		}
		// otherwise, send a byte entered by a user
		else
		{
			// if the entered character is not ENTER
			if (sbuf != '\n')
			{
				digitalWrite(3, HIGH);				// enable RS-485 transmit mode
				
				// TODO: send the entered byte
				
				delay(1);							// delay for 1ms
				digitalWrite(3, LOW);				// disable RS-485 transmit mode
			}
		}
		
		// TODO: read the received byte from serial port and display it in console
	}

	close(fd);	// close the serial connection
	return 0 ;
}
