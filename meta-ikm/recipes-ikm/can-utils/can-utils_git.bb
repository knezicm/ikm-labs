LICENSE = "Unknown"
LIC_FILES_CHKSUM = "file://LICENSES/GPL-2.0-only.txt;md5=f9d20a453221a1b7e32ae84694da2c37"

SRC_URI = "git://github.com/linux-can/can-utils.git;protocol=https;branch=master"

PV = "1.0+git${SRCPV}"
SRCREV = "917729409354e224404818d0a89d081dddafe85e"

S = "${WORKDIR}/git"

PACKAGES =+ "${PN}-ikm"
FILES:${PN}-ikm = "${bindir}/cansend ${bindir}/candump ${bindir}/cangen"

inherit cmake

EXTRA_OECMAKE = ""

