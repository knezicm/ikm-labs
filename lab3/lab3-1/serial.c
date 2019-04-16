#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main ()
{
	int fd;						// serial port file descriptor
	struct termios config;		// configuration termios structure
	unsigned char *sbuf = "Hello Serial!\n";	// send buffer initialized with some data
	int sbuf_len = strlen(sbuf);		// send buffer length
	unsigned char rbuf;			// receive buffer
	
	// TODO: open serial port and check for errors while opening

	// get attributes
	if (tcgetattr(fd, &config) < 0)
	{
		fprintf(stdout, "Unable to get configuration.\n");
		return -1;
	}
	
	// TODO: make raw config

	// TODO: set the speed to 19200 baudrate
	
	// min one character, timeout disabled
	config.c_cc[VMIN] = 1;
	config.c_cc[VTIME] = 0;

	// apply the config immediately
	if (tcsetattr(fd, TCSANOW, &config) < 0)
	{
		fprintf(stdout, "Unable to set the configuration.\n");
		return -1;
	}
	
	// loop infinitely until q(uit) character is received
	while (1)
	{
		// read data from serial port
		if (read(fd, &rbuf, 1) > 0)
		{
			// check for quit condition to break the infinite loop
			if (rbuf == 'q')
				break;
			// else print received character to the console
			else
			{
				putchar(rbuf);
				fflush(stdout);
			}
		}
		
		// send some data
		if (*sbuf != '\0')
			write(fd, sbuf++, 1);
		else
			sbuf -= sbuf_len;
		
		// wait 100ms to let data be processed
		usleep(100000);
	}

	// close the serial port
	close(fd);
	return 0;
}
