LICENSE = "CLOSED"

SRC_URI = "\
	file://mcp2515-can2.dtbo \
	file://README \
"

do_install() {
	install -d ${D}${datadir}
	install -m 0644 ${WORKDIR}/README ${D}${datadir} 
	cp -r ${WORKDIR}/mcp2515-can2.dtbo ${DEPLOY_DIR_IMAGE}
}

FILES:${PN} += "${datadir}/README"
