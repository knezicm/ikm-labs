LICENSE = "LGPL-2.1-only & Unknown"
LIC_FILES_CHKSUM = "file://COPYING.LESSER;md5=4fbd65380cdd255951079008b364516c \
                    file://tests/LICENSE;md5=9d793911062ac2215b80ebb5899aec14"

SRC_URI = "git://github.com/dhruvvyas90/libmodbus;protocol=https;branch=master"

PV = "3.1.2+git${SRCPV}"
SRCREV = "e89e4850a0ecdcc19f5a5bcded927b26223f1ed0"

S = "${WORKDIR}/git"

inherit autotools

EXTRA_OECONF = "ac_cv_func_malloc_0_nonnull=yes"
