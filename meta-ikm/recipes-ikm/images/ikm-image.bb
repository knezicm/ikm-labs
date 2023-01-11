inherit core-image

IMAGE_FEATURES += "ssh-server-openssh"
IMAGE_INSTALL += "linux-lin libxml2-dev libnl-dev iproute2 canopenlinux util-linux-ldattach mcp2515 can-utils-ikm libmodbus libmodbus-dev"

require recipes-ikm/images/include/user-configuration.inc
