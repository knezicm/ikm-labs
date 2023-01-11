LICENSE = "CLOSED"

SRC_URI = "git://github.com/lin-bus/linux-lin;protocol=https;branch=master \
           file://0001-Patched-Makefile.patch \
           "

PV = "1.0+git${SRCPV}"
SRCREV = "29f9c37d7e6f013817512f43ba13e14dd7cf810d"

S = "${WORKDIR}/git"

inherit module

DEPENDS = "libnl libxml2"

#EXTRA_OEMAKE:append:task-install = " -C ${STAGING_KERNEL_DIR} M=${S}"
MODULES_MODULE_SYMVERS_LOCATION = "sllin"

do_compile:prepend() {
	cd sllin;
}

do_compile:append() {
	cd ..
	${CC} -c ${CFLAGS} ${S}/lin_config/src/linc_parse_xml.c ${S}/lin_config/src/linc_parse_xml.h -I ${STAGING_DIR_TARGET}/usr/include/libxml2/
        ${CC} -c ${CFLAGS} ${S}/lin_config/src/pcl_config.c ${S}/lin_config/src/pcl_config.h -I ${STAGING_DIR_TARGET}/usr/include/libxml2/
        ${CC} -c ${CFLAGS} ${S}/lin_config/src/sllin_config.c ${S}/lin_config/src/sllin_config.h -I ${STAGING_DIR_TARGET}/usr/include/libnl3/ -I ${S}/sllin
        ${CC} -c ${CFLAGS} ${S}/lin_config/src/lin_config.c ${S}/lin_config/src/lin_config.h -I ${S}/sllin
        ${CC} ${CFLAGS} ${LDFLAGS} ${S}/linc_parse_xml.o ${S}/pcl_config.o ${S}/sllin_config.o ${S}/lin_config.o -lxml2 -L ${STAGING_DIR_TARGET}/usr/lib -lnl-3 -lnl-route-3 -o ${S}/lin_config/lin_config
}

do_install:prepend() {
	cd sllin;
}

do_install:append() {
	cd ..
	install -d ${D}${bindir}
	install -m 0777 ${S}/lin_config/lin_config ${D}${bindir}/lin_config
	install -m 0644 ${S}/lin_config/examples/master_slave.pclin ${D}${bindir}/master_slave.pclin
}

do_install() {
	unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
#	oe_runmake DEPMOD=echo MODLIB="${D}${nonarch_base_libdir}/modules/" \
#	           INSTALL_FW_PATH="${D}${nonarch_base_libdir}/firmware" \
#	           CC="${KERNEL_CC}" LD="${KERNEL_LD}" \
#	           O=${STAGING_KERNEL_BUILDDIR} \
#	           ${MODULES_INSTALL_TARGET}

	install -d ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}
	install -m 644 sllin.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}

	if [ ! -e "${B}/${MODULES_MODULE_SYMVERS_LOCATION}/Module.symvers" ] ; then
		bbwarn "Module.symvers not found in ${B}/${MODULES_MODULE_SYMVERS_LOCATION}"
		bbwarn "Please consider setting MODULES_MODULE_SYMVERS_LOCATION to a"
		bbwarn "directory below B to get correct inter-module dependencies"
	else
		install -Dm0644 "${B}/${MODULES_MODULE_SYMVERS_LOCATION}"/Module.symvers ${D}${includedir}/${BPN}/Module.symvers
		# Module.symvers contains absolute path to the build directory.
		# While it doesn't actually seem to matter which path is specified,
		# clear them out to avoid confusion
		sed -e 's:${B}/::g' -i ${D}${includedir}/${BPN}/Module.symvers
	fi
}

do_lin_config() {
	cd ${B}/lin_config/src
	oe_runmake
}

addtask lin_config after do_install

RPROVIDES:${PN} += "kernel-module-linux-lin"

FILES:${PN} += "${bindir}/lin_config ${bindir}/master_slave.pclin"
