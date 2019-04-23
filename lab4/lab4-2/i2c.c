#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>

// Address of the Device ID register
#define DEVID			0x00
// Address of the register holding low-byte of x-axis acceleration
#define DATAX0			0x32
// I2C address of ADXL345
#define ADXL345_I2C_ADDR	0x53

int main()
{
	int fd;
	// Register addresses:
	// You can get the address of DEVID register from regs[0] and
	// the address of DATAX0 register from regs[1]
	unsigned char regs[] = {DEVID, DATAX0};
	unsigned char rx_buffer[6];
	
	// TODO: Initialize array of structures struct i2c_msg to define I2C messages
	struct i2c_msg iomsgs[] = {
		// Add your code here
	};

	// TODO: Initialize array of structures struct i2c_rdwr_ioctl_data to define I2C transactions
	struct i2c_rdwr_ioctl_data msgset[] = {
		// Add your code here
	};

	// Try to open I2C device
	fd = open("/dev/i2c-1", O_RDWR);
	
	// Check for any errors
	if (fd < 0)
	{
		printf("Error while trying to open i2c device.\n");
		return -1;
	}
	
	// TODO: Initiate a combined I2C transaction to obtain Device ID
	
	// Print the obtained Device ID
	printf("Device ID is 0x%x.\n", rx_buffer[0]);

	// Loop forever printing acceleration values every second
	for (;;)
	{
		// TODO: Initiate a combined I2C transaction to obtain acceleration values

		// Print the obtained acceleration values
		printf("X: %d\n", (rx_buffer[1] << 8) | (rx_buffer[0]));
		printf("Y: %d\n", (rx_buffer[3] << 8) | (rx_buffer[2]));
		printf("Z: %d\n", (rx_buffer[5] << 8) | (rx_buffer[4]));
		printf("--------\n");
		sleep(1);
	}
	
	close(fd);
	
	return 0;
}
