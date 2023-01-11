LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://CANopenNode/LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57 \
                    file://LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57"

SRC_URI = "gitsm://github.com/CANopenNode/CANopenLinux;protocol=https;branch=master"

PV = "1.0+git${SRCPV}"
SRCREV = "c849a378bd70f63057be7ee2302b406328cbcda0"

S = "${WORKDIR}/git"

do_configure () {
	git submodule init
	git submodule update
}

do_compile () {
	oe_runmake;
	cd ${S}/cocomm;
	oe_runmake;
}

do_install () {
	install -d ${D}${bindir}
	install -m 0755 ${S}/canopend ${D}${bindir}
	install -m 0755 ${S}/cocomm/cocomm ${D}${bindir}
}

INSANE_SKIP:${PN} = "ldflags"
