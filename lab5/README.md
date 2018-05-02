#Readme for LAB5#

(sllin resursi https://rtime.felk.cvut.cz/can/lin-bus/)
## nije lose da se pokrene na pocetku
sudo apt-get update

## Ako je verzija kernela >= 4.11.9 koristiti
recent-kernel-workaround.patch

## potrebno za build sllin kernel modula

## instalacija kernel src headers
sudo apt-get install raspberrypi-kernel-headers

## kod za sllin
git clone git://rtime.felk.cvut.cz/linux-lin.git

# net_device patch for kernel >= 4.11.9
https://patchwork.kernel.org/patch/9826595/

make

sudo insmod ./sllin.ko baudrate=9600

sudo ldattach 25 /dev/ttyAMA0

sudo ip link set sllin0 up
ip link show dev sllin0
(sudo ip link set sllin0 down)

# potrebno za rad sa sllin modulom

# instalacija can-utils Linux-CAN / SocketCAN user space applications (https://github.com/linux-can/can-utils)
sudo apt-get install can-utils
# instalacija koriscenjem izvornog koda (https://elinux.org/Can-utils)
git clone https://github.com/linux-can/can-utils.git
cd can-utils
./autogen.sh
./configure
make
sudo make install

## Za integraciju sa can-utils koristiti
slcan_attach-sllin-workaround.patch

# potrebno za build lin_config daemon procesa

# instalacija libxml2-dev paketa
sudo apt-get -y install libxml2-dev

# instalacija libnl-route-3-dev paketa
sudo apt-get -y install libnl-route-3-dev

# PCAN-LIN konfiguracioni softver
https://www.peak-system.com/fileadmin/media/files/pcanlin.zip

# ELMOS LIN Frames

# Start Measurement frame
[0x00] 0x20 0x00 0xc8

# Read Measured Data frame
[0x03]
