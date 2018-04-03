#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#define SPI_CHAN	0
#define SPI_SPEED	1000000
#define DATA_SIZE	3

int main ()
{
  int fd;
  unsigned char data_buffer[3];

  // start the wiringPi library
  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  // initialize the SPI bus
  if ((fd = wiringPiSPISetup (SPI_CHAN, SPI_SPEED)) < 0)
  {
    fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
    return 1 ;
  }

  // Write register content
  // (see BYTE WRITE instruction in MCP2515 datasheet for details, page 68)
  // instruction type (BYTE WRITE)
  data_buffer[0] = 0x02;
  // define the register address (CNF1 in this case)
  data_buffer[1] = 0x2a;
  // byte to be written to the selected register
  // (SJW[1:0]=0b10, BRP[5:0]=0b010011)
  data_buffer[2] = 0x93;

  // print the data sent to register
  printf ("Data sent to the register: %x\n", data_buffer[2]);
  
  // exchange the data buffer
  if (wiringPiSPIDataRW (SPI_CHAN, data_buffer, DATA_SIZE) == -1)
  {
	printf ("SPI failure: %s\n", strerror (errno));
	return 1;
  }

  // Read register content
  // (see READ instruction in MCP2515 datasheet for details, page 68)
  // instruction type (READ)
  data_buffer[0] = 0x03;
  // dummy part
  data_buffer[2] = 0x00;
  // define the register address (CNF1 in this case)
  data_buffer[1] = 0x2a;
  
  // exchange the data buffer and print the register content
  if (wiringPiSPIDataRW (SPI_CHAN, data_buffer, DATA_SIZE) == -1)
  {
	printf ("SPI failure: %s\n", strerror (errno));
	return 1;
  }
  else
	printf ("Data read from the register: %x\n", data_buffer[2]);

  return 0 ;
}
