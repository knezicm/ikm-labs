#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main ()
{
	int fd;						// serial port file descriptor
	struct termios config;		// configuration termios structure
	unsigned char sbuf = 's';	// serial buffer initialized with s(tart) character
	
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
	while (sbuf != 'q')
	{
		// software loopback
		if (read(fd, &sbuf, 1) > 0)
			write(fd, &sbuf, 1);
	}

	// close the serial port
	close(fd);
	return 0;
}
