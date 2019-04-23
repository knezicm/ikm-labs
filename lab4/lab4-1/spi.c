#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdio.h>

// Address of the Device ID register
#define DEVID		0x00
// Address of the register holding low-byte of x-axis acceleration
#define DATAX0		0x32
// Position of read/write bit (R/W) in SPI command byte
#define RW_BIT_POS	7
// Position of multi byte bit (MB) in SPI command byte
#define MB_BIT_POS	6

int main()
{
	int fd;
	
	unsigned char rx_buffer[7];
	unsigned char tx_buffer[] = {
		// SPI Transaction 1: read Device ID register
		DEVID | (1 << RW_BIT_POS) & ~(1 << MB_BIT_POS), /* command: single byte (MB=0) read (R/W=1) from DEVID register */
		0x00, /* dummy byte sent when we receive data from ADXL345 */
		// SPI Transaction 2: read acceleration from all three axes
		DATAX0 | (1 << RW_BIT_POS) | (1 << MB_BIT_POS), /* command: multi byte (MB=1) read (R/W=1) starting from DATAX0 register */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /* 6 dummy bytes sent while we are receiving 6 bytes from ADXL345 (2 bytes per axis) */
	};
	
	// TODO: Initialize the spi_ioc_transfer structure array
	struct spi_ioc_transfer spi[] = {
		// Add your code here
	};
	
	// Try to open SPI device
	fd = open("/dev/spidev0.0", O_RDWR);
	
	// Check for any errors
	if (fd < 0)
	{
		printf("Error while trying to open spi device.\n");
		return -1;
	}

	// TODO: Set SPI parameters (mode, size, and speed) 
	
	// TODO: Initiate an SPI transaction to obtain Device ID
	
	// Print the obtained Device ID
	printf("Device ID is 0x%x.\n", rx_buffer[1]);
	
	// Loop forever printing acceleration values every second
	for (;;)
	{
		// TODO: Initiate an SPI transaction to obtain acceleration values

		// Print the obtained acceleration values
		printf("X: %d\n", (rx_buffer[2] << 8) | (rx_buffer[1]));
		printf("Y: %d\n", (rx_buffer[4] << 8) | (rx_buffer[3]));
		printf("Z: %d\n", (rx_buffer[6] << 8) | (rx_buffer[5]));
		printf("--------\n");
		sleep(1);
	}
		
	close(fd);
	
	return 0;
}
